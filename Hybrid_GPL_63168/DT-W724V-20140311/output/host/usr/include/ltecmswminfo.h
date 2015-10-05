/******************************************************************************
  版权所有  : 2007-2008，华为技术有限公司
  文 件 名  : wuprofile.h
  作    者  : l00101002
  版    本  : v1.0
  创建日期  : 2009-1-15
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-1-15
     作    者   : l00101002
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_WMINFO_API_H__
#define __ATP_WMINFO_API_H__

#include "atputil.h"
#include "at_if.h"

#define LTE_WM_INFO_LEN_32                      32
#define LTE_WM_INFO_LEN_256                    256
#define LTE_DS_FLOW_CHECK_COUNT                 100000

#define ATP_WAN_INTF_NAS0                 "nas0"

/* service status */
#define LTE_SRV_STAT_NO_SRV               "NoService"
#define LTE_SRV_STAT_LIMIT_SRV          "LimitedService"
#define LTE_SRV_STAT_AVAL_SRV           "AvailableService"
#define LTE_SRV_STAT_LIMREG_SRV      "LimitedRegionalService"
#define LTE_SRV_STAT_SLP_SRV               "SleepService"

/*service domain */
#define LTE_SRV_DOMAIN_NO_SRV       "NoService"
#define LTE_SRV_DOMAIN_CS_ONLY     "CS_capable"
#define LTE_SRV_DOMAIN_PS_ONLY     "PS_Capable"
#define LTE_SRV_DOMAIN_CS_PS            "CS_PS_Capable"
#define LTE_SRV_DOMAIN_SRCH              "Searching"

/*roam status*/
#define LTE_SRV_ROAM_STAT_NORMAL  "Normal"
#define LTE_SRV_ROAM_STAT_ROAM        "Roam" 

/*sim card status*/
#define LTE_SIMCARD_STAT_NONE                       255
#define LTE_SIMCARD_STAT_INVAILD                  0
#define LTE_SIMCARD_STAT_VAILD                       1
#define LTE_SIMCARD_STAT_CS_INVAILD           2
#define LTE_SIMCARD_STAT_PS_INVAILD           3
#define LTE_SIMCARD_STAT_CS_PS_INVAILD    4
#define LTE_SIMCARD_STAT_ROAM_SIM            240

/* sys sub mode */
#define LTE_SYS_SUBMODE_NO_SRV         "NoService"
#define LTE_SYS_SUBMODE_GSM                "GSM"
#define LTE_SYS_SUBMODE_GPRS               "GPRS"  
#define LTE_SYS_SUBMODE_EDGE               "EDGE"
#define LTE_SYS_SUBMODE_WCDMA        "WCDMA"
#define LTE_SYS_SUBMODE_HSDPA            "HSDPA"
#define LTE_SYS_SUBMODE_HSUPA            "HSUPA"
#define LTE_SYS_SUBMODE_LTE                   "LTE"

/* signal */
#define LTE_SIGNAL_QUALITY                      0
#define LTE_SIGNAL_RAW                              1
#define LTE_SIGNAL_PERCETNT                    2

/* syscfg */
#define LTE_SYSCFG_DOMAIN                      0
#define LTE_SYSCFG_LTEBAND                     1

/* domain */
#define LTE_DOMAIN_CS_ONLY                   "CSONLY"
#define LTE_DOMAIN_PS_ONLY                   "PSONLY"
#define LTE_DOMAIN_CS_PS                         "CS_PS"

/* ds flow*/
#define LTE_DS_FLOW_CHECK_TIME               10
#define LTE_DS_FLOW_MAX_UPRATE             50000000
#define LTE_DS_FLOW_MAX_DOWNRATE     150000000

/* card mode*/
#define LTE_CARDMODE_SIM                            "SIM"
#define LTE_CARDMODE_USIM                        "USIM"

/* antenna mode*/
#define LTE_ANTENNA_MODE_ATUO            "Auto"
#define LTE_ANTENNA_MODE_INNER           "Inner"
#define LTE_ANTENNA_MODE_OUTER          "Outer"

/*lte band*/
#define LTE_BAND_LTE1800                                0x4
#define LTE_BAND_LTE2600                                0x40
#define LTE_BAND_LTE800                                   0x80000
#define LTE_BAND_LTE1800_STR                       "LTE1800"
#define LTE_BAND_LTE2600_STR                       "LTE2600"
#define LTE_BAND_LTE800_STR                         "LTE800"

typedef struct
{
    unsigned long long ds_flow_tx;
    unsigned long long ds_flow_rx;
}LTE_Wminfo_ST;

#if defined(__cplusplus)
extern "C" {
#endif
VOS_UINT32 ATP_LTEGetSysinfo(StSysinfo *pstSysinfo);
VOS_UINT32 ATP_LTEGetServiceStatus(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetServiceDomain(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetRoamStatus(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetSysSubMode(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetCellId(VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetRegisterNetwork(VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetSignal(VOS_INT *iDst, VOS_UINT32 ulFlag);
VOS_UINT32 ATP_LTEGetSyscfg(VOS_CHAR *pcDst, VOS_UINT32 ulLen,VOS_UINT32 ulFlag);
VOS_UINT32 ATP_LTEGetDsFlow(LTE_Wminfo_ST  *pstDst);
VOS_VOID ATP_LTEGetWmInfoByTimer(VOS_VOID *pVoid);
VOS_UINT32 ATP_LTEGetSwVersion(VOS_CHAR   *pstDst, VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEGetImei(VOS_CHAR   *pstDst, VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTECardmode(VOS_CHAR   *pstDst, VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTESetCFUN(VOS_BOOL bMode);
VOS_UINT32 ATP_LTESetAntenna(const VOS_CHAR *pcAntenna);
VOS_UINT32 ATP_LTEGetAntenna(VOS_CHAR *pcAntenna, VOS_UINT32 ulLen);

#if defined(__cplusplus)
}
#endif

#endif
