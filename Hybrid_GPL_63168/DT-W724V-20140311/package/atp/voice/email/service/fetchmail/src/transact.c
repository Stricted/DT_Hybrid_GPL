/**
 * \file transact.c -- transaction primitives for the fetchmail driver loop
 *
 * Copyright 2001 by Eric S. Raymond
 * For license terms, see the file COPYING in this directory.
 */

#include  "config.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#ifdef HAVE_MEMORY_H
#include  <memory.h>
#endif /* HAVE_MEMORY_H */
#if defined(STDC_HEADERS)
#include  <stdlib.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if defined(HAVE_STDARG_H)
#include  <stdarg.h>
#else
#include  <varargs.h>
#endif
#include <limits.h>
#include <assert.h>
#include <regex.h>
#ifdef HAVE_NET_SOCKET_H
#include <net/socket.h>
#endif
#include <sys/socket.h>
#include <netdb.h>
#include "fm_md5.h"
#include "sqlite3.h"
#include "i18n.h"
#include "socket.h"
#include "fetchmail.h"

/** Macro to clamp the argument so it is >= INT_MIN. */
#define _FIX_INT_MIN(x) ((x) < INT_MIN ? INT_MIN : (x))
/** Macro to clamp the argument so it is <= INT_MAX. */
#define _FIX_INT_MAX(x) ((x) > INT_MAX ? INT_MAX : (x))
/** Macro to clamp the argument so it is representable as an int. */
#define CAST_TO_INT(x) ((int)(_FIX_INT_MIN(_FIX_INT_MAX(x))))
/** Macro to clamp the unsigned argument so it is representable as an int. */
#define UCAST_TO_INT(x) ((int)(_FIX_INT_MAX(x)))

/* global variables: please reinitialize them explicitly for proper
 * working in daemon mode */

/* session variables initialized in init_transact() */
int suppress_tags = FALSE;	/**< emit tags in the protocol? */
char tag[TAGLEN];		/**< buffer for the tag */
static int tagnum;		/**< local counter for the tag */
/** Macro to generate the tag and store it in #tag. */
#define GENSYM	(sprintf(tag, "A%04d", ++tagnum % TAGMOD), tag)
static const struct method *protocol; /**< description of the protocol used for the current poll */
char shroud[PASSWORDLEN*2+3];	/**< string to shroud in debug output */

/* session variables initialized in do_session() */
int mytimeout;		/**< value of nonreponse timeout */

/* mail variables initialized in readheaders() */
struct msgblk msgblk;   /**< stores attributes of the currently processed message */
static int accept_count /** count of accepted recipients */, reject_count /** count of rejected recipients */;

/** add given address to xmit_names if it exactly matches a full address
 * \returns nonzero if matched */
static int map_address(const char *addr,/**< address to match */
	struct query *ctl,		/**< contains list of aliases */
	struct idlist **xmit_names	/**< list of recipient names */)
{
    const char	*lname;

    lname = idpair_find(&ctl->localnames, addr);
    if (lname) {
	if (outlevel >= O_DEBUG)
	    report(stdout, GT_("mapped address %s to local %s\n"), addr, lname);
	save_str(xmit_names, lname, XMIT_ACCEPT);
	accept_count++;
    }
    return lname != NULL;
}

/** add given name to xmit_names if it matches declared localnames */
static void map_name(const char *name, struct query *ctl, struct idlist **xmit_names)
/** \param name		name to map */
/** \param ctl		list of permissible aliases */
/** \param xmit_names	list of recipient names parsed out */
{
    const char	*lname;

    lname = idpair_find(&ctl->localnames, name);
    if (!lname && ctl->wildcard)
	lname = name;

    if (lname != (char *)NULL)
    {
	if (outlevel >= O_DEBUG)
	    report(stdout, GT_("mapped %s to local %s\n"), name, lname);
	save_str(xmit_names, lname, XMIT_ACCEPT);
	accept_count++;
    }
}

static void find_server_names(const char *hdr,
			      struct query *ctl,
			      struct idlist **xmit_names)
/** parse names out of a RFC822 header into an ID list */
/** \param hdr		RFC822 header in question */
/** \param ctl		list of permissible aliases */
/** \param xmit_names	list of recipient names parsed out */
{
    if (hdr == (char *)NULL)
	return;
    else
    {
	char	*cp;

	for (cp = nxtaddr(hdr); cp != NULL; cp = nxtaddr(NULL))
	{
	    char	*atsign;

	    /* 
	     * Handle empty address from a To: header containing only 
	     * a comment.
	     */
	    if (!*cp)
		continue;

	    /*
	     * If the name of the user begins with a qmail virtual
	     * domain prefix, ignore the prefix.  Doing this here
	     * means qvirtual will work either with ordinary name
	     * mapping or with a localdomains option.
	     */
	    if (ctl->server.qvirtual)
	    {
		int sl = strlen(ctl->server.qvirtual);
 
		if (!strncasecmp((char *)cp, ctl->server.qvirtual, sl))
		    cp += sl;
	    }

	    if ((atsign = strchr((char *)cp, '@'))) {
		struct idlist	*idp;

		/* try to match full address first, this takes
		 * precedence over localdomains and alias mappings */
		if (map_address(cp, ctl, xmit_names))
		    goto nomap;

		/*
		 * Does a trailing segment of the hostname match something
		 * on the localdomains list?  If so, save the whole name
		 * and keep going.
		 */
		for (idp = ctl->server.localdomains; idp; idp = idp->next) {
		    char	*rhs;

		    rhs = atsign + (strlen(atsign) - strlen(idp->id));
		    if (rhs > atsign &&
			(rhs[-1] == '.' || rhs[-1] == '@') &&
			strcasecmp(rhs, idp->id) == 0)
		    {
			if (outlevel >= O_DEBUG)
			    report(stdout, GT_("passed through %s matching %s\n"), 
				  cp, idp->id);
			save_str(xmit_names, (const char *)cp, XMIT_ACCEPT);
			accept_count++;
			goto nomap;
		    }
		}

		/* if we matched a local domain, idp != NULL */
		if (!idp)
		{
		    /*
		     * Check to see if the right-hand part is an alias
		     * or MX equivalent of the mailserver.  If it's
		     * not, skip this name.  If it is, we'll keep
		     * going and try to find a mapping to a client name.
		     */
		    if (!is_host_alias(atsign+1, ctl, &ai0))
		    {
			save_str(xmit_names, cp, XMIT_REJECT);
			reject_count++;
			continue;
		    }
		}
		atsign[0] = '\0';
		map_name(cp, ctl, xmit_names);
	    nomap:;
	    }
	}
    }
}

/**
 * Return zero on a syntactically invalid address, nz on a valid one.
 *
 * This used to be strchr(a, '.'), but it turns out that lines like this
 *
 * Received: from punt-1.mail.demon.net by mailstore for markb@example.com
 *          id 938765929:10:27223:2; Fri, 01 Oct 99 08:18:49 GMT
 *
 * are not uncommon.  So now we just check that the following token is
 * not itself an email address.
 */
#define VALID_ADDRESS(a)	(!strchr((a), '@'))

/** write \a value into \a rbuf, indexed by \a tp, if there is
 * sufficient room left. */
#define RBUF_WRITE(value) do { if (tp < rbuf+sizeof(rbuf)-1) *tp++=(value); } while(0)

/** Try to extract real address from the Received line.
 * If a valid Received: line is found, we return the full address in
 * a buffer which can be parsed from nxtaddr().  This is to ensure that
 * the local domain part of the address can be passed along in 
 * find_server_names() if it contains one.
 * Note: We should return a dummy header containing the address 
 * which makes nxtaddr() behave correctly. 
 */
static char *parse_received(struct query *ctl, char *bufp)
{
    char *base, *ok = (char *)NULL;
    static char rbuf[HOSTLEN + USERNAMELEN + 4]; 
    struct addrinfo *ai0;

    /*
     * Try to extract the real envelope addressee.  We look here
     * specifically for the mailserver's Received line.
     * Note: this will only work for sendmail, or an MTA that
     * shares sendmail's convention for embedding the envelope
     * address in the Received line.  Sendmail itself only
     * does this when the mail has a single recipient.
     */
    if (outlevel >= O_DEBUG)
	report(stdout, GT_("analyzing Received line:\n%s"), bufp);

    /* search for whitepace-surrounded "by" followed by valid address */
    for (base = bufp;  ; base = ok + 2)
    {
	if (!(ok = strstr(base, "by")))
	    break;
	else if (!isspace((unsigned char)ok[-1]) || !isspace((unsigned char)ok[2]))
	    continue;
	else
	{
	    char	*sp, *tp;

	    /* extract space-delimited token after "by" */
	    for (sp = ok + 2; isspace((unsigned char)*sp); sp++)
		continue;
	    tp = rbuf;
	    for (; *sp && !isspace((unsigned char)*sp); sp++)
		RBUF_WRITE(*sp);
	    *tp = '\0';

	    /* look for valid address */
	    if (VALID_ADDRESS(rbuf))
		break;
	    else
		ok = sp - 1;	/* arrange to skip this token */
	}
    }
    if (ok)
    {
	/*
	 * If it's a DNS name of the mail server, look for the
	 * recipient name after a following "for".  Otherwise
	 * punt.
	 */
	if (is_host_alias(rbuf, ctl, &ai0))
	{
	    if (outlevel >= O_DEBUG)
		report(stdout, 
		      GT_("line accepted, %s is an alias of the mailserver\n"), rbuf);
	}
	else
	{
	    if (outlevel >= O_DEBUG)
		report(stdout, 
		      GT_("line rejected, %s is not an alias of the mailserver\n"), 
		      rbuf);
	    return(NULL);
	}

	/* search for whitepace-surrounded "for" followed by xxxx@yyyy */
	for (base = ok + 4 + strlen(rbuf);  ; base = ok + 2)
	{
	    if (!(ok = strstr(base, "for")))
		break;
	    else if (!isspace((unsigned char)ok[-1]) || !isspace((unsigned char)ok[3]))
		continue;
	    else
	    {
		char	*sp, *tp;

		/* extract space-delimited token after "for" */
		for (sp = ok + 3; isspace((unsigned char)*sp); sp++)
		    continue;
		tp = rbuf;
		for (; !isspace((unsigned char)*sp); sp++)
		    RBUF_WRITE(*sp);
		*tp = '\0';

		if (strchr(rbuf, '@'))
		    break;
		else
		    ok = sp - 1;	/* arrange to skip this token */
	    }
	}
	if (ok)
	{
	    flag	want_gt = FALSE;
	    char	*sp, *tp;

	    /* char after "for" could be space or a continuation newline */
	    for (sp = ok + 4; isspace((unsigned char)*sp); sp++)
		continue;
	    tp = rbuf;
	    RBUF_WRITE(':');	/* Here is the hack.  This is to be friends */
	    RBUF_WRITE(' ');	/* with nxtaddr()... */
	    if (*sp == '<')
	    {
		want_gt = TRUE;
		sp++;
	    }
	    while (*sp == '@')		/* skip routes */
		while (*sp && *sp++ != ':')
		    continue;
            while (*sp
                   && (want_gt ? (*sp != '>') : !isspace((unsigned char)*sp))
                   && *sp != ';')
		if (!isspace((unsigned char)*sp))
		{
		    RBUF_WRITE(*sp);
		    sp++;
		}    
		else
		{
		    /* uh oh -- whitespace here can't be right! */
		    ok = (char *)NULL;
		    break;
		}
	    RBUF_WRITE('\n');
	    *tp = '\0';
	    if (strlen(rbuf) <= 3)	/* apparently nothing has been found */
		ok = NULL;
	} else
	    ok = (char *)NULL;
    }

    if (!ok)
    {
	if (outlevel >= O_DEBUG)
	    report(stdout, GT_("no Received address found\n"));
	return(NULL);
    }
    else
    {
	if (outlevel >= O_DEBUG) {
	    char *lf = rbuf + strlen(rbuf)-1;
	    *lf = '\0';
	    if (outlevel >= O_DEBUG)
		report(stdout, GT_("found Received address `%s'\n"), rbuf+2);
	    *lf = '\n';
	}
	return(rbuf);
    }
}

