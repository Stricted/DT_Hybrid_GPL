#ifndef __DSLLINECMS_API_H__
#define __DSLLINECMS_API_H__

//include file
#include "atputil.h"
#include "cmo_def.h"



//#define ATP_DEBUG
#ifdef ATP_DEBUG
#define ATP_DSLLINE_DEBUG(format, args...)       {printf("FILE: %s FUNC %s  LINE: %d: ", __FILE__,__FUNCTION__,__LINE__);printf("\033[1;33;40m");printf(format, ##args);printf("\033[0;37;40m\n");}
#else
#define ATP_DSLLINE_DEBUG(x...)
#endif

#define DSL_STR_4_LEN              5
#define DSL_STR_8_LEN              9
#define DSL_STR_12_LEN             13
#define DSL_STR_16_LEN             17
#define DSL_STR_32_LEN             33
#define DSL_STR_64_LEN             65
#define DSL_STR_128_LEN            129


#define XDSL_DEFAULT_UP_INTERLEAVEDEPTH    64
#define XDSL_DEFAULT_DOWN_INTERLEAVEDEPTH  2

#define ADSL_GDMT                   "ADSL_G.dmt"
#define ADSL_T1413                  "ADSL_ANSI_T1.413"
#define ADSL_GLITE                  "ADSL_G.lite"
#define ADSL_READSL                 "ADSL_re-ads"
#define ADSL_ADSL2                  "ADSL_G.dmt.bis"
#define ADSL_ADSL2PLUS              "ADSL_2plus"
#define VDSL_VDSL2                  "VDSL"

#ifdef SUPPORT_ATP_XDSL
enum
{
    XDSL_MODE_UNKNOWN = 0,
    XDSL_MODE_ATM,
    XDSL_MODE_PTM,
    XDSL_MODE_VDSL_ATM,
    XDSL_MODE_VDSL_PTM,
};
#endif

/*the length for long testparams string*/
#define XDSL_TESTPARAM_HLOG_LEN         2559
#define XDSL_TESTPARAM_SNR_LEN           2047

#define ADSL_STATS_INFO_FILE             "/var/adslstats"
#define ADSL_SYS_CMD_LEN                 512
#define ADSL_COMM_CFG_DATA_LEN           64

#define XDSL_LATNDS_LEN             24
#define XDSL_ALLOWEDPROFILE_LEN             32

/*StandardsSupported*/
#define XDSL_STANDARD_G992_1_ANNEX_B    "G.992.1_Annex_B"
#define XDSL_STANDARD_G992_3_ANNEX_B    "G.992.3_Annex_B"
#define XDSL_STANDARD_G992_3_ANNEX_J    "G.992.3_Annex_J"
#define XDSL_STANDARD_G992_3_ANNEX_L    "G.992.3_Annex_L"
#define XDSL_STANDARD_G992_5_ANNEX_B    "G.992.5_Annex_B"
#define XDSL_STANDARD_G992_5_ANNEX_J    "G.992.5_Annex_J"
#define XDSL_STANDARD_G992_5_ANNEX_L    "G.992.5_Annex_L"
#define XDSL_STANDARD_G993_2_ANNEX_B    "G.993.2_Annex_B"

typedef struct tagXDSL_LINE_ARG_ST
{
    VOS_UINT32      ulSynStatus;
} XDSL_LINE_ARG_ST;

typedef struct tagDSL_LINE_STATS_TOTAL_SHOWTIME_ST
{
    VOS_UINT ulErroredSecs;           
    VOS_UINT ulSeverelyErroredSecs;
    VOS_UINT32	ulLossOfFraming;
	VOS_UINT32	ulInitTimeouts;
	VOS_UINT32	ulInitErrors;
	VOS_UINT32	ulLinkRetrain;
	VOS_UINT32	ulCellDelin;
	VOS_UINT32	ulTransmitBlocks;
	VOS_UINT32	ulReceiveBlocks;
	VOS_UINT32 	ulFECErrors;             
	VOS_UINT32 	ulATUCFECErrors;          
	VOS_UINT32 	ulHECErrors;           
	VOS_UINT32 	ulATUCHECErrors;          
	VOS_UINT32 	ulCRCErrors;            
	VOS_UINT32 	ulATUCCRCErrors;          
	
}DSL_LINE_STATS_TOTAL_SHOWTIME_ST;

typedef struct tagDSL_LINE_STATS_ST
{
    VOS_UINT ulBytesSent;           
    VOS_UINT ulBytesReceived;    
    VOS_UINT ulPacketsSent;   
    VOS_UINT ulPacketsReceived;   
    VOS_UINT ulErrorsSent;   
    VOS_UINT ulErrorsReceived;   
    VOS_UINT ulTotalStart;   
    VOS_UINT ulShowtimeStart;   
    DSL_LINE_STATS_TOTAL_SHOWTIME_ST stDslLineStatsTotal;
    DSL_LINE_STATS_TOTAL_SHOWTIME_ST stDslLineStatsShowtime;
}DSL_LINE_STATS_ST;

