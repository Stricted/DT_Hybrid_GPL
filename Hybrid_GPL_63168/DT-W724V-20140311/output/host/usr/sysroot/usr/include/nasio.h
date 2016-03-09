#ifndef __NASIO_H__
#define __NASIO_H__

#include "atptypes.h"
#include "atpconfig.h"
#include "msgapi.h"
#include "time.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define  ATP_NAS_DIR_NAME_MAXLEN     (VOS_UINT32)256
#define  ATP_NAS_FILE_NAME_MAXLEN    (VOS_UINT32)256
#define  ATP_NAS_PROCESSNAME_LEN     (VOS_UINT32)128

#define  ATP_NAS_FILE_TYPE           (VOS_UINT32)0   /*文件类型*/
#define  ATP_NAS_DIRECTORY_TYPE      (VOS_UINT32)1   /*目录类型*/
#define  ATP_NAS_REFERENCE_TYPE      (VOS_UINT32)2   /*引用类型*/
#define  ATP_NAS_UNKNOWN_TYPE        (VOS_UINT32)3   /*未知类型*/

#define  ATP_NAS_CLIENT_LOCAL   (VOS_UINT32)0    /* 客户端类型为HDD          */
#define  ATP_NAS_CLIENT_WEBDAV  (VOS_UINT32)1    /* 客户端类型为Mediencenter */
#define  ATP_NAS_SUBCADAVER     (VOS_UINT32)2    /* 客户端类型为subcadaver   */
#define  ATP_NAS_CLIENT_HTTP    (VOS_UINT32)3
#define  ATP_NAS_CLIENT_FTP     (VOS_UINT32)4
#define  ATP_NAS_CLIENT_SAMBA   (VOS_UINT32)5

#define  ATP_NAS_ALLOWED_CONNECT     (VOS_UINT32)1   /* 允许连接至服务器   */
#define  ATP_NAS_DISALLOWED_CONNECT  (VOS_UINT32)0   /* 不允许里连接服务器 */

/*最大等待市场待测试待优化*/
#define  ATP_NAS_CONNECT_WAIT_TIME (100000)/*28s不够，28s网关登mediacenter超时失败，但从网页登可以成功*/
#define  ATP_NAS_COMMON_WAIT_TIME  (200000)
#define  ATP_NAS_LS_WAIT_TIME      (600000)
#define  ATP_NAS_FILE_WAIT_TIME    (6*60*60*1000) //上传、下载文件最多等待6小时(大文件保护)

typedef VOS_UINT32 (*PFNasSetResponseData)(VOS_VOID *pstHook, const VOS_CHAR *pcDir, VOS_BOOL bHasSubFloder);

/*某路径下全部文件及子文件夹信息*/
typedef struct tagFileListNode
{
    VOS_CHAR    chFileName[ATP_NAS_FILE_NAME_MAXLEN];
    VOS_UINT32  nFileType;
    VOS_UINT32  privilege;
    time_t      nLastModTime;
    VOS_UINT64  nSize;
    struct tagFileListNode *pNext;
}FILELISTNODE, *PFILELISTNODE, **PPFILELISTNODE;

/*某路径下全部子文件夹信息*/
typedef struct tagDirListNode
{
    VOS_CHAR chDirName[ATP_NAS_DIR_NAME_MAXLEN];
    VOS_BOOL bHaveSubFold;            /*VOS_TRUE -- 有子文件夹 VOS_FALSE -- 无子文件夹*/
    struct tagDirListNode *pNext;
}DIRLISTNODE, *PDIRLISTNODE, **PPDIRLISTNODE;

typedef struct tagNasClient
{
    VOS_UINT32 nClientType;
	VOS_CHAR   localProcessName[ATP_MSG_MAX_NAME_LEN + 1];
	VOS_CHAR   clientProcessName[ATP_MSG_MAX_NAME_LEN + 1];
}NASCLIENT, *PNASCLIENT, **PPNASCLIENT;

typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_CreateDirectory)(VOS_CHAR *DirectoryFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_DeleteDirectory)(VOS_CHAR *DirectoryFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_MoveDirectory)(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_RenameDirectory)(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_CreateFile)(VOS_CHAR *FileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_DeleteFile)(VOS_CHAR *FileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_MoveFile)(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_CopyFile)(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_RenameFile)(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_UploadFile)(VOS_CHAR *localFileFullPath, VOS_CHAR *RemoteFileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_DownloadFile)(VOS_CHAR *RemoteFileFullPath, VOS_CHAR *localFileFullPath);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_LS_DIRECTORY)(VOS_CHAR *dstDirFullPath, FILELISTNODE **listNode);
typedef	VOS_UINT32 (*ATP_NAS_FUNC_PTR_LS_FILE)(VOS_CHAR *dstFileFullPath, FILELISTNODE **listNode);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_GetMtime)(VOS_CHAR *dstFilePath, time_t *mtime);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_SetMtime)(VOS_CHAR *dstFilePath, time_t mtime);
	
typedef struct tagNasIOConfig
{
	ATP_NAS_FUNC_PTR_CreateDirectory    pFuncCreateDirectory;
	ATP_NAS_FUNC_PTR_DeleteDirectory    pFuncDeleteDirectory;
	ATP_NAS_FUNC_PTR_MoveDirectory      pFuncMoveDirectory;
	ATP_NAS_FUNC_PTR_RenameDirectory    pFuncRenameDirectory;
	ATP_NAS_FUNC_PTR_CreateFile         pFuncCreateFile;
	ATP_NAS_FUNC_PTR_DeleteFile         pFuncDeleteFile;
	ATP_NAS_FUNC_PTR_MoveFile           pFuncMoveFile;
    ATP_NAS_FUNC_PTR_CopyFile           pFuncCopyFile;
	ATP_NAS_FUNC_PTR_RenameFile         pFuncRenameFile;
	ATP_NAS_FUNC_PTR_LS_DIRECTORY       pFuncLsDirectory;
	ATP_NAS_FUNC_PTR_LS_FILE            pFuncLsFile;
    ATP_NAS_FUNC_PTR_GetMtime           pFuncGetMtime;
    ATP_NAS_FUNC_PTR_SetMtime           pFuncSetMtime;
}ATP_NAS_FILEIO_CONFIG;

typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_ConnectServer)(VOS_CHAR *username, VOS_CHAR *password, VOS_UINT32 bConnect);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_DisconnectServer)(VOS_VOID);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_FetchSpaceInfo)(VOS_UINT64 *availSpace, VOS_UINT64 *usedSpace);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_LsDirectory)(const VOS_CHAR *dstDir, VOS_VOID *pstHook, PFNasSetResponseData pfSetResponseData);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_CheckConnect)(VOS_VOID);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_StartupSubcadaver)(VOS_VOID);
typedef VOS_UINT32 (*ATP_NAS_FUNC_PTR_ShutdownSubcadaver)(VOS_VOID);

/*为jobcenter访问subcadaver封装操作接口*/
typedef struct tagNasSubCadaverConfig
{
    ATP_NAS_FUNC_PTR_StartupSubcadaver  pFuncStartupSubCadaver;
    ATP_NAS_FUNC_PTR_ShutdownSubcadaver pFuncShutdownSubCadaver;
    ATP_NAS_FUNC_PTR_CreateDirectory    pFuncCreateDirectory;
    ATP_NAS_FUNC_PTR_ConnectServer      pFuncConnectServer;
    ATP_NAS_FUNC_PTR_DisconnectServer   pFuncDisconnectServer;
    ATP_NAS_FUNC_PTR_FetchSpaceInfo     pFuncFetchSpaceInfo;
    ATP_NAS_FUNC_PTR_LsDirectory        pFuncFetchLsDirectory;
    ATP_NAS_FUNC_PTR_CheckConnect       pFuncCheckConnect;
}ATP_NAS_SUBCADAVER_CONFIG;