/* shared by readheaders and readbody */
static int sizeticker; /**< internal state variable for print_ticker() */

/** Print ticker based on a amount of data transferred of \a bytes.
 * Increments \a *tickervar by \a bytes, and if it exceeds
 * \a SIZETICKER, print a dot and reduce *tickervar by \a SIZETICKER. */
static void print_ticker(int *tickervar, int bytes)
{
    *tickervar += bytes;
    while (*tickervar >= SIZETICKER)
    {
	if (want_progress())
	{
	    fputc('.', stdout);
	    fflush(stdout);
	}
	*tickervar -= SIZETICKER;
    }
}

/** Check if \a s is equal to a LF or CR LF sequence, followed by a NUL
 * byte. \todo FIXME merge this with end_of_header? */
#define EMPTYLINE(s)   (((s)[0] == '\r' && (s)[1] == '\n' && (s)[2] == '\0') \
                       || ((s)[0] == '\n' && (s)[1] == '\0'))

/** Check if \a s is an empty line. Accept "\r*\n" as EOH in order to be bulletproof against broken survers */
static int end_of_header (const char *s)
{
    while (s[0] == '\r')
	s++;
    return (s[0] == '\n' && s[1] == '\0');
}

#define VDB_DUMMYDB_PATH        "/var/dummyvdb.s3db"
#define VDB_SQL_DUMMY_PATH      "/var/dummyvdb.sql"

typedef enum tagATP_VDB_RET_CODE_EN
{
    ATP_VDB_RET_OK,
    ATP_VDB_RET_PARA_ERR,
    ATP_VDB_RET_MEM_ERR,
    ATP_VDB_RET_SQL_ERR,
    ATP_VDB_RET_SYS_ERR
} ATP_VDB_RET_CODE_EN;
typedef enum tagATP_VDB_TABLE_ID_EN 
{
    ATP_VDB_TABLE_DECT_MAIL,
} ATP_VDB_TABLE_ID_EN;
typedef long long               VDB_ENTRY_ID;

#define VDB_START_TAG_0     0x39
#define VDB_START_TAG_1     0x37


static char *VdbReadFileToBuffer(const char *pcFileName, unsigned int *pulFileLen, int moreLen)
{
    char            *pcRet;
    FILE            *pfFile;
	unsigned int    ulLen;

    if (NULL != pulFileLen)
    {
        (*pulFileLen) = 0;
    }

    // Open the sql file
	pfFile = fopen(pcFileName, "rb");
	if (NULL == pfFile)
    {
        printf("fopen [%s] fail.\n",pcFileName);
        return NULL;
    }

	// Get file length
    fseek(pfFile, 0, SEEK_END);
    ulLen = (unsigned int)ftell(pfFile);
    fseek(pfFile, 0, SEEK_SET);

	// Malloc memory
    pcRet = malloc(ulLen + 1 + moreLen + moreLen);
	if (NULL == pcRet)
    {        
        fclose(pfFile);
        printf("malloc fail.\n");
        return NULL;
    }
    fread((void *)(pcRet + moreLen), (unsigned int)ulLen, 1, pfFile);
    pcRet[ulLen + moreLen] = '\0';
    fclose(pfFile);

    // Set return len if necessary
    if (NULL != pulFileLen)
    {
        (*pulFileLen) = ulLen;
    }

    return pcRet;
}

int ATP_VDB_busy_handler_callback(void * pvMyData, int RetryCount)
{

	/* 当前需要delay的时间 */
	static const unsigned char delays[] = { 1, 2, 5, 10, 15, 20, 25, 25,  25,  50,  50, 100 };  /*ms*/
	/* 之前delay的总时间 */
  	static const unsigned char totals[] = { 0, 1, 3,  8, 18, 33, 53, 78, 103, 128, 178, 228 };  /*ms*/

	int delay = 0, prior = 0; 
	
#ifdef ATP_PCLINT
    pvMyData = pvMyData;
#endif

	#define NDELAY (sizeof(delays)/sizeof(delays[0]))
	#define TIMEOUT 2000  /*1000 ms*/


	printf("Datebase is locked! RetryCount = %d \n",RetryCount);

	if(RetryCount < 0)
	{
		return SQLITE_ABORT;
	}

	if( RetryCount < NDELAY )
	{  
    	delay = delays[RetryCount];  
    	prior = totals[RetryCount];  
  	}
	else
  	{  
    	delay = delays[NDELAY-1];  
    	prior = totals[NDELAY-1] + delay*(RetryCount-(NDELAY-1));  
  	}
	
  	if( prior + delay > TIMEOUT )
	{  
    	delay = TIMEOUT - prior;  
    	if( delay<=0 )
		{
			printf("Write vdb failed!!\n");
			
			return 0; /*尝试了一定的次数,还是失败!!*/
		}
  	}

	printf("delay %d ms \n",delay);
	usleep(delay * 1000);

	return 1; /*重新尝试*/
}

void *ATP_DummyVDB_Init()
{
    sqlite3         *db;
    int             bDbExist;
    int             lRet;
    char 			*pcSql;

    // Try to open with read only mode to check if the db exist
    db = NULL;
    bDbExist = 0;
    lRet = sqlite3_open_v2(VDB_DUMMYDB_PATH, &db, SQLITE_OPEN_READONLY, NULL);
    sqlite3_close(db);
    if (SQLITE_OK == lRet)      // DB already exist
    {
        bDbExist = 1;
    }
    // Try to open again
    db = NULL;
    lRet = sqlite3_open(VDB_DUMMYDB_PATH, &db);
    if (SQLITE_OK != lRet)
    {
        sqlite3_close(db);
		return NULL;
	}

	// set busy_handler
	sqlite3_busy_handler(db,ATP_VDB_busy_handler_callback,NULL);
	
    // Read the sql cmd file
    pcSql = VdbReadFileToBuffer(VDB_SQL_DUMMY_PATH, NULL, 0);
    if (NULL == pcSql)
    {
        sqlite3_close(db);
        return NULL;
    }

    //without db -> Create the db
    if(bDbExist==0)
	{
	    lRet = sqlite3_exec(db, pcSql, NULL, NULL, NULL);
	    free((void *)pcSql);
	    if (SQLITE_OK != lRet)
	    {
	        sqlite3_close(db);
	        return NULL;
	    } 
	}
	if (pcSql)
	{
	    free((void *)pcSql);
	    pcSql = NULL;
	}
    return db;
}

static int VdbGetEntryIdCallback(void *pvArg, int argc, char **argv, char **azColName)
{
    VDB_ENTRY_ID        llEntryId;
    VDB_ENTRY_ID        llTryEntryId;
    VDB_ENTRY_ID        *pllTryEntryId;

    if (NULL != argv[0])
    {
        llEntryId = (VDB_ENTRY_ID)atoll(argv[0]);
        pllTryEntryId = (VDB_ENTRY_ID *)pvArg;
		llTryEntryId	= *pllTryEntryId;
        if (llTryEntryId >= llEntryId)  // If any entry id is less then the tried one, we will try the next
        {
            llTryEntryId	= llEntryId + 1;
        }
		*pllTryEntryId	= llTryEntryId;
    }

    return 0;
}

static int VdbGetRecycledEntryID(void *pvVdbHdl, const char *pcTblName, VDB_ENTRY_ID *pllEntryId)
{
    int         lRet;
    char        *pcSqlCmd;

	if (NULL == pvVdbHdl)
	{
		return ATP_VDB_RET_SQL_ERR;
	}

    pcSqlCmd = sqlite3_mprintf("select id from '%q' order by id asc", pcTblName);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }
    // Start from 1
    (*pllEntryId) = (VDB_ENTRY_ID)1;

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetEntryIdCallback, pllEntryId, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return 0;
}
extern void *g_pvMailDb;
char maildate[64]={0};
char mailfrom[64]={0};
char Subject[128]={0};
extern long long emailentryid;//重新写库时的id记录当前的id,在每次扫描的时候初始化为1(fetchmail.c中)
extern long long g_llScanAccount;
int ATP_MAIL_AddEntry(void *pvVdbHdl,long long accountIdx, int mail_number,char *Uid,
		char *datetime, char *emailAddr,char *subject,char *text)//pvTabVale存储邮件的信息
{                  
    int                 lRet;
    char                *pcSqlCmd;
	VDB_ENTRY_ID		llEntryId;

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

	if (g_llScanAccount)
	{
		if (0 == VdbGetRecycledEntryID((sqlite3 *)pvVdbHdl, "emailsub", &llEntryId))
		{
			pcSqlCmd = sqlite3_mprintf("insert into emailsub(id,account,mailnumber,Uid,MailDatetime,emailAddr,subject,text) values (%lld,%lld,%d,'%q','%q','%q','%q','%q')",
				llEntryId, accountIdx,mail_number,Uid,datetime,emailAddr,subject,text);
		}
		else
		{
			pcSqlCmd = sqlite3_mprintf("insert into emailsub(id,account,mailnumber,Uid,MailDatetime,emailAddr,subject,text) values (NULL,%lld,%d,'%q','%q','%q','%q','%q')",
				accountIdx,mail_number,Uid,datetime,emailAddr,subject,text);
		}
	}
	else
	{
		pcSqlCmd = sqlite3_mprintf("insert into emailsub(id,account,mailnumber,Uid,MailDatetime,emailAddr,subject,text) values (%lld,%lld,%d,'%q','%q','%q','%q','%q')",
			emailentryid, accountIdx,mail_number,Uid,datetime,emailAddr,subject,text);
			emailentryid++;
	}
    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);

    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }
    return ATP_VDB_RET_OK;
}

// Delete all entries in a table
int ATP_VDB_DeleteAllEmailEntries(void *pvVdbHdl)
{
    int                 lRet;
    char                *pcSqlCmd;

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    // Build the cmd
    if(g_llScanAccount)
	{   
   	   pcSqlCmd = sqlite3_mprintf("delete from emailsub where account=%lld",g_llScanAccount);
	}
	else
	{
	    pcSqlCmd = sqlite3_mprintf("delete from emailsub");
	}
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }
    return ATP_VDB_RET_OK;
}
int headerline(char *header)
{
    char *tmp=header;
	char *row;//指定当前的行
	char *split="\r\n";
    int num=0;
	row=strtok(tmp,split);	
	while(row)
	{  
	   num++;
	   row=strtok(NULL,split);
	}
	return num;
}

void matchspecialtext(char *source)
{
	char *tmp;
	char *row;//指定当前的行
	char *split="+\r\n";
	int from=0,date=0,subject=0;
	tmp=source;
	row=strtok(tmp,split);
	while(row)
	{   
		if((strncasecmp(row,"from:",5) == 0)&&(from == 0))
		{
			from=1;
			row=row+5;
			if(row)
			{
				snprintf(mailfrom,sizeof(mailfrom),"%s",row);
			}
		}
		if((strncasecmp(row,"date:",5)==0)&&(date==0))
		{
			date=1;
			row=row+5;
			if(row)
			{
				snprintf(maildate,sizeof(maildate),"%s",row);
			}
		}
		if((strncasecmp(row,"subject:",8)==0)&&(subject==0))
		{
			subject=1;
			row += 8;
			if(row)
			{
				snprintf(Subject,sizeof(Subject),"%s",row);
			}
		}
		row=strtok(NULL,split);
	}
	if(from==0)
	{
	   strcpy(mailfrom,"null");
	}
	if(date==0)
	{
	   strcpy(maildate,"null");
	}
	if(subject==0)
	{
	   strcpy(Subject,"null");
	}
}

/** read message headers and ship to SMTP or MDA */
int pop3_base64=0;
char header[2048];
int readheaders(int sock,
		       long fetchlen,
		       long reallen,
		       struct query *ctl,
		       int num,
		       flag *suppress_readbody)
