#ifndef __WEBDAVCLIENT_H__
#define __WEBDAVCLIENT_H__

#include "atptypes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* CADAVER working mode */
#define ATP_WEBDAV_COMMAND_STATE (0)  /* working as a shell command*/
#define ATP_WEBDAV_WEBDAV_STATE  (1)  /* working as webdav client for nas*/
#define ATP_WEBDAV_SUBCAD_STATE  (2)  /* working as sub-cadaver for task center*/

#define ATP_WEBDAV_MANNUAL_AUTH  (0)
#define ATP_WEBDAV_SERVER_AUTH   (1)
#define ATP_WEBDAV_PROXY_AUTH    (2)

#define ATP_CADAVER_STARTUP_PARAM_NUMBER 3

#define ATP_CADAVER_PROCESSNAME               "cadaver"
#define ATP_CADAVER_PROCESS_SYNC_PRENAME      "sync"
#define ATP_CADAVER_PROCESS_BACK_PRENAME      "backup"
#define ATP_CADAVER_PROCESS_SUBCADAVER        "subcadaver"


#define ATP_WEBDAV_WAIT_SYN_TIME (VOS_UINT32)(30000) 
#define ATP_WEBDAV_WAIT_MSG_TIME (VOS_UINT32)(0xffffffff)  

#define ATP_PROPERTY_RESOURCETYPE        "resourcetype"
#define ATP_PROPERTY_GETLASTMODIFIED     "getlastmodified"
#define ATP_PROPERTY_GETCONTENTLENGTH    "getcontentlength"

VOS_VOID ATP_NAS_WebDavClient_Entry(VOS_INT argc, VOS_CHAR **argv);
VOS_VOID ATP_NAS_FindOutCadaverWorkMode(int *mode, int argc, char **argv);
VOS_VOID ATP_NAS_GetCurrentWorkMode(int *mode);
int cadaver_judge_ppp256(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __WEBDAVCLIENT_H__ */

