/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ltecmsapi.h
  *ipintf 对外api
  *\copyright 2007-2020，华为技术有限公司
  *\author f00110348
  *\date  2013-6-24
*/
#ifndef __LTECMS_API_H__
#define __LTECMS_API_H__
/* <DTS2013081901325 xiazhongyue 20130819 begin */
#ifdef ATP_DEBUG
#define ATP_LTE_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_LTE_DEBUG(x...)
#endif



#if defined(__cplusplus)
extern "C" {
#endif
#define LTE_IFNAME_LEN 32
#define LTE_BAND_LEN 32
#define LTE_NETWORK_LEN 32
#define LTE_CELLID_LEN 256
#define LTE_SERVICE_LEN 32
#define LTE_SERVICEDOMAIN_LEN 32 
#define LTE_ROAM_STATUS 16
#define LTE_SYSSUB_LEN 32
#define LTE_ANTENNA_LEN 8
#define LTE_STR_LEN_32 32
#define LTE_STR_LEN_64 64
#define LTE_UPTIME_FILE "/var/wan/rmnet0/uptime"
#define LTE_INFO_FILE "/var/lte/lteinfo"
#define LTE_STATUS_FILE "/var/lte/enable" 

#define LTE_USIMSTATE_INVALID 0
#define LTE_USIMSTATE_VALID 1
#define LTE_USIMSTATE_CS_INVALID 2
#define LTE_USIMSTATE_NOCARD 255
#define LTE_NOT_READY 100

#define NO_SERVICE				0
#define LIMITED_SERVICE			1
#define VALID_SERVICE			2
#define	LIMITED_DOMAIN_SERVICE	3
#define SLEEP_SERVICE			4

typedef struct 
{   
    VOS_BOOL   bEnable;                          /* 启用或禁用LTE       */
    VOS_UINT32 ulStatus;                         /* 连接状态 UP/DOWN     */       
    VOS_CHAR acInfName[LTE_IFNAME_LEN]; 
    VOS_UINT32 ulLastChange;
    unsigned long long ulCurrentUpRate;   
    unsigned long long ulCurrentDownRate;  
    VOS_CHAR  acBand[LTE_BAND_LEN];
    VOS_BOOL  bRoam;
    VOS_CHAR acRegisterNetwork[LTE_NETWORK_LEN];
    VOS_CHAR acCellID[LTE_CELLID_LEN];
    VOS_UINT32 ulSignalQuilty;
    VOS_INT32 SignalStrengthRaw;
    VOS_INT32 SignalStrenghPercent;
    VOS_CHAR acServiceStatus[LTE_SERVICE_LEN];
    VOS_CHAR acServiceDomain[LTE_SERVICEDOMAIN_LEN];
    VOS_CHAR acRoamStatus[LTE_ROAM_STATUS];
    VOS_CHAR acSysSubMode[LTE_SYSSUB_LEN];
    VOS_CHAR acAntenna[LTE_ANTENNA_LEN];
}LTE_INTERFACE_CFG_ST;

typedef struct
{
    VOS_CHAR acIMEI[LTE_STR_LEN_32];
    VOS_CHAR acIMSI[LTE_STR_LEN_32];
    VOS_CHAR acDevStatus[LTE_STR_LEN_32];
    VOS_CHAR acSimStatus[LTE_STR_LEN_32];
    VOS_CHAR acAntMode[LTE_STR_LEN_64];
    VOS_INT lCellId;
    VOS_INT lRSRP;
    VOS_INT lRSRQ;
    VOS_CHAR acSrvStatus[LTE_STR_LEN_32];
    VOS_INT lTAC;
}LTE_INFO_ST;

typedef struct _ATP_UTIL_RECORD_LIST
{
    VOS_CHAR    acKey[LTE_STR_LEN_64]; //for atp common: ATP_ACTUAL_LEN_64
	time_t      r_time;
    struct _ATP_UTIL_RECORD_LIST *pstNext;
}ATP_UTIL_RECORD_LIST;

extern VOS_UINT32 ATP_LTE_IntfEnableSet(VOS_BOOL bEnable);
extern VOS_UINT32 ATP_LTE_MobileStatsGet(VOS_UINT32 *pulGetdValues);

/* DTS2013081901325 xiazhongyue 20130819 end>   */
#if defined(__cplusplus)
}
#endif

#endif

