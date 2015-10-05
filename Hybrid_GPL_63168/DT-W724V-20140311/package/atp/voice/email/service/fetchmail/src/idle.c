/*****************************************************************************

NAME:
   idle.c -- code for interruptible delays without sleep(3).

ENTRY POINTS:
   interruptible_idle() -- delay for some time, interruptible by signal.

THEORY:
   Sometimes you need more than one time delay per program, so alarm(3)
won't cut it.  This code illustrates time delays with select(2).

AUTHOR:
   Eric S. Raymond <esr@thyrsus.com>, 1997.  This source code example
is part of fetchmail and the Unix Cookbook, and are released under the
MIT license.  Compile with -DMAIN to build the demonstrator.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fetchmail.h>	/* for ROOT_UID */
#include "msgapi.h"
#include "atputil.h"
#include "sqlite3.h"
#include "httpapi.h"
#include "voicedb.h"
#include "voicepub.h"


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

volatile int lastsig;		/* last signal received */

#ifdef SLEEP_WITH_ALARM
/*
 * The function of this variable is to remove the window during which a
 * SIGALRM can hose the code (ALARM is triggered *before* pause() is called).
 * This is a bit of a kluge; the real right thing would use sigprocmask(),
 * sigsuspend().  This workaround lets the interval timer trigger the first
 * alarm after the required interval and will then generate alarms
 * seconds until it is certain that the critical section (ie., the window)
 * is exited.
 */
static sig_atomic_t	alarm_latch = FALSE;

RETSIGTYPE gotsigalrm(int sig)
{
    set_signal_handler(sig, gotsigalrm);
    lastsig = sig;
    alarm_latch = TRUE;
}
#endif /* SLEEP_WITH_ALARM */

#ifdef __EMX__
/* Various EMX-specific definitions */
static int itimerflag;
extern struct addrinfo *ai0, *ai1;

void itimerthread(void* dummy)
{
    if (outlevel >= O_VERBOSE)
	report(stderr, 
	       GT_("fetchmail: thread sleeping for %d sec.\n"), poll_interval);
    while(1)
    {	
        _sleep2(poll_interval*1000);
        kill((getpid()), SIGALRM);
    }
}
#endif

int scanagain=0;
int delmail=0;

void del_imap_mail(int sock,int mailnumber)
{
    int ok;
	
	ok=gen_transact(sock, "STORE %d +FLAGS(\\Seen \\Deleted)", mailnumber);

	ok=gen_transact(sock, "EXPUNGE");

	if(ok==0)
	{

		ok=gen_transact(sock, "LOGOUT");
        if(ok==0)
		{
			printf("[delete success.]\n");
    	}
	}
	else
	{
	    printf("[delete failed.]\n");
	}
}
void sign_imap_mail(int sock,long long mailuid)
{
    int ok;
	ok=gen_transact(sock, "UID STORE %lld +FLAGS (\\Seen)",mailuid);//uid

	if(ok==0)
	{
		ok=gen_transact(sock, "LOGOUT");
        if(ok==0)
		{
			printf("[sign Seen success.]\n");
    	}
	}
	else
	{
	    printf("[sign Seen failed.]\n");
	}
}


static int do_authcert (int sock, const char *command, const char *name)
/* do authentication "external" (authentication provided by client cert) */
{
    char buf[256];

    if (name && name[0])
    {
        size_t len = strlen(name);
        if ((len / 3) + ((len % 3) ? 4 : 0)  < sizeof(buf))
            to64frombits (buf, name, strlen(name));
        else
            return PS_AUTHFAIL; /* buffer too small. */
    }
    else
        buf[0]=0;
    return gen_transact(sock, "%s EXTERNAL %s",command,buf);
}


