/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : webdavclient.c
  作    者  : l00203771
  版    本  : V1.0
  创建日期  : 2012-5-22
  描    述  : webdav client
  函数列表  :

  历史记录      :
   1.日    期   : 2012-5-22
     作    者   : l00203771
     修改内容   : 完成初稿

*********************************************************************************/
#define _GNU_SOURCE
#include "atptypes.h"
#include "msgapi.h"
#include "msg/miccoremsgtypes.h"

#include "cadaver.h"
#include "cadavertypes.h"
#include "msg/cadavermsgtypes.h"
#include "webdavclient.h"
#include "src/options.h"
#include "src/commands.h"
#include "ne_string.h"
//lint -save -e717 -e434 -e722 -e818
#ifndef ATP_DEBUG
#ifndef ATP_PCLINT
#define ATP_DEBUG
#endif
#endif

#ifdef ATP_DEBUG
#define ATP_WEBDAV_DEBUG(format, args...)  {fprintf(nas_webdav_debug_stream, "[WEBDAV == line:%d ",__LINE__);fprintf(nas_webdav_debug_stream, format, ##args);fflush(nas_webdav_debug_stream);}
#else
#define ATP_WEBDAV_DEBUG(format, args...)
#endif

#define ATP_NAS_RETRY_TIMES 7

#define ATP_WEBDAV_REQUEST_REPORT(ret) do\
                                       {\ 
                                           memset(&stResp, 0x00, sizeof(stResp));\ 
                                           stResp.ulMsgType = pstMsg->ulMsgType;\
                                           stResp.ulMsgData = ret;\
                                           stResp.ulDataLen = 0;\
                                           snprintf(stResp.aucRecvApp, sizeof(stResp.aucRecvApp), "%s", pstMsg->aucSendApp);\
                                           if(VOS_OK != ATP_MSG_Send(&stResp))\
                                           {\
                                               ATP_MSG_Send(&stResp);\
                                           }\
                                       }while(0)

#define ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, pstPath) do \ 
                                                      {  \
                                                          VOS_UINT32 nPathLen = ((ATP_MSG_HEADER_ST *)pstMsg)->ulDataLen; \
                                                          VOS_CHAR   *tail    = NULL; \
                                                          tail = (VOS_CHAR*)pstMsg + sizeof(ATP_MSG_HEADER_ST) + nPathLen-1; \
                                                          if ( nPathLen > 1 && *tail=='\0') \
                                                          { \
                                                              pstPath = (VOS_CHAR *)((VOS_CHAR *)pstMsg + sizeof(ATP_MSG_HEADER_ST)); \
                                                          } \
                                                          else \
                                                          { \
                                                              pstPath = NULL; \
                                                          } \
                                                      }while(0)

#define ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, pstPath1, pstPath2) do \
                                                                 {  \
                                                                     VOS_UINT32 nPathLen = ((ATP_MSG_HEADER_ST *)pstMsg)->ulDataLen; \
                                                                     VOS_CHAR   *tail    = NULL; \
                                                                     VOS_CHAR   *temp    = NULL; \
                                                                     tail = (VOS_CHAR*)pstMsg + sizeof(ATP_MSG_HEADER_ST) + nPathLen-1; \
                                                                     if (nPathLen>=(VOS_UINT32)4 /*最小得包含2个根目录(4个字节)  '/''\0''/''\0'*/ \
                                                                        && *tail=='\0' ) \
                                                                     { \
                                                                         temp = (VOS_CHAR *)pstMsg + sizeof(ATP_MSG_HEADER_ST); \
                                                                         pstPath1 = temp; \
                                                                         for (; *temp != '\0' && (VOS_UINT32)(temp-pstPath1)<nPathLen; temp++); /*校验合法性*/ \
                                                                         if (*temp == '\0' && (VOS_UINT32)(temp-pstPath1)<nPathLen) \
                                                                         { \
                                                                             pstPath2 = temp+1; \
                                                                             break; \
                                                                         } \
                                                                     } \
                                                                     pstPath1 = NULL; \
                                                                     pstPath2 = NULL; \
                                                                 }while(0)


static FILE *nas_webdav_debug_stream = NULL;
extern struct session session;
extern unsigned int neon_http_status_code;
extern int g_lCadaverPPP256Mode;
    
static VOS_VOID ATP_NAS_WebDavClient_ProcessMsg(ATP_MSG_HEADER_ST *pstMsg, VOS_UINT32 *pExit);
static void ATP_NAS_WebDavDebug_init(FILE *stream);
static VOS_UINT32 ATP_NAS_WebDavClient_Init(VOS_INT32 argc, VOS_CHAR **argv);
static VOS_UINT32 ATP_NAS_WebDavClient_Notify();
static VOS_VOID ATP_NAS_WebDavClient_Clean();
static VOS_CHAR *ATP_NAS_WebDAV_MsgRetParse(VOS_UINT32 retVal);
static VOS_UINT32 ATP_NAS_WebDAV_ProxyProcess(ATP_MSG_HEADER_ST *pstMsg);
static VOS_UINT32 ATP_NAS_UnsetProxy(VOS_VOID);
static VOS_UINT32 ATP_NAS_SetupProxy(VOS_CHAR *ProxyName, VOS_CHAR *ProxyPort, VOS_CHAR *ProxyUserName, VOS_CHAR *ProxyPassword);
static VOS_UINT32 ATP_NAS_LogOn_WebDavServer(ATP_MSG_HEADER_ST *pstMsgHeader);
static VOS_UINT32 ATP_NAS_LogOut_WebDavServer(VOS_VOID);
static VOS_UINT32 ATP_NAS_TRANS_CHAR_TO_LONG_NUMBER(VOS_CHAR *value, VOS_UINT64 *SpaceNum);
static VOS_UINT32 ATP_NAS_FetchSpaceInfo(VOS_CHAR *DirPath, VOS_CHAR *SpaceType,VOS_UINT64 *SpaceNum);
static VOS_UINT32 ATP_NAS_FetchSubDirectory(VOS_CHAR *DirPath, ATP_MSG_HEADER_ST **pstMsg);
static VOS_UINT32 ATP_NAS_FetchDirectoryInfo(VOS_CHAR *DirPath, ATP_MSG_HEADER_ST **pstMsg);
static VOS_UINT32 ATP_NAS_CreateDirectory(VOS_CHAR *DirPath);
static VOS_UINT32 ATP_NAS_DeleteDirectory(VOS_CHAR *DirPath);
static VOS_UINT32 ATP_NAS_UpLoadFile(VOS_CHAR *hddFileName, VOS_CHAR *webDavFileName);
static VOS_UINT32 ATP_NAS_DownloadFile(VOS_CHAR *hddFileName, VOS_CHAR *webDavFileName);
static VOS_UINT32 ATP_NAS_DeleteFile(VOS_CHAR *webDavFileName);
static VOS_UINT32 ATP_NAS_MoveFile(VOS_CHAR *srcFile, VOS_CHAR *dstFile);
static VOS_UINT32 ATP_NAS_CopyFile(VOS_CHAR *srcFile, VOS_CHAR *dstFile);
static VOS_UINT32 ATP_NAS_GetProperties(VOS_CHAR *res, VOS_CHAR *name, VOS_CHAR **value);
static VOS_UINT32 ATP_NAS_Check_ConnectionToServer(VOS_UINT32 *pnConnState);
static VOS_UINT32 ATP_NAS_SafeExitProcesses(VOS_VOID);
static VOS_CHAR *ATP_NAS_WebDavTranslateMessage(VOS_UINT32 nMsg);
static VOS_UINT32 ATP_NAS_FetchFolderSpace(VOS_CHAR *pchFolder, VOS_UINT64 *pullSpace);
static VOS_UINT32 ATP_NAS_CadaverBuildFullPath(VOS_CHAR *prePath, VOS_CHAR *lastname, VOS_CHAR **ppFullPath);

VOS_VOID ATP_NAS_WebDavClient_Entry(VOS_INT argc, VOS_CHAR **argv)
{
    VOS_UINT32           nRetVal =  VOS_OK;
    VOS_UINT32           nExit   =  VOS_FALSE;
    ATP_MSG_HEADER_ST   *pstMsg  =  NULL;

    ATP_NAS_WebDavDebug_init(stdout);

    ATP_WEBDAV_DEBUG("=========================================\n");
    ATP_WEBDAV_DEBUG("        welcome to webdav client tool    \n");
    ATP_WEBDAV_DEBUG("=========================================\n");
    
    nRetVal = ATP_NAS_WebDavClient_Init(argc, argv);
    if (VOS_OK != nRetVal)
    {
        ATP_NAS_WebDavClient_Clean();
        return;
    }

    nRetVal = ATP_NAS_WebDavClient_Notify();
    if (VOS_OK != nRetVal)
    {
        ATP_NAS_WebDavClient_Clean();
        return;
    }

    /* enter the infinite loop to process request command.. */
    for ( ; ; )
    {
        nRetVal = ATP_MSG_Recv(g_stMsgCtx.lSock, &pstMsg,  ATP_WEBDAV_WAIT_MSG_TIME);
        if (VOS_OK != nRetVal)
        {
            ATP_WEBDAV_DEBUG("[%s]\n", ATP_NAS_WebDAV_MsgRetParse(nRetVal));
            continue;
        }

        ATP_WEBDAV_DEBUG(" === %s receive a %s message from %s ===\n", (char*)get_option(opt_processname), ATP_NAS_WebDavTranslateMessage(pstMsg->ulMsgType), pstMsg->aucSendApp);

        /*Process message we received*/
        ATP_NAS_WebDavClient_ProcessMsg(pstMsg, &nExit);
 
        if (VOS_NULL_PTR != pstMsg)
        {
            free(pstMsg);
        }

        /*whether we should exit*/
        if (VOS_TRUE == nExit)
        {
            break;
        }
    }  

    ATP_WEBDAV_DEBUG("%s going to exit! Goodbye!\n", (char*)get_option(opt_processname));
    ATP_NAS_WebDavClient_Clean();

    return;
}

