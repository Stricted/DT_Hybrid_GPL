#ifndef __CADAVERTYPES_H__
#define __CADAVERTYPES_H__

#include "atptypes.h"
#include "msgapi.h"
#include "nasio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define  ATP_NAS_USER_NAME_MAXLEN    (VOS_UINT32)256
#define  ATP_NAS_USER_PASSWD_MAXLEN  (VOS_UINT32)256

#define  ATP_NAS_PROXY_URI_LEN       (VOS_UINT32)512

#define ATP_WEBDAV_ROOT_PATH       "/"

#define ATP_NAS_AVAILABLE_SPACE    "quota-available-bytes"    /*�ռ���ô�С*/
#define ATP_NAS_USED_SPACE         "quota-used-bytes"         /*�ռ����ô�С*/

/* error code */
#define  ATP_WEBDAV_RET_OK                  (VOS_UINT32)(0x00000000)
#define  ATP_WEBDAV_PARAM_ERR               (VOS_UINT32)(0x00000001)
#define  ATP_WEBDAV_NULL_PTR_ERR            (VOS_UINT32)(0x00000002) 
#define  ATP_WEBDAV_REGISTER_PROC_ERR       (VOS_UINT32)(0x00000003)
#define  ATP_WEBDAV_SYNC_WITH_FPROC_ERR     (VOS_UINT32)(0x00000004)
#define  ATP_WEBDAV_OPTIONS_NOFOUND_ERR     (VOS_UINT32)(0x00000005)
#define  ATP_WEBDAV_MALLOC_MEMORY_ERR       (VOS_UINT32)(0x00000006)
#define  ATP_WEBDAV_URI_PARSE_ERR           (VOS_UINT32)(0x00000007)
#define  ATP_WEBDAV_SSL_NOTSUPPORT_ERR      (VOS_UINT32)(0x00000008)
#define  ATP_WEBDAV_LONG_NUMBER_ERR         (VOS_UINT32)(0x00000009)
#define  ATP_WEBDAV_MOVE_ERR                (VOS_UINT32)(0x0000000A)
#define  ATP_WEBDAV_UNCORRECT_MSGTYPE_ERR   (VOS_UINT32)(0x0000000B)
#define  ATP_WEBDAV_PROXY_INFO_ERR          (VOS_UINT32)(0x0000000C)

/* HTTP RETCODE */
#define ATP_NAS_HTTP_200_OK                 (VOS_UINT32)(200)
#define ATP_NAS_HTTP_404_NOT_FOUND          (VOS_UINT32)(404)
#define ATP_NAS_HTTP_401_SERVER_AUTH        (VOS_UINT32)(401)
#define ATP_NAS_HTTP_407_PROXY_AUTH         (VOS_UINT32)(407)
#define ATP_NAS_HTTP_409_CONFLICT           (VOS_UINT32)(409)

/*Mediencenter�û���Ϣ*/
typedef struct tagMedUserInfo
{
    VOS_CHAR    chUserName[ATP_NAS_USER_NAME_MAXLEN];
    VOS_CHAR    chPassword[ATP_NAS_USER_PASSWD_MAXLEN];
    VOS_UINT32  nAllowdConnect;
}USERINFO, *PUSERINFO, **PPUSERINFO;

/*Proxy������Ϣ*/
typedef struct tagNasProxyInfo
{
    VOS_CHAR    chUserName[ATP_NAS_USER_NAME_MAXLEN];     
    VOS_CHAR    chPassword[ATP_NAS_USER_PASSWD_MAXLEN];   
    VOS_CHAR    chProxyUri[ATP_NAS_PROXY_URI_LEN];        //e.g: proxycn2.huawei.com:8080
}NASPROXY, *PNASPROXY, **PPNASPROXY;

/*���ô���������Ϣ��*/
typedef struct tagNasProxyRequest
{
    ATP_MSG_HEADER_ST stMsgHead; 
    NASPROXY          stProxy;
}ATP_NAS_PROXY_REQUEST_MSG;

/*��¼mediencenter������Ϣ�ṹ��*/
typedef struct tagNasLogRequest
{
    ATP_MSG_HEADER_ST stMsgHead;
    USERINFO          stUser;
}ATP_NAS_LOGON_REQUEST_STRUCT;

/*��ȡmediencenter�Ͽռ���Ϣ��Ӧ��Ϣ�ṹ��*/
typedef struct tagNasStorageSpaceRequest
{
    ATP_MSG_HEADER_ST stMsgHead;
    VOS_UINT64        availSpace;
    VOS_UINT64        usedSpace;
}ATP_NAS_SPACE_REQUEST_STRUCT;

/*��ȡmediencenter��ĳresource��mtime��Ϣ��Ӧ�ṹ��*/
typedef struct tagNasResourceMtimeRequest
{
    ATP_MSG_HEADER_ST stMsgHead;
    time_t lastmodified;
}ATP_NAS_MTIME_REQUEST_STRUCT;

/*��ȡmediencenter��ĳ�ļ��д�С����Ӧ��Ϣ�ṹ��*/
typedef struct tagNasFolderSpaceRequest
{
    ATP_MSG_HEADER_ST stMsgHead;
    VOS_UINT64        ullSpace;
}ATP_NAS_FOLDER_SPACE_STRUCT;
        
#ifdef __cplusplus
#if __cplusplus
        }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
        
#endif  /* __NASIO_H__ */

