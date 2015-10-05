#ifndef __DSLCHANNELCMS_API_H__
#define __DSLCHANNELCMS_API_H__

//#define ATP_DEBUG
#ifdef ATP_DEBUG
#define ATP_DSLCHANNEL_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_DSLCHANNEL_DEBUG(x...)
#endif

#define SUPPORT_ATP_DSL_DIAG

#define ADSL_STATS_INFO_FILE             "/var/adslstats"
#define ADSL_SYS_CMD_LEN                 512
#define ADSL_COMM_CFG_DATA_LEN           64

#define XDSL_MODE_ATM_STR                "G.992.3_Annex_K_ATM"
#define XDSL_MODE_PTM_STR                "G.992.3_Annex_K_PTM"
#define XDSL_MODE_VDSL_ATM_STR           "G.993.2_Annex_K_ATM"
#define XDSL_MODE_VDSL_PTM_STR           "G.993.2_Annex_K_PTM"

#define ADSL_GDMT                   "ADSL_G.dmt"
#define ADSL_T1413                  "ADSL_ANSI_T1.413"
#define ADSL_GLITE                  "ADSL_G.lite"
#define ADSL_READSL                 "ADSL_re-ads"
#define ADSL_ADSL2                  "ADSL_G.dmt.bis"
#define ADSL_ADSL2PLUS              "ADSL_2plus"
#define VDSL_VDSL2                  "VDSL"

#define XDSL_LINKENCAPSULATION_G922_ATM  "G.992.3_Annex_K_ATM"
#define XDSL_LINKENCAPSULATION_G922_PTM  "G.992.3_Annex_K_PTM"
#define XDSL_LINKENCAPSULATION_UNKNOWN  "G.994.1"    //Auto "Unknown Mode"

#define XDSL_LINKENCAPSULATION_G993_ATM  "G.993.2_Annex_K_ATM"
#define XDSL_LINKENCAPSULATION_G993_PTM  "G.993.2_Annex_K_PTM"

typedef enum 
{ 
	ATP_XDSL_STATUS_UP,
	ATP_XDSL_STATUS_DOWN,
	ATP_XDSL_STATUS_ERROR,
} ATP_XDSL_STATUS_EN;

typedef enum
{
    ATP_XDSL_MODE_ADSL,
    ATP_XDSL_MODE_VDSL,
    ATP_XDSL_MODE_ERROR
}ATP_XDSL_MODE_EN;

#if 0
enum
{
    XDSL_MODE_UNKNOWN = 0,
    XDSL_MODE_ATM,
    XDSL_MODE_PTM,
    
    /*Added by lvxin00135113@20091229 for support DT DSLConfig CI*/
    XDSL_MODE_VDSL_ATM,
    XDSL_MODE_VDSL_PTM,
    /*Added by lvxin00135113@20091229 for support DT DSLConfig CI*/
    
};
#endif

typedef struct 
{
#ifdef SUPPORT_ATP_XDSL
    VOS_UINT32 ulActiveMode;            // mode: XDSL_MODE_UNKNOWN/XDSL_MODE_ATM/XDSL_MODE_PTM
    VOS_UINT32 ulPath0UpStreamRate;     // path0's upstream rate
    VOS_UINT32 ulPath0DownStreamRate;   // path0's downstream rate
    VOS_UINT32 ulPath1UpStreamRate;     // path1's upstream rate
    VOS_UINT32 ulPath1DownStreamRate;   // path1's downstream rate
#endif
	ATP_XDSL_STATUS_EN enLinkState; 
	VOS_UINT32 ulUpStreamRate;
	VOS_UINT32 ulDownStreamRate;    
}ATP_XDSL_CONNECTION_INFO_ST;

typedef struct tagDSL_CHANNEL_STATS_TOTAL_ST
{
    VOS_UINT ulXTURFECErrors;           
    VOS_UINT ulXTUCFECErrors;    
    VOS_UINT ulXTURHECErrors;   
    VOS_UINT ulXTUCHECErrors;   
    VOS_UINT ulXTURCRCErrors;   
    VOS_UINT ulXTUCCRCErrors; 
}DSL_CHANNEL_STATS_TOTAL_SHOWTIME_ST;

typedef struct tagDSL_CHANNEL_STATS_ST
{
    VOS_UINT ulBytesSent;           
    VOS_UINT ulBytesReceived;    
    VOS_UINT ulPacketsSent;   
    VOS_UINT ulPacketsReceived;   
    VOS_UINT ulErrorsSent;   
    VOS_UINT ulErrorsReceived;   
    VOS_UINT ulTotalStart;   
    VOS_UINT ulShowtimeStart;   
     DSL_CHANNEL_STATS_TOTAL_SHOWTIME_ST stDslChannelStatsTotal;
     DSL_CHANNEL_STATS_TOTAL_SHOWTIME_ST stDslChannelStatsShowtime;
}DSL_CHANNEL_STATS_ST;

typedef struct tagDSL_CHANNEL_INTERFACE_CFG_ST
{
    VOS_BOOL Enable;           
    VOS_CHAR acStatus[32];    
    VOS_CHAR acAlias[64];    
    VOS_CHAR acName[64];   
    VOS_UINT ulLastChange;   
    VOS_CHAR acLowerLayers[1024];   
    VOS_UINT ulUpstreamCurrRate;
    VOS_UINT ulDownstreamCurrRate;
    VOS_CHAR  acLinkEncapsulationSupported[256];
    VOS_CHAR  acLinkEncapsulationUsed[32];

    /*扩展两个参数*/
    VOS_CHAR  acModulationType[32];       /* 指示连接调试方式 */
    VOS_UINT32 ulActiveMode;              /* mode: XDSL_MODE_UNKNOWN/XDSL_MODE_ATM/XDSL_MODE_PTM */
    DSL_CHANNEL_STATS_ST stDslChannelStats;
}DSL_CHANNEL_INTERFACE_CFG_ST;

VOS_INT32 DslChannelGetTotalShowtimeStatistic(int when,DSL_CHANNEL_STATS_TOTAL_SHOWTIME_ST *pstStatsTotalInfo);
VOS_INT32 DslGetChannelStatistics(DSL_CHANNEL_STATS_ST *pstDslLineStats);


VOS_INT32 ATP_XDSL_GetConnectionInfo(ATP_XDSL_CONNECTION_INFO_ST * pstConnInfo);

#endif