VOS_UINT32 ATP_NAS_GetPropertiesWithRetry(VOS_CHAR *pchResource, VOS_CHAR *pchPropertyName, VOS_CHAR **ppchValue, VOS_UINT32 nRetryTime)
{
    VOS_UINT32 nRet  = VOS_OK;
	VOS_UINT32 nLoop = nRetryTime;
	
    if (NULL == pchResource || NULL == pchPropertyName || NULL == ppchValue)
    {
	    return VOS_NOK;
    }

    if (0 == nRetryTime)
    {
        return VOS_NOK;
    }
	
	do
	{
	    nLoop -= (VOS_UINT32)1;
		
	    nRet = ATP_NAS_GetProperties(pchResource, pchPropertyName, ppchValue);
		if (VOS_OK != nRet)
	    {
		    ATP_WEBDAV_DEBUG("Fetch %s property fail, remain %ld times to retry\n", pchPropertyName, nLoop);
	    }
		else
		{
		    break;
	    }		
    }while(nLoop);	
	
	return nRet;
}

VOS_UINT32 ATP_NAS_GetFileStateWithRetry(VOS_CHAR *pchResource,  VOS_CHAR **ppPropertyName, VOS_CHAR **ppPropertyValue, VOS_UINT32 nRetryTime)
{
    VOS_UINT32 nRet  = VOS_OK;
    VOS_UINT32 nLoop = nRetryTime;

    if (NULL == pchResource || NULL == ppPropertyName || NULL == ppPropertyValue)
    {
        return VOS_NOK;
    }

    if (0 == nRetryTime)
    {
        return VOS_NOK;
    }

    do
    {
        nLoop -= (VOS_UINT32)1;

        nRet = ATP_NAS_Execute_MultiPropget(pchResource, ppPropertyName, ppPropertyValue);
        if (nRet != VOS_OK)
        {
            ATP_WEBDAV_DEBUG("Fetch multi-property fail, remain %ld times to retry\n", nLoop);
        }
        else
        {
            break;
        }
    }while(nLoop);

    return nRet;
}

VOS_UINT32 ATP_NAS_SetPropertiesWithRetry(VOS_CHAR *pchResource, VOS_CHAR *pchPropertyName, VOS_CHAR *pchValue, VOS_UINT32 nRetryTime)
{
    VOS_UINT32 nRet  = VOS_OK;
    VOS_UINT32 nLoop = nRetryTime;
        
    if (NULL == pchResource || NULL == pchPropertyName || NULL == pchValue || 0 == nRetryTime)
    {
        return VOS_NOK;
    }
        
    do
    {
        nLoop -= (VOS_UINT32)1;
            
        nRet = ATP_NAS_Execute_set(pchResource, pchPropertyName, pchValue);
        if (VOS_OK != nRet)
        {
            ATP_WEBDAV_DEBUG("Set %s property fail, remain %ld times to retry\n", pchPropertyName, nLoop);
        }
        else
        {
            break;
        }       
    }while(nLoop);  
        
    return nRet;

}