VOS_UINT32 ATP_NAS_IO_CONFIG(ATP_NAS_FILEIO_CONFIG *config, NASCLIENT *client);
VOS_UINT32 ATP_NAS_SubCadaverConfig(ATP_NAS_SUBCADAVER_CONFIG *config);

VOS_UINT32 ATP_NAS_LOCAL_CreateDirectory(VOS_CHAR *DirectoryFullPath);
VOS_UINT32 ATP_NAS_LOCAL_DeleteDirectory(VOS_CHAR *DirectoryFullPath);
VOS_UINT32 ATP_NAS_LOCAL_MoveDirectory(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
VOS_UINT32 ATP_NAS_LOCAL_RenameDirectory(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
VOS_UINT32 ATP_NAS_LOCAL_CreateFile(VOS_CHAR *FileFullPath);
VOS_UINT32 ATP_NAS_LOCAL_DeleteFile(VOS_CHAR *FileFullPath);
VOS_UINT32 ATP_NAS_LOCAL_MoveFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_LOCAL_CopyFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_LOCAL_RenameFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_LOCAL_Ls_DIRECTORY(VOS_CHAR *dstDirFullPath, FILELISTNODE **listNode);
VOS_UINT32 ATP_NAS_LOCAL_Ls_FILE(VOS_CHAR *dstFileFullPath, FILELISTNODE **listNode);
VOS_UINT32 ATP_NAS_LOCAL_GetMtime(VOS_CHAR *dstFilePath, time_t *mtime);
VOS_UINT32 ATP_NAS_LOCAL_SetMtime(VOS_CHAR *dstFilePath, time_t mtime);

VOS_VOID ATP_NAS_FreeFilelist(FILELISTNODE *listNode);

#ifdef SUPPORT_ATP_CADAVER
VOS_UINT32 ATP_NAS_WEBDAV_CreateDirectory(VOS_CHAR *DirectoryFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_DeleteDirectory(VOS_CHAR *DirectoryFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_MoveDirectory(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_RenameDirectory(VOS_CHAR *srcDirFullPath, VOS_CHAR *dstDirFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_CreateFile(VOS_CHAR *FileFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_DeleteFile(VOS_CHAR *FileFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_MoveFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_CopyFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_RenameFile(VOS_CHAR *srcFileFullPath, VOS_CHAR *dstFileFullPath);
VOS_UINT32 ATP_NAS_WEBDAV_LS_DIRECTORY(VOS_CHAR *dstDirFullPath, FILELISTNODE **listNode);
VOS_UINT32 ATP_NAS_WEBDAV_LS_FILE(VOS_CHAR *dstFileFullPath, FILELISTNODE **listNode);
VOS_UINT32 ATP_NAS_WEBDAV_GetMtime(VOS_CHAR *dstFilePath, time_t *mtime);
VOS_UINT32 ATP_NAS_WEBDAV_SetMtime(VOS_CHAR *dstFilePath, time_t mtime);

/*for subcadaver*/
VOS_UINT32 ATP_NAS_WEBDAV_StartupSubcadaver(VOS_VOID);
VOS_UINT32 ATP_NAS_WEBDAV_ShutdownSubcadaver(VOS_VOID);
VOS_UINT32 ATP_NAS_WEBDAV_ConnectServer(VOS_CHAR *username, VOS_CHAR *password, VOS_UINT32 bConnect);
VOS_UINT32 ATP_NAS_WEBDAV_DisconnectServer(VOS_VOID);
VOS_UINT32 ATP_NAS_WEBDAV_FetchSpaceInfo(VOS_UINT64 *availSpace, VOS_UINT64 *usedSpace);
VOS_UINT32 ATP_NAS_WEBDAV_LsDirectory(const VOS_CHAR *dstDir, VOS_VOID *pstHook, PFNasSetResponseData pfSetResponseData);
VOS_UINT32 ATP_NAS_WEBDAV_CheckConnect(VOS_VOID);
VOS_UINT32 ATP_NAS_WEBDAV_GetFolderSpace(VOS_CHAR *pchFolder,  VOS_UINT64 *pSpace);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __NASIO_H__ */