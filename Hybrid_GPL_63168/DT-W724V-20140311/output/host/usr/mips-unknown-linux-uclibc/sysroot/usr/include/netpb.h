#ifndef __NETPB_API_H__
#define __NETPB_API_H__

#define LA_NETPB_AESKEY_RELEASE "7CFLtXCQ9QXsb1EV"  /*DT提供的正式Key,用于发货*/
#define LA_NETPB_AESKEY_DEBUG   "HQn4atR7iyfqKma9"  /*DT提供的临时Key,用于调试*/

#define NETPB_REPLY_ERR          "/var/voice/netpb/responseErr"

#define NETPB_EXIT_NORMAL     0
#define NETPB_EXIT_ABNORMAL   1

/* ftpget ftpput tftp wget传输工具的返回码 */
#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

#define NETPB_ERROR_403  403
#define NETPB_ERROR_500  500
#define NETPB_ERROR_001  001
#define NETPB_NOERROR    0

#define NETPB_MAX_FILE_LEN    500*1024

#define NETPB_PARA_VALUE_NULL "ValueNull"

typedef enum tagNETPB_RETURN_EN
{
    NETPB_SUCC = 0,
    NETPB_ERROR,
    NETPB_ERROR_PARAM,
}NETPB_RETURN_EN;

typedef enum tagNETPB_CALLER_NAME_EN
{
	NETPB_CALLER_NAME_NULL ,
	NETPB_CALLER_NAME_VOICE,
	
}NETPB_CALLER_NAME_EN;

typedef struct tagNETPB_REQUEST_ST
{
	VOS_CHAR 	acServerName[ATP_STRING_LEN_128];
	VOS_CHAR 	acServerPath[ATP_STRING_LEN_128];
	VOS_CHAR 	acLocalPath[ATP_STRING_LEN_64];
	VOS_CHAR 	acReqUrlPara[ATP_STRING_LEN_512];
	VOS_CHAR 	acUserName[ATP_STRING_LEN_64];
	VOS_CHAR 	acUserPassWord[ATP_STRING_LEN_64];
}NETPB_REQUEST_ST;

typedef struct tagNETPB_RESPONSE_ST
{
    VOS_CHAR 	acExtraArg[32];
    VOS_CHAR 	acFileName[32];
    VOS_UINT32  ulResponse;
    VOS_INT32   lErrCode;
}NETPB_RESPONSE_ST;

typedef enum tagNETPB_PARA_EN
{
	NETPB_REQ_PARA_SERVERNAME = 1,
	NETPB_REQ_PARA_SERVERPATH,
	NETPB_REQ_PARA_LOCALPATH,
	NETPB_REQ_PARA_URLPARA,
	NETPB_REQ_PARA_USERNAME,
	NETPB_REQ_PARA_USRPASSWORD,
	NETPB_REQ_PARA_CALLERNAME,
	NETPB_REQ_PARA_EXTRAARG,
	NETPB_REQ_PARA_TESTMODE,
}NETPB_PARA_EN;

#endif

