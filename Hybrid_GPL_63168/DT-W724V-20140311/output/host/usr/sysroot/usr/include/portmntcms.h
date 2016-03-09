#ifndef __PORTREGIST_CMS_INIT_H__
#define __PORTREGIST_CMS_INIT_H__

typedef struct tagWELKNOWN_PORT_ST
{
    VOS_UINT16           ucTcp;      // 1: TCP; 0: UDP
    VOS_UINT16           ucRange;    // ¶Ë¿Ú·¶Î§
    VOS_UINT16          usPort;     // ¶Ë¿ÚºÅ
} WELKNOWN_PORT_ST,ATP_WELKNOWN_PORT_ST,ATP_WELKNOWN_PORT_ST;

typedef struct tagDYN_PORT_RANGE_LIST_ST
{
    struct tagDYN_PORT_RANGE_LIST_ST    *pstNext;
    ATP_WELKNOWN_PORT_ST                stPortRange;
} DYN_PORT_RANGE_LIST_ST;

typedef enum tagATP_E_ERR_PORTMNT
{
    /*PORT MANAGEMENT×é¼þµÄ´íÎóÂë*/
    ATP_E_ERR_PORTMNT_REG_PORT_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_PORTMNTCMS),
    ATP_E_ERR_PORTMNT_CREATE_ITEM_INVALID,
    ATP_E_ERR_PORTMNT_DEL_PORT_NOTFOUND,
    ATP_E_ERR_PORTMNT_LIST_EMPYT
    
}tagATP_E_ERR_PORTMNT;

VOS_UINT32 ATP_CMS_PortMngrDelRange(const ATP_WELKNOWN_PORT_ST *pstPortRange);
VOS_UINT32 ATP_CMS_PortMngrAddRange(const ATP_WELKNOWN_PORT_ST *pstPortRange);
VOS_BOOL ATP_CMS_PortMngrIsRangeFree(const ATP_WELKNOWN_PORT_ST *pstPortRange);
VOS_VOID ATP_CMS_PortMngrDisplay(VOS_VOID);
VOS_VOID ATP_CMS_PortMngrWriteDebug(VOS_VOID);

#ifdef ATP_DEBUG
#define ATP_PORT_DEBUG(format, args...)  {printf("PORT DEBUG =========>>>> file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_PORT_DEBUG(format, args...)
#endif


#endif
