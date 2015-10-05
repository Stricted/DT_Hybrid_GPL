#ifndef __ETHLINKCMS_API_H__
#define __ETHLINKCMS_API_H__

#ifdef ATP_DEBUG
#define ATP_ETHLINK_DEBUG(format, args...)       {printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__, __func__, __LINE__);printf(format, ##args);printf("\r\n");}
#else
#define ATP_ETHLINK_DEBUG(x...)
#endif

#define ETH_LINK_TYPE_XDSL 0
#define ETH_LINK_TYPE_ETH 1

#ifdef SUPPORT_ATP_ETH_EXTERNALMODEM
extern VOS_VOID EthLinkAutosenseUp(VOS_BOOL bLinkType);

extern VOS_VOID EthLinkAutosenseDown(VOS_BOOL bLinkType);

VOS_BOOL ATP_ETHLINK_GetExternalModem();
VOS_VOID ATP_ETHLINK_DSLModemStop();
#endif
#endif


