#ifndef __ATP_SCAN_H__
#define __ATP_SCAN_H__
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum tagATP_SCAN_RET_CODE_EN
{
    ATP_SCAN_RET_PARA_ERR       = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_SCANAPI),
    ATP_SCAN_RET_MEM_ERR,
    ATP_SCAN_RET_INTERNAL_ERR,
    ATP_SCAN_RET_SCANDIR_ERR,
    ATP_SCAN_RET_STAT64_ERR,
    ATP_SCAN_RET_NUM_REACH_LIMINT_ERR,
} ATP_SCAN_RET_CODE_EN;

/*此结构用于扫描目录时回调函数使用的参数*/
typedef struct tagSCANNER_ARG_ST
{
    VOS_BOOL                       bDbExist;        /*数据库是否已存在*/
    ATP_INOTIFY_TREE_NODE_ST       *pstCurNode;     /*当前正在扫描的目录的父目录对应于监控树的节点*/
    ATP_INOTIFYBASE_HANDLE_ST      *pstInotifyBase; /*inotifybase实例*/
} SCANNER_ARG_ST;


typedef VOS_UINT32 (*PFAtpScannerProc)(
                            const VOS_CHAR      *pcFullName,
                            const VOS_CHAR      *pcTitle,
                            const struct stat64   *st,
                            VOS_UINT32          *pulCnt,
                            VOS_VOID            *pvArg);

VOS_UINT32 ATP_SC_ScanDir(const VOS_CHAR *pcDir, PFAtpScannerProc pfProc,
                               VOS_VOID *pvArg, VOS_UINT32 *pulCnt);

/* 以下函数在"main/update.c"中 */
VOS_INT32 ATP_SCAN_Update_Init(VOS_VOID);
VOS_INT32 ATP_SCAN_Update_UnInit(VOS_VOID);
VOS_INT32 ATP_SCAN_Update_Id(VOS_INT64 llParentID, VOS_UINT32 uType);
VOS_VOID* ATP_SCAN_Send_Msg(VOS_VOID);

#ifdef ATP_DEBUG
#define ATP_SCANNER_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ##args);}
#else
#define ATP_SCANNER_DEBUG(x...)
#endif

#endif