typedef struct tagDSL_LINE_TESTPARAM_ST
{
    VOS_CHAR *pszHLOGpsds;                 
    VOS_CHAR *pszHLOGpsus;
    VOS_UINT ulHLOGMTds;
    VOS_UINT ulHLOGMTus;
    VOS_CHAR *pszQLNpsds;
    VOS_CHAR *pszQLNpsus;
    VOS_UINT ulQLNMTds;                 
    VOS_UINT ulQLNMTus;           
    VOS_CHAR *pszSNRpsds;
    VOS_CHAR *pszSNRpsus;          
    VOS_UINT ulSNRMTds;                  
    VOS_UINT ulSNRMTus;               
    VOS_CHAR acLATNds[24];
    VOS_CHAR acLATNus[24];
    VOS_CHAR acSATNds[24];
    VOS_CHAR acSATNus[24];
    VOS_UINT ulHLOGGds;   
    VOS_UINT ulHLOGGus;   
    VOS_UINT ulQLNGds;   
    VOS_UINT ulQLNGus;   
    VOS_UINT ulSNRGds;   
    VOS_UINT ulSNRGus;   
    VOS_UINT  ulBinAllocation; 
	VOS_UINT  ulFECSizeds; 
	VOS_UINT  ulFECSizeus; 
	VOS_UINT  ulCurPowerds; 
	VOS_UINT  ulCurPowerus;
}DSL_LINE_TESTPARAM_ST;

typedef struct tagDSL_LINE_INTERFACE_CFG_ST
{
    VOS_BOOL 	Enable;           
    VOS_CHAR 	acStatus[32];    
    VOS_CHAR 	acAlias[64];    
    VOS_CHAR 	acName[64];   
    VOS_UINT 	ulLastChange;   
    VOS_CHAR 	acLinkStatus[32]; 
    VOS_BOOL 	Upstream;
    VOS_UINT 	ulUpstreamMaxBitRate;
    VOS_UINT 	ulDownstreamMaxBitRate;
    VOS_INT 	iUpstreamNoiseMargin;
    VOS_INT 	iDownstreamNoiseMargin;
    VOS_INT 	iUpstreamAttenuation;
    VOS_INT 	iDownstreamAttenuation;
    VOS_INT 	iUpstreamPower;
    VOS_INT 	iDownstreamPower;
	VOS_CHAR	acFwVersion[DSL_STR_64_LEN];
    VOS_CHAR 	acXTURVendor[32];
    VOS_CHAR 	acXTURCountry[32];
    VOS_CHAR 	acXTUCVendor[32];
    VOS_CHAR 	acXTUCCountry[32];
    VOS_CHAR  	acStandardsSupported[256];
    VOS_CHAR  	acStandardUsed[32];
    VOS_CHAR  	acAllowedProfiles[32];
    VOS_CHAR  	acCurrentProfile[8];
    VOS_UINT   	ulUPBOKLE;
    VOS_INT     iTRELLISds;
    VOS_INT     iTRELLISus;
    VOS_UINT   	ulACTSNRMODEds;
    VOS_UINT   	ulACTSNRMODEus;
    VOS_UINT   	ulACTUALCE;
    VOS_CHAR  	acSNRMpbds[24];
    VOS_CHAR  	acSNRMpbus[24]; 

	/*add by c00176128_20120626,增加工程页面参数*/
	VOS_CHAR  	acTrainingResults[DSL_STR_12_LEN];
	VOS_CHAR  	acLinkPowerState[DSL_STR_4_LEN];
	VOS_UINT32  ulCodeWordSizeUs; /* RS codeword size upstream*/   
	VOS_UINT32  ulCodeWordSizeDs; /* RS codeword size downstream*/
	VOS_UINT32  ulInterleaveDelayUs; /*Interleave delay upstream*/    
	VOS_UINT32  ulInterleaveDelayDs; /*Interleave delay downstream*/
	VOS_CHAR  	acDataPath[DSL_STR_16_LEN];             	/* 指示快速或交织    */
    VOS_UINT  	ulUpInterleaveDepth;        				/* 上行交织深度      */
    VOS_UINT  	ulDownInterleaveDepth;      				/* 下行交织深度      */ 
    VOS_UINT  ulUpstreamCurrRate;         /* 上行 DSL 信道的当前业务带宽(以 Kbps 计) */    
    VOS_UINT  ulDownstreamCurrRate;       /* 下行 DSL 信道的当前业务带宽 (以 Kbps 计)*/
    

    /*扩展两个参数*/
    VOS_CHAR  acModulationType[32];       /* 指示连接调试方式 */
    VOS_UINT32 ulActiveMode;              /* mode: XDSL_MODE_UNKNOWN/XDSL_MODE_ATM/XDSL_MODE_PTM */
    DSL_LINE_STATS_ST stDslLineStats;
    DSL_LINE_TESTPARAM_ST stDslLineTestParams;
}DSL_LINE_INTERFACE_CFG_ST;

VOS_INT32 DslLineGetStatistic(DSL_LINE_INTERFACE_CFG_ST *pstWanInfInfo);
VOS_INT32 XdslGetTestParamStatistic(DSL_LINE_TESTPARAM_ST *pstTestParamInfo);
VOS_VOID XdslParseAdslInfo(const VOS_CHAR *pszInfo, const VOS_CHAR *pszVar, VOS_CHAR *pszStrVal, VOS_INT32 iLen);
VOS_INT32 XdslGetVdslTestparamLongStr(VOS_CHAR *pszParamName,VOS_CHAR *pszRetParams,VOS_INT32 iRetParamsLen);
VOS_INT32 XdslGetTotalShowtimeStatistic(int when,DSL_LINE_STATS_TOTAL_SHOWTIME_ST *pstStatsTotalInfo);
static VOS_UINT32 CliSplitCmdBuf(VOS_CHAR *acCmdBuf, VOS_UINT32 *pulParaNum, VOS_CHAR **pszCmdParas);
VOS_INT32 DslGetLineStatistics(DSL_LINE_STATS_ST *pstDslLineStats);
VOS_UINT32 XdslSysGetUpTime(VOS_VOID);
VOS_UINT32 DslGetTotalStatsByDomain(VOS_CHAR *pcDomain ,struct rtnl_link_stats *pstDevStats);

#endif