/** \param sock		to which the server is connected */
/** \param fetchlen	length of message according to fetch response */
/** \param reallen	length of message according to getsizes */
/** \param ctl		query control record */
/** \param num		index of message */
/** \param suppress_readbody	output: whether call to readbody() should be supressed */
{
    struct addrblk
    {
		int		offset;
		struct addrblk	*next;
    };
    struct addrblk	*to_addrchain = NULL;
    struct addrblk	**to_chainptr = &to_addrchain;
    struct addrblk	*resent_to_addrchain = NULL;
    struct addrblk	**resent_to_chainptr = &resent_to_addrchain;

    char		buf[MSGBUFSIZE+1];
    int			from_offs, reply_to_offs, resent_from_offs;
    int			app_from_offs, sender_offs, resent_sender_offs;
    int			env_offs;
    char		*received_for, *rcv, *cp;
    static char		*delivered_to = NULL;
    int 		n, oldlen, ch, remaining, skipcount;
    size_t		linelen;
    int			delivered_to_count;
    struct idlist 	*idp;
    flag		no_local_matches = FALSE;
    flag		has_nuls;
    int			olderrs, good_addresses, bad_addresses;
    int			retain_mail = 0, refuse_mail = 0;
    flag		already_has_return_path = FALSE;

    sizeticker = 0;
    has_nuls = FALSE;
    msgblk.return_path[0] = '\0';
    olderrs = ctl->errcount;

    /* read message headers */
    msgblk.reallen = reallen;

    /*
     * We used to free the header block unconditionally at the end of 
     * readheaders, but it turns out that if close_sink() hits an error
     * condition the code for sending bouncemail will actually look
     * at the freed storage and coredump...
     */
    xfree(msgblk.headers);
    free_str_list(&msgblk.recipients);
    xfree(delivered_to);

    /* initially, no message digest */
    memset(ctl->digest, '\0', sizeof(ctl->digest));
    received_for = NULL;
    from_offs = reply_to_offs = resent_from_offs = app_from_offs = 
	sender_offs = resent_sender_offs = env_offs = -1;
    oldlen = 0;
    msgblk.msglen = 0;
    skipcount = 0;
    delivered_to_count = 0;
    ctl->mimemsg = 0;

    for (remaining = fetchlen; remaining > 0 || protocol->delimited; )
    {
	char *line, *rline;

	line = (char *)xmalloc(sizeof(buf));
	linelen = 0;
	line[0] = '\0';
	do {
	    do {
		char	*sp, *tp;

		set_timeout(mytimeout);
		if ((n = SockRead(sock, buf, sizeof(buf)-1)) == -1) {
		    set_timeout(0);
		    free(line);
		    return(PS_SOCKET);
		}
		set_timeout(0);
		/*
		 * Smash out any NULs, they could wreak havoc later on.
		 * Some network stacks seem to generate these at random,
		 * especially (according to reports) at the beginning of the
		 * first read.  NULs are illegal in RFC822 format.
		 */
		for (sp = tp = buf; sp < buf + n; sp++)
		    if (*sp)
			*tp++ = *sp;
		*tp = '\0';
		n = tp - buf;
	    } while
		  (n == 0);

	    remaining -= n;
	    linelen += n;
	    msgblk.msglen += n;

	    /*
	     * Try to gracefully handle the case where the length of a
	     * line exceeds MSGBUFSIZE.
	     */
	    if (n && buf[n-1] != '\n') 
	    {
		rline = (char *) realloc(line, linelen + 1);
		if (rline == NULL)
		{
		    free (line);
		    return(PS_IOERR);
		}
		line = rline;
		memcpy(line + linelen - n, buf, n);
		line[linelen] = '\0';
		ch = ' '; /* So the next iteration starts */
		continue;
	    }

	    /* lines may not be properly CRLF terminated; fix this for qmail */
	    /* we don't want to overflow the buffer here */
	    if (ctl->forcecr && buf[n-1]=='\n' && (n==1 || buf[n-2]!='\r'))
	    {
		char * tcp;
		rline = (char *) realloc(line, linelen + 2);
		if (rline == NULL)
		{
		    free (line);
		    return(PS_IOERR);
		}
		line = rline;
		memcpy(line + linelen - n, buf, n - 1);
		tcp = line + linelen - 1;
		*tcp++ = '\r';
		*tcp++ = '\n';
		*tcp = '\0';
		/* n++; - not used later on */
		linelen++;
	    }
	    else
	    {
		rline = (char *) realloc(line, linelen + 1);
		if (rline == NULL)
		{
		    free (line);
		    return(PS_IOERR);
		}
		line = rline;
		memcpy(line + linelen - n, buf, n + 1);
	    }

	    /* check for end of headers */
	    if (end_of_header(line))
	    {
eoh:
		if (linelen != strlen (line))
		    has_nuls = TRUE;
		free(line);
		goto process_headers;
	    }

	    /*
	     * Check for end of message immediately.  If one of your folders
	     * has been mangled, the delimiter may occur directly after the
	     * header.
	     */
	    if (protocol->delimited && line[0] == '.' && EMPTYLINE(line+1))
	    {
		if (suppress_readbody)
		    *suppress_readbody = TRUE;
		goto eoh; /* above */
	    }

	    /*
	     * At least one brain-dead website (netmind.com) is known to
	     * send out robotmail that's missing the RFC822 delimiter blank
	     * line before the body! Without this check fetchmail segfaults.
	     * With it, we treat such messages as spam and refuse them.
	     *
	     * Frederic Marchal reported in February 2006 that hotmail
	     * or something improperly wrapped a very long TO header
	     * (wrapped without inserting whitespace in the continuation
	     * line) and found that this code thus refused a message
	     * that should have been delivered.
	     *
	     * XXX FIXME: we should probably wrap the message up as
	     * message/rfc822 attachment and forward to postmaster (Rob
	     * MacGregor)
	     */
	    if (!refuse_mail
		&& !ctl->server.badheader == BHACCEPT
		&& !isspace((unsigned char)line[0])
		&& !strchr(line, ':'))
	    {
		if (linelen != strlen (line))
		    has_nuls = TRUE;
		if (outlevel > O_SILENT)
		    report(stdout,
			   GT_("incorrect header line found - see manpage for bad-header option\n"));
		if (outlevel >= O_VERBOSE)
		    report (stdout, GT_("line: %s"), line);
		refuse_mail = 1;
	    }

	    /* check for RFC822 continuations */
	    set_timeout(mytimeout);
	    ch = SockPeek(sock);
	    set_timeout(0);
	} while
	    (ch == ' ' || ch == '\t');	/* continuation to next line? */

	/* write the message size dots */
	if ((outlevel > O_SILENT && outlevel < O_VERBOSE) && linelen > 0)
	{
	    print_ticker(&sizeticker, linelen);
	}

	/*
	 * Decode MIME encoded headers. We MUST do this before
	 * looking at the Content-Type / Content-Transfer-Encoding
	 * headers (RFC 2046).
	 */
	if ( ctl->mimedecode )
	{
	    char *tcp;
	    UnMimeHeader(line);
	    /* the line is now shorter. So we retrace back till we find
	     * our terminating combination \n\0, we move backwards to
	     * make sure that we don't catch some \n\0 stored in the
	     * decoded part of the message */
	    for (tcp = line + linelen - 1; tcp > line && (*tcp != 0 || tcp[-1] != '\n'); tcp--) { }
	    if  (tcp > line) linelen = tcp - line;
	}


	/* skip processing if we are going to retain or refuse this mail */
	if (retain_mail || refuse_mail)
	{
	    free(line);
	    continue;
	}

	/* we see an ordinary (non-header, non-message-delimiter) line */
	if (linelen != strlen (line))
	    has_nuls = TRUE;

	/*
	 * The University of Washington IMAP server (the reference
	 * implementation of IMAP4 written by Mark Crispin) relies
	 * on being able to keep base-UID information in a special
	 * message at the head of the mailbox.  This message should
	 * neither be deleted nor forwarded.
	 *
	 * An example for such a message is (keep this in so people
	 * find it when looking where the special code is to handle the
	 * data):
	 *
	 *   From MAILER-DAEMON Wed Nov 23 11:38:42 2005
	 *   Date: 23 Nov 2005 11:38:42 +0100
	 *   From: Mail System Internal Data <MAILER-DAEMON@mail.example.org>
	 *   Subject: DON'T DELETE THIS MESSAGE -- FOLDER INTERNAL DATA
	 *   Message-ID: <1132742322@mail.example.org>
	 *   X-IMAP: 1132742306 0000000001
	 *   Status: RO
	 *
	 *   This text is part of the internal format of your mail folder, and is not
	 *   a real message.  It is created automatically by the mail system software.
	 *   If deleted, important folder data will be lost, and it will be re-created
	 *   with the data reset to initial values.
	 *
	 * This message is only visible if a POP3 server that is unaware
	 * of these UWIMAP messages is used besides UWIMAP or PINE.
	 *
	 * We will just check if the first message in the mailbox has an
	 * X-IMAP: header.
	 */
#ifdef POP2_ENABLE
	/*
	 * We disable this check under POP2 because there's no way to
	 * prevent deletion of the message.  So at least we ought to
	 * forward it to the user so he or she will have some clue
	 * that things have gone awry.
	 */
	if (servport("pop2") != servport(protocol->service))
#endif /* POP2_ENABLE */
	    if (num == 1 && !strncasecmp(line, "X-IMAP:", 7)) {
		free(line);
		retain_mail = 1;
		continue;
	    }

	/*
	 * This code prevents fetchmail from becoming an accessory after
	 * the fact to upstream sendmails with the `E' option on.  It also
	 * copes with certain brain-dead POP servers (like NT's) that pass
	 * through Unix from_ lines.
	 *
	 * Either of these bugs can result in a non-RFC822 line at the
	 * beginning of the headers.  If fetchmail just passes it
	 * through, the client listener may think the message has *no*
	 * headers (since the first) line it sees doesn't look
	 * RFC822-conformant) and fake up a set.
	 *
	 * What the user would see in this case is bogus (synthesized)
	 * headers, followed by a blank line, followed by the >From, 
	 * followed by the real headers, followed by a blank line,
	 * followed by text.
	 *
	 * We forestall this lossage by tossing anything that looks
	 * like an escaped or passed-through From_ line in headers.
	 * These aren't RFC822 so our conscience is clear...
	 */
	if (!strncasecmp(line, ">From ", 6) || !strncasecmp(line, "From ", 5))
	{
	    free(line);
	    continue;
	}

	/*
	 * We remove all Delivered-To: headers if dropdelivered is set
	 * - special care must be taken if Delivered-To: is also used
	 * as envelope at the same time.
	 *
	 * This is to avoid false mail loops errors when delivering
	 * local messages to and from a Postfix or qmail mailserver.
	 */
	if (ctl->dropdelivered && !strncasecmp(line, "Delivered-To:", 13)) 
	{
	    if (delivered_to ||
	    	ctl->server.envelope == STRING_DISABLED ||
		!ctl->server.envelope ||
		strcasecmp(ctl->server.envelope, "Delivered-To") ||
		delivered_to_count != ctl->server.envskip)
		free(line);
	    else 
		delivered_to = line;
	    delivered_to_count++;
	    continue;
	}

	/*
	 * If we see a Status line, it may have been inserted by an MUA
	 * on the mail host, or it may have been inserted by the server
	 * program after the headers in the transaction stream.  This
	 * can actually hose some new-mail notifiers such as xbuffy,
	 * which assumes any Status line came from a *local* MDA and
	 * therefore indicates that the message has been seen.
	 *
	 * Some buggy POP servers (including at least the 3.3(20)
	 * version of the one distributed with IMAP) insert empty
	 * Status lines in the transaction stream; we'll chuck those
	 * unconditionally.  Nonempty ones get chucked if the user
	 * turns on the dropstatus flag.
	 */
	{
	    char	*tcp;

	    if (!strncasecmp(line, "Status:", 7))
		tcp = line + 7;
	    else if (!strncasecmp(line, "X-Mozilla-Status:", 17))
		tcp = line + 17;
	    else
		tcp = NULL;
	    if (tcp) {
		while (*tcp && isspace((unsigned char)*tcp)) tcp++;
		if (!*tcp || ctl->dropstatus)
		{
		    free(line);
		    continue;
		}
	    }
	}

	if (ctl->rewrite)
	    line = reply_hack(line, ctl->server.truename, &linelen);

	/*
	 * OK, this is messy.  If we're forwarding by SMTP, it's the
	 * SMTP-receiver's job (according to RFC821, page 22, section
	 * 4.1.1) to generate a Return-Path line on final delivery.
	 * The trouble is, we've already got one because the
	 * mailserver's SMTP thought *it* was responsible for final
	 * delivery.
	 *
	 * Stash away the contents of Return-Path (as modified by reply_hack)
	 * for use in generating MAIL FROM later on, then prevent the header
	 * from being saved with the others.  In effect, we strip it off here.
	 *
	 * If the SMTP server conforms to the standards, and fetchmail gets the
	 * envelope sender from the Return-Path, the new Return-Path should be
	 * exactly the same as the original one.
	 *
	 * We do *not* want to ignore empty Return-Path headers.  These should
	 * be passed through as a way of indicating that a message should
	 * not trigger bounces if delivery fails.  What we *do* need to do is
	 * make sure we never try to rewrite such a blank Return-Path.  We
	 * handle this with a check for <> in the rewrite logic above.
	 *
	 * Also, if an email has multiple Return-Path: headers, we only
	 * read the first occurance, as some spam email has more than one
	 * Return-Path.
	 *
	 */
	if ((already_has_return_path==FALSE) && !strncasecmp("Return-Path:", line, 12) && (cp = nxtaddr(line)))
	{
	    char nulladdr[] = "<>";
	    already_has_return_path = TRUE;
	    if (cp[0]=='\0')	/* nxtaddr() strips the brackets... */
		cp=nulladdr;
	    strncpy(msgblk.return_path, cp, sizeof(msgblk.return_path));
	    msgblk.return_path[sizeof(msgblk.return_path)-1] = '\0';
	    if (!ctl->mda) {
		free(line);
		continue;
	    }
	}

	if (!msgblk.headers)
	{
	    oldlen = linelen;
	    msgblk.headers = (char *)xmalloc(oldlen + 1);
	    (void) memcpy(msgblk.headers, line, linelen);
	    msgblk.headers[oldlen] = '\0';
	    free(line);
	    line = msgblk.headers;
	}
	else
	{
	    char *newhdrs;
	    int	newlen;

	    newlen = oldlen + linelen;
	    newhdrs = (char *) realloc(msgblk.headers, newlen + 1);
	    if (newhdrs == NULL) {
		free(line);
		return(PS_IOERR);
	    }
	    msgblk.headers = newhdrs;
	    memcpy(msgblk.headers + oldlen, line, linelen);
	    msgblk.headers[newlen] = '\0';
	    free(line);
	    line = msgblk.headers + oldlen;
	    oldlen = newlen;
	}

	/* find offsets of various special headers */
	if (!strncasecmp("From:", line, 5))
	    from_offs = (line - msgblk.headers);
	else if (!strncasecmp("Reply-To:", line, 9))
	    reply_to_offs = (line - msgblk.headers);
	else if (!strncasecmp("Resent-From:", line, 12))
	    resent_from_offs = (line - msgblk.headers);
	else if (!strncasecmp("Apparently-From:", line, 16))
	    app_from_offs = (line - msgblk.headers);
	/*
	 * Netscape 4.7 puts "Sender: zap" in mail headers.  Perverse...
	 *
	 * But a literal reading of RFC822 sec. 4.4.2 supports the idea
	 * that Sender: *doesn't* have to be a working email address.
	 *
	 * The definition of the Sender header in RFC822 says, in
	 * part, "The Sender mailbox specification includes a word
	 * sequence which must correspond to a specific agent (i.e., a
	 * human user or a computer program) rather than a standard
	 * address."  That implies that the contents of the Sender
	 * field don't need to be a legal email address at all So
	 * ignore any Sender or Resent-Sender lines unless they
	 * contain @.
	 *
	 * (RFC2822 says the contents of Sender must be a valid mailbox
	 * address, which is also what RFC822 4.4.4 implies.)
	 */
	else if (!strncasecmp("Sender:", line, 7) && (strchr(line, '@') || strchr(line, '!')))
	    sender_offs = (line - msgblk.headers);
	else if (!strncasecmp("Resent-Sender:", line, 14) && (strchr(line, '@') || strchr(line, '!')))
	    resent_sender_offs = (line - msgblk.headers);

#ifdef __UNUSED__
 	else if (!strncasecmp("Message-Id:", line, 11))
	{
	    if (ctl->server.uidl)
 	    {
	        char id[IDLEN+1];

		line[IDLEN+12] = 0;		/* prevent stack overflow */
 		sscanf(line+12, "%s", id);
 	        if (!str_find( &ctl->newsaved, num))
		{
 		    struct idlist *newl = save_str(&ctl->newsaved,id,UID_SEEN);
		    newl->val.status.num = num;
		}
 	    }
 	}
#endif /* __UNUSED__ */

	/* if multidrop is on, gather addressee headers */
	if (MULTIDROP(ctl))
	{
	    if (!strncasecmp("To:", line, 3)
		|| !strncasecmp("Cc:", line, 3)
		|| !strncasecmp("Bcc:", line, 4)
		|| !strncasecmp("Apparently-To:", line, 14))
	    {
		*to_chainptr = (struct addrblk *)xmalloc(sizeof(struct addrblk));
		(*to_chainptr)->offset = (line - msgblk.headers);
		to_chainptr = &(*to_chainptr)->next; 
		*to_chainptr = NULL;
	    }

	    else if (!strncasecmp("Resent-To:", line, 10)
		     || !strncasecmp("Resent-Cc:", line, 10)
		     || !strncasecmp("Resent-Bcc:", line, 11))
	    {
		*resent_to_chainptr = (struct addrblk *)xmalloc(sizeof(struct addrblk));
		(*resent_to_chainptr)->offset = (line - msgblk.headers);
		resent_to_chainptr = &(*resent_to_chainptr)->next; 
		*resent_to_chainptr = NULL;
	    }

	    else if (ctl->server.envelope != STRING_DISABLED)
	    {
		if (ctl->server.envelope 
		    && strcasecmp(ctl->server.envelope, "Received"))
		{
		    if (env_offs == -1 && !strncasecmp(ctl->server.envelope,
						       line,
						       strlen(ctl->server.envelope)))
		    {				
			if (skipcount++ < ctl->server.envskip)
			    continue;
			env_offs = (line - msgblk.headers);
		    }    
		}
		else if (!received_for && !strncasecmp("Received:", line, 9))
		{
		    if (skipcount++ < ctl->server.envskip)
			continue;
		    received_for = parse_received(ctl, line);
		}
	    }
	}
    }

process_headers:

    if (retain_mail) {
	return(PS_RETAINED);
    }

    if (refuse_mail)
	return(PS_REFUSED);
    /*
     * This is the duplicate-message killer code.
     *
     * When mail delivered to a multidrop mailbox on the server is
     * addressed to multiple people on the client machine, there will
     * be one copy left in the box for each recipient.  This is not a
     * problem if we have the actual recipient address to dispatch on
     * (e.g. because we've mined it out of sendmail trace headers, or
     * a qmail Delivered-To line, or a declared sender envelope line).
     *
     * But if we're mining addressees out of the To/Cc/Bcc fields, and
     * if the mail is addressed to N people, each recipient will
     * get N copies.  This is bad when N > 1.
     *
     * Foil this by suppressing all but one copy of a message with a
     * given set of headers.
     *
     * Note: This implementation only catches runs of successive
     * messages with the same ID, but that should be good
     * enough. A more general implementation would have to store
     * ever-growing lists of seen message-IDs; in a long-running
     * daemon this would turn into a memory leak even if the 
     * implementation were perfect.
     * 
     * Don't mess with this code casually.  It would be way too easy
     * to break it in a way that blackholed mail.  Better to pass
     * the occasional duplicate than to do that...
     *
     * Matthias Andree:
     * The real fix however is to insist on Delivered-To: or similar
     * headers and require that one copy per recipient be dropped.
     * Everything else breaks sooner or later.
     */
    if (MULTIDROP(ctl) && msgblk.headers)
    {
	MD5_CTX context;

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)msgblk.headers, strlen(msgblk.headers));
	MD5Final(ctl->digest, &context);

	if (!received_for && env_offs == -1 && !delivered_to)
	{
	    /*
	     * Hmmm...can MD5 ever yield all zeroes as a hash value?
	     * If so there is a one in 18-quadrillion chance this 
	     * code will incorrectly nuke the first message.
	     */
	    if (!memcmp(ctl->lastdigest, ctl->digest, DIGESTLEN))
		return(PS_REFUSED);
	}
	memcpy(ctl->lastdigest, ctl->digest, DIGESTLEN);
    }

    /*
     * Hack time.  If the first line of the message was blank, with no headers
     * (this happens occasionally due to bad gatewaying software) cons up
     * a set of fake headers.  
     *
     * If you modify the fake header template below, be sure you don't
     * make either From or To address @-less, otherwise the reply_hack
     * logic will do bad things.
     */
    if (msgblk.headers == (char *)NULL)
    {
	snprintf(buf, sizeof(buf),
		"From: FETCHMAIL-DAEMON\r\n"
		"To: %s@%s\r\n"
		"Subject: Headerless mail from %s's mailbox on %s\r\n",
		user, fetchmailhost, ctl->remotename, ctl->server.truename);
	msgblk.headers = xstrdup(buf);
    }

    /*
     * We can now process message headers before reading the text.
     * In fact we have to, as this will tell us where to forward to.
     */

    /* Check for MIME headers indicating possible 8-bit data */
    ctl->mimemsg = MimeBodyType(msgblk.headers, ctl->mimedecode);