static VOS_VOID ATP_NAS_WebDavClient_ProcessMsg(ATP_MSG_HEADER_ST *pstMsg, VOS_UINT32 *pExit)
{
    VOS_UINT32   nRetVal  =  VOS_OK;
    VOS_UINT32   nIdx     =  0;
    ATP_MSG_HEADER_ST  stResp;
    ATP_MSG_HEADER_ST *pstResp = NULL;
    
    if (NULL == pstMsg || NULL == pExit)
    {
        return;
    }

    *pExit = VOS_FALSE;

    neon_http_status_code = 0;

    switch (pstMsg->ulMsgType)
    {
        case ATP_WEBDAV_SET_PROXY:    /*设置proxy,网关通过代理连接mediencenter服务器*/
        case ATP_WEBDAV_UNSET_PROXY:  /*取消proxy,网关直接连接mediencenter服务器*/
        {
            nRetVal = ATP_NAS_WebDAV_ProxyProcess(pstMsg);           
            break;
        }
        case ATP_WEBDAV_LOGON_NETWORKSTORAGE:  /* 登录mediencenter服务器 */
        {            
            nRetVal = ATP_NAS_LogOn_WebDavServer(pstMsg);
            if (session.connected)
            {
                nRetVal = VOS_OK;
            }
            else
            {
                nRetVal = VOS_NOK;
            }
            ATP_WEBDAV_REQUEST_REPORT(nRetVal); 
            break;
        }
        case ATP_WEBDAV_LOGOUT_NETWORKSTORAGE: /*登出mediencenter服务器*/
        {
            nRetVal = ATP_NAS_LogOut_WebDavServer();
            ATP_WEBDAV_REQUEST_REPORT(nRetVal); 
            break;
        }
        case ATP_WEBDAV_EXIT_PROCESS:  /*父进程通知cadaver进程退出*/
        {
            *pExit = VOS_TRUE;
            ATP_WEBDAV_REQUEST_REPORT(VOS_OK);
            break;
        }
        case ATP_MSG_SAFE_EXIT_PROCESS: /*父进程通知cadaver进程退出*/
        {
            *pExit = VOS_TRUE;
            ATP_MSG_SendSimpleMsg(NULL, ATP_MSG_SAFE_EXIT_PROCESS, VOS_OK);/*回给mic*/
            break;
        }
        case ATP_WEBDAV_CHECK_STORAGE_SPACE:  /*查询当前mediencenter上已用空间大小及可用空间大小, 待优化*/
        {
            ATP_NAS_SPACE_REQUEST_STRUCT stSpace;

            memset(&stSpace, 0x00, sizeof(stSpace));
            
            if (VOS_OK != (nRetVal=ATP_NAS_FetchSpaceInfo(ATP_WEBDAV_ROOT_PATH, ATP_NAS_AVAILABLE_SPACE, &(stSpace.availSpace)))
              ||VOS_OK != (nRetVal=ATP_NAS_FetchSpaceInfo(ATP_WEBDAV_ROOT_PATH, ATP_NAS_USED_SPACE, &(stSpace.usedSpace))))
            {
                ATP_WEBDAV_DEBUG("Fetch storage space info error...ret[%d]\n", nRetVal);
            }

            stSpace.stMsgHead.ulMsgType = ATP_WEBDAV_CHECK_STORAGE_SPACE;
            stSpace.stMsgHead.ulMsgData = nRetVal;
            stSpace.stMsgHead.ulDataLen = sizeof(ATP_NAS_SPACE_REQUEST_STRUCT) - sizeof(ATP_MSG_HEADER_ST);
            snprintf(stSpace.stMsgHead.aucRecvApp, sizeof(stSpace.stMsgHead.aucRecvApp), "%s", pstMsg->aucSendApp);
            if(VOS_OK != ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stSpace))
            {
                ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stSpace);
            }
            break;
        }
        case ATP_WEBDAV_CHECK_FOLDER_SPACE:
        {
            ATP_NAS_FOLDER_SPACE_STRUCT stRespMsg;
            VOS_CHAR *pchFolder = NULL;

            memset(&stRespMsg, 0x00, sizeof(stRespMsg));

            /* 获取待检查空间大小文件夹路径 */
            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, pchFolder); 
            //ATP_WEBDAV_DEBUG("=== check space of %s\n", pchFolder);

            nRetVal = ATP_NAS_FetchFolderSpace(pchFolder, &(stRespMsg.ullSpace));
            if (VOS_OK!=nRetVal)
            {
                ATP_WEBDAV_DEBUG("Error: check folder space fail...\n");
                ATP_WEBDAV_REQUEST_REPORT(nRetVal);
            }
            else
            {
                stRespMsg.stMsgHead.ulMsgType = ATP_WEBDAV_CHECK_FOLDER_SPACE;
                stRespMsg.stMsgHead.ulMsgData = VOS_OK;
                stRespMsg.stMsgHead.ulDataLen = sizeof(ATP_NAS_FOLDER_SPACE_STRUCT) - sizeof(ATP_MSG_HEADER_ST);
                snprintf(stRespMsg.stMsgHead.aucRecvApp, sizeof(stRespMsg.stMsgHead.aucRecvApp), "%s", pstMsg->aucSendApp);
                if (VOS_OK != ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stRespMsg))
                {
                    ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stRespMsg);
                }
            }   
            break;
        }
        case ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE: /*查询某个目录下的全部子目录, 待优化*/
        {                
            VOS_CHAR *dirPath = NULL;
            
            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, dirPath); 
                
            nRetVal = ATP_NAS_FetchSubDirectory(dirPath, &pstResp);  
            if (VOS_OK != nRetVal)
            {
                ATP_WEBDAV_REQUEST_REPORT(nRetVal);
                break;
            }
            
            if (NULL == pstResp) /*空目录*/
            {
                ATP_WEBDAV_REQUEST_REPORT(VOS_OK);
            }
            else
            {
                pstResp->ulMsgType = ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE;
                pstResp->ulMsgData = VOS_OK;
                snprintf(pstResp->aucRecvApp, sizeof(pstResp->aucRecvApp), "%s", pstMsg->aucSendApp);
                if (VOS_OK != ATP_MSG_Send(pstResp))
                {
                    ATP_MSG_Send(pstResp);
                }
                free(pstResp);
            }

            break;
        }
        case ATP_WEBDAV_GET_DIR_ALL_STATE:  /*查询某个目录下的全部子目录及文件, 待优化*/
        {
            VOS_CHAR *dirPath = NULL;

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, dirPath); 

            nRetVal = ATP_NAS_FetchDirectoryInfo(dirPath, &pstResp); 
            if (VOS_OK != nRetVal)
            {
                ATP_WEBDAV_REQUEST_REPORT(nRetVal);
                break;
            }
            
            if (NULL == pstResp) /*空目录*/
            {
                ATP_WEBDAV_REQUEST_REPORT(VOS_OK);
            }
            else
            {
                pstResp->ulMsgType = ATP_WEBDAV_GET_DIR_ALL_STATE;
                pstResp->ulMsgData = VOS_OK;
                snprintf(pstResp->aucRecvApp, sizeof(pstResp->aucRecvApp), "%s", pstMsg->aucSendApp);
                if (VOS_OK != ATP_MSG_Send(pstResp))
                {
                    ATP_MSG_Send(pstResp);
                }
                free(pstResp);
            }

            break;
        }
        case ATP_WEBDAV_GET_FILE_STATE:  /* 查询某个文件或目录的属性 */
        {
            FILELISTNODE      *nodeTmp = NULL;
            VOS_CHAR *filePath = NULL;
            VOS_CHAR *pPropertyName[] = {{ATP_PROPERTY_RESOURCETYPE}, {ATP_PROPERTY_GETLASTMODIFIED}, {ATP_PROPERTY_GETCONTENTLENGTH}, {NULL}};
            VOS_CHAR *pPropertyValue[] = {{NULL}, {NULL}, {NULL}, {NULL}};

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, filePath); 
            ATP_WEBDAV_DEBUG("=== check state of %s\n", filePath);

            pstResp = (ATP_MSG_HEADER_ST *)malloc(sizeof(ATP_MSG_HEADER_ST) + sizeof(FILELISTNODE));
            if (NULL == pstResp)
            {
                ATP_WEBDAV_REQUEST_REPORT(VOS_NOK);
                break;
            }
            //lint -save -e826 
            nodeTmp = (FILELISTNODE *)((VOS_CHAR *)pstResp + sizeof(ATP_MSG_HEADER_ST));
            memset(nodeTmp, 0x00, sizeof(FILELISTNODE));
            //lint -restore
            nRetVal = ATP_NAS_GetFileStateWithRetry(filePath, &pPropertyName, &pPropertyValue, ATP_NAS_RETRY_TIMES);
            if (VOS_OK == nRetVal)
            {
                if (pPropertyValue[0][0] == '\0')
                {
                    nodeTmp->nFileType = ATP_NAS_FILE_TYPE;
                }
                else if (strstr(pPropertyValue[0], "collection") != NULL)
                {
                    nodeTmp->nFileType = ATP_NAS_DIRECTORY_TYPE;
                }
                else
                {
                    nodeTmp->nFileType = ATP_NAS_UNKNOWN_TYPE;
                }
                            
                nodeTmp->nLastModTime = ne_httpdate_parse(pPropertyValue[1]);

                ATP_NAS_TRANS_CHAR_TO_LONG_NUMBER(pPropertyValue[2], &(nodeTmp->nSize));
            }
            else
            {
                ATP_WEBDAV_REQUEST_REPORT(nRetVal);
                free(pstResp);
                break;
            }

            pstResp->ulMsgType = ATP_WEBDAV_GET_FILE_STATE;
            pstResp->ulDataLen = sizeof(FILELISTNODE);
            pstResp->ulMsgData = VOS_OK;
            snprintf(pstResp->aucRecvApp, sizeof(pstResp->aucRecvApp), "%s", pstMsg->aucSendApp);
            if (VOS_OK != ATP_MSG_Send(pstResp))
            {
                ATP_MSG_Send(pstResp);
            }
            free(pstResp);
            break;
        }
        case ATP_WEBDAV_GETMTIME:
        {
            VOS_CHAR *filePath = NULL;
            VOS_CHAR *pValue   = NULL;
            ATP_NAS_MTIME_REQUEST_STRUCT stRespMtime;

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, filePath); 
            ATP_WEBDAV_DEBUG("=== check mtime of %s\n", filePath);

            memset(&stRespMtime, 0x00, sizeof(stRespMtime));
            nRetVal = ATP_NAS_GetPropertiesWithRetry(filePath, ATP_PROPERTY_GETLASTMODIFIED, &pValue, ATP_NAS_RETRY_TIMES);
            if (nRetVal == VOS_OK)
            {
                stRespMtime.lastmodified = ne_httpdate_parse(pValue);
            }
            else
            {
                ATP_WEBDAV_REQUEST_REPORT(nRetVal);
                break;
            }

            stRespMtime.stMsgHead.ulMsgType = ATP_WEBDAV_GETMTIME;
            stRespMtime.stMsgHead.ulDataLen = sizeof(ATP_NAS_MTIME_REQUEST_STRUCT) - sizeof(ATP_MSG_HEADER_ST);
            stRespMtime.stMsgHead.ulMsgData = VOS_OK;
            snprintf(stRespMtime.stMsgHead.aucRecvApp, sizeof(stRespMtime.stMsgHead.aucRecvApp), "%s", pstMsg->aucSendApp);
            if (VOS_OK != ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stRespMtime))
            {
                ATP_MSG_Send((ATP_MSG_HEADER_ST*)&stRespMtime);
            }
            free(pstResp);
            break;
                
        }
        case ATP_WEBDAV_SETMTIME:
        {
            VOS_CHAR *pchResource = NULL;
            VOS_CHAR *pchMtime    = NULL;

            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, pchResource, pchMtime); 
            ATP_WEBDAV_DEBUG("=== set mtime[%s] for %s\n", pchMtime, pchResource);
            
            nRetVal = ATP_NAS_SetPropertiesWithRetry(pchResource, ATP_PROPERTY_GETLASTMODIFIED, pchMtime, ATP_NAS_RETRY_TIMES);

            ATP_WEBDAV_REQUEST_REPORT(nRetVal); 

            break;
        }
        case ATP_WEBDAV_CREATE_DIR:  /* 创建某个目录 */
        {
            VOS_CHAR *dirPath = NULL;

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, dirPath); 

            for (nIdx = 0; nIdx < 5; nIdx++)
            {
                nRetVal = ATP_NAS_CreateDirectory(dirPath);
                if (VOS_OK != nRetVal)
                {
                    ATP_WEBDAV_DEBUG("Create folder %s fail, remain %d times to retry\n", dirPath, (4-nIdx));
                    continue;
                }
                else
                {
                    break;
                }
            }

            ATP_WEBDAV_REQUEST_REPORT(nRetVal); 
            break;
        }
        case ATP_WEBDAV_DELETE_DIR:  /*删除某个目录*/
        {
            VOS_CHAR *dirPath = NULL;

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, dirPath); 

            for (nIdx = 0; nIdx < 3; nIdx++)
            {
                nRetVal = ATP_NAS_DeleteDirectory(dirPath);
                if (VOS_OK != nRetVal)
                {
                    ATP_WEBDAV_DEBUG("Delete folder %s fail, remain %d times to retry\n", dirPath, (2-nIdx));
                    continue;
                }
                else
                {
                    break;
                }
            }

            ATP_WEBDAV_REQUEST_REPORT(nRetVal); 
            break;
        }
        case ATP_WEBDAV_DELETE_FILE:    /*删除某个文件*/
        {
            VOS_CHAR *filePath = NULL;

            ATP_WEBDAV_PARSE_SINGLE_PATH(pstMsg, filePath); 

            /* We will try for 3 times while the mediencenter return 404 */
            for (nIdx = 0; nIdx < 3; nIdx++)
            {
                nRetVal = ATP_NAS_DeleteFile(filePath);
                if (VOS_OK != nRetVal && (unsigned int)404 == neon_http_status_code)
                {
                    ATP_WEBDAV_DEBUG("Delete file %s fail[HTTP %d], remain %d times to retry\n", 
                                                filePath, neon_http_status_code, ((VOS_UINT32)2-nIdx));
                    continue;
                }
                else
                {
                    /* If return OK, or other fail reason except for 404, just break out */
                    break;
                }
            }

            ATP_WEBDAV_DEBUG("Delete file %s %s, return code[%d]\n", filePath, (VOS_OK==nRetVal)?"success":"fail", neon_http_status_code);
            
            ATP_WEBDAV_REQUEST_REPORT(nRetVal);
            break;
        }
        case ATP_WEBDAV_UPLOAD_FILE:    /* 将文件由HDD上传至mediencenter */
        {
            VOS_CHAR *hddFilePath = NULL;
            VOS_CHAR *davFilePath = NULL;

            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, hddFilePath, davFilePath); 

            /* If mediencenter reponse 409, we will try it for 5 times */
            for (nIdx = 0; nIdx < 5; nIdx++)
            {
                nRetVal = ATP_NAS_UpLoadFile(hddFilePath, davFilePath);
                if (VOS_OK != nRetVal && ((unsigned int)409 == neon_http_status_code || (unsigned int)502 == neon_http_status_code))
                {
                    ATP_WEBDAV_DEBUG("Upload file %s fail[HTTP %d], remain %d times to upload again\n", 
                                                               hddFilePath, neon_http_status_code, ((VOS_UINT32)4-nIdx));
                    continue;
                }
                else
                {
                    /* If return OK, or other fail reason except for 409, just break out */
                    break;
                }
            }

            ATP_WEBDAV_DEBUG("Upload file %s %s, return code: %d\n", hddFilePath, ((VOS_OK==nRetVal)?"success":"fail"), neon_http_status_code);
                                  
            ATP_WEBDAV_REQUEST_REPORT(nRetVal);
            break;
        }
        case ATP_WEBDAV_DOWNLOAD_FILE:  /* 将文件由mediencenter下载至HDD */
        {
            VOS_CHAR *hddFilePath = NULL;
            VOS_CHAR *davFilePath = NULL;

            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, hddFilePath, davFilePath); 

            /* If mediencenter reponse 404, we will try it for 5 times */
            for (nIdx = 0; nIdx < 5; nIdx++)
            {
                nRetVal = ATP_NAS_DownloadFile(hddFilePath, davFilePath);
                if (VOS_OK != nRetVal && ((unsigned int)404 == neon_http_status_code ||(unsigned int)502 == neon_http_status_code))
                {
                    ATP_WEBDAV_DEBUG("Download file %s fail[HTTP %d], remain %d times to download again\n", 
                                                           davFilePath, neon_http_status_code, ((VOS_UINT32)4-nIdx));
                    continue;
                }
                else
                {
                    /* If return OK, or other fail reason except for 404, just break out */
                    break;
                }
            }

            ATP_WEBDAV_DEBUG("Download file %s %s, return code: %d\n", davFilePath, ((VOS_OK==nRetVal)?"success":"fail"), neon_http_status_code);
                         
            ATP_WEBDAV_REQUEST_REPORT(nRetVal);
            break;
        }