void imap_login(int sock,char *server,char *account,char *password,char *protocol,int number,int flag)
{
	int ok = 0;

	if ((ok = gen_transact(sock, "CAPABILITY")) == PS_SUCCESS)
	{
		if(sock!=-1)
		{
		    gen_transact(sock, "LOGIN \"%s\" \"%s\"", account, password);
			sleep(1);
			gen_transact(sock, "SELECT \"%s\"","INBOX");
			sleep(1);
			printf("[ok login.]\n");
			if(flag==0)
			{
				del_imap_mail(sock,number);
			}
			if(flag==1)
			{
			   sign_imap_mail(sock,number);
			}
		}
	}
	else
	{
	    printf("[socket create failed.]\n");
	}
}

void del_pop3_mail(int sock,int mailnumber)
{
   int ok;
 

   ok=gen_transact(sock, "DELE %d", mailnumber);
   if(ok==0)
   {
 
       ok = gen_transact(sock, "QUIT");
	   if(ok==0)
	   {
	       printf("[delete success.]\n");
	   }
   }
   else
   {
       printf("[delete failed.]\n");
   }
}



void pop3_login(int sock,char *server,char *account,char *password,char *protocol,int number)
{
    int ok;

	printf("[sock=%d.]\n",sock);
    if(sock!=-1)
	{
		ok = gen_transact(sock, "USER %s", account);
		if (ok==PS_SUCCESS)
		{
		    printf("[username right!]\n");
		}
		else
		{
			printf("[username [%s] does not exist!]\n",account);
		}
		ok = gen_transact(sock, "PASS %s", password);
		if(ok==PS_SUCCESS)
	 	{
	 	    printf("[password right.]\n");
	    }
		else
		{
		    printf("[password wrong.]\n");
		}
		sleep(1);
		del_pop3_mail(sock,number);
	}
	else
	{
	     printf("[socket create failed.]\n");
		 return ;
	}
	printf("[delete success.]\n");
}

extern void *g_pvMailDb;

//在vdb库中删除对应记录，下次扫描的时候删除的记录在邮箱中叶被删除了
int ATP_VDB_DeleteEmailEntry(void *pvVdbHdl, long long accountidx,int mailnumber)
{
    int                 lRet;
    const char          *pcTblName="emailsub";
    char                *pcSqlCmd;

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }
    // Build the cmd
    pcSqlCmd = sqlite3_mprintf("delete from %s where account=%lld and mailnumber=%d",pcTblName, accountidx,mailnumber);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
	//printf("lRet=%d\n\n",lRet);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }
    return ATP_VDB_RET_OK;
}


long long g_llScanAccount=0;

