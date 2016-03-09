#include <features.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <resolv.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/nameser.h>
#include <sys/utsname.h>
#include <sys/un.h>
#include "atptypes.h"
#ifdef SUPPORT_ATP_VOICE
#include "voicepub.h"
#endif
#include "httpapi.h"
#include <dirent.h>

#define ATP_NAME_RESOLVE_HAS_IPV6


#define REPLY_TIMEOUT_SEC 6
#define REPLY_TIMEOUT_USEC 500000
#define MAX_RETRIES 1
#define MAX_ALIASES	5

#ifdef SUPPORT_ATP_FON
#define RET_NO_SUCH_NAME -100
#endif

/* 1:ip + 1:full + MAX_ALIASES:aliases + 1:NULL */
#define 	ALIAS_DIM		(2 + MAX_ALIASES + 1)
#define HYBRID_LTE_ENABLE_FILE "/var/lte/enable"
#define LTE_WAN4_IPADDR_FILE "/var/wan/rmnet0/ipaddr"
#define LTE_WAN6_IPADDR_FILE "/var/wan/rmnet0/ipaddr6"



/* WAN连接状态 */
typedef enum
{
    /* 未初始化 */
    ATP_WAN_STATUS_UNCONFIGURED,
    /* 正在连接 */
    ATP_WAN_STATUS_CONNECTING,
    /* 正在认证 */
    ATP_WAN_STATUS_AUTHENTICATING,
    /* 已连接 */
    ATP_WAN_STATUS_CONNECTED,
    /* 未连接 */
    ATP_WAN_STATUS_PENDING,
    /* 正在断开 */
    ATP_WAN_STATUS_DISCONNECTING,
    /* 已断开 */
    ATP_WAN_STATUS_DISCONNECTED, 
}ATP_WAN_STATUS_EN;

#undef DEBUG
//#define DEBUG
/* #define DEBUG */

#ifdef DEBUG
#define DPRINTF(X,args...) fprintf(stderr, X, ##args)
#else
#define DPRINTF(X,args...)
#endif /* DEBUG */


/* Global stuff (stuff needing to be locked to be thread safe)... */

/* Structs */
struct resolv_header {
	int id;
	int qr,opcode,aa,tc,rd,ra,rcode;
	int qdcount;
	int ancount;
	int nscount;
	int arcount;
};

struct resolv_question {
	char * dotted;
	int qtype;
	int qclass;
};

struct resolv_answer {
	char * dotted;
	int atype;
	int aclass;
	int ttl;
	int rdlength;
	const unsigned char * rdata;
	int rdoffset;
	char* buf;
	size_t buflen;
	size_t add_count;
};

/* function prototypes */
static int handy_dns_lookup(const char * name, int type,
						unsigned char ** outpacket, struct resolv_answer * a,
						char* wanaddr);

static int handy_encode_dotted(const char * dotted, unsigned char * dest, int maxlen);
static int handy_decode_dotted(const unsigned char * message, int offset,
						   char * dest, int maxlen);
static int handy_length_dotted(const unsigned char * message, int offset);
static int handy_encode_header(struct resolv_header * h, unsigned char * dest, int maxlen);
static int handy_decode_header(const unsigned char * data, struct resolv_header * h);
static int handy_encode_question(struct resolv_question * q,
							 unsigned char * dest, int maxlen);
static int handy_decode_answer(const unsigned char * message, int offset,
						   struct resolv_answer * a);
static int handy_length_question(const unsigned char * message, int offset);

#if 1
VOS_UINT32 ATP_UTIL_GetSysTime(struct timeval* pstTimeVal)
{
    struct timespec ts;

    if (!pstTimeVal)
    {
        return VOS_NOK;
    }

	if (syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &ts))
	{
        pstTimeVal->tv_sec   = 0;
        pstTimeVal->tv_usec  = 0;
	    perror("syscall:__NR_clock_gettime");
        return VOS_NOK;
	}

    pstTimeVal->tv_sec   = ts.tv_sec;
    pstTimeVal->tv_usec  = ts.tv_nsec/1000; 
    return VOS_OK;
}
#else
VOS_UINT32 ATP_UTIL_GetSysTime(struct timeval* pstTimeVal)
{
    struct timespec ts;

    if (!pstTimeVal)
    {
        return VOS_NOK;
    }

    if (gettimeofday(pstTimeVal, NULL) < 0)
    {
        pstTimeVal->tv_sec   = 0;
        pstTimeVal->tv_usec  = 0;
        return VOS_NOK;
    }

    pstTimeVal->tv_sec   = ts.tv_sec;
    pstTimeVal->tv_usec  = ts.tv_nsec/1000; 
    return VOS_OK;
}
#endif