#ifdef SDPS_ENABLE
    if (ctl->server.sdps && sdps_envfrom)
    {
	/* We have the real envelope return-path, stored out of band by
	 * SDPS - that's more accurate than any header is going to be.
	 */
	strlcpy(msgblk.return_path, sdps_envfrom, sizeof(msgblk.return_path));
	free(sdps_envfrom);
    } else
#endif /* SDPS_ENABLE */
    /*
     * If there is a Return-Path address on the message, this was
     * almost certainly the MAIL FROM address given the originating
     * sendmail.  This is the best thing to use for logging the
     * message origin (it sets up the right behavior for bounces and
     * mailing lists).  Otherwise, fall down to the next available 
     * envelope address (which is the most probable real sender).
     * *** The order is important! ***
     * This is especially useful when receiving mailing list
     * messages in multidrop mode.  if a local address doesn't
     * exist, the bounce message won't be returned blindly to the 
     * author or to the list itself but rather to the list manager
     * (ex: specified by "Sender:") which is much less annoying.  This 
     * is true for most mailing list packages.
     */
    if( !msgblk.return_path[0] ){
	char *ap = NULL;
	if (resent_sender_offs >= 0 && (ap = nxtaddr(msgblk.headers + resent_sender_offs)));
	else if (sender_offs >= 0 && (ap = nxtaddr(msgblk.headers + sender_offs)));
	else if (resent_from_offs >= 0 && (ap = nxtaddr(msgblk.headers + resent_from_offs)));
	else if (from_offs >= 0 && (ap = nxtaddr(msgblk.headers + from_offs)));
	else if (reply_to_offs >= 0 && (ap = nxtaddr(msgblk.headers + reply_to_offs)));
	else if (app_from_offs >= 0 && (ap = nxtaddr(msgblk.headers + app_from_offs))) {}
	/* multi-line MAIL FROM addresses confuse SMTP terribly */
	if (ap && !strchr(ap, '\n')) {
	    strncpy(msgblk.return_path, ap, sizeof(msgblk.return_path));
	    msgblk.return_path[sizeof(msgblk.return_path)-1] = '\0';
	}
    }

    /* cons up a list of local recipients */
    msgblk.recipients = (struct idlist *)NULL;
    accept_count = reject_count = 0;
    /* is this a multidrop box? */
    if (MULTIDROP(ctl))
    {
#ifdef SDPS_ENABLE
	if (ctl->server.sdps && sdps_envto)
	{
	    /* We have the real envelope recipient, stored out of band by
	     * SDPS - that's more accurate than any header is going to be.
	     */
	    find_server_names(sdps_envto, ctl, &msgblk.recipients);
	    free(sdps_envto);
	} else
#endif /* SDPS_ENABLE */ 
	    if (env_offs > -1) {	    /* We have the actual envelope addressee */
		if (outlevel >= O_DEBUG) {
		    const char *tmps = msgblk.headers + env_offs;
		    size_t l = strcspn(tmps, "\r\n");
		    report(stdout, GT_("Parsing envelope \"%s\" names \"%-.*s\"\n"), ctl->server.envelope, UCAST_TO_INT(l), tmps);
		}
		find_server_names(msgblk.headers + env_offs, ctl, &msgblk.recipients);
	    }
	else if (delivered_to && ctl->server.envelope != STRING_DISABLED &&
		ctl->server.envelope && !strcasecmp(ctl->server.envelope, "Delivered-To"))
	{
	    if (outlevel >= O_DEBUG) {
		const char *tmps = delivered_to + 2 + strlen(ctl->server.envelope);
		size_t l = strcspn(tmps, "\r\n");
		report(stdout, GT_("Parsing envelope \"%s\" names \"%-.*s\"\n"), ctl->server.envelope, UCAST_TO_INT(l), tmps);
	    }
	    find_server_names(delivered_to, ctl, &msgblk.recipients);
	    xfree(delivered_to);
	} else if (received_for) {
	    /*
	     * We have the Received for addressee.  
	     * It has to be a mailserver address, or we
	     * wouldn't have got here.
	     * We use find_server_names() to let local 
	     * hostnames go through.
	     */
	    if (outlevel >= O_DEBUG) {
		const char *tmps = received_for + 2;
		size_t l = strcspn(tmps, "\r\n");
		report(stdout, GT_("Parsing Received names \"%-.*s\"\n"), UCAST_TO_INT(l), tmps);
	    }
	    find_server_names(received_for, ctl, &msgblk.recipients);
	} else {
	    /*
	     * We haven't extracted the envelope address.
	     * So check all the "Resent-To" header addresses if 
	     * they exist.  If and only if they don't, consider
	     * the "To" addresses.
	     */
	    register struct addrblk *nextptr;
	   if (outlevel >= O_DEBUG)
		   report(stdout, GT_("No envelope recipient found, resorting to header guessing.\n"));
	    if (resent_to_addrchain) {
		/* delete the "To" chain and substitute it 
		 * with the "Resent-To" list 
		 */
		while (to_addrchain) {
		    nextptr = to_addrchain->next;
		    free(to_addrchain);
		    to_addrchain = nextptr;
		}
		to_addrchain = resent_to_addrchain;
		resent_to_addrchain = NULL;
	    }
	    /* now look for remaining adresses */
	    while (to_addrchain) {
		if (outlevel >= O_DEBUG) {
		    const char *tmps = msgblk.headers+to_addrchain->offset;
		    size_t l = strcspn(tmps, "\r\n");
		    report(stdout, GT_("Guessing from header \"%-.*s\".\n"), UCAST_TO_INT(l), tmps);
		}

		find_server_names(msgblk.headers+to_addrchain->offset, ctl, &msgblk.recipients);
		nextptr = to_addrchain->next;
		free(to_addrchain);
		to_addrchain = nextptr;
	    }
	}
	if (!accept_count)
	{
	    no_local_matches = TRUE;
	    save_str(&msgblk.recipients, run.postmaster, XMIT_ACCEPT);
	    if (outlevel >= O_DEBUG)
		report(stdout,
		      GT_("no local matches, forwarding to %s\n"),
		      run.postmaster);
	}
    }
    else	/* it's a single-drop box, use first localname */
	save_str(&msgblk.recipients, ctl->localnames->id, XMIT_ACCEPT);


    /*
     * Time to either address the message or decide we can't deliver it yet.
     */
    if (ctl->errcount > olderrs)	/* there were DNS errors above */
    {
	if (outlevel >= O_DEBUG)
	    report(stdout,
		   GT_("forwarding and deletion suppressed due to DNS errors\n"));
	return(PS_TRANSIENT);
    }
    else
    {
	/* set up stuffline() so we can deliver the message body through it */ 
	if ((n = open_sink(ctl, &msgblk,
			   &good_addresses, &bad_addresses)) != PS_SUCCESS)
	{
	    return(n);
	}
    }

    n = 0;
    /*
     * Some server/sendmail combinations cause problems when our
     * synthetic Received line is before the From header.  Cope
     * with this...
     */
    if ((rcv = strstr(msgblk.headers, "Received:")) == (char *)NULL)
	rcv = msgblk.headers;
	
    /* handle ">Received:" lines too */
    while (rcv > msgblk.headers && rcv[-1] != '\n')
	rcv--;
    if (rcv > msgblk.headers)
    {
	char	c = *rcv;

	*rcv = '\0';
	n = stuffline(ctl, msgblk.headers);
	*rcv = c;
    }
    if (!run.invisible && n != -1)
    {
	/* utter any per-message Received information we need here */
        if (ctl->server.trueaddr) {
	    char saddr[50];
	    int e;

	    e = getnameinfo(ctl->server.trueaddr, ctl->server.trueaddr_len,
		    saddr, sizeof(saddr), NULL, 0,
		    NI_NUMERICHOST);
	    if (e)
		snprintf(saddr, sizeof(saddr), "(%-.*s)", (int)(sizeof(saddr) - 3), gai_strerror(e));
	    snprintf(buf, sizeof(buf),
		    "Received: from %s [%s]\r\n", 
		    ctl->server.truename, saddr);
	} else {
	    snprintf(buf, sizeof(buf),
		  "Received: from %s\r\n", ctl->server.truename);
	}
	n = stuffline(ctl, buf);
	if (n != -1)
	{
	    /*
	     * We SHOULD (RFC-2821 sec. 4.4/p. 53) make sure to only use
	     * IANA registered protocol names here.
	     */
	    snprintf(buf, sizeof(buf),
		    "\tby %s with %s (fetchmail-%s",
		    fetchmailhost,
		    protocol->name,
		    VERSION);
	    if (ctl->server.tracepolls)
	    {
		snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
			" polling %s account %s",
			ctl->server.pollname,
			ctl->remotename);
		if (ctl->folder)
		    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
			    " folder %s",
			    ctl->folder);
	    }
	    snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), ")\r\n");
	    n = stuffline(ctl, buf);
	    if (n != -1)
	    {
		buf[0] = '\t';
		if (good_addresses == 0)
		{
		    snprintf(buf+1, sizeof(buf)-1, "for <%s> (by default); ",
			    rcpt_address (ctl, run.postmaster, 0));
		}
		else if (good_addresses == 1)
		{
		    for (idp = msgblk.recipients; idp; idp = idp->next)
			if (idp->val.status.mark == XMIT_ACCEPT)
			    break;	/* only report first address */
		    if (idp)
			snprintf(buf+1, sizeof(buf)-1,
				"for <%s>", rcpt_address (ctl, idp->id, 1));
		    snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf)-1,
			    " (%s); ",
			    MULTIDROP(ctl) ? "multi-drop" : "single-drop");
		}
		else
		    buf[1] = '\0';

		snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "%s\r\n",
			rfc822timestamp());
		n = stuffline(ctl, buf);
	    }
	}
    }
    if (n != -1)
	n = stuffline(ctl, rcv);	/* ship out rest of msgblk.headers */  
	snprintf(header,sizeof(header)-1,"%s",rcv);
	header[strlen(header)]='\0';
	if(strstr(rcv,"Content-Transfer-Encoding: base64"))
	{
		pop3_base64=1;
	}
	else
    {
	    pop3_base64=0;
	}
	matchspecialtext(rcv);

    //标题字符转换,转换为utf-8格式
    char *mailbody;
	char tmpsubject[128];
	memset(tmpsubject, 0, sizeof(tmpsubject));;
	mailbody=ScanIconvTryConvert(Subject);
    memset(Subject,0,strlen(Subject));
	//subject
	replace(mailbody,tmpsubject);
	memset(Subject,0,sizeof(Subject));
	snprintf(Subject,strlen(tmpsubject)+1,"%s",tmpsubject);

    if (n == -1)
    {
	report(stdout, GT_("writing RFC822 msgblk.headers\n"));
	release_sink(ctl);
	return(PS_IOERR);
    }
    if (want_progress())
	fputc('#', stdout);

    /* write error notifications */
    if (no_local_matches || has_nuls || bad_addresses)
    {
	int	errlen = 0;
	char	errhd[USERNAMELEN + POPBUFSIZE], *errmsg;
	errmsg = errhd;
	strlcpy(errhd, "X-Fetchmail-Warning: ", sizeof(errhd));
	if (no_local_matches)
	{
	    if (reject_count != 1)
		strlcat(errhd, GT_("no recipient addresses matched declared local names"), sizeof(errhd));
	    else
	    {
		for (idp = msgblk.recipients; idp; idp = idp->next)
		    if (idp->val.status.mark == XMIT_REJECT)
			break;
		snprintf(errhd+strlen(errhd), sizeof(errhd)-strlen(errhd),
			GT_("recipient address %s didn't match any local name"), idp->id);
	    }
	}

	if (has_nuls)
	{   
	    if (errhd[sizeof("X-Fetchmail-Warning: ")])
		snprintf(errhd+strlen(errhd), sizeof(errhd)-strlen(errhd), "; ");
	    snprintf(errhd+strlen(errhd), sizeof(errhd)-strlen(errhd),
			GT_("message has embedded NULs"));
	}

	if (bad_addresses)
	{   
	    if (errhd[sizeof("X-Fetchmail-Warning: ")])
		snprintf(errhd+strlen(errhd), sizeof(errhd)-strlen(errhd), "; ");
	    snprintf(errhd+strlen(errhd), sizeof(errhd)-strlen(errhd),
			GT_("SMTP listener rejected local recipient addresses: "));
	    errlen = strlen(errhd);
	    for (idp = msgblk.recipients; idp; idp = idp->next)
		if (idp->val.status.mark == XMIT_RCPTBAD)
		    errlen += strlen(idp->id) + 2;

	    errmsg = (char *)xmalloc(errlen + 3);
	    strcpy(errmsg, errhd);
	    for (idp = msgblk.recipients; idp; idp = idp->next)
		if (idp->val.status.mark == XMIT_RCPTBAD)
		{
		    strcat(errmsg, idp->id);
		    if (idp->next)
			strcat(errmsg, ", ");
		}
	}
	strcat(errmsg, "\r\n");

	/* ship out the error line */
	stuffline(ctl, errmsg);

	if (errmsg != errhd)
	    free(errmsg);
    }

    /* issue the delimiter line */
    cp = buf;
    *cp++ = '\r';
    *cp++ = '\n';
    *cp = '\0';
    n = stuffline(ctl, buf);
    if ((size_t)n == strlen(buf))
	return PS_SUCCESS;
    else
	return PS_SOCKET;
}

