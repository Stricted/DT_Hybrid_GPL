#ifndef __WGET_CMS_API_H__
#define __WGET_CMS_API_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include "atputil.h"
#include "atpconfig.h"
#include "httpapi.h"

#ifdef ATP_DEBUG
#define WGET_DEBUG(format, args...)  {printf("WGET DEBUG============file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define WGET_DEBUG(format, args...)
#endif


#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

/* ftpget ftpput tftp wget传输工具的命令行选项 */
#define TRANS_TOOL_OPT_DOWNLOAD        1
#define TRANS_TOOL_OPT_UPLOAD        2
#define TRANS_TOOL_OPT_VERBOSE        4
#define TRANS_TOOL_OPT_USER            8
#define TRANS_TOOL_OPT_PASSWORD        16
#define TRANS_TOOL_OPT_LOCALPATH    32
#define TRANS_TOOL_OPT_REMOTEPATH    64
#define TRANS_TOOL_OPT_REMOTEPORT    128
#define TRANS_TOOL_OPT_BINDIP       256
#define TRANS_TOOL_OPT_REMOTEIP     512
// 1024,2048,4096
#define TRANS_TOOL_OPT_ZIP            8192

#ifdef SUPPORT_ATP_WGET_IPV6
#define ATP_IPV4_IP     0
#define ATP_IPV6_IP     1
#define ATP_LEN_64      64
#endif

#define HTTP_UPLOAD_BOUNDARY        "HuaweiBusyboxHttpUploader"
#define HTTP_UPLOAD_BOUNDARY_LEN    (25)

#define HTTP_HEADER_MAX_LEN         (1024 * 4)

#define ATP_HTTP_DIRECTIVE_CODE_WGET_PUT     ATP_HTTP_DIRECTIVE_CODE_EXTEND
#define ATP_HTTP_STATUS_CODE_WGET_206        206   



extern VOS_INT32              g_iHttpsMode;

extern VOS_UINT32 HttpClientDoAuthorization(ATP_HTTP_CLIENT_ST        *pstHttpClient,
                                            const ATP_HTTP_PACKET_ST  *pstPacket);

extern VOS_UINT32 HttpClientReadDataBlock(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        VOS_INT32               *plTotalBufLen,
                                        VOS_INT32               lBufOffset,
                                        VOS_INT32               *plBytesRecved,
                                        VOS_INT32               lChunkLen);

VOS_UINT32 HttpClientBackupForNextPacket(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        VOS_CHAR                **ppcRecvBuf,
                                        VOS_INT32               *plBufLen,
                                        VOS_INT32               lTotalRecvd,
                                        VOS_INT32               lTotalProcessed);

typedef struct tagATP_TRANS_GLOBALS 
{
    VOS_UINT32          ulTransEnd;      // 传输结束位置
    VOS_UINT32          ulMaxTrans;      // 下载文件最大大小
    const VOS_CHAR      *pcTransBegin;   // 传输开始地址
    const VOS_CHAR      *pcTransEnd;     // 传输结束地址
    VOS_BOOL            bCompress;

    VOS_UINT32          ulTotalLen;
    VOS_UINT32          ulMcastSeqNum;
    VOS_UINT32          ulMcastLen;
    VOS_UINT8           *pucMcastBuf;

    VOS_UINT32          ulTimoutCnt;
    VOS_CHAR            *pcLocalFile;
} ATP_TRANS_GLOBALS;

typedef struct tagATP_WGET_STK_ST
{
    ATP_HTTP_CLIENT_ST      *pstHttpClient;  
    
    ATP_TRANS_GLOBALS       *pstTransGlobals;
} ATP_WGET_STK_ST;

typedef struct tagATP_WGET_HOST_INFO
{        
    const VOS_CHAR  *pcPath;    
    const VOS_CHAR  *pcLocalIP;      // 本地绑定IP
    VOS_CHAR        *pcHost;
    VOS_CHAR        *pcUserName;
    VOS_CHAR        *pcPassWord;
    VOS_UINT32      ulPort;
    VOS_INT32       iIshttps;
    VOS_CHAR        *pcDomain;
} ATP_WGET_HOST_INFO;

typedef enum tagATP_WGET_HEADER_IDX_EN
{
    ATP_HTTP_HEADER_WGET_RANGE                  = ATP_HTTP_HEADER_EXTEND,
    ATP_HTTP_HEADER_WGET_CONTENTLEN,
};

extern ATP_WGET_STK_ST         *g_pstWgetHdl;


ATP_WGET_STK_ST *ATP_WGET_Init(ATP_TRANS_GLOBALS  *pstTransGlobals);

VOS_VOID WgetDoDownload(ATP_HTTP_CLIENT_ST      *pstHttpClient);

VOS_VOID WgetDoUpload(ATP_HTTP_CLIENT_ST      *pstHttpClient);

VOS_VOID Wget_Safe_Exit(VOS_UINT32 ulRetVal);

#endif