extern int SSLOpen(int sock);
static void fetchmail_msgproc(ATP_MSG_HEADER_ST *pstMsg)
{
	VOS_CHAR		*pcBody;
	struct query *ctl=querylist; 
	ATP_EmailDel_MSG_ST *g_pstDelItem = NULL;
	ATP_EmailSign_MSG_ST *g_pstSignItem = NULL;
	char *realhost ;
	int sock=-1;
	switch (pstMsg->ulMsgType)
	{
		case ATP_MSG_EMAIL_REVEDLL://删除邮件的消息
			printf("[Delete msg recvd.]\n");
			if (NULL != g_pstDelItem)
			{
				free((void *)g_pstDelItem);
			}
			g_pstDelItem = (ATP_EmailDel_MSG_ST *)malloc(sizeof(ATP_EmailDel_MSG_ST));
			if (NULL != g_pstDelItem)
			{
				memcpy(g_pstDelItem, (void *)(pstMsg + 1), sizeof(ATP_EmailDel_MSG_ST));
				//添加判断，对指定的邮件进行删除
				for(ctl=querylist;ctl;ctl=ctl->next)
				{
				
					if (ai0) 
					{
	                    fm_freeaddrinfo(ai0); ai0 = NULL;
	                }
				    if(ctl->idx==g_pstDelItem->llAccountIdx)
				    {
				        realhost = ctl->server.via ? ctl->server.via : ctl->server.pollname;
				        printf("[come to judge protocol.]\n");
				        if(ctl->server.protocol==P_IMAP)
				        {
				            printf("[imap.]\n");
							sock = SockOpen(realhost, 
			                ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),
			                ctl->server.plugin, &ai0);
							//进行ssl连接
							if (ctl->use_ssl && (SSLOpen(sock) == -1))
							{
							    printf("[SSL connection failed.]\n");
							}							
				            imap_login(sock,ctl->server.pollname,ctl->remotename,ctl->password,ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),g_pstDelItem->lMailNumber,0);
							break;
				        }
						if(ctl->server.protocol==P_POP3)
						{	
						    printf("[pop3.]\n");
							realhost = ctl->server.via ? ctl->server.via : ctl->server.pollname;
							sock = SockOpen(realhost, 
			                ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),
			                ctl->server.plugin, &ai0);
							//进行ssl连接
							if (ctl->use_ssl && (SSLOpen(sock) == -1))
							{
							    printf("[SSL connection failed.]\n");
							}
						    pop3_login(sock,ctl->server.pollname,ctl->remotename,ctl->password,ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),g_pstDelItem->lMailNumber);
							break;
						}
				    }
				}
				if(ctl==NULL)
				{
				    printf("[user does not exist,delete mail failed.]\n");
				}
			}
            
			if (NULL != g_pstDelItem)
			{
				free((void *)g_pstDelItem);
				g_pstDelItem = NULL;
			}
		    break;
		case ATP_MSG_EMAIL_REVESCAN :
				g_llScanAccount= *((long long *)(pstMsg + 1));
				scanagain=1;
			break;
		case ATP_MSG_EMAIL_SIGN_READ: 
			printf("[Sign IMAP mail msg recvd.]\n");
			if (NULL != g_pstSignItem)
			{
				free((void *)g_pstSignItem);
			}
			g_pstSignItem = (ATP_EmailSign_MSG_ST *)malloc(sizeof(ATP_EmailSign_MSG_ST));
			if (NULL != g_pstSignItem)
			{
				memcpy(g_pstSignItem, (void *)(pstMsg + 1), sizeof(ATP_EmailSign_MSG_ST));
				for(ctl=querylist;ctl;ctl=ctl->next)
				{
				
					if (ai0) 
					{
						fm_freeaddrinfo(ai0); ai0 = NULL;
					}
					if(ctl->idx==g_pstSignItem->llAccountIdx)
					{
						realhost = ctl->server.via ? ctl->server.via : ctl->server.pollname;
						printf("[come to judge protocol whether is imap.]\n");
						if(ctl->server.protocol==P_IMAP)
						{
							printf("[imap mail sign.]\n");
							sock = SockOpen(realhost, 
							ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),
							ctl->server.plugin, &ai0);
                            //进行ssl连接
							if (ctl->use_ssl && (SSLOpen(sock)== -1))
							{
							    printf("[SSL connection failed.]\n");
							}
							imap_login(sock,ctl->server.pollname,ctl->remotename,ctl->password,ctl->server.service ? ctl->server.service : ( ctl->use_ssl ? ctl->server.base_protocol->sslservice : ctl->server.base_protocol->service ),g_pstSignItem->llMailUid,1);
							break;
						}
						
					}
				}
				if(ctl==NULL)
				{
				     printf("[please check whether this account is exist.]\n");
				}
			}
			else
			{
			    printf("[malloc space error.]\n");
			}
            
            if (NULL != g_pstSignItem)
            {
                free((void *)g_pstSignItem);
				g_pstSignItem = NULL;
            }
			break;
		default :
		
			break;
	}
}