char mailtext[650]={0};
int maillength=0;
extern int attachment;
char mail[3076]={0};

void boundary(char *head,char *des_boundary,int length)
{
    char tmphead[2048]={0};
   	char *split="\r\n";
	char *row;
	int rowlength=0;
	char *boundarystring;
	snprintf(tmphead,sizeof(tmphead),"%s",head);
	row=strtok(tmphead,split);	
	while(row)
	{  
	   if(strstr(row,"boundary="))
	   {
	        rowlength=strlen(row)+1;
			boundarystring=(char*)malloc(rowlength);
	        sscanf(row,"%*[^\"]\"%[^\"]\"",boundarystring);
			snprintf(des_boundary,length,"%s",boundarystring);
			free(boundarystring);
			break;
	   }
	   row=strtok(NULL,split);
	}
}

char *matchtextmail(char *source,char *destination,char *bunda)
{
	char *tmp=NULL;
	char *tmp7bit=NULL;
	char destinationdecode[650];
	char dest_mailtext[650];
	char boundary_string[50]={0};
	char bundary_use[8];
	int i=0;
	if(!source)
	{
	    char *emptysource="null";
	    memset(destination,emptysource,strlen(emptysource));
		destination[4]='\0';
		return destination;
	}
	//printf("source=%s\n\n",source);
	snprintf(bundary_use,sizeof(bundary_use),"--%s",bunda);
	memset(destinationdecode,0,sizeof(destinationdecode));
	tmp=strstr(source,"Content-Transfer-Encoding: ");
	if(tmp)
	{
	    //找到开始位置
	    tmp += 26; 
		while(*tmp == ' ')
		{
		    tmp++;
		}
		if(strncasecmp(tmp,"base64",6)==0)
        {
            //找到base64编码的位置
	        tmp += 6;
			while(*tmp == ' ')
			{
			    tmp++;
		    }
			//找到结束位置
			if((strstr(tmp,bundary_use)))
		    {
		        int length=strlen(tmp)-strlen(strstr(tmp,bundary_use));
				if(length>648)
				{
			        memcpy(destinationdecode,tmp,sizeof(destinationdecode)-1);
					destinationdecode[sizeof(destinationdecode)-1]='\0';
				}
				else
				{
				    memcpy(destinationdecode,tmp,length);
					destinationdecode[length+1]='\0';
				}
		    }
			else
			{
			    if(strlen(tmp)>650)
		    	{
		    	    memcpy(destinationdecode,tmp,sizeof(destinationdecode));
					destinationdecode[sizeof(destinationdecode)-1]='\0';
		    	}
				else
				{
					memcpy(destinationdecode,tmp,strlen(tmp));
					destinationdecode[strlen(tmp)]='\0';
				}
			}
			int j=0;
			for(i=0;i<strlen(destinationdecode);i++)
			{
			    if((destinationdecode[i]!='\r')&&(destinationdecode[i]!='\n')&&(destinationdecode[i]!=' '))
			    {
			        dest_mailtext[j]=destinationdecode[i];
					j++;
			    }
			}
			dest_mailtext[j]='\0';
			memset(tmp,0,strlen(tmp));
			Base64Decode(tmp,dest_mailtext,strlen(dest_mailtext)+1);
			memcpy(destination,tmp,strlen(tmp));
			destination[strlen(tmp)]='\0';
			return destination;
    	}
		else if(strncasecmp(tmp,"7bit",4)==0)
	    {   
	        tmp += 4;
			while(*tmp==' ')
				tmp++;
			if(NULL != ( tmp7bit = strstr(tmp,bundary_use)))
			{
			    if((strlen(tmp)-strlen(tmp7bit))<650)
		    	{
					memcpy(destination,tmp,strlen(tmp)-strlen(tmp7bit));
					destination[strlen(tmp)-strlen(tmp7bit)]='\0';
			    }
				else
				{
				    memcpy(destination,tmp,sizeof(destinationdecode)-1);
					destination[sizeof(destinationdecode)-1]='\0';
				}
			}
			else if( NULL != ( tmp7bit = strrchr(tmp,'}') ) )
			{
			    tmp7bit += 2;
				if(strlen(tmp7bit)<650)
				{
				    memcpy(destination,tmp7bit,strlen(tmp7bit));
				    destination[strlen(tmp7bit)]='\0';
				}
				else
				{
				    memcpy(destination,tmp7bit,sizeof(destinationdecode)-1);
					destination[sizeof(destinationdecode)-1]='\0';	
				}
			} 
			else
		 	{
		  	    if(strlen(tmp)<650)
	 	    	{
				    memcpy(destination,tmp,strlen(tmp));
				    destination[strlen(tmp)]='\0';
	 	    	}
				else
				{ 
					memcpy(destination,tmp,sizeof(destinationdecode));
					destination[sizeof(destinationdecode)-1]='\0';		
				}
			}
			return destination;
	    }
		else if(strncasecmp(tmp,"8bit",4)==0)
		{
		    tmp += 4;
			while(*tmp == ' ')
			{
			    tmp++;
		    }
			if(NULL!=(tmp7bit=strstr(tmp,bundary_use)))
		    {
			    if((strlen(tmp)-strlen(tmp7bit))<650)
		    	{
					memcpy(destination,tmp,strlen(tmp)-strlen(tmp7bit));
					destination[strlen(tmp)-strlen(tmp7bit)]='\0';
			    }
				else
				{
				    memcpy(destination,tmp,sizeof(destinationdecode)-1);
					destination[sizeof(destinationdecode)-1]='\0';
				}

		    }
			else
		 	{
			 	if(strlen(tmp)<650)
	 	    	{
				    memcpy(destination,tmp,strlen(tmp));
				    destination[strlen(tmp)]='\0';
		 	  	}
				else
				{ 
					memcpy(destination,tmp,sizeof(destinationdecode));
					destination[sizeof(destinationdecode)-1]='\0';
				}
			}	
			return destination;	
		}
		else if(strstr(tmp,"quoted-printable"))
	    { 
	        tmp += 16; 
			while( *tmp == ' ' )
			{
			    tmp++;
			}
			if(NULL!=(tmp7bit=strstr(tmp,bundary_use)))
		    {
		        if((strlen(tmp)-strlen(tmp7bit))<650)
		    	{
					memcpy(destination,tmp,strlen(tmp)-strlen(tmp7bit));
					destination[strlen(tmp)-strlen(tmp7bit)]='\0';
			    }
				else
				{
				    memcpy(destination,tmp,sizeof(destinationdecode)-1);
					destination[sizeof(destinationdecode)-1]='\0';
				}
		    }
			else
		 	{
			 	if(strlen(tmp)<650)
		 		{
				    memcpy(destination,tmp,strlen(tmp));
				    destination[strlen(tmp)]='\0';
		 		}
				else
				{
					memcpy(destination,tmp,sizeof(destinationdecode));
					destination[sizeof(destinationdecode)-1]='\0';
				}
			}
			return destination;
	    }
    }
	else
	{
	    if(strlen(source)<650)
    	{
			memcpy(destination,source,strlen(source));
			destination[strlen(source)]='\0';
    	}
		else
		{
			memcpy(destination,source,sizeof(destinationdecode));
		    destination[sizeof(destinationdecode)-1]='\0';
		}
		//snprintf(destination,sizeof(destinationdecode),"%s",source);
	    return destination;
	} 
}
int DectEmailTextFilter(char *pcParaValue, char *pcSubText)
{
    char    acSubText[EMAIL_TEXT_MAXLEN];
    char    *pcElement = NULL;
    char    *pcElementStart = NULL;
    char    *pcElementEnd = NULL;
    int     iHtmlLabel = 0;
    int     iHtmlLabelStart = 0;
    int     iStrlen = 0;
    int     iLabelStrlen = 0;
    int     iIdx = 0;
    int     iIdx2 = 0;
    int     iIdx3 = 0;    
    char    acOneLabel[HTMLLABEL_MAXLEN];   
    char    acHtmlLabel[HTMLLABEL_MAXNUM][HTMLLABEL_MAXLEN] = {"<font", "<div", "<br/>", "<br>", "<html", "</", "&nbsp;", "&amp;", "<span", "<img", "<head", "<title", "<link", "<abbr", "<address", "<big", 
                "<body", "<button", "<col", "<form", "<frame", "<hr", "<input", "<label", "<option", "<select", "<table", "<!--", "<!DOCTYPE", "<acronym", "<applet", "<area", "<h1", "<h2", "<h3", "<h4", 
                "<base", "<basefont", "<bdo", "<blockguote", "<caption", "<center", "<cite", "<code", "<colgroup", "<dd", "<del", "<dir", "<dfn", "<dl", "<dt", "<em", "<fieldset", "<frameset", "<caption", 
                "<h5", "<h6", "<head", "<iframe", "<ins", "<isindex", "<kbd", "<legend", "<li", "<map", "<menu", "<meta", "<noframes", "<noscript", "<object", "<ol", "<optgroup", "<param", "<pre", 
                "<same", "<script", "<small", "<strike", "<strong", "<style", "<sub", "<sup", "<tbody", "<td", "<textarea", "<tfoot", "<th", "<thead", "<title", "<tr", "<tt", "<u", "<ul", "<var", "<xmp", 
                "<p", "<b", "<a", "<i", "<q", "<s"};
    
    if (NULL == pcParaValue || NULL == pcSubText)
    {
        return -1;
    }
    
    memset(acSubText, 0, sizeof(acSubText));
    snprintf(acSubText, sizeof(acSubText), "%s", pcParaValue);    

    iStrlen = strlen(acSubText);
    while (iIdx < iStrlen)
    {    
        pcElement = &(acSubText[iIdx]);
        
        iHtmlLabel = 0;
        iHtmlLabelStart = 0;
        if ('<' == acSubText[iIdx] || '&' == acSubText[iIdx])
        {
            /*判断是否是html标签的开始部分*/
            for (iIdx3=0; iIdx3<HTMLLABEL_MAXNUM; iIdx3++)
            {
                iLabelStrlen = strlen(acHtmlLabel[iIdx3]);
                memset(acOneLabel, 0, sizeof(acOneLabel));
                snprintf(acOneLabel, iLabelStrlen + 1, "%s", pcElement);

                if (0 == strcmp(acOneLabel, acHtmlLabel[iIdx3]))
                {
                    iHtmlLabelStart = 1;
                    break;
                }
            }
            
            if (1 == iHtmlLabelStart)
            {
                /*如果是html标签的开始部分，则寻找结束部分*/
                pcElementStart = &(acSubText[iIdx]);            
                if (0 == strcmp(acOneLabel, "&nbsp;"))
                {                    
                    snprintf(pcElementStart, iStrlen - iIdx, " %s", pcElementStart+6);
                    iIdx = 0;
                    iStrlen = strlen(acSubText);
                    continue;
                }
                
                if (0 == strcmp(acOneLabel, "&amp;"))
                {                    
                    snprintf(pcElementStart, iStrlen - iIdx, "&%s", pcElementStart+5);
                    iIdx = 0;
                    iStrlen = strlen(acSubText);
                    continue;
                }
                
                if (0 == strcmp(acOneLabel, "<br/>"))
                {                    
                    snprintf(pcElementStart, iStrlen - iIdx, "\n%s", pcElementStart+5);
                    iIdx = 0;
                    iStrlen = strlen(acSubText);
                    continue;
                }
                
                if (0 == strcmp(acOneLabel, "<br>"))
                {                    
                    snprintf(pcElementStart, iStrlen - iIdx, "\n%s", pcElementStart+4);
                    iIdx = 0;
                    iStrlen = strlen(acSubText);
                    continue;
                }
                
                for (iIdx2=iIdx; iIdx2<iStrlen; iIdx2++)
                {
                    if (('<' == *pcElementStart && '>' == acSubText[iIdx2]))
                    {                      
                        pcElementEnd = &(acSubText[iIdx2]);                    
                        snprintf(pcElementStart, iStrlen - iIdx, "%s", pcElementEnd+1);
                        iIdx = 0;
                        iHtmlLabel = 1;
                        break;
                    }
                }
                
                if (iStrlen == iIdx2)
                {
                    iIdx++;
                }
                
                iStrlen = strlen(acSubText);
                
            }
            
            if (0 == iHtmlLabel)
            {
                /*没有找到标签结束标志，则非html标签*/
                iIdx++;
            }
        }
        else
        {
            /*非html标签*/
            iIdx++;
        }
    }
    
    snprintf(pcSubText, EMAIL_TEXT_MAXLEN, "%s", acSubText);

    return 0;
}