#ifdef SUPPORT_ATP_VOICE
/*****************************************************************************
 函 数 名: ATP_UTIL_VoiceGetVoipStatus
 功能描述: 获取VoIP状态，空闲还是忙
           VoIP状态由所有SIP帐号状态所反映，只要有一个SIP帐号为忙则返回忙状态的结果
 输入参数: 无
 输出参数: 无
 返 回 值: VoIP状态: VOS_FALSE为空闲;VOS_TRUE为忙
*****************************************************************************/
VOS_BOOL ATP_UTIL_VoiceGetVoipStatus(VOS_VOID)
{
    FILE        *pstFile;
    VOS_UINT32  ulStatus;
    VOS_CHAR    acSrcData[VOICE_STATUS_LEN_MAX];

    memset(acSrcData, 0, VOICE_STATUS_LEN_MAX);
    pstFile = fopen(FILE_PATH_VOIP_STATUS, "rb");
    if (VOS_NULL_PTR == pstFile)
    {
        return VOS_FALSE;
    }

    fread(acSrcData, 1, VOICE_STATUS_LEN_MAX, pstFile);
    fclose(pstFile);
    acSrcData[VOICE_STATUS_LEN_MAX - 1] = '\0';
    ulStatus = (VOS_UINT32)atoi(acSrcData);
    if (ulStatus > 0)
    {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}
#ifdef SUPPORT_ATP_VOICE_DT
VOS_INT32 ATP_UTIL_VoiceGetSipInterval(VOS_VOID)
{
        FILE *      fp = VOS_NULL_PTR;
        VOS_CHAR        acFile[VOICE_STATUS_LEN_MAX] = {0};   
        
        fp = fopen(FILE_PATH_VOIP_INTERVAL, "r");
        if ( VOS_NULL_PTR == fp )
        {
            return 0;
        }
        fread(acFile, 1, VOICE_STATUS_LEN_MAX, fp);
        fclose(fp);
        return (VOS_UINT32)atoi(acFile);
}
#endif
/*****************************************************************************
 函 数 名: ATP_UTIL_VoiceGetVoipStatus
 功能描述: 获取VoIP状态，空闲还是忙
           VoIP状态由所有SIP帐号状态所反映，只要有一个SIP帐号为忙则返回忙状态的结果
 输入参数: 无
 输出参数: 无
 返 回 值: VoIP状态: VOS_FALSE为空闲;VOS_TRUE为忙
*****************************************************************************/
VOS_BOOL ATP_UTIL_VoiceHasUrgCall(VOS_VOID)
{
    FILE        *pstFile;
    VOS_UINT32  ulStatus;
    VOS_CHAR    acSrcData[VOICE_STATUS_LEN_MAX];

    memset(acSrcData, 0, VOICE_STATUS_LEN_MAX);
    pstFile = fopen(FILE_PATH_URGCALL_STATUS, "rb");
    if (VOS_NULL_PTR == pstFile)
    {
        return VOS_FALSE;
    }

    fread(acSrcData, 1, VOICE_STATUS_LEN_MAX, pstFile);
    fclose(pstFile);
    acSrcData[VOICE_STATUS_LEN_MAX - 1] = '\0';
    ulStatus = (VOS_UINT32)atoi(acSrcData);
    if (ulStatus > 0)
    {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}
#endif


//copy from atp_gethostbyname
static int handy_encode_header(struct resolv_header *h, unsigned char *dest, int maxlen)
{
#ifdef ATP_PCLINT
    h = h;
#endif
	if (maxlen < HFIXEDSZ)
		return -1;

	dest[0] = (h->id & 0xff00) >> 8;
	dest[1] = (h->id & 0x00ff) >> 0;
	dest[2] = (h->qr ? 0x80 : 0) |
		((h->opcode & 0x0f) << 3) |
		(h->aa ? 0x04 : 0) |
		(h->tc ? 0x02 : 0) |
		(h->rd ? 0x01 : 0);
	dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0f);
	dest[4] = (h->qdcount & 0xff00) >> 8;
	dest[5] = (h->qdcount & 0x00ff) >> 0;
	dest[6] = (h->ancount & 0xff00) >> 8;
	dest[7] = (h->ancount & 0x00ff) >> 0;
	dest[8] = (h->nscount & 0xff00) >> 8;
	dest[9] = (h->nscount & 0x00ff) >> 0;
	dest[10] = (h->arcount & 0xff00) >> 8;
	dest[11] = (h->arcount & 0x00ff) >> 0;

	return HFIXEDSZ;
}

static int handy_decode_header(const unsigned char *data, struct resolv_header *h)
{
    
#ifdef ATP_PCLINT
        data = data;
#endif
	h->id = (data[0] << 8) | data[1];
	h->qr = (data[2] & 0x80) ? 1 : 0;
	h->opcode = (data[2] >> 3) & 0x0f;
	h->aa = (data[2] & 0x04) ? 1 : 0;
	h->tc = (data[2] & 0x02) ? 1 : 0;
	h->rd = (data[2] & 0x01) ? 1 : 0;
	h->ra = (data[3] & 0x80) ? 1 : 0;
	h->rcode = data[3] & 0x0f;
	h->qdcount = (data[4] << 8) | data[5];
	h->ancount = (data[6] << 8) | data[7];
	h->nscount = (data[8] << 8) | data[9];
	h->arcount = (data[10] << 8) | data[11];

	return HFIXEDSZ;
}

/* Encode a dotted string into nameserver transport-level encoding.
   This routine is fairly dumb, and doesn't attempt to compress
   the data */

static int handy_encode_dotted(const char *dotted, unsigned char *dest, int maxlen)
{
	int used = 0;
       char *c;
       int l;

	while (dotted && *dotted) {
		c = (char *)strchr(dotted, '.');
		l = c ? c - dotted : (int)strlen(dotted);

		if (l >= ((maxlen - used) - 1))
			return -1;

		dest[used++] = (unsigned char)l;
		memcpy(dest + used, dotted, (size_t)l);
		used += l;

		if (c)
			dotted = c + 1;
		else
			break;
	}

	if (maxlen < 1)
		return -1;

	dest[used++] = 0;

	return used;
}

/* Decode a dotted string from nameserver transport-level encoding.
   This routine understands compressed data. */

static int handy_decode_dotted(const unsigned char *data, int offset,
					char *dest, int maxlen)
{
	int l;
	int measure = 1;
	int total = 0;
	int used = 0;

	if (!data)
		return -1;

	while (0 !=(l=data[offset++])) {
		if (measure)
		    total++;
		if ((l & 0xc0) == (0xc0)) {
			if (measure)
				total++;
			/* compressed item, redirect */
			offset = ((l & 0x3f) << 8) | data[offset];
			measure = 0;
			continue;
		}

		if ((used + l + 1) >= maxlen)
			return -1;

		memcpy(dest + used, data + offset, (size_t)l);
		offset += l;
		used += l;
		if (measure)
			total += l;

		if (data[offset] != 0)
			dest[used++] = '.';
		else
			dest[used++] = '\0';
	}

	/* The null byte must be counted too */
	if (measure) {
	    total++;
	}

	DPRINTF("Total decode len = %d\n", total);

	return total;
}

static int handy_length_dotted(const unsigned char *data, int offset)
{
	int orig_offset = offset;
	int l;

	if (!data)
		return -1;

	while (0 != (l = data[offset++])) {

		if ((l & 0xc0) == (0xc0)) {
			offset++;
			break;
		}

		offset += l;
	}

	return offset - orig_offset;
}

static int handy_encode_question(struct resolv_question *q,
					  unsigned char *dest, int maxlen)
{
	int i;
#ifdef ATP_PCLINT
       q= q;
#endif

	i = handy_encode_dotted(q->dotted, dest, maxlen);
	if (i < 0)
		return i;

	dest += i;
	maxlen -= i;

	if (maxlen < 4)
		return -1;

	dest[0] = (q->qtype & 0xff00) >> 8;
	dest[1] = (q->qtype & 0x00ff) >> 0;
	dest[2] = (q->qclass & 0xff00) >> 8;
	dest[3] = (q->qclass & 0x00ff) >> 0;

	return i + 4;
}

static int handy_length_question(const unsigned char *message, int offset)
{
	int i;
#ifdef ATP_PCLINT
           message = message;
#endif

	i = handy_length_dotted(message, offset);
	if (i < 0)
		return i;

	return i + 4;
}

static int handy_decode_answer(const unsigned char *message, int offset,
					struct resolv_answer *a)
{
	char temp[256];
	int i;

	i = handy_decode_dotted(message, offset, temp, sizeof(temp));
	if (i < 0)
		return i;

	message += offset + i;

	a->dotted = strdup(temp);
	a->atype = (message[0] << 8) | message[1];
	message += 2;
	a->aclass = (message[0] << 8) | message[1];
	message += 2;
	a->ttl = (message[0] << 24) |
		(message[1] << 16) | (message[2] << 8) | (message[3] << 0);
	message += 4;
	a->rdlength = (message[0] << 8) | message[1];
	message += 2;
	a->rdata = message;
	a->rdoffset = offset + i + RRFIXEDSZ;

	DPRINTF("i=%d,rdlength=%d\n", i, a->rdlength);

	return i + RRFIXEDSZ + a->rdlength;
}

#define LOOP_ADDR "127.0.0.1"
#define DNS_PACKET_LEN_MAX (512)
#define CHECK_RET(x) {if ((x) < 0){DPRINTF("%s %d fail with errno[%d, %s]\r\n", __func__, __LINE__, errno, strerror(errno)); break;}}

static int BuildQuery(const char *pcName, int ulType, unsigned char* pucPacket, unsigned int* pulLen)
{
    int                     lQueryID = -1;
    int                     lRet = -1;
    struct resolv_header    stHead;
	struct resolv_question  stQuest;
	struct resolv_answer    stAnswer;
	char                    acQueryName[MAXDNAME] = {0};
	unsigned int            ulLen = 0;

	lQueryID = time(NULL) + 1;
	lQueryID &= 0xffff;

	memset(&stHead, 0, sizeof(stHead));	
    memset(&stQuest, 0, sizeof(stQuest));
    memset(&stAnswer, 0, sizeof(stAnswer));

    /*构造头部*/
	stHead.id = lQueryID;
	stHead.qdcount = 1;
	stHead.rd = 1;
	lRet = handy_encode_header(&stHead, pucPacket, PACKETSZ);
	if (lRet < 0)
	{
	    return -1;
	}
	ulLen += lRet;

    /*构造查询部分*/
	snprintf(acQueryName, sizeof(acQueryName), "%s", pcName);
    stQuest.dotted = (char *)acQueryName;
    stQuest.qtype = ulType;
    stQuest.qclass = 1; 

    lRet = handy_encode_question(&stQuest, pucPacket + ulLen, PACKETSZ - ulLen);
	if (lRet < 0)
	{
	    return -1;
	}
    ulLen += lRet;

    *pulLen = ulLen;
    return 0;
}

static int RecvReply(int lfd, unsigned char* pucPacket)
{
    fd_set          stfds;
    struct timeval  stTime;     
    
    FD_ZERO(&stfds);
	FD_SET(lfd, &stfds);
	stTime.tv_sec = REPLY_TIMEOUT_SEC;
	stTime.tv_usec = REPLY_TIMEOUT_USEC;
	if (select(lfd + 1, &stfds, NULL, NULL, &stTime) <= 0)
	{
	    DPRINTF("%s %d\r\n", __func__, __LINE__);
	    return -1;
	}
	if (recv(lfd, pucPacket, DNS_PACKET_LEN_MAX, 0) < HFIXEDSZ)
	{
	    DPRINTF("%s %d\r\n", __func__, __LINE__);
	    return -1;
	}
	return 0;
}

/*检查Name 和 Type*/
static int GetType(const unsigned char* pucPacket, const char *pcName, unsigned short* pusType)
{
    struct resolv_header    stHead;
    unsigned int            ulPos = 0;
    int                     lRet = -1;
    char                    acQueryName[MAXDNAME] = {0};
    unsigned short          usQueryType = 0;                  
    
    memset(&stHead, 0, sizeof(stHead));	
    
    handy_decode_header(pucPacket, &stHead);
    if ((stHead.qr != 1) || (stHead.qdcount != 1))
    {
        DPRINTF("\n");
        return -1;
    }

    /*check name*/
    ulPos = HFIXEDSZ;    
	lRet = handy_decode_dotted(pucPacket, ulPos, acQueryName, sizeof(acQueryName));
	if (0 != strcasecmp(pcName, acQueryName))
	{
	    DPRINTF("%s, %s\n", pcName, acQueryName);
    	return -1;
	}
	lRet = handy_length_dotted(pucPacket, ulPos);
	if (lRet < 0)
	{
	    return -1;
	}
	ulPos += lRet;

	/*check type*/
	usQueryType = ntohs(*((unsigned short*)(pucPacket + ulPos)));
	if ((usQueryType != T_A) && (usQueryType != T_AAAA))
	{
	    DPRINTF("%d \n", usQueryType);
	    return -1;
	}
	*pusType = usQueryType;
	return 0;	
}

static int ResolvReply(const unsigned char* pucPacket, unsigned short usType, struct resolv_answer *pstResolv)
{
    unsigned int            ulPos = 0;
    int            j = 0;
    struct resolv_header    stHead;
    int                     lRet = -1;
    unsigned int            ulFirstAnswer = 1;
    struct resolv_answer    stAnswer;

    memset(&stHead, 0, sizeof(stHead));
    memset(&stAnswer, 0, sizeof(stAnswer));
    
    handy_decode_header(pucPacket, &stHead);

    if ((stHead.rcode > 0) || (stHead.ancount < 1))
    {
		/* negative result, not present */
		return -1;
	}
    
    ulPos = HFIXEDSZ;

	lRet = handy_length_question(pucPacket, ulPos);
	if (lRet < 0)
	{
		return -1;
	}
	ulPos += lRet;
	DPRINTF("Decoding answer at pos %d\n", ulPos);

	for (j = 0; j < stHead.ancount; j++, ulPos += lRet)
	{
		lRet = handy_decode_answer(pucPacket, ulPos, &stAnswer);

		if (lRet < 0)
		{
			DPRINTF("failed decode\n");
			return -1;
		}

		if (ulFirstAnswer)
		{
			stAnswer.buf = pstResolv->buf;
			stAnswer.buflen = pstResolv->buflen;
			stAnswer.add_count = pstResolv->add_count;
			memcpy(pstResolv, &stAnswer, sizeof(stAnswer));
			if ((pstResolv->atype != T_SIG) && (0 == pstResolv->buf))
			{
				break;
			}
			if (pstResolv->atype != (int)usType)
			{
				free(pstResolv->dotted);
                pstResolv->dotted = NULL;
				continue;
			}
			pstResolv->add_count = (size_t)((stHead.ancount - j) - 1);
			if (((size_t)pstResolv->rdlength + sizeof(struct in_addr*)) *pstResolv->add_count > pstResolv->buflen)
			{
				break;
			}
			pstResolv->add_count = 0;
			ulFirstAnswer = 0;
		}
		else
		{
			free(stAnswer.dotted);
			if (stAnswer.atype != (int)usType)
			{
				continue;
			}
			if (pstResolv->rdlength != stAnswer.rdlength)
			{
				free(pstResolv->dotted);
                pstResolv->dotted = NULL;
				DPRINTF("Answer address len(%u) differs from original(%u)\n",
						stAnswer.rdlength, pstResolv->rdlength);
				return -1;
			}
			memcpy(pstResolv->buf + (pstResolv->add_count * ((size_t)stAnswer.rdlength)), stAnswer.rdata, (size_t)stAnswer.rdlength);
			++pstResolv->add_count;

            if (pstResolv->add_count >= 7)
            {
                DPRINTF("8 records at most.\n");
                break;
            }
		}
	}
    return 0;
}

//lint -save -e826
static int resolv_v4(const unsigned char* pucPacket,
                    const char * name,
					struct hostent * result_buf,
					char * buf, 
					size_t buflen,
					struct hostent ** result,
                    struct resolv_answer *pA)
{
    struct in_addr *in;
    struct in_addr **addr_list;
    char **alias;
    
    int i;

    *result=NULL;
    if (!name)
        return EINVAL;

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen < sizeof(char *)*(ALIAS_DIM))
        return ERANGE;
    alias=(char **)buf;
    buf+=sizeof(char **)*(ALIAS_DIM);
    buflen-=sizeof(char **)*(ALIAS_DIM);

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    alias[0] = buf;
    alias[1] = NULL;

    /* First check if this is already an address */
    if (inet_aton(name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        result_buf->h_aliases = alias;
        *result=result_buf;
        return NETDB_SUCCESS;
    }

    pA->buf = buf;
    pA->buflen = buflen;
    pA->add_count = 0;
    
    i = ResolvReply(pucPacket, T_A, pA); 
    if (i < 0) {
        DPRINTF("handy_dns_lookup\n");
        return TRY_AGAIN;
    }
    
    if (((size_t)pA->rdlength + sizeof(struct in_addr*)) * pA->add_count + 256 > buflen)
    {
        free(pA->dotted);
        DPRINTF("buffer too small for all addresses\n");
        return ERANGE;
    }
    else if(pA->add_count > 0)
    {
        memmove(buf - sizeof(struct in_addr*)*2, buf, pA->add_count * ((size_t)pA->rdlength));
        addr_list = (struct in_addr**)(buf + pA->add_count * ((size_t)pA->rdlength));
        addr_list[0] = in;
        for (i = (int)pA->add_count-1; i>=0; --i)
            addr_list[i+1] = (struct in_addr*)((buf - sizeof(struct in_addr*)*2) + pA->rdlength * i);
        addr_list[pA->add_count + 1] = 0;
        buflen -= (size_t)(((char*)&(addr_list[pA->add_count + 2])) - buf);
        buf = (char*)&addr_list[pA->add_count + 2];
    }
    if (NULL != pA->dotted)
    {
        strncpy(buf, pA->dotted, buflen);
        free(pA->dotted);
        pA->dotted = NULL;
    }
    

    if (pA->atype == 1) { /* ADDRESS T_A=1 */
        memcpy(in, pA->rdata, sizeof(*in));
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        result_buf->h_aliases = alias; /* TODO: generate the full list */
        *result=result_buf;
        return NETDB_SUCCESS;
    }
    return TRY_AGAIN;
}

static int resolv_v6(const unsigned char* pucPacket,
                const char *name,
                struct hostent * result_buf,
                char * buf, size_t buflen,
                struct hostent ** result,
                struct resolv_answer *pA)
{
    struct in6_addr *in;
    struct in6_addr **addr_list;
    int i;
    struct in6_addr stAddr;
    
    memset(&stAddr, 0, sizeof(struct in6_addr));
    
    *result=NULL;

    if (!name)
        return EINVAL;

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in6_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in6_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    /* First check if this is already an address */
    if (inet_pton(AF_INET6, name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET6;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        *result=result_buf;
        return NETDB_SUCCESS;
    }



    pA->buf = buf;
    pA->buflen = buflen;
    
    i = ResolvReply(pucPacket, T_AAAA, pA); 
    if (i < 0) {
        DPRINTF("handy_dns_lookup\n");
        return TRY_AGAIN;
    }
    
    if (((size_t)pA->rdlength + sizeof(struct in6_addr*)) * pA->add_count + 256 > buflen)
    {
        free(pA->dotted);
        DPRINTF("buffer too small for all addresses\n");
        return ERANGE;
    }
    else if(pA->add_count > 0)
    {
        memmove(buf - sizeof(struct in6_addr*)*2, buf, pA->add_count * ((size_t)pA->rdlength));
        addr_list = (struct in6_addr**)(buf + pA->add_count * (size_t)pA->rdlength);
        addr_list[0] = in;
        for (i = (int)pA->add_count-1; i>=0; --i)
            addr_list[i+1] = (struct in6_addr*)((buf - sizeof(struct in6_addr*)*2) + pA->rdlength * i);
        addr_list[pA->add_count + 1] = 0;
        buflen -= (size_t)(((char*)&(addr_list[pA->add_count + 2])) - buf);
        buf = (char*)&addr_list[pA->add_count + 2];
    }

    if (NULL != pA->dotted)
    {
        strncpy(buf, pA->dotted, buflen);
        free(pA->dotted);
        pA->dotted = NULL;
    }
	

    if (pA->atype == T_AAAA) {	/* ADDRESS */
        memcpy(in, pA->rdata, sizeof(*in));
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET6;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        DPRINTF("%p\n", result_buf);
        *result=result_buf;
        return NETDB_SUCCESS;
    } else {
        return TRY_AGAIN;
    }    
}
//lint -restore



/*Always request A and AAAA RR at the same time together ( Independent of the used IP-Stack => Dual-Stack / IPv4 Only / IPv6 Only)*/
/*同时发送AAAA+A这两类查询*/
/*返回两个结果*/
static int LookUp_A_and_AAAA(const char *pcName,
                            struct hostent *pstV4Buff,
                            struct hostent *pstV6Buff,
                            struct hostent **ppstV4Result,
                            struct hostent **ppstV6Result)
{
    int                     lfd = -1;
    int                     lRet = -1;
    unsigned int            ulLen = 0;
    unsigned short          usType = 0;
    
    struct resolv_answer    stResolv;
    
	struct sockaddr_in      stSockAddr;//既是源地址，又是目的地址

	static char             acBufV4[sizeof(struct in_addr) +
                                    sizeof(struct in_addr *)*2 +
                                    sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */] = {0};
    static char             acBufV6[sizeof(struct in6_addr) +
                                    sizeof(struct in6_addr *)*2 +
                                    sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */] = {0};                    
	
	unsigned char*          pucPacket = (unsigned char*)malloc(PACKETSZ);

	
	if (NULL == pucPacket)
	{
	    return -1;
	}
    DPRINTF("%s pcName is %s\r\n", __func__, pcName);
    memset(&stResolv, 0, sizeof(stResolv));
    memset(&stSockAddr, 0, sizeof(stSockAddr));  
    do 
    {
        /*建立socket，建立失败再尝试一次*/
    	lfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    	if (lfd < 0)
    	{
    	    lfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    	}
    	CHECK_RET(lfd);
    	DPRINTF("\n");
    	/*绑定源地址*/
    	stSockAddr.sin_family = AF_INET;
    	stSockAddr.sin_addr.s_addr = 0;
    	lRet = bind(lfd, (struct sockaddr *)(&stSockAddr), sizeof(stSockAddr));
    	CHECK_RET(lRet);
        DPRINTF("\n");

    	/*指定目的地址*/
        stSockAddr.sin_port = htons(NAMESERVER_PORT);
        stSockAddr.sin_addr.s_addr = inet_addr(LOOP_ADDR);
        DPRINTF("\n");

        /*建立连接，建立失败重试一次*/
    	lRet = connect(lfd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
    	if (lRet < 0)
    	{
    	    lRet = connect(lfd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
    	}
    	CHECK_RET(lRet);
    	DPRINTF("\n");
        /*发送两个请求*/        
    	lRet = BuildQuery(pcName, T_AAAA, pucPacket, &ulLen);
    	CHECK_RET(lRet);
    	lRet = send(lfd, pucPacket, ulLen, 0);
    	CHECK_RET(lRet);
    	lRet = BuildQuery(pcName, T_A, pucPacket, &ulLen);
    	CHECK_RET(lRet);
    	lRet = send(lfd, pucPacket, ulLen, 0);
    	CHECK_RET(lRet);
        DPRINTF("\n");

        /*接收回复*/
        memset(pucPacket, 0, PACKETSZ);
        RecvReply(lfd, pucPacket);
        CHECK_RET(lRet);        
        /*获取回复的类型*/
        GetType(pucPacket, pcName, &usType);        
        
        DPRINTF("\n");
        if (T_A == usType)
        {
            DPRINTF("\n");
            resolv_v4(pucPacket, pcName, pstV4Buff, acBufV4, sizeof(acBufV4), ppstV4Result, &stResolv);
        }
        else if (T_AAAA == usType)
        {
            DPRINTF("\n");
            resolv_v6(pucPacket, pcName, pstV6Buff, acBufV6, sizeof(acBufV6), ppstV6Result, &stResolv);
        }
        DPRINTF("\n");

        /*接收第二个回复*/
        memset(pucPacket, 0, PACKETSZ);
        memset(&stResolv, 0, sizeof(stResolv));
        RecvReply(lfd, pucPacket);
        GetType(pucPacket, pcName, &usType);
    	DPRINTF("\n");
    	if (T_A == usType)
        {
            DPRINTF("\n");
            resolv_v4(pucPacket, pcName, pstV4Buff, acBufV4, sizeof(acBufV4), ppstV4Result, &stResolv);
        }
        else if (T_AAAA == usType)
        {
            DPRINTF("\n");
            resolv_v6(pucPacket, pcName, pstV6Buff, acBufV6, sizeof(acBufV6), ppstV6Result, &stResolv);
        }
        DPRINTF("%p, %p\n", *ppstV4Result, *ppstV6Result);
	}while(0);

    free(pucPacket);
	DPRINTF("lfd is %d\n", lfd);
	if (lfd > 0)
	{
	    close(lfd);
	}
	DPRINTF("%p, %p\n", *ppstV4Result, *ppstV6Result);
	return lRet;

}

//__UCLIBC_MUTEX_STATIC(mylock, PTHREAD_MUTEX_INITIALIZER);
//lint -save -e801
static int handy_dns_lookup(const char *name, int type,
				 unsigned char **outpacket, struct resolv_answer *a,
				 char* wanaddr)
{
	int i, j, len, fd, pos, rc;
	struct timeval tv;
	fd_set fds;
	struct resolv_header h;
	struct resolv_question q;
	struct resolv_answer ma;
    struct sockaddr_in  bindAddr;
	int first_answer = 1;
	int retries = 0;
	unsigned char * packet = malloc(PACKETSZ);
	char *dns, *lookup = malloc(MAXDNAME);
    int local_id = -1;
	struct sockaddr_in sa;
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
	int v6;
	struct sockaddr_in6 sa6;
    struct sockaddr_in6  bindAddr6;
    int record_flag = 0;
#endif
#ifdef ATP_PCLINT
    wanaddr = wanaddr;
#endif

	fd = -1;
    memset(&h, 0, sizeof(struct resolv_header));
	
	if (!packet || !lookup)
	    goto fail;

	DPRINTF("Looking up type %d answer for '%s'\n", type, name);
    local_id = time(NULL);

    ++local_id;
    local_id &= 0xffff;

	while (retries < MAX_RETRIES) {
		if (fd != -1)
			close(fd);

		memset(packet, 0, PACKETSZ);

		memset(&h, 0, sizeof(h));

		//++local_id;
		//local_id &= 0xffff;
		h.id = local_id;
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
        if (NULL == wanaddr)
        {
        #if 1
            if (T_AAAA == type)
            {
                if (!record_flag)
                {
    		        dns = "::1";
                }
                else
                {
                    dns = "127.0.0.1";
                }
            }
            else
            {
                if (!record_flag)
                {
                    dns = "127.0.0.1";
                }
                else
                {
    		        dns = "::1";
                }    
            }
        #else
            dns = "127.0.0.1";
        #endif
        }
        else if (inet_pton(AF_INET6, wanaddr, &sa6.sin6_addr) > 0)
        {
            dns = "::1";
        }
        else
        {
            dns = "127.0.0.1";
        }
#else
        dns = "127.0.0.1";
#endif
        DPRINTF("got server %s\n", dns);


		h.qdcount = 1;
		h.rd = 1;

		DPRINTF("encoding header %d\n", h.rd);

		i = handy_encode_header(&h, packet, PACKETSZ);
		if (i < 0)
			goto fail;

		strncpy(lookup,name,MAXDNAME);
		DPRINTF("lookup name: %s\n", lookup);
		q.dotted = (char *)lookup;
		q.qtype = type;
		q.qclass = 1; /* CLASS_IN C_IN*/

		j = handy_encode_question(&q, packet+i, PACKETSZ-i);
		if (j < 0)
			goto fail;

		len = i + j;

		DPRINTF("On try %d, sending query to port %d of machine %s\n",
				retries+1, NAMESERVER_PORT, dns);

#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		v6 = inet_pton(AF_INET6, dns, &sa6.sin6_addr) > 0;
		fd = socket(v6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
		fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif
		if (fd < 0) {
			retries++;
            if ((retries >= MAX_RETRIES) && (!record_flag))
            {
                retries = 0;
                record_flag = 1;
            }
		    continue;
		}

		/* Connect to the UDP socket so that asyncronous errors are returned */
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		if (v6) {
            memset((void *)(&bindAddr6), 0, sizeof(bindAddr6));
            bindAddr6.sin6_family = AF_INET6;
            if ((NULL == wanaddr) || (inet_pton(AF_INET6, wanaddr, &bindAddr6.sin6_addr) <= 0))
            {
                DPRINTF("Bind any addr!\n");
            }
            if (bind(fd, (struct sockaddr *)(&bindAddr6), sizeof(bindAddr6)) < 0)
            {
                DPRINTF("Bind addr %s failed!\n", wanaddr);
                goto fail;
            }
		    sa6.sin6_family = AF_INET6;
		    sa6.sin6_port = htons(NAMESERVER_PORT);
		    /* sa6.sin6_addr is already here */
		    rc = connect(fd, (struct sockaddr *) &sa6, sizeof(sa6));
		} else {
#endif
		    sa.sin_family = AF_INET;
		    sa.sin_port = htons(NAMESERVER_PORT);
        #if 0
		    __UCLIBC_MUTEX_LOCK(__resolv_lock);
		    /* 'dns' is really __nameserver[] which is a global that
		       needs to hold __resolv_lock before access!! */
		    sa.sin_addr.s_addr = inet_addr(dns);
		    __UCLIBC_MUTEX_UNLOCK(__resolv_lock);
        #else
            // TODO: Doing binding
            memset((void *)(&bindAddr), 0, sizeof(bindAddr));
            bindAddr.sin_family = AF_INET;
            //bindAddr.sin_addr.s_addr = localAddr;
            if ((NULL == wanaddr) || (inet_pton(AF_INET, wanaddr, &bindAddr.sin_addr) <= 0))
            {
                bindAddr.sin_addr.s_addr = 0;
            }
            if (bind(fd, (struct sockaddr *)(&bindAddr), sizeof(bindAddr)) < 0)
            {
                DPRINTF("Bind addr %s failed!\n", inet_ntoa(bindAddr.sin_addr));
                goto fail;
            }
            sa.sin_addr.s_addr = inet_addr(dns);
        #endif
		    rc = connect(fd, (struct sockaddr *) &sa, sizeof(sa));
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		}
#endif
		if (rc < 0) {
		    if (errno == ENETUNREACH) {
				/* routing error, presume not transient */
				//goto fail;
				retries++;
		    } else {
				/* retry */
				retries++;
		    }
            if ((retries >= MAX_RETRIES) && (!record_flag))
            {
                retries = 0;
                record_flag = 1;
            }
			continue;
		}

		DPRINTF("Transmitting packet of length %d, id=%d, qr=%d\n",
				len, h.id, h.qr);

		send(fd, packet, (unsigned int)len, 0);

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = REPLY_TIMEOUT_SEC;
		tv.tv_usec = REPLY_TIMEOUT_USEC;
		if (select(fd + 1, &fds, NULL, NULL, &tv) <= 0) {
		    DPRINTF("Timeout\n");

			/* timed out, so retry send and receive,
			 * to next nameserver on queue */
			goto fail;
			//goto again;
		}

		len = recv(fd, packet, 512, 0);
		if (len < HFIXEDSZ) {
			/* too short ! */
			goto again;
		}

		handy_decode_header(packet, &h);

		DPRINTF("id = %d, qr = %d\n", h.id, h.qr);

		if ((h.id != local_id) || (!h.qr)) {
			/* unsolicited */
			goto again;
		}


		DPRINTF("Got response %s\n", "(i think)!");
		DPRINTF("qrcount=%d,ancount=%d,nscount=%d,arcount=%d\n",
				h.qdcount, h.ancount, h.nscount, h.arcount);
		DPRINTF("opcode=%d,aa=%d,tc=%d,rd=%d,ra=%d,rcode=%d\n",
				h.opcode, h.aa, h.tc, h.rd, h.ra, h.rcode);

		if ((h.rcode) || (h.ancount < 1)) {
			/* negative result, not present */
			goto fail;
		}

		pos = HFIXEDSZ;

		for (j = 0; j < h.qdcount; j++) {
			DPRINTF("Skipping question %d at %d\n", j, pos);
			i = handy_length_question(packet, pos);
			DPRINTF("Length of question %d is %d\n", j, i);
			if (i < 0)
				goto again;
			pos += i;
		}
		DPRINTF("Decoding answer at pos %d\n", pos);

		first_answer = 1;
		for (j=0;j<h.ancount;j++,pos += i)
		{
			i = handy_decode_answer(packet, pos, &ma);

			if (i<0) {
				DPRINTF("failed decode %d\n", i);
				goto again;
			}

			if ( first_answer )
			{
				ma.buf = a->buf;
				ma.buflen = a->buflen;
				ma.add_count = a->add_count;
				memcpy(a, &ma, sizeof(ma));
				//if (a->atype != T_SIG && (0 == a->buf || (type != T_A && type != T_AAAA)))
				if (a->atype != 24 && (0 == a->buf || (type != 1 && type != 28)))
				{
					break;
				}
				if (a->atype != type)
				{
					free(a->dotted);
                    a->dotted = NULL;
					continue;
				}
				a->add_count = (size_t)((h.ancount - j) - 1);
				if (((size_t)a->rdlength + sizeof(struct in_addr*)) * a->add_count > a->buflen)
				{
					break;
				}
				a->add_count = 0;
				first_answer = 0;
			}
			else
			{
				free(ma.dotted);
				if (ma.atype != type)
				{
					continue;
				}
				if (a->rdlength != ma.rdlength)
				{
					free(a->dotted);
                    a->dotted = NULL;
					DPRINTF("Answer address len(%u) differs from original(%u)\n",
							ma.rdlength, a->rdlength);
					goto again;
				}
				memcpy(a->buf + (a->add_count * ((size_t)ma.rdlength)), ma.rdata, (size_t)ma.rdlength);
				++a->add_count;

                if (a->add_count >= 7)
                {
                    DPRINTF("8 records at most.\n");
                    break;
                }
			}
		}

		DPRINTF("Answer name = |%s|\n", a->dotted);
		DPRINTF("Answer type = |%d|\n", a->atype);

		close(fd);

		if (outpacket)
			*outpacket = packet;
		else
			free(packet);
		free(lookup);

		return (len);				/* success! */

	again:
        retries++;
        
        if ((retries >= MAX_RETRIES) && (!record_flag))
        {
            retries = 0;
            record_flag = 1;
        }
	}

 fail:
	if (fd != -1)
	    close(fd);
    //lint -save -e644
	if (lookup)
	    free(lookup);
	if (packet)
	    free(packet);
    //lint -restore
    
#ifdef SUPPORT_ATP_FON
    /* no such name */
    if (3 == h.rcode)
        return RET_NO_SUCH_NAME;
#endif    
    
    return -1;
}
//lint -restore

//lint -save -e826
static int gethostbyname_handy_r(const char * name,
					struct hostent * result_buf,
					char * buf, size_t buflen,
					struct hostent ** result,
					char* wanaddr,
					const char *pcCheckFile)
{
    struct in_addr *in;
    struct in_addr **addr_list;
    char **alias;
    unsigned char *packet;
    struct resolv_answer a;
    int i;
#ifdef ATP_PCLINT
    pcCheckFile = pcCheckFile;
#endif
    *result=NULL;
    if (!name)
        return EINVAL;

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen < sizeof(char *)*(ALIAS_DIM))
        return ERANGE;
    alias=(char **)buf;
    buf+=sizeof(char **)*(ALIAS_DIM);
    buflen-=sizeof(char **)*(ALIAS_DIM);

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    alias[0] = buf;
    alias[1] = NULL;

    /* First check if this is already an address */
    if (inet_aton(name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        result_buf->h_aliases = alias;
        *result=result_buf;
        return NETDB_SUCCESS;
    }

//去掉dns跨协议的限制,
//R5版本上不再有/var/dns/ipv4文件，因此注释掉如下代码
#if 0
    if (NULL != pcCheckFile)
    {
        i = 0;
        fp = fopen(pcCheckFile,"r");
        if (fp != NULL)
        {
            fgets(cmd, 32, fp);        
            i = atoi(cmd);
            fclose(fp);
        }
        if (i != 1)
        {
            return EINVAL;
        }
	}
#endif

    for (;;) {
        a.buf = buf;
        a.buflen = buflen;
        a.add_count = 0;

        i = handy_dns_lookup(name, 1, &packet, &a, wanaddr);

        if (i < 0) {
            DPRINTF("handy_dns_lookup\n");
            
#ifdef SUPPORT_ATP_FON   
            if (RET_NO_SUCH_NAME == i)
                return RET_NO_SUCH_NAME;
#endif       

            return TRY_AGAIN;
        }

        if (((size_t)a.rdlength + sizeof(struct in_addr*)) * a.add_count + 256 > buflen)
        {
            free(a.dotted);
            free(packet);
            DPRINTF("buffer too small for all addresses\n");
            return ERANGE;
        }
        else if(a.add_count > 0)
        {
            memmove(buf - sizeof(struct in_addr*)*2, buf, a.add_count * ((size_t)a.rdlength));
            addr_list = (struct in_addr**)(buf + a.add_count * ((size_t)a.rdlength));
            addr_list[0] = in;
            for (i = (int)a.add_count-1; i>=0; --i)
                addr_list[i+1] = (struct in_addr*)((buf - sizeof(struct in_addr*)*2) + a.rdlength * i);
            addr_list[a.add_count + 1] = 0;
            buflen -= (size_t)(((char*)&(addr_list[a.add_count + 2])) - buf);
            buf = (char*)&addr_list[a.add_count + 2];
        }
        if (NULL != a.dotted)
        {
            strncpy(buf, a.dotted, buflen);
            free(a.dotted);
            a.dotted = NULL;
        }
        

        if (a.atype == 1) { /* ADDRESS T_A=1 */
            memcpy(in, a.rdata, sizeof(*in));
            result_buf->h_name = buf;
            result_buf->h_addrtype = AF_INET;
            result_buf->h_length = sizeof(*in);
            result_buf->h_addr_list = (char **) addr_list;
#ifdef __UCLIBC_MJN3_ONLY__
#warning TODO -- generate the full list
#endif
            result_buf->h_aliases = alias; /* TODO: generate the full list */
            free(packet);
            break;
        } else {
            free(packet);
            return TRY_AGAIN;
        }
    }

    *result=result_buf;
    return NETDB_SUCCESS;
}

struct hostent *atp_gethostbyname(const char *name, unsigned int localAddr)
{
    int ret;
    static struct hostent h;
    static char buf[sizeof(struct in_addr) +
                          sizeof(struct in_addr *)*2 +
                          sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
    struct hostent *hp;
    struct in_addr wanin;

    wanin.s_addr = localAddr;

    ret = gethostbyname_handy_r(name, &h, buf, sizeof(buf), &hp, inet_ntoa(wanin), NULL);
    if (NETDB_SUCCESS != ret)
    {
        return NULL;
    }

    return hp;
}

int handy_gethostbyname2_r(const char *name, int family,
					 struct hostent * result_buf,
					 char * buf, size_t buflen,
					 struct hostent ** result,
					 char * bindaddr)
{
    struct in6_addr *in;
    struct in6_addr **addr_list;
    unsigned char *packet;
    struct resolv_answer a;
    int i;
    //int nest = 0;
    int lRet = 1;
    char cmd[32] = {0};
    FILE *fp;
    struct in6_addr stAddr;
    struct in_addr stAddrIpv4;
    
    
    memset(&stAddr, 0, sizeof(struct in6_addr));
    memset(&stAddrIpv4, 0, sizeof(struct in_addr));
    
    if (family == AF_INET)
    {
        if (inet_pton(AF_INET6, name, &stAddr)) 
        {
            return EINVAL;
        }
        return gethostbyname_handy_r(name, result_buf, buf, buflen, result, bindaddr, "/var/dns/ipv4");
    }

    *result=NULL;
    if (family != AF_INET6)
        return EINVAL;

    if (!name)
        return EINVAL;

    /* 未开启IPV6功能直接返回 */
    fp = fopen("/proc/sys/net/ipv6/conf/all/enable","r");
    if (fp != NULL)
    {
        fgets(cmd, 32, fp);        
        lRet = atoi(cmd);
        fclose(fp);
    }
    if (lRet != 1)
    {
        return EINVAL;
    }

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in6_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in6_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    /* First check if this is already an address */
    if (inet_pton(AF_INET6, name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET6;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        *result=result_buf;
        return NETDB_SUCCESS;
    }

    if (inet_aton(name, &stAddrIpv4))
    {
        return EINVAL;
    }

    fp = fopen("/var/dns/ipv6","r");
    if (fp != NULL)
    {
        fgets(cmd, 32, fp);        
        lRet = atoi(cmd);
        fclose(fp);
    }
    if (lRet != 1)
    {
        return EINVAL;
    }

    memset((char *) &a, '\0', sizeof(a));

    for (;;) {
        a.buf = buf;
        a.buflen = buflen;

        i = handy_dns_lookup(buf, T_AAAA, &packet, &a, bindaddr);

        if (i < 0) {
            return TRY_AGAIN;
        }

        if (((size_t)a.rdlength + sizeof(struct in6_addr*)) * a.add_count + 256 > buflen)
        {
            free(a.dotted);
            free(packet);
            DPRINTF("buffer too small for all addresses\n");
            return ERANGE;
        }
        else if(a.add_count > 0)
        {
            memmove(buf - sizeof(struct in6_addr*)*2, buf, a.add_count * ((size_t)a.rdlength));
            addr_list = (struct in6_addr**)(buf + a.add_count * (size_t)a.rdlength);
            addr_list[0] = in;
            for (i = (int)a.add_count-1; i>=0; --i)
                addr_list[i+1] = (struct in6_addr*)((buf - sizeof(struct in6_addr*)*2) + a.rdlength * i);
            addr_list[a.add_count + 1] = 0;
            buflen -= (size_t)(((char*)&(addr_list[a.add_count + 2])) - buf);
            buf = (char*)&addr_list[a.add_count + 2];
        }

        if (NULL != a.dotted)
        {
            strncpy(buf, a.dotted, buflen);
            free(a.dotted);
            a.dotted = NULL;
        }
		

        if (a.atype == T_AAAA) {	/* ADDRESS */
            memcpy(in, a.rdata, sizeof(*in));
            result_buf->h_name = buf;
            result_buf->h_addrtype = AF_INET6;
            result_buf->h_length = sizeof(*in);
            result_buf->h_addr_list = (char **) addr_list;
            free(packet);
            break;
        } else {
            free(packet);
            return TRY_AGAIN;
        }
    }

    *result=result_buf;
    return NETDB_SUCCESS;
}
//lint -restore

struct hostent *ATP_UTIL_Gethostbyname(const char *name, int family, char* bindaddr)
{
    int ret;
    static struct hostent h;
    static char buf[sizeof(struct in6_addr) +
                          sizeof(struct in6_addr *)*2 +
                          sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
    struct hostent *hp;
    
    if ((strlen(name) == 0) || name[0] == '.')//不对根域名发起查询
    {
        printf("Don't query <Root>\n");
        return NULL;
    }

    ret = handy_gethostbyname2_r(name, family, &h, buf, sizeof(buf), &hp, bindaddr);
    if (NETDB_SUCCESS != ret)
    {
        return NULL;
    }

    return hp;
}

/*!
  \brief 同时查询A和AAAA的函数
        输入两个struct hostent**，
        查询到A时，*ppstV4Result不为NULL，否则为NULL
        查询到AAAA时，*ppstV6Result不为NULL，否则为NULL
  \param[in]  *pcName: 要查询的域名
  \param[out]  **ppstV4Result: A查询的返回值
  \param[out]  **ppstV6Result: AAAA查询的返回值
  \return 失败时返回值小于0，成功大于0
*/
int ATP_UTIL_Lookup_A_and_AAAA(const char *pcName, struct hostent **ppstV4Result, struct hostent **ppstV6Result)
{
    static struct hostent stV4Buff;
    static struct hostent stV6Buff;
    memset(&stV4Buff, 0, sizeof(stV4Buff));
    memset(&stV6Buff, 0, sizeof(stV6Buff));
    DPRINTF("%p\n", &stV4Buff);
    DPRINTF("%p\n", &stV6Buff);
    if ((NULL == pcName) || (NULL == ppstV4Result) || (NULL == ppstV6Result))
    {
        return -1;
    }
    if ((strlen(pcName) == 0) || (pcName[0] == '.'))//不对根域名发起查询
    {
        printf("Don't query <Root>\n");
        return -1;
    }
    *ppstV4Result = NULL;
    *ppstV6Result = NULL;
    return LookUp_A_and_AAAA(pcName, &stV4Buff, &stV6Buff, ppstV4Result, ppstV6Result);
}

#ifdef SUPPORT_ATP_FON
//封装atp_util_gethostbyname,fon进程的查询全部绑定br0地址，区分私网流量
struct hostent *ATP_FON_Gethostbyname(const char *name, int family, int *pbnosuchname)
{
    VOS_INT             lSockfd;
    struct ifreq        stIfr;
    VOS_CHAR acIP[16];
    struct sockaddr_in* pstaddr = NULL;
    int ret = 0;
    static struct hostent h;
    static char buf[sizeof(struct in6_addr) +
                          sizeof(struct in6_addr *)*2 +
                          sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
    struct hostent *hp;

    
    lSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == lSockfd)
    {
        goto error;
    }
    memset(&stIfr, 0, sizeof(stIfr));
    snprintf(stIfr.ifr_name, sizeof(stIfr.ifr_name), "%s", DNS_FON_BIND_IF);
    /*get lan ip*/
    if (ioctl(lSockfd, SIOCGIFADDR, &stIfr) < 0)
    {
        close(lSockfd);
        goto error;
    }
    close(lSockfd);
    memset(acIP, 0, sizeof(acIP));
    pstaddr = (struct sockaddr_in*)&stIfr.ifr_addr;
    inet_ntop(AF_INET, &(pstaddr->sin_addr), acIP, sizeof(acIP));

    if ((strlen(name) == 0) || name[0] == '.')//不对根域名发起查询
    {
        printf("Don't query <Root>\n");
        goto error;
    }

    ret = handy_gethostbyname2_r(name, family, &h, buf, sizeof(buf), &hp, acIP);
    if (NETDB_SUCCESS != ret)
    {
        goto error;
    }

    return hp;    

error:   
    if (pbnosuchname)
    {
        if (RET_NO_SUCH_NAME == ret)
            *pbnosuchname = 1;
        else
            *pbnosuchname = 0;
    }
    
    return NULL;
}
#endif


struct addrinfo *ATP_UTIL_MakeAddrInfo(
                                VOS_UINT32               ulSockFlag,
                                const struct sockaddr   *pstSockAddr,
                                socklen_t                      socklen)
{
    struct addrinfo     *pstRet;

    pstRet = (struct addrinfo *)malloc(sizeof(struct addrinfo) + socklen);
    if (NULL == pstRet)
    {
        return NULL;
    }
    memset((struct addrinfo *)pstRet, 0, (sizeof(struct addrinfo) + socklen));

    pstRet->ai_family = AF_INET;
    pstRet->ai_socktype = SOCK_STREAM;
    if (0 != (ulSockFlag & ATP_SOCK_IP_VER))
    {
        pstRet->ai_family = AF_INET6;
    }
    if (0 != (ulSockFlag & ATP_SOCK_UDP))
    {
        pstRet->ai_socktype = SOCK_DGRAM;
    }
    pstRet->ai_protocol = IPPROTO_IP;

    pstRet->ai_addrlen = socklen;
    pstRet->ai_addr = (struct sockaddr *)(pstRet + 1);
    memcpy((VOS_VOID *)(pstRet + 1), (VOS_VOID *)pstSockAddr, socklen);

    return pstRet;
}


struct addrinfo *ATP_UTIL_getaddrinfo(const char *name, int family, unsigned int port, char *bindaddr)
{
    unsigned int      ulIdx;
    unsigned int      ulSockFlag;
    socklen_t          len;
    struct addrinfo *pstRet;
    struct addrinfo *pstLast;
    struct addrinfo *pstTemp;
    struct hostent   *pstIpAddress;
    struct sockaddr_storage stAddr;
    struct sockaddr_in   *pstAddr;
    struct sockaddr_in6 *pstAddrV6;

    pstIpAddress = ATP_UTIL_Gethostbyname(name, family, bindaddr);
    if (NULL == pstIpAddress)
    {
        return NULL;
    }

    ulSockFlag = 0;
    if (AF_INET6 == family)
    {
        ulSockFlag = 0x04;  //ATP_SOCK_IP_VER;
    }

    ulIdx    = 0;
    pstRet  = NULL;
    pstLast = NULL;
    while (NULL != pstIpAddress->h_addr_list[ulIdx])
    {
        memset((void *)(&stAddr), 0, sizeof(stAddr));
        if (AF_INET6 == family)
        {
            pstAddrV6 = (struct sockaddr_in6 *)(&stAddr);
            len = sizeof(struct sockaddr_in6);
            pstAddrV6->sin6_family = (unsigned short)family;
            pstAddrV6->sin6_port = htons((unsigned short)port);
            memcpy((void *)(&(pstAddrV6->sin6_addr.s6_addr)),(void *)(pstIpAddress->h_addr_list[ulIdx]), 16);
        }
        else
        {
            pstAddr = (struct sockaddr_in *)(&stAddr);
            len = sizeof(struct sockaddr_in);
            pstAddr->sin_family = (unsigned short)family;
            pstAddr->sin_port = htons((unsigned short)port);
            memcpy((void *)(&(pstAddr->sin_addr.s_addr)), (void *)(pstIpAddress->h_addr_list[ulIdx]), 4);
        }
        pstTemp = ATP_UTIL_MakeAddrInfo(ulSockFlag, (const struct sockaddr *)(&stAddr), len);
        if (NULL == pstTemp)
        {
            if (NULL != pstRet)
            {
                freeaddrinfo(pstRet);
            }
            return NULL;
        }
        if ((NULL == pstRet) || (NULL == pstLast))
        {
            pstRet  = pstTemp;
            pstLast = pstRet;
        }
        else
        {
            pstLast->ai_next = pstTemp;
            pstLast = pstTemp;
        }
        ulIdx++;
    }

    return pstRet;
}

#define WAN6_STATUS_FILE "/var/wan/ppp256/status6"
#define WAN4_STATUS_FILE "/var/wan/ppp256/status"
#define WAN_STATUS_CONNECTED 3

VOS_BOOL ATP_UTIL_IsBondingEnable()
{
    VOS_INT32   lDial = 0;
    FILE* pstFile = VOS_NULL;
    VOS_CHAR acLine[ATP_STRING_LEN_32] = {0};
    
    pstFile = fopen(HYBRID_LTE_ENABLE_FILE, "r");
    if (NULL == pstFile)
    {
        return VOS_FALSE;
    }
    if (fgets(acLine, sizeof(acLine), pstFile))
    {
        lDial = atol(acLine);
    }
    fclose(pstFile);
    return (VOS_TRUE == lDial);
}

/*!
  \brief W724v函数用于判断LTE V6的wan是否up
  \param[in] void: 无
  \return 
*/
VOS_BOOL ATP_UTIL_IsLTEWanV6Up(void)
{
    FILE        *pFile = NULL;
    VOS_CHAR    acLine[ATP_STRING_LEN_256] = {0};
    
    pFile = fopen(LTE_WAN6_IPADDR_FILE, "r");
	
    if (NULL == pFile) 
    {
        return VOS_FALSE;
    }
	
    if (NULL == fgets(acLine, sizeof(acLine), pFile))
    {
        fclose(pFile);
        return VOS_FALSE;
    }
	
    fclose(pFile);
	
    return VOS_TRUE;
}

VOS_BOOL ATP_UTIL_IsExistDefaultRoute(VOS_BOOL isIpv6)
{
    FILE* fp = VOS_NULL_PTR;
    VOS_CHAR acFile[ATP_BUFFER_512] = {0};
    VOS_CHAR acCmd[ATP_STRING_LEN_256] = {0};
    VOS_CHAR acLine[ATP_BUFFER_512] = {0};
    VOS_BOOL bRet = VOS_FALSE;

    if (isIpv6)
    {
        snprintf(acFile, sizeof(acFile), "%s", ROUTE6_STATUS_FILE);
        snprintf(acCmd, sizeof(acCmd), "ip -6 ro > %s", ROUTE6_STATUS_FILE);  
    }
    else
    {
        snprintf(acFile, sizeof(acFile), "%s", ROUTE_STATUS_FILE);
        snprintf(acCmd, sizeof(acCmd), "ip ro > %s", ROUTE_STATUS_FILE);

    }
    system(acCmd); 
    fp = fopen(acFile, "r");      
    if (VOS_NULL_PTR == fp)
    {
        printf("\r\n ATP_UTIL_IsExistDefaultRoute: Open route_file is error.\r\n");        
        return bRet;
    }

    while (VOS_NULL_PTR != fgets(acLine, ATP_BUFFER_512,fp))
    {
        if (VOS_NULL_PTR != strstr(acLine, "default"))
        {
            bRet = VOS_TRUE;
            break;
        }
    }

	if (VOS_FALSE == bRet)
	{
        printf("\r\n ATP_UTIL_IsExistDefaultRoute: no default route in [%s].\r\n", acFile);        
	}

    fclose(fp);
    remove(acFile);
    return bRet;
}

/*!
  \brief W724v函数用于判断LTE V4的wan是否up
  \param[in] void: 无
  \return 
*/
VOS_BOOL ATP_UTIL_IsLTEWanV4Up(void)
{
    FILE        *pFile = NULL;
    VOS_CHAR    acLine[ATP_STRING_LEN_256] = {0};
    VOS_CHAR    *pszTmp = NULL;
    
    pFile = fopen(LTE_WAN4_IPADDR_FILE, "r");
    if (NULL == pFile) 
    {
        return VOS_FALSE;
    }
    
    pszTmp = fgets(acLine, sizeof(acLine), pFile);
    if (NULL == pszTmp)
    {
        fclose(pFile);
        return VOS_FALSE;
    }
    fclose(pFile);
    return VOS_TRUE;
}


/*!
  \brief W724v函数用于判断V6的wan是否up
  \param[in] void: 无
  \return 
*/
VOS_BOOL ATP_UTIL_IsWanV6Up(void)
{
    FILE        *pFile = NULL;
    VOS_INT32   lDial = 0;
    VOS_CHAR    acLine[ATP_STRING_LEN_256] = {0};
    
    pFile = fopen(WAN6_STATUS_FILE, "r");
    if (NULL == pFile) 
    {
        return VOS_FALSE;
    }
    if (fgets(acLine, sizeof(acLine), pFile))
    {
        lDial = atol(acLine);
    }
    fclose(pFile);
    return (WAN_STATUS_CONNECTED == lDial);
}


/*!
  \brief W724v函数用于判断V4的wan是否up
  \param[in] void: 无
  \return 
*/
VOS_BOOL ATP_UTIL_IsWanV4Up(void)
{
    FILE        *pFile = NULL;
    VOS_INT32   lDial = 0;
    VOS_CHAR    acLine[ATP_STRING_LEN_256] = {0};
    
    pFile = fopen(WAN4_STATUS_FILE, "r");
    if (NULL == pFile) 
    {
        return VOS_FALSE;
    }
    if (fgets(acLine, sizeof(acLine), pFile))
    {
        lDial = atol(acLine);
    }
    fclose(pFile);
    return (WAN_STATUS_CONNECTED == lDial);
}

#define ATP_GRE2_STATUS6_FILE    "/var/wan/gre2/status6"
#define ATP_GRE2_STATUS_FILE     "/var/wan/gre2/status"
VOS_BOOL ATP_UTIL_IsLTETunnel4Up(void)
{ 
    FILE        *pstFile = VOS_NULL;
    VOS_CHAR    acStatus4[ATP_ACTUAL_LEN_32] = {0}; 
    VOS_UINT32  ulLTETunnelStatus4 = 0;  
    
    pstFile = fopen(ATP_GRE2_STATUS_FILE, "r");
    if (VOS_NULL != pstFile)
    {
        fgets(acStatus4, sizeof(acStatus4), pstFile);
        ulLTETunnelStatus4 = (VOS_UINT32)atoi(acStatus4);
        fclose(pstFile);
        pstFile = VOS_NULL;
    }        
            
    return (WAN_STATUS_CONNECTED == ulLTETunnelStatus4);   
}

VOS_BOOL ATP_UTIL_IsLTETunnel6Up(void)
{ 
    FILE        *pstFile = VOS_NULL;   
    VOS_CHAR    acStatus6[ATP_ACTUAL_LEN_32] = {0};    
    VOS_UINT32  ulLTETunnelStatus6 = 0;          
    
    pstFile = fopen(ATP_GRE2_STATUS6_FILE, "r");
    if (VOS_NULL != pstFile)
    {   
        fgets(acStatus6, sizeof(acStatus6), pstFile);
        ulLTETunnelStatus6 = (VOS_UINT32)atoi(acStatus6);
        fclose(pstFile);
        pstFile = VOS_NULL;
    }
           
    return (WAN_STATUS_CONNECTED == ulLTETunnelStatus6);

}
#define ATP_SYS_READ_BUF_SIZE 128
#define ATP_SYS_MEDIUM_LEN 240

/******************************************************************************
  函数名称  : GPL_UTIL_RmDelimitor
  功能描述  : 把字符串中的'\0'去掉
  输入参数  : 
              1.  *pcBuf: 待去除'\0'的字符串
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 去除'\0'成功则返回真VOS_TRUE， 
              去除'\0'失败则返回假VOS_FALSE，

  修改历史     :
  1.日    期   : 2009-02-14
    作    者   : z65940
    修改内容   : 剥离VTOP OS适配函数
******************************************************************************/
static VOS_BOOL GPL_UTIL_RmDelimitor( VOS_CHAR *pcBuf)
{
    VOS_BOOL  bRet = VOS_TRUE;    
    VOS_INT   i = 0;
    VOS_INT   j = 0;


    if ( pcBuf == VOS_NULL ) 
    {
        return VOS_FALSE;
    }

    while((*(pcBuf + i) != '\0') || (*(pcBuf + i + 1) != '\0'))
    {
        if(*(pcBuf + i) == '\0')
        {
            i++;
            continue;
        }
        *(pcBuf + j) = *(pcBuf + i);
        j++;
        i++;
    }
    *(pcBuf + j) = '\0';

    return bRet;

}
/******************************************************************************
  函数名称  : GPL_UTIL_FindPidByName
  功能描述  : 根据进程名得到进程号，如果有多个匹配则会列出所有匹配的进程ID
  输入参数  : 
              1.  pszPidName: 进程名
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 返回值不为NULL表示查找成功；返回值为NULL表示查找失败

  修改历史     :
  1.日    期   : 2009-02-14
    作    者   : z65940
    修改内容   : 剥离VTOP OS适配函数

******************************************************************************/
static VOS_INT32* GPL_UTIL_FindPidByName( VOS_CHAR* pszPidName)
{
    DIR    *pDir;
    struct dirent *pNext = VOS_NULL;
    VOS_INT32*   pPidList = VOS_NULL;
    VOS_UINT32    i = 0;

    FILE *pCmdline = VOS_NULL;
    VOS_CHAR    acFilename[ATP_SYS_READ_BUF_SIZE];
    VOS_CHAR    acBuffer[ATP_SYS_READ_BUF_SIZE];

    if ( pszPidName == VOS_NULL ) 
    {
        return VOS_NULL;
    }

#ifdef ATP_PCLINT
    pszPidName = pszPidName;
#endif

    pDir = opendir("/proc");
    if (!pDir) 
    {
        printf("cms:Cannot open /proc");
        return VOS_NULL;
    }

    while ((pNext = readdir(pDir)) != VOS_NULL) 
    {
        /* 重新初始化 */
        memset(acFilename, 0, sizeof(acFilename));
        memset(acBuffer, 0, sizeof(acBuffer));  

        /* 从/proc中过滤掉".."*/
        if (strcmp(pNext->d_name, "..") == 0)
        {
            continue;
        }

        /* 进程名字必须是数字 */
        if (!isdigit(*pNext->d_name))
        {
            continue;
        }

        snprintf(acFilename, sizeof(acFilename), "/proc/%s/cmdline", pNext->d_name);
        if (VOS_NULL == (pCmdline = fopen(acFilename, "r")) ) 
        {
            continue;
        }
        if (fgets(acBuffer, ATP_SYS_READ_BUF_SIZE-1, pCmdline) == VOS_NULL) 
        {
            fclose(pCmdline);
            continue;
        }
        fclose(pCmdline);

        /* acBuffer里面数据样例 "Name:   binary_name" */
        /*把'\0'去除*/
        GPL_UTIL_RmDelimitor(acBuffer);
        if (strstr(acBuffer, pszPidName) != VOS_NULL) 
        {
            pPidList = (VOS_INT32*)realloc( pPidList, sizeof(VOS_INT32) * (i + 2));
            if (!pPidList) 
            {
                printf("cms: Out of memeory!\n");
				closedir(pDir);
                return VOS_NULL;
            }
                pPidList[i++] = strtol(pNext->d_name, VOS_NULL, 0);
        }
    }
    closedir(pDir);

    if (pPidList)
    {
        pPidList[i] = 0;
    }
    else if ( strcmp(pszPidName, "init") == 0) 
    {
        /* init进程认为进程ID:1 */
        pPidList = (VOS_INT32*)realloc( pPidList, sizeof(VOS_INT32));
        if (!pPidList) 
        {
            printf("cms: Out of memeory!\n");
            return VOS_NULL;
        }
        pPidList[0] = 1;
    } 
    else 
    {
        pPidList = (VOS_INT32*)realloc( pPidList, sizeof(VOS_INT32));
        if (!pPidList) 
        {
            printf("cms: Out of memeory!\n");
            return VOS_NULL;
        }
        pPidList[0] = -1;
    }
    return pPidList;
}


/******************************************************************************
  函数名称  : GPL_UTIL_GetPid
  功能描述  : 根据命令获取到进程号
  输入参数  : 
              1.  pcCommand: 命令名
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 非0表示查找成功；0表示查找失败或者不存在

  修改历史     :
  1.日    期   : 2009-02-14
    作    者   : z65940
    修改内容   : 剥离VTOP OS适配函数


******************************************************************************/
VOS_INT GPL_UTIL_GetPid(VOS_CHAR * pcCommand)
{
    VOS_CHAR   acCmdline[ATP_SYS_MEDIUM_LEN];
    VOS_CHAR   *pcTmp1 = VOS_NULL;
    VOS_CHAR   *pcTmp2 = VOS_NULL;
    pid_t *pPid = VOS_NULL;
    VOS_INT    lRet = 0;

    if ( pcCommand == VOS_NULL )
    {
        return lRet;
    }
    
    pcTmp1 = pcCommand;
    pcTmp2 = acCmdline;
    while ( *pcTmp1 != '\0') 
    {
        if (*pcTmp1 != ' ') 
        {
            *pcTmp2 = *pcTmp1;
            pcTmp2++;
        }
        pcTmp1++;
    }
    *pcTmp2='\0';

    /* pPid内存分配由ATP_UTIL_FindPidByName分配 */
    pPid = GPL_UTIL_FindPidByName(acCmdline);
    if ( pPid != VOS_NULL ) 
    {
       lRet = (VOS_INT)(*pPid);
       free(pPid);
       pPid = VOS_NULL;
    }

    return lRet;
}

/*Start of Protocol 2014-3-25 16:22 for 路由机制修改 by t00189036*/
#ifdef SUPPORT_ATP_HYBRID_BONDING
/******************************************************************************
  函数名称  : ATP_UTIL_SetFilterlistControlMark
  功能描述  : 设置Filterlist可控的mark标记
  输入参数  : 
              1.  ulSockfd:待设置的fd
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 是否设置成功

  修改历史      :
   1.日    期   : 2014-3-24
     作    者   : t00189036
     修改内容   : 完成初稿

******************************************************************************/
VOS_INT32 ATP_UTIL_SetFilterlistControlMark(VOS_INT32 ulSockfd)
{
    VOS_UINT32  ulMark = NON_BLACKLIST_MARK;
    return setsockopt(ulSockfd, SOL_SOCKET, SO_MARK, &ulMark, sizeof(ulMark));
}

static VOS_INT InitIPv4Socket(const ATP_UTIL_PKT_INFO_ST *pstPkt)
{
    VOS_INT lFd = 0;
    struct sockaddr_in stSrcAddr;
    struct sockaddr_in stDstAddr;

    /*1、创建socket*/
    lFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (lFd < 0)
    {
        return lFd;
    }
    
    /*2、设置源相关信息*/
    memset(&stSrcAddr, 0, sizeof(stSrcAddr));
    stSrcAddr.sin_family = AF_INET;    
    stSrcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (pstPkt->usSrcPort)
    {
        stSrcAddr.sin_port = htons(pstPkt->usSrcPort);
    }    
    if (bind(lFd, (struct sockaddr *)&stSrcAddr, sizeof(stSrcAddr)) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Bind socket fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }

    /*3、设置目的相关信息*/
    stDstAddr.sin_family = AF_INET; 
    if (inet_aton(pstPkt->acDstIP, &stDstAddr.sin_addr) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Get dst ip fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }
    if (pstPkt->usDstPort)
    {
        stDstAddr.sin_port = htons(pstPkt->usDstPort);
    }
    if (connect(lFd, (struct sockaddr *)&stDstAddr, sizeof(stDstAddr)) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Connect socket fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }
    
    return lFd;
}

static VOS_INT InitIPv6Socket(const ATP_UTIL_PKT_INFO_ST *pstPkt)
{
    VOS_INT lFd = 0;
    struct sockaddr_in6 stSrcAddr6;
    struct sockaddr_in6 stDstAddr6;

    /*1、创建socket*/
    lFd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (lFd < 0)
    {
        return lFd;
    }

    /*2、设置源地址信息*/
    memset(&stSrcAddr6, 0, sizeof(stSrcAddr6));
    stSrcAddr6.sin6_family = AF_INET6;    
    if (pstPkt->usSrcPort)
    {
        stSrcAddr6.sin6_port = htons(pstPkt->usSrcPort);
    }    
    if (bind(lFd, (struct sockaddr *)&stSrcAddr6, sizeof(stSrcAddr6)) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Bind socket fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }

    /*3、设置目的地址信息*/
    memset(&stDstAddr6, 0, sizeof(stDstAddr6));
    stDstAddr6.sin6_family = AF_INET6;   
    if (inet_pton(AF_INET6, pstPkt->acDstIP, &stDstAddr6.sin6_addr) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Get dst ip fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }
    if (pstPkt->usDstPort)
    {
        stDstAddr6.sin6_port = htons(pstPkt->usDstPort);
    }
    if (connect(lFd, (struct sockaddr *)&stDstAddr6, sizeof(stDstAddr6)) < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Connect socket fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
        close(lFd);
        return -1;
    }
    return lFd;
}

/******************************************************************************
  函数名称  : ATP_UTIL_GetOutputIntf
  功能描述  : 根据报文特征获取报文发送接口
  输入参数  : 
              1.  *pstPkt:报文特征。
              入参请保证和业务本身要发送的报文信息准确一致，信息不一致会导致某些场景下获取到的接口错误
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 返回发包接口名，找不到发包接口时返回NULL

  修改历史      :
   1.日    期   : 2014-4-21
     作    者   : t00189036
     修改内容   : 完成初稿

******************************************************************************/
VOS_CHAR* ATP_UTIL_GetOutputIntf(const ATP_UTIL_PKT_INFO_ST *pstPkt)
{
    VOS_CHAR *pcBuf = "This is a test packet to get output intf!";
    VOS_INT lFd = 0;    
    VOS_UINT32 ulMark = 0;
    VOS_UINT lRet = 0;     
    struct in_addr stAddr4;
    static VOS_CHAR acOutputIntf[ATP_STRING_LEN_32] = {0};
    VOS_BOOL bIsIPv4 = VOS_TRUE;
    VOS_INT lTcClass = 0;

    if ((VOS_NULL == pstPkt) || (0 == pstPkt->acDstIP[0]))
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Invalid input para.\n", getpid(), __func__, __LINE__);
        return VOS_NULL;
    }
    if (IPPROTO_UDP != pstPkt->ucProtocol)
    {
        /*目前只用到udp*/
        printf("[PID:%d, FUNC:%s, LINE: %d] Not support protocol %d.\n", getpid(), __func__, __LINE__, pstPkt->ucProtocol);
        return VOS_NULL;
    }
    
    /*1、初始化socket*/
    memset(&stAddr4, 0, sizeof(stAddr4));    
    if (inet_aton(pstPkt->acDstIP, &stAddr4) > 0)
    {
        lFd = InitIPv4Socket(pstPkt);
    }
    else
    {
        bIsIPv4 = VOS_FALSE;
        lFd = InitIPv6Socket(pstPkt);
    }
    if (lFd < 0)
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Init socket fail. Error info:%s.\n", getpid(), __func__, __LINE__, strerror(errno));
    }
    
    /*2、设置其他信息*/
    ulMark = (pstPkt->ulMark | ATP_CONNTRACK_GET_OUTPUT_TEST_MARK | NON_BLACKLIST_MARK);
	//lint -save -e40 : Undeclared identifier 'SO_MARK'. It's defined in asm/socket.h
    (VOS_VOID)setsockopt(lFd, SOL_SOCKET, SO_MARK, &ulMark, sizeof(ulMark));
	//lint -restore

    if (pstPkt->ucTos)
    {
        if (bIsIPv4)
        {
            (VOS_VOID)setsockopt(lFd, IPPROTO_IP, IP_TOS, &pstPkt->ucTos, sizeof(pstPkt->ucTos));
        }
        else
        {
            lTcClass = (VOS_INT)pstPkt->ucTos;
            (VOS_VOID)setsockopt(lFd, IPPROTO_IPV6, IPV6_TCLASS, &lTcClass, sizeof(lTcClass));
        }
    }

    /*3、发送报文*/
    lRet = send(lFd, pcBuf, strlen(pcBuf), 0);
    printf("[PID:%d, FUNC:%s, LINE: %d] Error %d lRet:%d.\n", getpid(), __func__, __LINE__, errno, lRet);
    close(lFd);

    /*4、根据特殊lRet值解析出接口*/
    if ((lRet <= ATP_SENDTO_RET_INDEX_SHIFT_MIN) && (lRet >= ATP_SENDTO_RET_INDEX_SHIFT_MAX))
    {
        printf("[PID:%d, FUNC:%s, LINE: %d] Error %d lRet:%d.\n", getpid(), __func__, __LINE__, errno, ATP_SENDTO_RET_INDEX_SHIFT_MIN - lRet);
        if_indextoname(ATP_SENDTO_RET_INDEX_SHIFT_MIN - lRet, acOutputIntf);
        return acOutputIntf;  
    }
    
    return VOS_NULL;
}
#endif
/*End of Protocol 2014-3-25 16:22 for by t00189036*/