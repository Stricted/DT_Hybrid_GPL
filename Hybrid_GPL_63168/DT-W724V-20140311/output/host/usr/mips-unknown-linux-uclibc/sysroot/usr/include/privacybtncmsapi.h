#ifndef __ATP_PRIVACY_BUTTON_API_H__
#define __ATP_PRIVACY_BUTTON_API_H__

//#define ATP_DEBUG  1

//lint --e{750}
#ifdef ATP_DEBUG
#define PRIVACY_DEBUG(format, args...)       {printf("\n");printf("FUNC: %s LINE: %d: ", __func__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define PRIVACY_DEBUG(x...)
#endif
//lint -restore
VOS_VOID ATP_PRIVACY_AutoReconnectStart(VOS_VOID *pvNotifier);
VOS_UINT32 ATP_PRIVACY_StartForWan(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);
VOS_VOID ATP_PRIVACY_DestroyTimer(VOS_VOID);
VOS_VOID ATP_PrivacyPppIntfClientStop(const char *pszDomain);
VOS_VOID ATP_PRIVACY_CheckDestroyTimer(VOS_VOID);

#endif