int interruptible_idle(int seconds)
/* time for a pause in the action; return TRUE if awakened by signal */
{
    int awoken = FALSE;

#ifndef __EMX__
#ifdef SLEEP_WITH_ALARM		/* not normally on */
    /*
     * We can't use sleep(3) here because we need an alarm(3)
     * equivalent in order to implement server nonresponse timeout.
     * We'll just assume setitimer(2) is available since fetchmail
     * has to have a BSDoid socket layer to work at all.
     */
    /* 
     * This code stopped working under glibc-2, apparently due
     * to the change in signal(2) semantics.  (The siginterrupt
     * line, added later, should fix this problem.) John Stracke
     * <francis@netscape.com> wrote:
     *
     * The problem seems to be that, after hitting the interval
     * timer while talking to the server, the process no longer
     * responds to SIGALRM.  I put in printf()s to see when it
     * reached the pause() for the poll interval, and I checked
     * the return from setitimer(), and everything seemed to be
     * working fine, except that the pause() just ignored SIGALRM.
     * I thought maybe the itimer wasn't being fired, so I hit
     * it with a SIGALRM from the command line, and it ignored
     * that, too.  SIGUSR1 woke it up just fine, and it proceeded
     * to repoll--but, when the dummy server didn't respond, it
     * never timed out, and SIGALRM wouldn't make it.
     *
     * (continued below...)
     */
    {
    struct itimerval ntimeout;

    ntimeout.it_interval.tv_sec = 5; /* repeat alarm every 5 secs */
    ntimeout.it_interval.tv_usec = 0;
    ntimeout.it_value.tv_sec  = seconds;
    ntimeout.it_value.tv_usec = 0;

    alarm_latch = FALSE;
    set_signal_handler(SIGALRM, gotsigalrm);	/* first trap signals */
    setitimer(ITIMER_REAL,&ntimeout,NULL);	/* then start timer */
    /* there is a very small window between the next two lines */
    /* which could result in a deadlock.  But this will now be  */
    /* caught by periodic alarms (see it_interval) */
    if (!alarm_latch)
	pause();
    /* stop timer */
    ntimeout.it_interval.tv_sec = ntimeout.it_interval.tv_usec = 0;
    ntimeout.it_value.tv_sec  = ntimeout.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL,&ntimeout,NULL);	/* now stop timer */
    set_signal_handler(SIGALRM, SIG_IGN);
    }
#else
    /* 
     * So the workaround I used is to make it sleep by using
     * select() instead of setitimer()/pause().  select() is
     * perfectly happy being called with a timeout and
     * no file descriptors; it just sleeps until it hits the
     * timeout.  The only concern I had was that it might
     * implement its timeout with SIGALRM--there are some
     * Unices where this is done, because select() is a library
     * function--but apparently not.
     */
    {
    struct timeval timeout;
    fd_set hReadset;
    int ret;
    ATP_MSG_HEADER_ST *pstMsg;

    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    do {
	lastsig = 0;
    FD_ZERO(&hReadset);
    FD_SET(g_stMsgCtx.lSock, &hReadset);
	ret = select((g_stMsgCtx.lSock + 1), &hReadset,0,0, &timeout);
    if ((ret > 0) && (FD_ISSET(g_stMsgCtx.lSock, &hReadset)))
    {
        if (0 == ATP_MSG_Recv(g_stMsgCtx.lSock, &pstMsg, 1 * 1000))
        {
            fetchmail_msgproc(pstMsg);
            free((void *)pstMsg);
			if(scanagain)//重新扫描标志为1
	        {
		        printf("scan again!");
		        break;
	        }
        }
        lastsig = SIGCHLD;
    }
    } while (lastsig == SIGCHLD);
    }
#endif
#else /* EMX */
    alarm_latch = FALSE;
    set_signal_handler(SIGALRM, gotsigalrm);
    _beginthread(itimerthread, NULL, 32768, NULL);
    /* see similar code above */
    if (!alarm_latch)
	pause();
    set_signal_handler(SIGALRM, SIG_IGN);
#endif /* ! EMX */
    if (lastsig == SIGUSR1 || ((seconds && getuid() == ROOT_UID)
	&& lastsig == SIGHUP))
       awoken = TRUE;

    /* now lock out interrupts again */
    set_signal_handler(SIGUSR1, SIG_IGN);
    if (getuid() == ROOT_UID)
	set_signal_handler(SIGHUP, SIG_IGN);

    return(awoken ? lastsig : 0);
}

#ifdef MAIN
int main(int argc, char **argv)
{
    for (;;)
    {
	printf("How may I serve you, master?\n");
	interruptible_idle(5);
    }
}
#endif /* MAIN */

/* idle.c ends here */