#if 0
        case ATP_WEBDAV_MV_FILE:
        case ATP_WEBDAV_RENAME_FILE:
        case ATP_WEBDAV_MV_DIR:         /*将文件夹移动至某处*/
        case ATP_WEBDAV_RENAME_DIR:     /*将文件夹改名*/
        {
            VOS_CHAR *srcFilePath = NULL;
            VOS_CHAR *dstFilePath = NULL;
            VOS_CHAR *pchBuffer   = NULL;
            VOS_UINT32 nLen       = 0;
                       
            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, srcFilePath, dstFilePath); 

            if (NULL == srcFilePath || NULL == dstFilePath)
            {
                ATP_WEBDAV_DEBUG("Error: parse path fail...\n");
                ATP_WEBDAV_REQUEST_REPORT(VOS_NOK);  /*临时采用这样的做法, 返回OK, 后续都要更改*/
                break;
            }

            nLen = strlen("mv ") + strlen(srcFilePath) + strlen(dstFilePath) + 17;
                
            pchBuffer = malloc(nLen);
            if (pchBuffer == NULL)
            {
                ATP_WEBDAV_DEBUG("Malloc memory fail...\n");
                ATP_WEBDAV_REQUEST_REPORT(VOS_NOK);
                break;
            }

            memset(pchBuffer, 0x00, nLen);

            snprintf(pchBuffer, nLen, "mv \"%s\" \"%s\"", srcFilePath, dstFilePath); 
            
            cadaver_execute_command(pchBuffer);

            free(pchBuffer);

            ATP_WEBDAV_REQUEST_REPORT(VOS_OK);  /*临时采用这样的做法, 返回OK, 后续都要更改*/
            break;
        }
#endif
#if 1
        case ATP_WEBDAV_MV_FILE:      
        case ATP_WEBDAV_RENAME_FILE:
        case ATP_WEBDAV_MV_DIR:         
        case ATP_WEBDAV_RENAME_DIR:     
        {
            VOS_CHAR *srcFilePath = NULL;
            VOS_CHAR *dstFilePath = NULL;

            VOS_CHAR *srcHtmlPath = NULL;
            VOS_CHAR *dstHtmlPath = NULL;
                       
            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, srcFilePath, dstFilePath); 

            srcHtmlPath = ATP_NAS_Escape_path(srcFilePath);

            dstHtmlPath = ATP_NAS_Escape_path(dstFilePath);

            /* If mediencenter return 404 NOT FOUND, we will try it for 5 times */
            for (nIdx = 0; nIdx < 5; nIdx++)
            {
                nRetVal = ATP_NAS_MoveFile(srcHtmlPath, dstHtmlPath);
                if (VOS_OK != nRetVal && (unsigned int)404 == neon_http_status_code)
                {
                    ATP_WEBDAV_DEBUG("Move %s to %s fail[HTTP %d], remain %d times to retry\n", srcFilePath, 
                                             dstFilePath, neon_http_status_code, ((VOS_UINT32)4-nIdx));
                    continue;
                }
                else
                {
                    /*If return VOS_OK or error except for 404, break out*/
                    break;
                }
            }

            ATP_WEBDAV_DEBUG("Move %s to %s %s[HTTP %d]\n", srcFilePath, dstFilePath, ((VOS_OK==nRetVal)?"success":"fail"), neon_http_status_code);
            
            ATP_WEBDAV_REQUEST_REPORT(nRetVal);

            free(srcHtmlPath);
            free(dstHtmlPath);
            
            break;
        }
#endif
        case ATP_WEBDAV_COPY_FILE:   /*复制某个文件*/
        {
            VOS_CHAR *srcFilePath = NULL;
            VOS_CHAR *dstFilePath = NULL;

            VOS_CHAR *srcHtmlPath = NULL;
            VOS_CHAR *dstHtmlPath = NULL;
                       
            ATP_WEBDAV_PARSE_DOUBLE_PATH(pstMsg, srcFilePath, dstFilePath); 

            srcHtmlPath = ATP_NAS_Escape_path(srcFilePath);

            dstHtmlPath = ATP_NAS_Escape_path(dstFilePath);

            /* If mediencenter return 404 NOT FOUND, we will try it for 5 times */
            for (nIdx = 0; nIdx < 5; nIdx++)
            {
                nRetVal = ATP_NAS_Execute_copy(srcHtmlPath, dstHtmlPath);
                if (VOS_OK != nRetVal && (unsigned int)404 == neon_http_status_code)
                {
                    ATP_WEBDAV_DEBUG("Copy %s to %s fail[HTTP %d], remain %d times to retry\n", 
                                                   srcFilePath, dstFilePath, neon_http_status_code, ((VOS_UINT32)4-nIdx));
                    continue;
                }
                else
                {
                    break;
                }
            }

            ATP_WEBDAV_DEBUG("Copy %s to %s %s, return code[%d]\n", srcFilePath, dstFilePath, (VOS_OK==nRetVal)?"success":"fail", neon_http_status_code);
            
            ATP_WEBDAV_REQUEST_REPORT(nRetVal);

            free(srcHtmlPath);
            free(dstHtmlPath);
            
            break;
        }
        case ATP_WEBDAV_CONNECT_CHECK:  /* 检查当前网关与mediencenter之间的连接状态 */
        {
            /*待补充，需要检验socket以及options探测服务器*/
            if (!session.connected)
            {
                ATP_WEBDAV_REQUEST_REPORT(VOS_NOK);
            }
            else
            {
                /* 目前做法: 通过获取服务器上可用空间大小来替代options探测 */
                VOS_UINT64 availSpace = 0;

                nRetVal = ATP_NAS_FetchSpaceInfo(ATP_WEBDAV_ROOT_PATH, ATP_NAS_AVAILABLE_SPACE, &availSpace);
                if ((VOS_OK == nRetVal)
                 ||((unsigned int)404 == neon_http_status_code))
                {
                    ATP_WEBDAV_REQUEST_REPORT(VOS_OK);
                }
                else
                {
                    ATP_WEBDAV_DEBUG("Warning: connection maybe disconnect\n");
                    ATP_WEBDAV_REQUEST_REPORT(VOS_NOK);
                }         
            }
            break;
        }
        case ATP_WEBDAV_PROPFIND:       /*获取某个property*/
        case ATP_WEBDAV_PROPSET:        /*设置某property*/
        case ATP_WEBDAV_COPY_DIR:       /*将文件夹拷贝至某处*/
        case ATP_WEBDAV_OPEN_FILE:      /*打开某文件*/
        case ATP_WEBDAV_WRITE_FILE:     /*写入某文件*/
        case ATP_WEBDAV_CLOSE_FILE:     /*关闭某文件*/
        case ATP_WEBDAV_LOCK:           /*为某个资源加锁*/
        case ATP_WEBDAV_UNLOCK:         /*为某个资源解锁*/
        default: 
        {
            ATP_WEBDAV_DEBUG("Error: none completed command...\n");
            break;
        }
    }

    return;
    
}

static VOS_CHAR *ATP_NAS_WebDavTranslateMessage(VOS_UINT32 nMsg)
{
    switch(nMsg)
    {
        case ATP_WEBDAV_SET_PROXY:               
            return "ATP_WEBDAV_SET_PROXY";
        case ATP_WEBDAV_UNSET_PROXY:             
            return "ATP_WEBDAV_UNSET_PROXY";
        case ATP_WEBDAV_LOGON_NETWORKSTORAGE:    
            return "ATP_WEBDAV_LOGON_NETWORKSTORAGE";
        case ATP_WEBDAV_LOGOUT_NETWORKSTORAGE:   
            return "ATP_WEBDAV_LOGOUT_NETWORKSTORAGE";
        case ATP_WEBDAV_EXIT_PROCESS:            
            return "ATP_WEBDAV_EXIT_PROCESS";
        case ATP_MSG_SAFE_EXIT_PROCESS:          
            return "ATP_MSG_SAFE_EXIT_PROCESS";
        case ATP_WEBDAV_CHECK_STORAGE_SPACE:     
            return "ATP_WEBDAV_CHECK_STORAGE_SPACE";
        case ATP_WEBDAV_CHECK_FOLDER_SPACE:
            return "ATP_WEBDAV_CHECK_FOLDER_SPACE";
        case ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE:
            return "ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE";
        case ATP_WEBDAV_GET_DIR_ALL_STATE:       
            return "ATP_WEBDAV_GET_DIR_ALL_STATE";
        case ATP_WEBDAV_GET_FILE_STATE:          
            return "ATP_WEBDAV_GET_FILE_STATE";
        case ATP_WEBDAV_GETMTIME:                
            return "ATP_WEBDAV_GETMTIME";
        case ATP_WEBDAV_SETMTIME:
            return "ATP_WEBDAV_SETMTIME";
        case ATP_WEBDAV_CREATE_DIR:              
            return "ATP_WEBDAV_CREATE_DIR";
        case ATP_WEBDAV_DELETE_DIR:              
            return "ATP_WEBDAV_DELETE_DIR";
        case ATP_WEBDAV_DELETE_FILE:             
            return "ATP_WEBDAV_DELETE_FILE";
        case ATP_WEBDAV_UPLOAD_FILE:             
            return "ATP_WEBDAV_UPLOAD_FILE";
        case ATP_WEBDAV_DOWNLOAD_FILE:           
            return "ATP_WEBDAV_DOWNLOAD_FILE";
        case ATP_WEBDAV_MV_FILE:                 
            return "ATP_WEBDAV_MV_FILE";
        case ATP_WEBDAV_RENAME_FILE:             
            return "ATP_WEBDAV_RENAME_FILE";
        case ATP_WEBDAV_COPY_FILE:               
            return "ATP_WEBDAV_COPY_FILE";
        case ATP_WEBDAV_CONNECT_CHECK:           
            return "ATP_WEBDAV_CONNECT_CHECK";
        case ATP_WEBDAV_PROPFIND:                
            return "ATP_WEBDAV_PROPFIND";
        case ATP_WEBDAV_PROPSET:                 
            return "ATP_WEBDAV_PROPSET";
        case ATP_WEBDAV_MV_DIR:                  
            return "ATP_WEBDAV_MV_DIR";
        case ATP_WEBDAV_COPY_DIR:                
            return "ATP_WEBDAV_COPY_DIR";
        case ATP_WEBDAV_RENAME_DIR:              
            return "ATP_WEBDAV_RENAME_DIR";
        case ATP_WEBDAV_OPEN_FILE:               
            return "ATP_WEBDAV_OPEN_FILE";
        case ATP_WEBDAV_WRITE_FILE:              
            return "ATP_WEBDAV_WRITE_FILE";
        case ATP_WEBDAV_CLOSE_FILE:              
            return "ATP_WEBDAV_CLOSE_FILE";
        case ATP_WEBDAV_LOCK:                    
            return "ATP_WEBDAV_LOCK";
        case ATP_WEBDAV_UNLOCK:                  
            return "ATP_WEBDAV_UNLOCK";
        default:                                 
            return "ATP_WEBDAV_UNKNOWN_MESSAGE";
    }
}    
    
