/******************************************************************************
  ��Ȩ����  : 2009����Ϊ�������޹�˾
  �� �� ��  : dnsapi.h
  ��    ��  : c47036
  ��    ��  : v1.0
  ��������  : 2009-2-20
  ��    ��  : DNS API

  ��ʷ��¼      :
   1.��    ��   : 2009-2-20
     ��    ��   : c47036
     �޸�����   : ��ɳ���

******************************************************************************/

#ifndef __SYS_CMSINT_API_H__
#define __SYS_CMSINT_API_H__

#include <sys/sysinfo.h>
#ifdef SUPPORT_ATP_LIBZ
#include "zlib.h"
#endif
#include "atplog.h"

#define ProvCode_Sub_Len  4
#define MAX_PRV_COD_LEN      64
#define MAX_MAN_OUI_LEN       6
#define MAX_SER_NUM_LEN      64
#define MAX_STD_VER_LEN      64

#ifdef SUPPORT_ATP_LIBZ
#ifdef SUPPORT_DESKTOP
#define ATP_SYS_ZIP_FILE_NAME_D     "tmpzip"
#else
#define ATP_SYS_ZIP_FILE_NAME_D     "/var/tmpzip"
#endif
#endif
/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C"
{
#endif                      
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
