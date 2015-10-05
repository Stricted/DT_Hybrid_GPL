#ifndef __INTERFACE_STATUS_H__
#define __INTERFACE_STATUS_H__

#undef  INTF_STATUS_DECL  
#define INTF_STATUS_DECL(x)  x,


typedef enum
{
    INTF_STATUS_UP,
    INTF_STATUS_DOWN,
    INTF_STATUS_NOT_PRESENT,
    INTF_STATUS_LOWERLAYER_DOWN,
    INTF_STATUS_UNKNOWN,
    INTF_STATUS_DORMANT,
    INTF_STATUS_ERROR,
    INTF_STATUS_MAX
}EN_INTF_STATUS;
#undef  INTF_STATUS_DECL  

typedef VOS_UINT32 (*pfnIntfStatusChgPreProc)(VOS_VOID *pvData, VOS_VOID *pvData2);
typedef VOS_UINT32 (*pfnIntfStatusChgPostProc)(VOS_VOID *pvData, VOS_VOID *pvData2);
typedef VOS_UINT32 (*pfnIntfStatusProc)(VOS_VOID *pvData, VOS_VOID *pvData2);

typedef struct __TagIntfStatusProcNode
{
    pfnIntfStatusChgPreProc pfn_Inf_Status_Change_Pre_Proc;
    pfnIntfStatusChgPostProc pfn_Inf_Status_Change_Post_Proc;
    pfnIntfStatusProc pfn_Inf_Status_Proc;
}IntfStatusProcEntry;

typedef struct __TagIntfStatusProc
{
    IntfStatusProcEntry astIntfFSMProc[INTF_STATUS_MAX];
};

#endif