static void ATP_NAS_WebDavDebug_init(FILE *stream)
{
#ifdef ATP_PCLINT
    stream = stream;
#endif

#ifdef ATP_DEBUG
    nas_webdav_debug_stream = stream;
    
    if (stream) setvbuf(stream, NULL, _IONBF, 0);        
#endif
}

/*****************************************************
The process start with params like:

1、cadaver  sync     taskid  
2、cadaver  backup   taskid  
3、cadaver  restore  taskid   
4、cadaver  subcadaver
*****************************************************/
static VOS_UINT32 ATP_NAS_WebDavClient_Init(VOS_INT32 argc, VOS_CHAR **argv)
{
    VOS_CHAR   buffer[128];

#ifdef ATP_PCLINT
    argc = argc;
#endif
    
    if (NULL == argv[1])
    {
        return ATP_WEBDAV_PARAM_ERR;
    }

    if (strcmp(argv[1], ATP_CADAVER_PROCESS_SYNC_PRENAME)==0)
    {
        if (NULL == argv[2])
        {
            ATP_WEBDAV_DEBUG("Error cadaver parameter, need to known nasapp's pid\n");
            return ATP_WEBDAV_PARAM_ERR;
        }
        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "cadaver%s", argv[2]);
        set_option(opt_processname, ne_strdup(buffer));

        ATP_WEBDAV_DEBUG("cadaver process name : %s\n", buffer);
          
        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "nassync%s", argv[2]);
        set_option(opt_fprocessname, ne_strdup(buffer));
 
        ATP_WEBDAV_DEBUG("cadaver father process name : %s\n", buffer);
    }
    else if (strcmp(argv[1], ATP_CADAVER_PROCESS_BACK_PRENAME)==0)
    {
        if (NULL == argv[2])
        {
            ATP_WEBDAV_DEBUG("Error cadaver parameter, need to known nasapp's pid\n");
            return ATP_WEBDAV_PARAM_ERR;
        }
        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "cadaver%d", atoi(argv[2])+10);               
        set_option(opt_processname, ne_strdup(buffer));

        ATP_WEBDAV_DEBUG("cadaver process name : %s\n", buffer);
          
        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "nasbackup%s", argv[2]);
        set_option(opt_fprocessname, ne_strdup(buffer));
 
        ATP_WEBDAV_DEBUG("cadaver father process name : %s\n", buffer);
    }
    else if (strcmp(argv[1], ATP_CADAVER_PROCESS_SUBCADAVER)==0)
    {
        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "cadaver");
        set_option(opt_processname, ne_strdup(buffer));

        ATP_WEBDAV_DEBUG("cadaver process name : %s\n", buffer);

        memset(buffer, 0x00, sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "nascenter");
        set_option(opt_fprocessname, ne_strdup(buffer));

        ATP_WEBDAV_DEBUG("cadaver father process name : %s\n", buffer);
    }
    else
    {
        ATP_WEBDAV_DEBUG("Unknown process name[%s]!...\n", argv[1]);
        return ATP_WEBDAV_PARAM_ERR;
    }

    ATP_WEBDAV_DEBUG("cadaver process ID: %d\n", getpid());
    
    return VOS_OK;
}

static VOS_UINT32 ATP_NAS_WebDavClient_Notify()
{
    VOS_UINT32         unRet        = VOS_NOK;
    ATP_MSG_HEADER_ST  stMsg        = {0};
    VOS_CHAR           *pMyName     = NULL;
    VOS_CHAR           *pFatherName = NULL;
    
    pMyName     = get_option(opt_processname);
    pFatherName = get_option(opt_fprocessname);
    if (NULL == pMyName || NULL == pFatherName)
    {
        return VOS_NOK;
    }
        
    /* register myself */
    unRet = ATP_MSG_Init(pMyName);
    if (unRet != VOS_OK)
    {
        sleep(1);
        unRet = ATP_MSG_Init(pMyName);
        if (unRet != VOS_OK)
        {
            ATP_WEBDAV_DEBUG("Register process %s fail! Ret(%x)\n", pMyName, unRet);
            return VOS_NOK;
        }
    }

    /* tell father process I'm starting up */
    ATP_MSG_SimpleBuilder(&stMsg, pFatherName, ATP_CADAVER_PROCESS_STARTUP, 0);
    unRet = ATP_MSG_Send(&stMsg);
    if (VOS_OK != unRet)
    {
        unRet = ATP_MSG_Send(&stMsg);
        if (VOS_OK != unRet)
        {
            ATP_WEBDAV_DEBUG("Process %s sync with fprocess %s fail! Ret(%x)\n", pMyName, pFatherName, unRet);
            return VOS_NOK;
        }
    }

    return VOS_OK;
}

static VOS_VOID ATP_NAS_WebDavClient_Clean()
{   
    VOS_CHAR *tmp;
        
    ATP_NAS_SafeExitProcesses();

    if ((tmp = get_option(opt_processname))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_fprocessname))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_medusername))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_medpassword))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_proxyusername))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_proxypassword))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_proxy))!= NULL)
    {
        free(tmp);
    }

    if ((tmp = get_option(opt_processname))!= NULL)
    {
        free(tmp);
    }
    
    return;
}

static VOS_CHAR *ATP_NAS_WebDAV_MsgRetParse(VOS_UINT32 retVal)
{
    switch(retVal)
    {
        case VOS_OK:return "VOS_OK";
        case ATP_MSG_RET_PARA_ERR:return "ATP_MSG_RET_PARA_ERR";
        case ATP_MSG_RET_TIMEOUT: return "ATP_MSG_RET_TIMEOUT";
        case ATP_MSG_RET_MEM_ERR: return "ATP_MSG_RET_MEM_ERR";
        case ATP_MSG_RET_PEER_DEAD:return "ATP_MSG_RET_PEER_DEAD";
        case ATP_MSG_RET_INTERNAL_ERR:return "ATP_MSG_RET_INTERNAL_ERR";
        default:return "ATP_MSG_RET_PARA_UNKNOWN";
    }
}

VOS_VOID ATP_NAS_FindOutCadaverWorkMode(int *mode, int argc, char **argv)
{
    if (NULL == mode)
    {
        return;
    }

    *mode = ATP_WEBDAV_COMMAND_STATE;
        
    if (argc > 1 && argv[1])
    {
        if (strcmp(argv[1], "sync")==0
          ||strcmp(argv[1], "backup")==0
          ||strcmp(argv[1], "restore")==0)
        {
            *mode = ATP_WEBDAV_WEBDAV_STATE;
        }
        else if (strcmp(argv[1], "subcadaver")==0)
        {
            *mode = ATP_WEBDAV_SUBCAD_STATE;
        }
        else
        {
            *mode = ATP_WEBDAV_COMMAND_STATE;
        }
    }
}

VOS_VOID ATP_NAS_GetCurrentWorkMode(int *mode)
{
    VOS_CHAR *workmode = NULL;

    if (NULL == mode)
    {
        return;
    }

    *mode = ATP_WEBDAV_COMMAND_STATE;

    workmode = get_option(opt_workmode);
    if (NULL == workmode)
    {
        return;
    }

    if(strcmp(workmode, "command_mode")==0)
    {
        *mode = ATP_WEBDAV_COMMAND_STATE;
        return;
    }

    if (strcmp(workmode, "webdav_mode")==0)
    {
        *mode =  ATP_WEBDAV_WEBDAV_STATE;
        return;
    }

    if (strcmp(workmode, "subdav_mode")==0)
    {
        *mode = ATP_WEBDAV_SUBCAD_STATE;
        return;
    }    
}