//判断是否存在html文件
int htmltag(char *source)
{
    char tmp[650]={0};
    char *html=NULL;
    int flag=0;
	//snprintf(tmp,sizeof(tmp),"%s",source);
	//printf("1959 source=%s\n\n",tmp);
    html=(strstr(tmp,"<html"))||(strstr(tmp,"<HTML"));
	//printf("1961 html=%x,%s\n\n",html,html);
    if(html)
    { 
        flag=1;
    }
    return flag;
}

//去除标签//去除html之后的内容
char *stripHTMLtag(char *source,char *destination)
{
	int i,j,k,tag = 0;
	char *tmpsource=source;
	char tmpdestination[650]={0};    
	char acDeststr[EMAIL_TEXT_MAXLEN];
	char acDeststr2[EMAIL_TEXT_MAXLEN];
	char tmp[650]={0};
	char tmphtml[650]={0};	
    int  count=0;//统计连续出现<的个数
	char *tmp_style=NULL;
	int htmlfalg=0;
	i = j = 0;
	k=1;
    memset(acDeststr, 0, sizeof(acDeststr));
    memset(acDeststr2, 0, sizeof(acDeststr2));
	if(!source || !destination)
	{
		printf("%s %s %d source=%x, destination=%x\n", __FUNCTION__, __FILE__, __LINE__, source, destination);
	}

	if(tmpsource)
	{
	    htmlfalg=htmltag(tmpsource);
		//printf("%s %s %d htmlfalg=%s\n", __FUNCTION__, __FILE__, __LINE__, htmlfalg);
		if(htmlfalg)
		{
            tmp_style=(strstr(tmpsource,"<style"))||(strstr(tmpsource,"<STYLE"));
			//printf("%s %s %d tmp_style=%s\n", __FUNCTION__, __FILE__, __LINE__, tmp_style);

            if(tmp_style)
           	{
           	    if(strlen(tmpsource)-strlen(tmp_style)<649)
           	    {
           	        strncpy(tmpdestination,tmpsource,strlen(tmpsource)-strlen(tmp_style));
           	    }
				else
				{
				    strncpy(tmpdestination,tmpsource,649);
				}

				tmp_style=(strstr(tmpsource,"</style>"))||(strstr(tmpsource,"</STYLE>"));
				if(tmp_style)
				{
				     tmp_style += 8;
					 if(tmp_style)
					 {
					     if((strlen(tmpdestination)+strlen(tmp_style)) < (sizeof(tmpdestination)-1))
				     	 {
					         strcat(tmpdestination,tmp_style);
						 }
						 else
						 {
						     strncat(tmpdestination,tmp_style,(sizeof(tmpdestination)-strlen(tmpdestination)-1));
						 }
				 	 }
				}
				//printf("%s %s %d tmpdestination=%s\n", __FUNCTION__, __FILE__, __LINE__, tmpdestination);
           	}
			else
			{
				snprintf(tmpdestination,sizeof(tmpdestination),"%s",tmpsource);
			}

			tmp_style=(strstr(tmpdestination,"<script"))||(strstr(tmpdestination,"<SCRIPT"));
			if(tmp_style)
			{
			    if(strlen(tmpdestination)-strlen(tmp_style)<649)
			    {
			        strncpy(tmp,tmpdestination,strlen(tmpdestination)-strlen(tmp_style));
			    }
				else
				{
				    strncpy(tmp,tmpdestination,649);
				}
				tmp_style=(strstr(tmpdestination,"</script>"))||(strstr(tmpdestination,"</SCRIPT>"));
				if(tmp_style)
				{
					 tmp_style += 9;
					 if(tmp_style)
					 {
						 if((strlen(tmp)+strlen(tmp_style)) < (sizeof(tmp)-1))
						 {
							 strcat(tmp,tmp_style);
						 }
						 else
						 {
							 strncat(tmp,tmp_style,(sizeof(tmp)-1-strlen(tmp)));
						 }
					 }
				}
			}
			else
			{
				snprintf(tmp ,sizeof(tmp),"%s",tmpdestination);
			}
			
            memset(tmpdestination,0,sizeof(tmpdestination));
			tmphtml[0]='<';
			while (tmp[i]) 
			{
			    //在有<的情况下，临时保存<后面的内容
			    if(tag)
				{
				    tmphtml[k] = tmp[i];
					k++;
				}
				//检测<
				if (tmp[i] == '<')
				{
				    count++;
					if(count == 2)
				    {
				        int length= strlen(tmpdestination)+strlen(tmphtml);//处理中间存在<的情况
				        
				        strncat( tmpdestination,tmphtml,( length < sizeof(tmpdestination)-1 )?strlen(tmphtml):( sizeof(tmpdestination)-1-strlen(tmpdestination) ) );
                        memset(tmphtml,0,sizeof(tmphtml)); 
						tmphtml[0]='<';
					    k=1;
						count=0;
					}
					tag = 1;
				}
				
				if (!tag)
					tmpdestination[j++] = tmp[i];
				
				if (tmpsource[i] == '>')
				{
					tag = 0;          
					memset(tmphtml,0,sizeof(tmphtml)); 
					tmphtml[0]='<';
					k=1;
				}
				i++;
			}
			memset(tmp,0,sizeof(tmp));
			snprintf(acDeststr,sizeof(tmpdestination),"[This message maybe different with the real mail] %s%s",tmpdestination,tmphtml);
			//printf("%s %s %d destination=%s\n", __FUNCTION__, __FILE__, __LINE__, destination);
		}
		else
		{
            snprintf(acDeststr,sizeof(tmpdestination),"%s",tmpsource);
		}
	}
	else
	{
	    snprintf(acDeststr,sizeof(tmp),"%s","null");
	}
    DectEmailTextFilter(acDeststr, acDeststr2);
    
    snprintf(destination, EMAIL_TEXT_MAXLEN, "%s", acDeststr2);
    
	return destination;
}

