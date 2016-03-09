#ifndef __ATP_RSS_API_H__
#define __ATP_RSS_API_H__

#include "atputil.h"
#include "voicedb.h"

#define RSS_REFRESH_TIME_MIN    2
#define RSS_REFRESH_TIME_MAX    60
#define RSS_CHANNEL_IDX_MAX     100
#define RSS_GET_INFO_INTERVAL   120
#define RSS_DB_TITEL_LEN        21
#define RSS_DB_TIMESTATUS_LEN   9
#define RSS_DB_PREITEM_LEN      14
#define RSS_DB_INFO_MAXLEN      35*1024
#define RSS_DOWN_FILE_MAXLEN    500*1024  //500K
#define HTMLLABEL_MAXNUM        102

#define RSS_REFRESH_STATUS_FILE "/var/rssrefresh"




extern ATP_UTIL_TIMER_ST         *g_pstRssTimer;

typedef struct tagDECT_RSSSUBLIST_FIND_ST
{
    VOS_CHAR      acTitle[ATP_STRING_LEN_128];
	VOS_CHAR      acText[ATP_STRING_LEN_512];
	VDB_ENTRY_ID  llEntryId;
} DECT_RSSSUBLIST_FIND_ST;

typedef struct tagDECT_RSS_PID_ST
{
    pid_t         pid;;
	VOS_CHAR      acChannelUrl[ATP_ACTUAL_LEN_256];
} DECT_RSS_PID_ST;

VOS_VOID ATP_DectRss_ReadChannel(const ATP_MSG_HEADER_ST* pstMsg);
VOS_VOID ATP_DectRss_ReadSubList(const ATP_MSG_HEADER_ST* pstMsg);
VOS_VOID ATP_DectRss_RefreshSubList(const ATP_MSG_HEADER_ST* pstMsg);
VOS_VOID ATP_DectRss_VDBSaveToDB(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 ATP_DectRss_Init();
VOS_UINT32 ATP_DectRss_DBSaveToVDB();
VOS_UINT32 ATP_DBSaveRssChannel();
VOS_UINT32 ATP_DectRssChannel_DBSaveToVDB();
VOS_UINT32 RSS_RecordWanStatus(VOS_BOOL bIpV4, VOS_BOOL bStatus);
//static VOS_UINT32 ATP_TimerRssSaveDb(VOS_VOID);
#endif

