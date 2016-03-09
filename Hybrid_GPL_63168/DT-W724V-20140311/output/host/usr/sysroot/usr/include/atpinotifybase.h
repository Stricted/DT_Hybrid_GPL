#ifndef __ATP_INOTIFYBASE_H__
#define __ATP_INOTIFYBASE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inotify.h>

#define MAX_PATHNAME_SIZE 1024

typedef enum tagATP_INOTIFY_RET_CODE_EN
{
    ATP_INOTIFY_RET_PARA_ERR       = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_INOTIFYBASEAPI),
    ATP_INOTIFY_RET_MEM_ERR,
    ATP_INOTIFY_RET_INTERNAL_ERR,
} ATP_INOTIFY_RET_CODE_EN;


typedef struct tagATP_INOTIFY_TREE_NODE_ST
{
    VOS_INT32                                            nWd;		      /* watch descriptor */
    VOS_INT64                                            llObjId;         /*数据库中的对象ID*/
    struct tagATP_INOTIFY_TREE_NODE_ST  *pstParent;    /*父节点指针*/
    struct tagATP_INOTIFY_TREE_NODE_ST  *pstChild;       /*子节点指针*/
    struct tagATP_INOTIFY_TREE_NODE_ST  *pstNext;       /*兄弟节点指针*/
    VOS_CHAR	                                       *pszDirName; /*当前文件夹名字，不含父节点文件夹名字，总是以/结尾*/
} ATP_INOTIFY_TREE_NODE_ST;


typedef struct tagATP_INOTIFYBASE_HANDLE_ST
{
	VOS_INT32                                             nFd;	         /* watch descriptor */
       VOS_UINT32                                           ulMask;     /*Event mask*/
	ATP_INOTIFY_TREE_NODE_ST                  stDirRoot ;   /*目录树根*/
} ATP_INOTIFYBASE_HANDLE_ST;

typedef VOS_UINT32 (*PFTravelProc)(VOS_INT64 llObjID);
typedef VOS_UINT32 (*PFTravelProcEx)(VOS_INT64 llObjID, VOS_VOID* pArgv);


ATP_INOTIFYBASE_HANDLE_ST *ATP_INOTIFYBASE_Init(const VOS_UINT32 ulMask);

VOS_UINT32 ATP_INOTIFYBASE_FolderCreate(
        const ATP_INOTIFYBASE_HANDLE_ST *pstInotifybase,
        ATP_INOTIFY_TREE_NODE_ST *pstNode,
        const VOS_CHAR *pszName,
        const VOS_INT64 llObjID);

ATP_INOTIFY_TREE_NODE_ST *ATP_INOTIFYBASE_FindFolder(
						ATP_INOTIFY_TREE_NODE_ST *pBase,
						char* pstrFolder);
	
ATP_INOTIFY_TREE_NODE_ST *ATP_INOTIFYBASE_Addfolder(
	const ATP_INOTIFYBASE_HANDLE_ST  *pstIinotifybase_instance,
	ATP_INOTIFY_TREE_NODE_ST *pstCurNode,
	const VOS_CHAR *pszFolder,
	VOS_INT64  llObjID);

struct inotify_event *ATP_INOTIFYBASE_GetEvent(
        const ATP_INOTIFYBASE_HANDLE_ST  *pstInotifybase, const VOS_UINT32 ulTimeout);


ATP_INOTIFY_TREE_NODE_ST *ATP_INOTIFYBASE_LocateNode(
        const ATP_INOTIFY_TREE_NODE_ST    *pstCurNode, 
        const VOS_INT32 nWd);

VOS_UINT32 ATP_INOTIFYBASE_Travel(
        const ATP_INOTIFY_TREE_NODE_ST *pstCurNode, 
        PFTravelProc pfProc);

VOS_UINT32 ATP_INOTIFYBASE_Travel_Ex(
    const ATP_INOTIFY_TREE_NODE_ST *pstCurNode, 
    PFTravelProcEx pfProc, void* pArg);


VOS_CHAR *ATP_INOTIFYBASE_GetObjFullName(
        const ATP_INOTIFY_TREE_NODE_ST  *pstCurNode, 
        const VOS_CHAR *pszName);


VOS_UINT32 ATP_INOTIFYBASE_FolderRemove(
        ATP_INOTIFY_TREE_NODE_ST *pstRemoveNode);


VOS_UINT32 ATP_INOTIFYBASE_FolderMoveFrom(
        const VOS_INT32   nFd,    
        const ATP_INOTIFY_TREE_NODE_ST *pstNode);


VOS_UINT32 ATP_INOTIFYBASE_FolderMoveInternal( 
        ATP_INOTIFY_TREE_NODE_ST *pstPreParent, 
        ATP_INOTIFY_TREE_NODE_ST *pstMove,
        ATP_INOTIFY_TREE_NODE_ST *pstNewParent);

#endif