//针对imap，imap读取邮件正文的时候先读取的是头部信息，接着是正文，所以先将头部去掉，存储下正文，再对正文进行处理
char *body(char *head,char *head_body,char *destination)
{
      int head_length=0;
      int head_body_length=0;
      char *tmp_head=head;
      char *tmp_head_body=head_body;
      char body[1024]={0};
	  char *row;//指定当前的行
	  char *split="\r\n";
	  int num1=0;
	  int num2=0;
	  int headline=0;

      if((NULL==tmp_head_body)||(tmp_head==NULL))
      {
          strncpy(body,"null",4);
		  body[strlen(body)]='\0';
          memcpy(destination,body,strlen(body));
		  destination[strlen(body)]='\0';
          return destination;
      }
      else
      {
          head_length=strlen(tmp_head);
          head_body_length=strlen(head_body);
          if(head_body_length <= head_length)
          {
              strncpy(body,"null",4);
		      body[strlen(body)]='\0';
              memcpy(destination,body,strlen(body));
			  destination[strlen(body)]='\0';
              return destination;
          }
          else
          {
              body[0]='\0';
			  headline=headerline(tmp_head);
			  row=strtok(tmp_head_body,split);	
			  while(row)
			  {  
				   num1++;
				   if( (num1 > headline) && (strlen(body)<sizeof(body)-1-strlen(row)))
				   {
				       num2++;
					   strncat(body, row, 1023-strlen(body));
				   }
				   row=strtok(NULL,split);
			  }
              body[strlen(body)]='\0';
			  
              memcpy(destination,body,strlen(body));
			  destination[strlen(body)]='\0';
              return destination;
          }
      }
}

int readbody(int sock, struct query *ctl, flag forward, int len)
/** read and dispose of a message body presented on \a sock */
/** \param ctl		query control record */
/** \param sock		to which the server is connected */
/** \param forward	TRUE to forward */
/** \param len		length of message */
{
    int	linelen;
    char buf[MSGBUFSIZE+4];
    char *inbufp = buf;
    flag issoftline = FALSE;
	int length=0;
	int k=0;
    /*
     * Pass through the text lines in the body.
     *
     * Yes, this wants to be ||, not &&.  The problem is that in the most
     * important delimited protocol, POP3, the length is not reliable.
     * As usual, the problem is Microsoft brain damage; see FAQ item S2.
     * So, for delimited protocols we need to ignore the length here and
     * instead drop out of the loop with a break statement when we see
     * the message delimiter.
     */
    memset(mail,0,sizeof(mail)); 
    while ( protocol->delimited || len > 0 )
    {
		set_timeout(mytimeout);
		
		/* XXX FIXME: for undelimited protocols that ship the size, such
		 * as IMAP, we might want to use the count of remaining characters
		 * instead of the buffer size -- not for fetchmail 6.3.X though */
		 
		if ((linelen = SockRead(sock, inbufp, sizeof(buf)-4-(inbufp-buf)))==-1)
		{
		    set_timeout(0);
	        release_sink(ctl);
	        return(PS_SOCKET);
		}

		set_timeout(0);
		/* write the message size dots */
		if (linelen > 0)
		{
		    print_ticker(&sizeticker, linelen);
		}

		/* Mike Jones, Manchester University, 2006:
		 * "To fix IMAP MIME Messages in which fetchmail adds the remainder of
		 * the IMAP packet including the ')' character (part of the IMAP)
		 * Protocol causing the addition of an extra MIME boundary locally."
		 *
		 * However, we shouldn't do this for delimited protocols:
		 * many POP3 servers (Microsoft, qmail) goof up message sizes
		 * so we might end truncating messages prematurely.
		 */
		if (!protocol->delimited && linelen > len) {
		    /* FIXME: HACK ALERT! This \r\n is only here to make sure the
		     * \n\0 hunt works later on. The \n generated here was not
		     * part of the original message!
		     * The real fix will be to use buffer + length strings,
		     * rather than 0-terminated C strings. */
		    inbufp[len++] = '\r';
		    inbufp[len++] = '\n';
		    inbufp[len] = '\0';
		    linelen = len;
		}

		len -= linelen;
		length += linelen;
			
		/* check for end of message */
		if (protocol->delimited && *inbufp == '.')
		{
		    if (EMPTYLINE(inbufp+1))
			break;
		    else
			msgblk.msglen--;	/* subtract the size of the dot escape */
		}

		msgblk.msglen += linelen;
		if (ctl->mimedecode && (ctl->mimemsg & MSG_NEEDS_DECODE)) 
		{
		    issoftline = UnMimeBodyline(&inbufp, protocol->delimited, issoftline);
		    if (issoftline && (sizeof(buf)-1-(inbufp-buf) < 200))
		    {
			/*
			 * Soft linebreak, but less than 200 bytes left in
			 * input buffer. Rather than doing a buffer overrun,
			 * ignore the soft linebreak, NL-terminate data and
			 * deliver what we have now.
			 * (Who writes lines longer than 2K anyway?)
			 */
			*inbufp = '\n'; *(inbufp+1) = '\0';
			issoftline = 0;
		    }
		}
		/* ship out the text line */
		if (forward && (!issoftline))
		{
		    int	n=0;
		    inbufp = buf;
		    /* guard against very long lines */
		    buf[MSGBUFSIZE+1] = '\r';
		    buf[MSGBUFSIZE+2] = '\n';
		    buf[MSGBUFSIZE+3] = '\0';

		    n = stuffline(ctl, buf);
			
			if(strlen(buf)>1)
			{
			    int j=0;
				for(j=0;j<strlen(buf);j++)
				if(strlen(mail)<3076)
				{
					mail[k]=buf[j];
					k++;
				}
			}

		    if (n < 0)
		    {
				report(stdout, GT_("error writing message text\n"));
				release_sink(ctl);
				return(PS_IOERR);
		    }
		    
		    else if (want_progress())
		    {
				fputc('*', stdout);
				fflush(stdout);
		    }	
		}	
		//检测到了附件 标志 或者是正文部分大于5k，就退出，这样既可以处理图片问题，也可以处理背景音乐的问题
		if(strstr(buf,"Disposition: attachment"))
		{
		 	attachment=1;
			int shutreturn;
		    shutreturn=shutdown(sock,2);//对于面向连接的协议，建议先调用 Shutdown，然后再调用 Close 方法
            if(!shutreturn)
        	{
            	//printf("[shutdown success shutreturn=%d]\n\n",shutreturn);
        	}
			else
			{
				//printf("[shutdown failed]\n\n");
			}
			//如果附件超过1M这断开socket ;
			close(sock);
			int  bDontLinger=FALSE;//SO_LINGER 关闭间隔:零 关闭方式:强制 等待关闭与否:否
			setsockopt(sock,SOL_SOCKET, SO_LINGER,(const char*)&bDontLinger, sizeof(int));
		}
	}	
	char tmpmailtext[650];
	memset(tmpmailtext,0,sizeof(tmpmailtext));
	//去除头部后，最大只存储1024字节的数据
	char dropheader[1024]={0};
	//获取去除头部信息的内容

	char boundy[6]={0};
	boundary(header,boundy,sizeof(boundy));
	if(ctl->server.protocol == P_IMAP)
	{
	    body(header,mail,dropheader);
		//获取imap正文部分的内容
		matchtextmail(dropheader,tmpmailtext,boundy);
	}
	else
	{
	    //获取pop3正文部分的内容
	    matchtextmail(mail,tmpmailtext,boundy);
	}
	//取消\r\n
	memset(mailtext,0,sizeof(mailtext));
	stripHTMLtag(tmpmailtext,mailtext);
	memset(dropheader,0,sizeof(dropheader));
	int h=0;
	int m=0;
	
	for(h=0;h<strlen(mailtext);h++)
	{
	    if((mailtext[h]!='\r')&&(mailtext[h]!='\n'))
	    {
			dropheader[m]=mailtext[h];
		    m++;
	    }
	}
	dropheader[m]='\0';
	memset(mailtext,0,sizeof(mailtext));
	snprintf(mailtext,sizeof(mailtext),"%s",dropheader);

	memset(dropheader,0,sizeof(dropheader));
	//只针对标记base64在邮件头部的imap和所有的pop3 base64 信件
	if(pop3_base64)
	{
	    char mailtext_tmp[650];
	    memset(mailtext_tmp,0,sizeof(mailtext_tmp));
	    Base64Decode(mailtext_tmp,mailtext,strlen(mailtext));
	    memset(mailtext,0,sizeof(mailtext));
	    snprintf(mailtext,sizeof(mailtext),"%s",mailtext_tmp);
	    memset(mailtext_tmp,0,sizeof(mailtext_tmp));
	} 
	return(PS_SUCCESS);
}