/******************************************************************************
  函数名称  : ATP_NAS_WebDAV_ProxyProcess
  功能描述  : 为NAS设置proxy
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_WebDAV_ProxyProcess(ATP_MSG_HEADER_ST *pstMsg)
{
    VOS_UINT32 retVal     = VOS_OK;
    VOS_UINT32 bConnected = session.connected;
    ATP_NAS_PROXY_REQUEST_MSG *pstRequest = (ATP_NAS_PROXY_REQUEST_MSG *)pstMsg;
    VOS_CHAR  *port     = NULL;
        
    if (NULL == pstMsg)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    if(pstRequest->stMsgHead.ulMsgType == ATP_WEBDAV_SET_PROXY
     &&(pstRequest->stProxy.chUserName[0] == '\0'
        ||pstRequest->stProxy.chPassword[0] == '\0'
        ||pstRequest->stProxy.chProxyUri[0] == '\0'))
    {
        ATP_WEBDAV_DEBUG("Proxy information lost...\n");
        return ATP_WEBDAV_PROXY_INFO_ERR;
    }

    /*设置或取消代理时若当前存在连接需要先关闭连接*/
    if (bConnected)
    {
        ATP_NAS_LogOut_WebDavServer();
    }

    if (pstMsg->ulMsgType == ATP_WEBDAV_SET_PROXY)
    {            
        port = strchr(pstRequest->stProxy.chProxyUri, ':');
        if (port != NULL)
        {
            *port = '\0';
             port++;
        }
        else
        {
            port = "8080";
        }

        retVal = ATP_NAS_SetupProxy(pstRequest->stProxy.chProxyUri, port, pstRequest->stProxy.chUserName, pstRequest->stProxy.chPassword);
    }
    else if (pstMsg->ulMsgType == ATP_WEBDAV_UNSET_PROXY)
    {
        retVal = ATP_NAS_UnsetProxy();
    }
    else
    {
        ATP_WEBDAV_DEBUG("unknown message for proxy process... msgtype[%d]\n", pstMsg->ulMsgType);
        retVal = ATP_WEBDAV_UNCORRECT_MSGTYPE_ERR;
    }

    if (bConnected) /*恢复原有连接*/
    {
        open_connection(get_option(opt_mediencenter));
    }

    return retVal;
    
}
/******************************************************************************
  函数名称  : ATP_NAS_UnsetProxy
  功能描述  : 取消cadaver内保持的代理信息
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_UnsetProxy(VOS_VOID)
{
    VOS_CHAR *PreProxyName     = NULL;
    VOS_CHAR *PreProxyPort     = NULL; 
    VOS_CHAR *PreProxyUserName = NULL;
    VOS_CHAR *PreProxyPassword = NULL;

    PreProxyName = get_option(opt_proxy);
    PreProxyPort = get_option(opt_proxy_port);
    PreProxyUserName = get_option(opt_proxyusername);
    PreProxyPassword = get_option(opt_proxypassword);

    if (NULL != PreProxyName)
    {
        free(PreProxyName);
        set_option(opt_proxy, NULL);
    }

    if (NULL != PreProxyPort)
    {
        free(PreProxyPort);
        set_option(opt_proxy_port, NULL);
    }

    if (NULL != PreProxyUserName)
    {
        free(PreProxyUserName);
        set_option(opt_proxyusername, NULL);
    }

    if (NULL != PreProxyPassword)
    {
        free(PreProxyPassword);
        set_option(opt_proxypassword, NULL);
    }

    return VOS_OK;
}

/******************************************************************************
  函数名称  : ATP_NAS_SetupProxy
  功能描述  : cadaver设置代理信息
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_SetupProxy(VOS_CHAR *ProxyName, VOS_CHAR *ProxyPort, VOS_CHAR *ProxyUserName, VOS_CHAR *ProxyPassword)
{
    ATP_NAS_UnsetProxy();

    set_option(opt_proxy, ne_strdup(ProxyName));
    set_option(opt_proxy_port, ne_strdup(ProxyPort));
    set_option(opt_proxyusername, ne_strdup(ProxyUserName));
    set_option(opt_proxypassword, ne_strdup(ProxyPassword));

    ATP_WEBDAV_DEBUG("Proxy:%s\n", ProxyName);
    ATP_WEBDAV_DEBUG("port:%s\n", ProxyPort);
    ATP_WEBDAV_DEBUG("User:%s\n", ProxyUserName);
    ATP_WEBDAV_DEBUG("Passwd:%s\n", ProxyPassword);
    
    return VOS_OK;
}

/******************************************************************************
  函数名称  : ATP_NAS_RefreshMediencenterUrl
  功能描述  : 刷新mediencenter服务器地址
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_RefreshMediencenterUrl()
{
    FILE *pFile = NULL;
    VOS_CHAR * pchOldMediencenter = NULL;
    VOS_CHAR acLine[512];
    VOS_CHAR acTemp[512];

    pchOldMediencenter = (VOS_CHAR *)get_option(opt_mediencenter);

    memset(acLine, 0x00, sizeof(acLine));
    memset(acTemp, 0x00, sizeof(acTemp));

    pFile = fopen("/var/webdav", "r");
    if (NULL == pFile)
    {
        return VOS_OK;  //没有文件信息就用以前老的地址
    }
    else
    {
        if (fgets(acLine, sizeof(acLine), pFile))
        {
            if ('\n' == acLine[strlen(acLine)-1])
            {
                acLine[strlen(acLine)-1] = '\0';
            }
            //默认德电的服务器均是ssl加密的
            if (NULL == strstr(acLine, "http"))
            {
                snprintf(acTemp, sizeof(acTemp), "https://%s", acLine);
            }
            else
            {
                snprintf(acTemp, sizeof(acTemp), "%s", acLine);
            }

            //更新mediencenter服务器地址
            if (0 != strcmp(acTemp, pchOldMediencenter))
            {
                ne_free(pchOldMediencenter);
                set_option(opt_mediencenter, ne_strdup(acTemp));
            }
        }
		
		close(pFile);
    }

    return VOS_OK;

}

/******************************************************************************
  函数名称  : ATP_NAS_LogOn_WebDavServer
  功能描述  : 为某个NAS用户登录mediencenter服务器
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_LogOn_WebDavServer(ATP_MSG_HEADER_ST *pstMsgHeader)
{
    //lint -save -e826
    USERINFO  *pUserInfo   = (USERINFO *)((VOS_CHAR *)pstMsgHeader+sizeof(ATP_MSG_HEADER_ST));
    VOS_CHAR  *oldusername = NULL;
    VOS_CHAR  *oldpassword = NULL;
    //lint -restore
            
    if (NULL == pstMsgHeader)
    {
        return VOS_NOK ;
    }

    if (session.connected)
    {
        ATP_NAS_LogOut_WebDavServer();
    }
    
    oldusername = get_option(opt_medusername);
    if (oldusername != NULL)
    {
        free(oldusername);
    }
    
    oldpassword = get_option(opt_medpassword);
    if (oldpassword != NULL)
    {
        free(oldpassword);
    }
    
    /*设置用户名及密码*/
    set_option(opt_medusername, ne_strdup(pUserInfo->chUserName));
    set_option(opt_medpassword, ne_strdup(pUserInfo->chPassword));

    //更新一下mediencenter服务器地址
    ATP_NAS_RefreshMediencenterUrl();

    ATP_WEBDAV_DEBUG("mediencenter info: %s\n", (char *)get_option(opt_mediencenter));
    ATP_WEBDAV_DEBUG("mediencenter user: %s\n", (char *)get_option(opt_medusername));
    //ATP_WEBDAV_DEBUG("mediencenter passwd: %s\n", (char *)get_option(opt_medpassword));

    /* get the connection type */
    cadaver_judge_ppp256();
    if (ATP_CADAVER_IPV4IPV6_NEITHER == g_lCadaverPPP256Mode)
    {
        /* now is in subcadaver mode but ipv4/6 can not be used neighther */
        ATP_WEBDAV_DEBUG("Wan Status: IPv4/IPv6 not in USE! wait for 7s\n");
        sleep(7);
        cadaver_judge_ppp256();
    }

    if (pUserInfo->nAllowdConnect)
    {
        return open_connection(get_option(opt_mediencenter));
    }
    else
    {
        return VOS_OK;
    }  
}

/******************************************************************************
  函数名称  : ATP_NAS_LogOut_WebDavServer
  功能描述  : 为当前NAS用户登出mediencenter服务器
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_LogOut_WebDavServer(VOS_VOID)
{
    if (session.connected)
    {
        close_connection();
    }
    return VOS_OK;
}

static VOS_UINT32 ATP_NAS_TRANS_CHAR_TO_LONG_NUMBER(VOS_CHAR *value, VOS_UINT64 *SpaceNum)
{
    VOS_CHAR *pTemp = value;
        
    if (NULL == value || NULL== SpaceNum)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    *SpaceNum = 0;

    for(pTemp=value; *pTemp!='\0'; pTemp++)
    {
        if (*pTemp<'0' || *pTemp>'9')
        {
            return ATP_WEBDAV_LONG_NUMBER_ERR;
        }

        *SpaceNum *= 10;
        //lint -save -e737
        *SpaceNum += *pTemp-'0';
        //lint -restore
    }

    return VOS_OK;
    
}

/******************************************************************************
  函数名称  : ATP_NAS_FetchSpaceInfo
  功能描述  : 获取空间大小
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_FetchSpaceInfo(VOS_CHAR *DirPath, VOS_CHAR *SpaceType,VOS_UINT64 *SpaceNum)
{
    VOS_UINT32 nRetVal  =  VOS_OK;
    VOS_CHAR   *value   =  NULL;
        
    if (NULL==DirPath || NULL==SpaceType || NULL==SpaceNum)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    if (strcmp(SpaceType, ATP_NAS_AVAILABLE_SPACE)!=0 
     && strcmp(SpaceType, ATP_NAS_USED_SPACE)!=0)
    {
        return ATP_WEBDAV_PARAM_ERR;
    }

    nRetVal = ATP_NAS_GetPropertiesWithRetry(DirPath, SpaceType, &value, ATP_NAS_RETRY_TIMES);
    if (nRetVal!=VOS_OK || value ==NULL)
    {
        ATP_WEBDAV_DEBUG("Get space info fail...retcode[%d]\n", nRetVal);
        return nRetVal;
    }

    ATP_WEBDAV_DEBUG("Space %s Number: %s\n", SpaceType, value);

    nRetVal = ATP_NAS_TRANS_CHAR_TO_LONG_NUMBER(value, SpaceNum);
    if (nRetVal != VOS_OK)
    {
        ATP_WEBDAV_DEBUG("Transform character number to long long fail...\n");
        return nRetVal;
    }
    
    return VOS_OK;
}

/******************************************************************************
  函数名称  : ATP_NAS_FetchFolderSpace
  功能描述  : 获取某个文件夹的大小
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_FetchFolderSpace(VOS_CHAR *pchFolder, VOS_UINT64 *pullSpace)
{
    VOS_UINT32 ret     = VOS_OK;
    VOS_UINT32 nRetVal = VOS_OK;
    VOS_CHAR  *name    = NULL;
    VOS_CHAR  *pchSubFolder  = NULL;
    VOS_UINT64 ullSize = 0;
    VOS_UINT64 ullSubSize = 0;
    struct resource *reslist = NULL;
    struct resource *current = NULL;
    struct resource *next    = NULL;
    struct resource *head    = NULL;
    struct resource *tail    = NULL;

    if (NULL==pchFolder || NULL==pullSpace)
    {
        return VOS_NOK;
    }

    *pullSpace = 0;
        
    ret = execute_ls2(pchFolder, &reslist);
    if (ret == NE_OK) 
    {
	    if (reslist == NULL) /* 空文件夹 */
        {
            *pullSpace = 0;
            return VOS_OK;
	    }
        
        /*首先遍历子文件, 获取全部子文件大小, 并构建子文件夹链表*/
        for (current=reslist; current!=NULL; current=next)
        {
            next = current->next;
                
            if (current->type == resr_normal) 
            {
                ullSize += current->size;
            }

            /*构造子文件夹链表*/
            if (current->type == resr_collection)
            {
                if (head==NULL)
                {
                    head = current;
                    tail = head;
                    tail->next = NULL;
                }
                else
                {
                    if (tail!=NULL)
                    {
                        tail->next = current;
                        tail = tail->next;
                        tail->next = NULL;
                    }
                }
                continue;
            }

            free_resource(current);
        }

        *pullSpace = ullSize; /*当前目录下全部文件大小*/

        /*遍历子文件夹链表, 递归获取各个子文件夹大小*/
        for (current=head; current!=NULL; current=next)
        {
            next = current->next;

            /*获取文件名*/
            if (ne_path_has_trailing_slash(current->uri))
            {
	            current->uri[strlen(current->uri)-1] = '\0';
            }
            name = strrchr(current->uri, '/');
            if (name != NULL && strlen(name+1) > 0) 
            {
	            name++;
            } 
            else 
            {
	            name = current->uri;
            }

            name = ne_path_unescape(name);

            pchSubFolder = NULL;

            /*构造子文件夹在mediencenter上的绝对路径*/
            nRetVal = ATP_NAS_CadaverBuildFullPath(pchFolder, name, &(pchSubFolder));
            if (nRetVal!=VOS_OK || NULL==pchSubFolder)
            {
                for (; current!=NULL; current=next)
                {
                    next = current->next;
                    free_resource(current);
                }

                free(name);

                *pullSpace = 0;

                return VOS_NOK; 
            }

            ullSubSize = 0;
            free(name);
            
            nRetVal = ATP_NAS_FetchFolderSpace(pchSubFolder, &ullSubSize);
            if (nRetVal!=VOS_OK)
            {
                for (; current!=NULL; current=next)
                {
                    next = current->next;
                    free_resource(current);
                }

                free(pchSubFolder);
                *pullSpace = 0;

                return VOS_NOK; 
            }

            *pullSpace += ullSubSize;  /* 加上每个子文件夹大小 */
            
            free(pchSubFolder);
            free_resource(current);
        }

        return VOS_OK;
    }
    else
    {
        return VOS_NOK;
    }

}

