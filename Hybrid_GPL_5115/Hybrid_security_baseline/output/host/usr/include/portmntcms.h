#ifndef __PORTREGIST_CMS_INIT_H__
#define __PORTREGIST_CMS_INIT_H__

typedef enum tagATP_E_ERR_PORTMNT
{
    /*PORT MANAGEMENT×é¼þµÄ´íÎóÂë*/
    ATP_E_ERR_PORTMNT_REG_PORT_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_PORTMNTCMS),
    ATP_E_ERR_PORTMNT_CREATE_ITEM_INVALID,
    ATP_E_ERR_PORTMNT_DEL_PORT_NOTFOUND,
    ATP_E_ERR_PORTMNT_LIST_EMPYT
    
}tagATP_E_ERR_PORTMNT;

extern VOS_UINT32  RegistPortRange(VOS_CHAR *pstModuleName, VOS_UINT32 ulStartPort, VOS_UINT32 ulEndPort);
extern VOS_UINT32  RegistPort(VOS_CHAR *pstModuleName, VOS_UINT32 ulPortNum);
extern VOS_UINT32  DeletePortRange(VOS_CHAR *pstModuleName, VOS_UINT32 ulStartPort, VOS_UINT32 ulEndPort);
extern VOS_UINT32  DeletePort(VOS_CHAR *pstModuleName,VOS_UINT32 ulPortNum);
#ifdef ATP_DEBUG
#define ATP_PORT_DEBUG(format, args...)  {printf("PORT DEBUG =========>>>> file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_PORT_DEBUG(format, args...)
#endif


#endif