void init_transact(const struct method *proto)
/** initialize state for the send and receive functions */
{
    suppress_tags = FALSE;
    tagnum = 0;
    tag[0] = '\0';	/* nuke any tag hanging out from previous query */
    protocol = proto;
    shroud[0] = '\0';
}

/** shroud a password in the given buffer */
static void enshroud(char *buf)
{
    char *cp=NULL;

    if (shroud[0] && (cp = strstr(buf, shroud)))
    {
       char    *sp=NULL;

       sp = cp + strlen(shroud);
       *cp++ = '*';
       while (*sp)
           *cp++ = *sp++;
       *cp = '\0';
    }
}

#if defined(HAVE_STDARG_H)
/** assemble command in printf(3) style and send to the server */
void gen_send(int sock, const char *fmt, ... )
#else
void gen_send(sock, fmt, va_alist)
int sock;		/** socket to which server is connected */
const char *fmt;	/** printf-style format */
va_dcl
#endif
{
    char buf [MSGBUFSIZE+1];
    va_list ap;

    if (protocol->tagged && !suppress_tags)
        snprintf(buf, sizeof(buf) - 2, "%s ", GENSYM);
    else
	buf[0] = '\0';

#if defined(HAVE_STDARG_H)
    va_start(ap, fmt);
#else
    va_start(ap);
#endif
    vsnprintf(buf + strlen(buf), sizeof(buf)-2-strlen(buf), fmt, ap);
    va_end(ap);

    snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "\r\n");
    SockWrite(sock, buf, strlen(buf));

    if (outlevel >= O_MONITOR)
    {
	enshroud(buf);
	buf[strlen(buf)-2] = '\0';
	report(stdout, "%s> %s\n", protocol->name, buf);
    }
}

/** get one line of input from the server */
int base64=0;
int WinWebMail=0;
int gen_recv(int sock  /** socket to which server is connected */,
	     char *buf /** buffer to receive input */,
	     int size  /** length of buffer */)
{
    size_t n;
    int oldphase = phase;	/* we don't have to be re-entrant */
    phase = SERVER_WAIT;
    set_timeout(mytimeout);
    if (SockRead(sock, buf, size) == -1)
    {
		set_timeout(0);
		phase = oldphase;
		if(is_idletimeout())
		{
		    resetidletimeout();
		    return(PS_IDLETIMEOUT);
		}
		else
		    return(PS_SOCKET);
    }
    else
    {
	    if(strstr(buf,"WinWebMail"))
	    {
			WinWebMail=1;
	    }
		set_timeout(0);
		n = strlen(buf);
		if (n > 0 && buf[n-1] == '\n')
		    buf[--n] = '\0';
		if (n > 0 && buf[n-1] == '\r')
		    buf[--n] = '\0';
		if (outlevel >= O_MONITOR)
		    report(stdout, "%s< %s\n", protocol->name, buf);
		phase = oldphase;
		return(PS_SUCCESS);
    }
}

/** \addtogroup gen_recv_split
 * @{
 * gen_recv_split() splits the response from a server which is too
 * long to fit into the buffer into multiple lines. If the prefix is
 * set as "MY FEATURES" and the response from the server is too long
 * to fit in the buffer, as in:
 *
 *   "MY FEATURES ABC DEF GHI JKLMNOPQRS TU VWX YZ"
 *
 * Repeated calls to gen_recv_split() may return:
 *
 *   "MY FEATURES ABC DEF GHI"
 *   "MY FEATURES JKLMNOPQRS"
 *   "MY FEATURES TU VWX YZ"
 *
 * A response not beginning with the prefix "MY FEATURES" will not be
 * split.
 *
 * To use:
 * - Declare a variable of type struct RecvSplit
 * - Call gen_recv_split_init() once
 * - Call gen_recv_split() in a loop, preferably with the same buffer
 *   size as the "buf" array in struct RecvSplit
 */

static void overrun(const char *f, size_t l) __attribute__((noreturn));

/** Internal error report function. If this happens, the calling site
 * needs to be adjusted to set a shorter prefix, or the prefix capacity
 * needs to be raised in struct RecvSplit. */
static void overrun(const char *f, size_t l)
{
    report(stderr, GT_("Buffer too small. This is a bug in the caller of %s:%lu.\n"), f, (unsigned long)l);
    abort();
}

/** Initialize \a rs for later use by gen_recv_split. */
void gen_recv_split_init (const char *prefix /** prefix to match/repeat */,
	struct RecvSplit *rs /** structure to be initialized */)
{
    if (strlcpy(rs->prefix, prefix, sizeof(rs->prefix)) > sizeof(rs->prefix))
	overrun(__FILE__, __LINE__);
    rs->cached = 0;
    rs->buf[0] = '\0';
}

/** Function to split replies at blanks, and duplicate prefix.
 * gen_recv_split_init() must be called before this can be used. */
int gen_recv_split(int sock  /** socket to which server is connected */,
	     char *buf /** buffer to receive input */,
	     int size  /** length of buffer, must be the same for all calls */,
	     struct RecvSplit *rs /** cached information across calls */)
{
    size_t n = 0;
    int foundnewline = 0;
    char *p;
    int oldphase = phase;	/* we don't have to be re-entrant */

    assert(size > 0);

    /* if this is not our first call, prepare the buffer */
    if (rs->cached)
    {
	/*
	 * if this condition is not met, we lose data
	 * because the cached data does not fit into the buffer.
	 * this cannot happen if size is the same throughout all calls.
	 */
	assert(strlen(rs->prefix) + strlen(rs->buf) + 1 <= (size_t)size);

	if ((strlcpy(buf, rs->prefix, size) >= (size_t)size)
		|| (strlcat(buf, rs->buf, size) >= (size_t)size)) {
	    overrun(__FILE__, __LINE__);
	}

	n = strlen(buf);
	/* clear the cache for the next call */
	rs->cached = 0;
	rs->buf[0] = '\0';
    }

    if ((size_t)size > n) {
	int rr;

	phase = SERVER_WAIT;
	set_timeout(mytimeout);
	rr = SockRead(sock, buf + n, size - n);
	set_timeout(0);
	phase = oldphase;
	if (rr == -1)
	    return PS_SOCKET;
    }

    n = strlen(buf);
    if (n > 0 && buf[n-1] == '\n')
    {
	buf[--n] = '\0';
	foundnewline = 1;
    }
    if (n > 0 && buf[n-1] == '\r')
	buf[--n] = '\0';

    if (foundnewline				/* we have found a complete line */
	|| strncasecmp(buf, rs->prefix, strlen(rs->prefix))	/* mismatch in prefix */
	|| !(p = strrchr(buf, ' '))		/* no space found in response */
	|| p < buf + strlen(rs->prefix))	/* space is at the wrong location */
    {
	if (outlevel >= O_MONITOR)
	    report(stdout, "%s< %s\n", protocol->name, buf);
	return(PS_SUCCESS);
    }

    /* we are ready to cache some information now. */
    rs->cached = 1;
    if (strlcpy(rs->buf, p, sizeof(rs->buf)) >= sizeof(rs->buf)) {
	overrun(__FILE__, __LINE__);
    }
    *p = '\0'; /* chop off what we've cached */
    if (outlevel >= O_MONITOR)
	report(stdout, "%s< %s\n", protocol->name, buf);
    if (outlevel >= O_DEBUG)
	report(stdout, "%s< %s%s...\n", protocol->name, rs->prefix, rs->buf);
    return(PS_SUCCESS);
}
/** @} */

#if defined(HAVE_STDARG_H)
int gen_transact(int sock, const char *fmt, ... )
#else
int gen_transact(int sock, fmt, va_alist)
int sock;		/** socket to which server is connected */
const char *fmt;	/** printf-style format */
va_dcl
#endif
/** assemble command in printf(3) style, send to server, fetch a response */
{
    int ok;
    char buf [MSGBUFSIZE+1];
    va_list ap;
    int oldphase = phase;	/* we don't have to be re-entrant */
    phase = SERVER_WAIT;

    if (protocol->tagged && !suppress_tags)
	snprintf(buf, sizeof(buf) - 2, "%s ", GENSYM);
    else
	buf[0] = '\0';

#if defined(HAVE_STDARG_H)
    va_start(ap, fmt) ;
#else
    va_start(ap);
#endif
    vsnprintf(buf + strlen(buf), sizeof(buf)-2-strlen(buf), fmt, ap);
    va_end(ap);
    snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "\r\n");
    ok = SockWrite(sock, buf, strlen(buf));
    if (ok == -1 || (size_t)ok != strlen(buf)) {
	/* short write, bail out */
	return PS_SOCKET;
    }

    if (outlevel >= O_MONITOR)
    {
	enshroud(buf);
	buf[strlen(buf)-2] = '\0';
	report(stdout, "%s> %s\n", protocol->name, buf);
    }
    /* we presume this does its own response echoing */
    ok = (protocol->parse_response)(sock, buf);
    phase = oldphase;
    return(ok);
}

/* transact.c ends here */