/******************************************************************************
  函数名称  : ATP_NAS_FetchDirectoryInfo
  功能描述  : 判断某个文件夹下是否含有子文件夹
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_JudgeHaveSubDirectory(VOS_CHAR *DirPath, VOS_CHAR *pchName, VOS_BOOL *bHaveSubFolder)
{
    VOS_UINT32 ret     = VOS_OK;
    VOS_UINT32 nodeNum = 0;
    VOS_UINT32 nLen    = 0;
    VOS_CHAR   *pchFullPath  = NULL;
    struct resource *reslist = NULL;
    struct resource *current = NULL;
    struct resource *next    = NULL;

    if (NULL==DirPath || NULL==pchName || NULL==bHaveSubFolder)
    {
        return VOS_NOK;
    }

    *bHaveSubFolder = VOS_FALSE;

    /*construct the full path*/
    nLen = strlen(DirPath) + strlen(pchName) + 2;

    pchFullPath = malloc(nLen);
    if (NULL==pchFullPath)
    {
        ATP_WEBDAV_DEBUG("malloc memory fail...\n");
        return VOS_NOK;
    }

    memset(pchFullPath, 0x00, nLen);

    if (DirPath[strlen(DirPath)-1]!='/')
    {
        snprintf(pchFullPath, nLen, "%s/%s", DirPath, pchName);
    }
    else
    {
        snprintf(pchFullPath, nLen, "%s%s", DirPath, pchName);
    }
    
    ret = execute_ls2(pchFullPath, &reslist);
    if (ret == NE_OK) 
    {
	    if (reslist == NULL) 
        {
            free(pchFullPath);
            return VOS_OK;
	    }

        for(current = reslist; current!=NULL; current=next)
        {
            next = current->next;
            
            if (current->type == resr_collection)
            {
                nodeNum++;
            }

            free_resource(current);
        }

        if (nodeNum>0)
        {
            *bHaveSubFolder = VOS_TRUE;
        }
    }
    else
    {
        free(pchFullPath);
        return VOS_NOK;
    }

    free(pchFullPath);
    return VOS_OK;
}

/******************************************************************************
  函数名称  : ATP_NAS_FetchDirectoryInfo
  功能描述  : 获取某个文件夹下的全部子文件夹信息
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_FetchSubDirectory(VOS_CHAR *DirPath, ATP_MSG_HEADER_ST **pstMsg)
{
    VOS_UINT32 ret     = VOS_OK;
    VOS_UINT32 nodeNum = 0;
    VOS_UINT32 reaNum  = 0;
    VOS_UINT32 nRetry  = 0;
    VOS_CHAR  *name    = NULL;
    struct resource *reslist = NULL;
    struct resource *current = NULL;
    struct resource *next    = NULL;
    DIRLISTNODE     *temp    = NULL;

    if (NULL==DirPath || NULL==pstMsg)
    {
        return VOS_NOK;
    }

    *pstMsg = NULL;

    /*Mediencenter offten returns error, we will try this for 3 times*/
    for (nRetry=0; nRetry<3; nRetry++)
    {
        ret = execute_ls2(DirPath, &reslist);
        if (ret == NE_OK && NULL != reslist)
        {
            break;
        }

        if (ret != NE_OK)
        {
            ATP_WEBDAV_DEBUG("Fetch subfolder for %s fail\n", DirPath);
        }

        if (ret == NE_OK && NULL == reslist)
        {
            ATP_WEBDAV_DEBUG("It's an empty directory\n");
        }

        if (nRetry < 2)
        {
            ATP_WEBDAV_DEBUG("Try ls again\n");
        }
    }
    
    if (ret == NE_OK && NULL != reslist) 
    {
        /*first: count the node number*/
        for(current = reslist; current != NULL; current = current->next)
        {
            if (current->type == resr_collection)
            {
                nodeNum++;
            }
        }

        if (0 == nodeNum)
        {
            ATP_WEBDAV_DEBUG("There's no sub folder found\n");
            
            for (current = reslist; current != NULL; current = next)
            {
                next = current->next;
                free_resource(current);
            }
            
            return VOS_OK;  /*empty directory*/
        }

        *pstMsg = (ATP_MSG_HEADER_ST *)malloc(sizeof(ATP_MSG_HEADER_ST) + nodeNum*sizeof(DIRLISTNODE));
        if (NULL == *pstMsg)
        {
            ATP_WEBDAV_DEBUG("malloc memory fail...\n");
            return VOS_NOK;
        }

        /*将获取到的信息设置进消息体中*/
	    for (reaNum = 0, current = reslist; reaNum<=nodeNum&&current!=NULL; current = next) 
        {
		    next = current->next;

            if (current->type != resr_collection)
            {
                free_resource(current);
                continue;
            }
            //lint -save -e826
            /*偏移至合理的位置上去*/
            temp = (DIRLISTNODE*)((VOS_CHAR *)(*pstMsg) + sizeof(ATP_MSG_HEADER_ST) + reaNum*sizeof(DIRLISTNODE));
            memset((char *)temp, 0x00, sizeof(DIRLISTNODE));
            //lint -restore

            /*获取文件名*/
            if (ne_path_has_trailing_slash(current->uri))
            {
	            current->uri[strlen(current->uri)-1] = '\0';
            }
            name = strrchr(current->uri, '/');
            if (name != NULL && strlen(name+1) > 0) 
            {
	            name++;
            } 
            else 
            {
	            name = current->uri;
            }

            name = ne_path_unescape(name);

            if (strlen(name)<sizeof(temp->chDirName))
            {
                snprintf(temp->chDirName, sizeof(temp->chDirName), "%s", name);

                temp->bHaveSubFold = VOS_TRUE;
                #if 0  //每个子文件夹均遍历效率太慢
                /*校验该目录是否存在子文件夹*/
                ATP_NAS_JudgeHaveSubDirectory(DirPath, name, &(temp->bHaveSubFold));
                #endif
                free(name);
            }
            else
            {
                /*当前文件名过长，无法记录*/
                ATP_WEBDAV_DEBUG("erro: Dir name too long:%s\n", name);
                free(name);
                free_resource(current);
                continue;
            }
            
            free_resource(current);

            reaNum++;
	    }
        
        (*pstMsg)->ulMsgData = VOS_OK;
        (*pstMsg)->ulDataLen = reaNum*sizeof(DIRLISTNODE);

        return VOS_OK;
    }    
    else if (ret != NE_OK)
    {
        return VOS_NOK;   /*ls fail*/
    }
    else
    {
        return VOS_OK;    /*empty folder*/
    }
}

