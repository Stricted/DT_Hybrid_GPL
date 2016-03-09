/******************************************************************************
  版权所有  : 2009，华为技术有限公司
  文 件 名  : dnsapi.h
  作    者  : c47036
  版    本  : v1.0
  创建日期  : 2009-2-20
  描    述  : DNS API

  历史记录      :
   1.日    期   : 2009-2-20
     作    者   : c47036
     修改内容   : 完成初稿

******************************************************************************/

#ifndef __SYS_CMSINT_API_H__
#define __SYS_CMSINT_API_H__

#include <sys/sysinfo.h>
#ifdef SUPPORT_ATP_LIBZ
#include "zlib.h"
#endif
#include "atplog.h"
#include "pppcapi.h"


#ifdef SUPPORT_ATP_DT_W724V_PROVISIONINGCODE
#define ProvCode_Sub_Len  3
#define MAX_PRV_COD_LEN      15
#else
#define ProvCode_Sub_Len  4
#define MAX_PRV_COD_LEN      64
#endif

#define MAX_MAN_OUI_LEN       6
#define MAX_SER_NUM_LEN      64
#define MAX_STD_VER_LEN      64

// easysupport changed
#define ATP_CWMP_EASYSUPPORT_CHANGED   (1<<12)
// only auto configuration changed
#define ATP_CWMP_EASYSUPPORT_AUTOCONFIG   (1<<13)

/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C"
{
#endif                      

#define ATP_SYS_PROVCODE_ACS_SERVICE_DIGIT      4
#define ATP_SYS_PROVCODE_SCRAT_DIGIT            5
#define ATP_SYS_PROVCODE_ACCESS_DIGIT           6
#define ATP_SYS_PROVCODE_FON_DIGIT              8
#define ATP_SYS_PROVCODE_VOIP_DIGIT             10

#define ATP_SYS_PROVCODE_BNG_MODE             4

#define ATP_FILE_BNG_MODE             "/var/sysinfo/sys_bnginfo"

VOS_UINT32 ATP_SYSINFO_PrvCodeBNG_NotifyRegister(AtpNotifierProc                pfProc,
		                					    VOS_UINT32                      ulPriority,
        		      						    VOS_VOID                        *pvNotifyHook);
/* BEGIN: add for DTS2013101108878 【Hybrid】Scrat BNG 协议部分实现 by l00214441 2013.10.11 */ 
VOS_BOOL ATP_SYS_ProvCode_BNGIsSet(VOS_VOID);

VOS_BOOL ATP_SYS_BNGMode(VOS_VOID);
/* END: add for DTS2013101108878 【Hybrid】Scrat BNG 协议部分实现 by l00214441 2013.10.11 */ 

VOS_UINT32 ATP_SYS_SaveBNGInfo(VOS_BOOL bBngInfo);

VOS_BOOL ATP_IS_ValidateSubCode(VOS_CHAR* pcValue);
VOS_UINT32 SysGetUpTime();
#ifdef SUPPORT_ATP_LIBZ
VOS_UINT32 ATP_SYS_GetEncodeBase64File(const VOS_CHAR *pcFile, VOS_CHAR **ppcStr);
VOS_UINT32 ATP_SYS_GetDecodeBase64String(const VOS_CHAR *pcContent, const VOS_CHAR *pcDstFile);
#endif
#ifdef SUPPORT_ATP_SETUP_FIRST_USE_DATE
VOS_VOID ATP_SYS_SetupFirstUseDate(const VOS_CHAR *pcDate);
#endif
/* start add of provisioningcode for DT W724V for Access/VoIP by y00179387 */
#ifndef SUPPORT_ATP_DT_W724V_PROVISIONINGCODE
#ifdef SUPPORT_ATP_DT_PROVISIONINGCODE
VOS_UINT32 ATP_SYS_SetManualProvisioningCode(VOS_VOID);
#endif
#else
VOS_UINT32 ATP_SYS_SetManualProvisioningCode(VOS_UINT32 ulProvCodePosition, VOS_CHAR acProvCodeDigit);
#endif
/* end add of provisioningcode for DT W724V for Access/VoIP by y00179387 */

#if defined(__cplusplus)
}
#endif

#endif