/******************************************************************************
  函数名称  : ATP_NAS_FetchDirectoryInfo
  功能描述  : 获取某个文件夹下的全部子文件及文件夹信息
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_FetchDirectoryInfo(VOS_CHAR *DirPath, ATP_MSG_HEADER_ST **ppstMsg)
{
    VOS_BOOL   bMaybeEmpty = VOS_FALSE;
    VOS_UINT32 ret     = VOS_OK;
    VOS_UINT32 nodeNum = 0;
    VOS_UINT32 reaNum  = 0;
    VOS_UINT32 nRetry  = 0;
    VOS_CHAR  *name    = NULL;
    struct resource *reslist = NULL;
    struct resource *current = NULL;
    struct resource *next    = NULL;
    FILELISTNODE    *temp    = NULL;

    if (NULL==DirPath || NULL==ppstMsg)
    {
        return VOS_NOK;
    }

    *ppstMsg = NULL;
        
    /*Mediencenter offten returns error, we will try this for 3 times*/
    for (nRetry = 0; nRetry < 5; nRetry++)
    {
        ret = execute_ls2(DirPath, &reslist);
        if (ret == NE_OK && NULL != reslist)
        {
            break;
        }

        if (ret != NE_OK)
        {
            ATP_WEBDAV_DEBUG("Fetch subinfo for %s fail\n", DirPath);
        }

        if (ret == NE_OK && NULL == reslist)
        {
            ATP_WEBDAV_DEBUG("It's an empty directory\n");
            bMaybeEmpty = VOS_TRUE;
        }

        if (nRetry < 4)
        {
            ATP_WEBDAV_DEBUG("Try ls again\n");
        }
    }

    if (VOS_TRUE == bMaybeEmpty && NE_OK != ret)
    {
        ret = NE_OK;
        reslist = NULL;
    }

    if (ret == NE_OK)
    {   
        /*first: count the node number*/
        for(current = reslist; current!=NULL; current=current->next)
        {
            nodeNum++;
        }

        if (0 == nodeNum)
        {
            for (current = reslist; current != NULL; current = next)
            {
                next = current->next;
                free_resource(current);
            }
            
            return VOS_OK;  /*empty directory*/
        }

        *ppstMsg = (ATP_MSG_HEADER_ST *)malloc(sizeof(ATP_MSG_HEADER_ST) + nodeNum*sizeof(FILELISTNODE));
        if (NULL==*ppstMsg)
        {
            ATP_WEBDAV_DEBUG("malloc memory fail...\n");
            return VOS_NOK;
        }
        
        /*将获取到的信息设置进消息体中*/
	    for (reaNum = 0, current = reslist; reaNum<nodeNum&&current!=NULL; current = next) 
        {
		    next = current->next;
            //lint -save -e826
            /*偏移至合理的位置上去*/
            temp = (FILELISTNODE*)((VOS_CHAR *)(*ppstMsg) + (sizeof(ATP_MSG_HEADER_ST) + reaNum*sizeof(FILELISTNODE)));
            //lint -restore
            memset((char *)temp, 0x00, sizeof(FILELISTNODE));

            /*获取文件名*/
            if (ne_path_has_trailing_slash(current->uri))
            {
	            current->uri[strlen(current->uri)-1] = '\0';
            }
            name = strrchr(current->uri, '/');
            if (name != NULL && strlen(name+1) > 0) 
            {
	            name++;
            } 
            else 
            {
	            name = current->uri;
            }

            name = ne_path_unescape(name);

            if (strlen(name)<sizeof(temp->chFileName))
            {
                snprintf(temp->chFileName, sizeof(temp->chFileName), "%s", name);
                free(name);
            }
            else
            {
                /*当前文件名过长，无法记录*/
                ATP_WEBDAV_DEBUG("Error: File name too long:%s\n", name);
                free(name);
                free_resource(current);
                continue;
            }

            /*获取文件类型*/
            switch(current->type)
            {
                case resr_normal: 
                {
                    temp->nFileType = ATP_NAS_FILE_TYPE;
                    break;
                }
                case resr_collection:
                {
                    temp->nFileType = ATP_NAS_DIRECTORY_TYPE;
                    break;
                }
                case resr_reference:
                {
                    temp->nFileType = ATP_NAS_REFERENCE_TYPE;
                    break;
                }
                default:
                {
                    temp->nFileType = ATP_NAS_UNKNOWN_TYPE;
                    break;
                }
            }

            /*获取文件大小及最后修改时间戳*/
            temp->nSize = current->size;
            temp->nLastModTime = current->modtime;
            temp->pNext = NULL;           
          
            free_resource(current);

            reaNum++;
	    }

        (*ppstMsg)->ulMsgData = VOS_OK;
        (*ppstMsg)->ulDataLen = reaNum*sizeof(FILELISTNODE);

        return VOS_OK;    
    }
    else
    {
        return VOS_NOK;
    }

}

/******************************************************************************
  函数名称  : ATP_NAS_CreateDirectory
  功能描述  : 创建某个文件夹
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_CreateDirectory(VOS_CHAR *DirPath)
{
    if (NULL==DirPath)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    return ATP_NAS_Execute_mkcol(DirPath);
}

/******************************************************************************
  函数名称  : ATP_NAS_DeleteDirectory
  功能描述  : 删除某个文件夹
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_DeleteDirectory(VOS_CHAR *DirPath)
{
    if (NULL==DirPath)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    return ATP_NAS_Execute_rmcol(DirPath);
}

/******************************************************************************
  函数名称  : ATP_NAS_UpLoadFile
  功能描述  : 上载某个文件
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_UpLoadFile(VOS_CHAR *hddFileName, VOS_CHAR *webDavFileName)
{
    if (NULL == hddFileName || NULL == webDavFileName)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    return ATP_NAS_Execute_put(hddFileName, webDavFileName);
}

/******************************************************************************
  函数名称  : ATP_NAS_DownloadFile
  功能描述  : 下载某个文件
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_DownloadFile(VOS_CHAR *hddFileName, VOS_CHAR *webDavFileName)
{
    if (NULL == hddFileName || NULL == webDavFileName)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    return ATP_NAS_Execute_get(webDavFileName, hddFileName);
}

/******************************************************************************
  函数名称  : ATP_NAS_DeleteFile
  功能描述  : 删除某个文件
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_DeleteFile(VOS_CHAR *webDavFileName)
{
    if (NULL == webDavFileName)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    return ATP_NAS_Execute_delete(webDavFileName);
}

/******************************************************************************
  函数名称  : ATP_NAS_MoveFile
  功能描述  : 移动某个文件
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_MoveFile(VOS_CHAR *srcFile, VOS_CHAR *dstFile)
{
    if (NULL == srcFile || NULL == dstFile)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    if (strcmp(srcFile, dstFile) == 0)
    {
        return VOS_OK;
    }

    return ATP_NAS_Execute_move(srcFile, dstFile);
}

/******************************************************************************
  函数名称  : ATP_NAS_MoveFile
  功能描述  : 复制某个文件
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_CopyFile(VOS_CHAR *srcFile, VOS_CHAR *dstFile)
{
    if (NULL == srcFile || NULL == dstFile)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }

    if (strcmp(srcFile, dstFile) == 0)
    {
        return VOS_OK;
    }

    return ATP_NAS_Execute_move(srcFile, dstFile);
}

/******************************************************************************
  函数名称  : ATP_NAS_GetProperties
  功能描述  : 获取某个property值
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_GetProperties(VOS_CHAR *res, VOS_CHAR *name, VOS_CHAR **value)
{
    if (NULL == res || NULL == name || NULL == value)
    {
        return ATP_WEBDAV_NULL_PTR_ERR;
    }
        
    ATP_NAS_Execute_propget(res, name, value);
    *value = session.propvalue;

    return ATP_NAS_TranslateError(session);
}

/******************************************************************************
  函数名称  : ATP_NAS_Check_ConnectionToServer
  功能描述  : 检查当前CADAVER与MEDIENCENTER之间的连接状态
  输入参数  : 
  调用函数  : 
  被调函数  : 
  输出参数  : 
  返 回 值  : 待定
******************************************************************************/
static VOS_UINT32 ATP_NAS_Check_ConnectionToServer(VOS_UINT32 *pnConnState)
{
#ifdef ATP_PCLINT
    pnConnState = pnConnState;
#endif
    return VOS_OK;
}

static VOS_UINT32 ATP_NAS_SafeExitProcesses(VOS_VOID)
{   
    return ATP_MSG_SendSimpleMsg(NULL, ATP_MSG_SAFE_EXIT_PROCESS, VOS_OK);
}

static VOS_UINT32 ATP_NAS_CadaverBuildFullPath(VOS_CHAR *prePath, VOS_CHAR *lastname, VOS_CHAR **ppFullPath)
{
    VOS_UINT32  nLen = 0;
    VOS_UINT32  nPrePathLen = 0;
    VOS_UINT32  nLastNameLen = 0;

    if (NULL==prePath || NULL==lastname || NULL==ppFullPath)
    {
        return VOS_NOK;
    }

    *ppFullPath = NULL;

    nPrePathLen  = strlen(prePath);
    nLastNameLen = strlen(lastname);

    if (prePath[nPrePathLen-1] == '/' && lastname[0]=='/') 
    {
        nLen = nPrePathLen + nLastNameLen;
    }
    else if (prePath[nPrePathLen-1] == '/' && lastname[0]!='/')
    {
        nLen = nPrePathLen + nLastNameLen + 1;
    }
    else if (prePath[nPrePathLen-1] != '/' && lastname[0]=='/')
    {
        nLen = nPrePathLen + nLastNameLen + 1;
    }
    else if (prePath[nPrePathLen-1] != '/' && lastname[0]!='/')
    {
        nLen = nPrePathLen + 1 + nLastNameLen + 1;
    }
    else
    {
        return VOS_NOK;
    }

    *ppFullPath = malloc(nLen);
    if (*ppFullPath == NULL)
    {
        return VOS_NOK;
    }

    memset(*ppFullPath, 0x00, nLen);

    if (prePath[nPrePathLen-1] == '/' && lastname[0]=='/')
    {
        snprintf(*ppFullPath, nLen, "%s%s", prePath, lastname+1);
    }
    else if (prePath[nPrePathLen-1] == '/' && lastname[0]!='/')
    {
        snprintf(*ppFullPath, nLen, "%s%s", prePath, lastname);
    }
    else if (prePath[nPrePathLen-1] != '/' && lastname[0]=='/')
    {
        snprintf(*ppFullPath, nLen, "%s%s", prePath, lastname);
    }
    else if (prePath[nPrePathLen-1] != '/' && lastname[0]!='/')
    {
        snprintf(*ppFullPath, nLen, "%s/%s", prePath, lastname);
    }
    else
    {
        return VOS_NOK;
    }
    
    return VOS_OK;
}
//lint -restore

