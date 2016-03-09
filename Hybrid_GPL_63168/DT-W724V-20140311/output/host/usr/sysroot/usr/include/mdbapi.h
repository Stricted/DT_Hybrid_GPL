#ifndef __ATP_MDB_API_H__
#define __ATP_MDB_API_H__

#include <sys/sysinfo.h>
#include "atptypes.h"
#include "sqlite3.h"
#include "ixml.h"
#include "membuffer.h"

#define ATP_MDB_USE_RAMDB                1
#define ATP_MDB_RAMDB_NAME             "/var/mdb5.db"
#define ATP_DMS_USBROOT_D                "/mnt"

#define ATP_MDB_MAX_SQL_LEN_D           (512)//(4096)
#define UPDATE_SOCKET_NAME		"/var/tmp/update.socket"

/* DLNA Error Code */
#define UPNP_SOAP_E_NO_SUCH_OBJECT                  701
#define UPNP_SOAP_E_INVALID_SEARCH_CRITERIA         708
#define UPNP_SOAP_E_INVALID_SORT_CRITERIA           709
#define UPNP_SOAP_E_NO_SUCH_CONTAINER               710
#define UPNP_SOAP_E_RESTRICTED_OBJECT               711
#define UPNP_SOAP_E_BAD_METADATA                    712
#define UPNP_SOAP_E_RESTRICTED_PARENT_OBJECT        713
#define UPNP_SOAP_E_NO_SUCH_RESOURCE                714
#define UPNP_SOAP_E_RESOURCE_ACCESS_DENIED          715
#define UPNP_SOAP_E_CANNOT_PROCESS_REQUEST          720

/* START ADD: tKF29060 20111020 for DTS2011101500424 同时下发Search导致dms进程挂死 */
#define MAX_EXP_COUNT	32			//最多支持同时输入32条规则
#define MAX_EXP_LENGTH	1024		//表达式最大长度为1024个字符
/* END ADD: tKF29060 20111020 for DTS2011101500424 本段从原mdbsearch.c中移过来 */

#ifdef SUPPORT_ATP_DMS_UPLOAD
/* 全局变量upload目录的id */
extern VOS_INT64 g_llUploadID;
extern VOS_UINT32 g_ulDlnaCertification;

typedef struct tagATP_MDB_CREATEOBJECT_NOTE_ST
{
    VOS_INT64		llObjectId;
    VOS_UINT32		ulFlags;	//1:AutoDestroy,0:不启用
    VOS_INT32		lCreateTime;
    struct tagATP_MDB_CREATEOBJECT_NOTE_ST	*next;
} ATP_MDB_CREATEOBJECT_NOTE_ST;

/* 全局变量等待upload的列表 */
extern ATP_MDB_CREATEOBJECT_NOTE_ST *g_pstUploadList;
#endif

typedef enum tagATP_MDB_RET_CODE_EN
{
    ATP_MDB_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_MDBCORE),
    ATP_MDB_RET_MEM_ERR,
    ATP_MDB_RET_SYS_ERR,
    ATP_MDB_RET_INTERNAL_ERR,
    ATP_MDB_RET_OBJID_ERR,
    ATP_MDB_RET_SQL_ERR,
    ATP_MDB_RET_MSG_PEER_DEAD,
    ATP_MDB_RET_MSG_TIMEOUT,
} ATP_MDB_RET_CODE_EN;

/*
 *  Browsing Container Architecture interfaces
 */

typedef struct tagATP_MDB_QUERY_CTX_ST ATP_MDB_QUERY_CTX_ST;
typedef struct tagATP_MDB_BROWSE_CONTAINER_ST ATP_MDB_BROWSE_CONTAINER_ST;
typedef struct tagMDB_BROWSEDB_ARG_ST
{
    const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer;
    ATP_MDB_QUERY_CTX_ST              *pstQueryCtx;
    const VOS_CHAR                    *pcParentId;
    VOS_INT64                         llLastItemId;
    IXML_Element                      *pstElement;
    IXML_Element                      *pstResNode;
    VOS_UINT32                        ulType;
    VOS_BOOL                          bIsStaticConatiner;
} MDB_BROWSEDB_ARG_ST;


typedef VOS_UINT32 (*PFMdbBrowseProc)(const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx);

struct tagATP_MDB_BROWSE_CONTAINER_ST
{
    const VOS_CHAR      *pcName;
    VOS_UINT16          usContainerId;
    VOS_UINT8           ucObjectType;
    VOS_UINT8           ucSearchClass;
    VOS_UINT16          usParentId;
    VOS_UINT16          usChildCnt;
    PFMdbBrowseProc     pfBrowseProc;
    const struct tagATP_MDB_BROWSE_CONTAINER_ST   *pstParent;
    const struct tagATP_MDB_BROWSE_CONTAINER_ST   *pstChildren;
};

extern const ATP_MDB_BROWSE_CONTAINER_ST g_stRootContainer;

typedef struct tagATP_MDB_ROOT_DIR_ST
{
    VOS_CHAR                *pcDir;
    VOS_INT64               llObjectId;
} ATP_MDB_ROOT_DIR_ST;


/*
 *  Public interfaces for accessing MediaDB
 */
struct tagATP_MDB_QUERY_CTX_ST
{
    // Input
    const VOS_CHAR      *pcObjectId;
    const VOS_CHAR      *pcSortCriteria;
    const VOS_CHAR      *pcFilter;
    VOS_UINT32          ulStartIdx;
    VOS_UINT32          ulReqCnt;
    VOS_INT32            lAddressFamily;

    // Parsed results
    VOS_UINT8           *pucFilterFlags;
    VOS_CHAR            *pcSortSqlStr;

    // Output
    VOS_UINT32          ulReturnCnt;
    VOS_UINT32          ulMatchCnt;
    IXML_Document       *pstResultDoc;
    IXML_Node           *pstDidlNode;

    // Error
    VOS_INT32           lUpnpErrCode;
    VOS_CHAR            *pcErrStr;      // Max len is LINE_SIZE(180)
};

/* START ADD: tKF29060 20111020 for DTS2011101500424 同时下发Search导致dms进程挂死 */
/* 以下操作标识符用来处理SearchCriteria语法识别 */
typedef enum tagOPERATE_EN
{
    OP_NULL = 0,	//0为NULL
    OP_EXP,			//1为表达式
    OP_LBRACKET,	//2为(
    OP_RBRACKET,	//3为)
    OP_LOG,			//4为逻辑操作符and或or
} OPERATE_EN;

/* 以下结构体用来存储搜索条件的分析结果 */
typedef struct tagSearchCriteria {
    VOS_UINT32 ulExpCount;
    VOS_CHAR *azRelExp[MAX_EXP_COUNT];
    VOS_CHAR acSearchExp[MAX_EXP_LENGTH];
} SEARCHCRITERIAST;

typedef struct tagATP_MDB_SEARCH_CONDITION_ST
{
    SEARCHCRITERIAST stSearchCrit;		//存储语法解析的结果
    VOS_INT32 lBracketCount;			//记录括号的匹配情况
    OPERATE_EN enOpFlag;				//记录每次操作的标识
} ATP_MDB_SEARCH_CONDITION_ST;
/* END ADD: tKF29060 20111020 for DTS2011101500424 本段从原mdbsearch.c中移过来 */

#ifdef SUPPORT_ATP_DMS_UPLOAD
struct tagATP_MDB_CREATEOBJECT_CTX_ST
{
    // Input
    const VOS_CHAR      *pcContainerId;
    const VOS_CHAR      *pcElements;
    VOS_INT32            lAddressFamily;

    // Parsed results
    VOS_CHAR            *pcParentID;
    VOS_INT32            ulRestricted;
    VOS_CHAR            *pcTitle;
    VOS_CHAR            *pcDate;
    VOS_CHAR            *pcClass;
    VOS_CHAR            *pcSize;
    VOS_CHAR            *pcProtocolInfo;
    VOS_CHAR            *pcIfoFileURI;
    VOS_CHAR            *pcPostfix;
    VOS_UINT32           ulProfileId;

    // Output
    VOS_CHAR      		*pcObjectId;
    IXML_Document       *pstResultDoc;

    // Other
    IXML_Node           *pstDidlNode;
    IXML_Element        *pstElement;
    IXML_Element        *pstResNode;

    // Error
    VOS_INT32           lUpnpErrCode;
    VOS_CHAR            *pcErrStr;      // Max len is LINE_SIZE(180)
};

typedef struct tagATP_MDB_CREATEOBJECT_CTX_ST ATP_MDB_CREATEOBJECT_CTX_ST;
#endif

VOS_UINT32 ATP_MDB_BrowseStaticContainer(
        const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx);

VOS_UINT32 ATP_MDB_BrowseAll(
        const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseFolder(
        const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseMusicGenre(
        ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseMusicArtist(
        ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseMusicAlbum(
        ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseVideoCreator(
        ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_BrowseVideoAlbum(
        ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcDynamicId,
        VOS_BOOL                          bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx,
        VOS_UINT32                        ulType);

VOS_UINT32 ATP_MDB_XmlAppendChildElement(IXML_Document *pstDoc, IXML_Node *pstNode,
        const VOS_CHAR *pcName, const VOS_CHAR *pcValue,
        IXML_Element  **ppstElement);

VOS_UINT32 ATP_MDB_AppendSearchClass(
        ATP_MDB_QUERY_CTX_ST    *pstQueryCtx,
        IXML_Element            *pstElement,
        VOS_UINT32              ulSearchClass);

VOS_UINT32 ATP_MDB_AdaptColItemBuilder(
        const ATP_MDB_BROWSE_CONTAINER_ST   *pstContainer,
        ATP_MDB_QUERY_CTX_ST                *pstQueryCtx,
        IXML_Element                        *pstElement,
        IXML_Element                        *pstResNode,
        VOS_UINT32                          ulProfileId,
        VOS_UINT32                          ulIdx,
        const VOS_CHAR                      *pcDbData);

typedef VOS_INT32 (*PfMdbDbCallback)(VOS_VOID *pvArg, VOS_INT32 argc, VOS_CHAR **argv, VOS_CHAR **azColName);

VOS_UINT32 ATP_MDB_DbSearch(
        sqlite3                 *db,
        ATP_MDB_QUERY_CTX_ST    *ctx,
        VOS_INT64               llContainerId,
        const VOS_CHAR          *pcSearchCriteria,
        VOS_CHAR				*pcParentID,
        PfMdbDbCallback         pfCallback,
        VOS_VOID                *pvArg,
        VOS_UINT32              *pulMatched);

VOS_UINT32 ATP_MDB_DbBrowseObject(
        sqlite3                 *db,
        ATP_MDB_QUERY_CTX_ST    *ctx,
        VOS_INT64               llItemId,
        VOS_UINT32              ulType,
        VOS_BOOL                bBrowseDirectChildren,
        PfMdbDbCallback         pfCallback,
        VOS_VOID                *pvArg,
        VOS_UINT32              *pulMatched);

VOS_UINT32 ATP_MDB_DbBrowseByType(
        sqlite3                 *db,
        ATP_MDB_QUERY_CTX_ST    *ctx,
        VOS_INT64               llItemId,
        VOS_UINT32              ulType,
        VOS_BOOL                bBrowseDirectChildren,
        PfMdbDbCallback         pfCallback,
        VOS_VOID                *pvArg,
        VOS_UINT32              *pulMatched);

VOS_UINT32 ATP_MDB_Browse(
        const VOS_CHAR          *pcObjId,
        VOS_BOOL                bBrowseDirectChildren,
        ATP_MDB_QUERY_CTX_ST    *pstQuery);

VOS_UINT32 ATP_MDB_Search(
        const VOS_CHAR          *pcObjId,
        const VOS_CHAR          *pcSearchCriteria,
        ATP_MDB_QUERY_CTX_ST    *pstQuery);

VOS_INT32 MDBSearchCriteriaSyntax(const VOS_CHAR *pcSearchCriteria, ATP_MDB_SEARCH_CONDITION_ST *pstSearchCondition);

VOS_INT32 MDBSearchCriteriaCheck(
        const VOS_CHAR			*pcContainerID,
        VOS_CHAR				*pcParentID,
        VOS_INT32				*plObjectClassIndex,
        VOS_CHAR				*pcSqlSearchArg,
        VOS_BOOL                *pbIsStaticConatiner,
        ATP_MDB_SEARCH_CONDITION_ST *pstSearchCondition);

/*
 *	SQL interfaces
 */

#ifdef SUPPORT_DESKTOP
#define ATP_MDB_SQL_NAME_D                  "../etc/create_db.sql"
#else
#define ATP_MDB_SQL_NAME_D                  "/etc/create_db.sql"
#endif

typedef enum tagATP_MDB_OBJECT_CLASS_EN
{
    ATP_MDB_OBJECT_UNKNOWN,
    ATP_MDB_OBJECT_CONTAINER,

    ATP_MDB_OBJECT_CONTAINER_FOLDER,
    ATP_MDB_OBJECT_CONTAINER_PLAYLIST,

    ATP_MDB_OBJECT_CONTAINER_MUSIC_GENRE,
    ATP_MDB_OBJECT_CONTAINER_MUSIC_ARTIST,
    ATP_MDB_OBJECT_CONTAINER_MUSIC_ALBUM,

    ATP_MDB_OBJECT_CONTAINER_VIDEO_CREATOR,
    ATP_MDB_OBJECT_CONTAINER_MOVIE_ACTOR,
    ATP_MDB_OBJECT_CONTAINER_VIDEO_ALBUM,

    ATP_MDB_OBJECT_CONTAINER_PHOTO_ALBUM,

    ATP_MDB_OBJECT_AUDIO, // 11
    ATP_MDB_OBJECT_VIDEO, // 12
    ATP_MDB_OBJECT_PICTURE, // 13
    // Add new class here

    ATP_MDB_OBJECT_SEARCH_ALL       = 0x80,
    ATP_MDB_OBJECT_SEARCH_ALL_MUSIC,
    ATP_MDB_OBJECT_SEARCH_ALL_VIDEO,
    ATP_MDB_OBJECT_SEARCH_ALL_PICTURE
} ATP_MDB_OBJECT_CLASS_EN;

#define ATP_MDB_GET_OBJECT_CLASS(x)         ((x) & 0x0000007F)
#define ATP_MDB_IS_OBJECT_RESTRICTED(x)     (((x) & 0x00000080) >> 7)
#define ATP_MDB_IS_OBJECT_CONTAINER(x)      ((((x) & 0x0000007F) <= ATP_MDB_OBJECT_CONTAINER_PHOTO_ALBUM) ? (VOS_TRUE) : (VOS_FALSE))

sqlite3 *ATP_MDB_DbInit(const VOS_CHAR *pcDbFile, VOS_BOOL *pbDbExist);

sqlite3 *ATP_MDB_DbInitReadOnly(const VOS_CHAR *pcDbFile);

sqlite3 *ATP_MDB_DbInitReadWrite(const VOS_CHAR *pcDbFile);

/*
 *  All objects
 */

VOS_UINT32 ATP_MDB_DbUpdateItemValues(sqlite3 *db, VOS_INT64 llParentId, VOS_UINT32 ulItemType,
        const VOS_CHAR *pcTitle, const VOS_CHAR *pcFileName, 
        VOS_UINT64          ullSize, 
        const VOS_CHAR      *pcDate,
        VOS_UINT32          ulBitsPerSample,    
        VOS_UINT64          ullDuration,    // colorDepth
        VOS_UINT32          nrAudioChannels,
        VOS_UINT64          ullResolution,
        VOS_UINT32          ulSampleFrequency,
        VOS_UINT32          ulBitRate,
        const VOS_CHAR   *pcLastModify,
        VOS_UINT32         ulTrack,
        VOS_INT64          llACTORId,
        VOS_INT64          llALBUMId,
        VOS_INT64          llARTISTId,
        VOS_INT64          llAUTHORId,
        VOS_INT64          llCREATORId,
        VOS_INT64          llDIRECTORId,
        VOS_INT64          llGENREId,
        VOS_INT64          llPRODUCERId,
        VOS_INT64          llPUBLISHERId,
        VOS_INT64          llItemId);

VOS_UINT32 ATP_MDB_DbAddItem(sqlite3 *db, VOS_INT64 llParentId, VOS_UINT32 ulItemType,
        const VOS_CHAR *pcTitle, const VOS_CHAR *pcFileName, 
        VOS_UINT64          ullSize, 
        const VOS_CHAR      *pcDate,
        VOS_UINT32          ulBitsPerSample,    
        VOS_UINT64          ullDuration,    // colorDepth
        VOS_UINT32          nrAudioChannels,
        VOS_UINT64          ullResolution,
        VOS_UINT32          ulSampleFrequency,
        VOS_UINT32          ulBitRate,
        const VOS_CHAR   *pcLastModify,
        VOS_UINT32         ulTrack,
        VOS_INT64          llACTORId,
        VOS_INT64          llALBUMId,
        VOS_INT64          llARTISTId,
        VOS_INT64          llAUTHORId,
        VOS_INT64          llCREATORId,
        VOS_INT64          llDIRECTORId,
        VOS_INT64          llGENREId,
        VOS_INT64          llPRODUCERId,
        VOS_INT64          llPUBLISHERId,
        VOS_INT64          *pllItemId);

VOS_UINT32 ATP_MDB_DbGetItemIdByName(sqlite3 *db, VOS_INT64 llParentId, 
        const VOS_CHAR *pszFileName, VOS_INT64 *pllItemId);

VOS_UINT32 ATP_MDB_DbDelItem(sqlite3 *db,  VOS_INT64 llItemId);

VOS_UINT32 ATP_MDB_DbDelSubItem(sqlite3 *db, VOS_INT64 llParentId);

VOS_UINT32 ATP_MDB_DbUpdateItemParentIDandName(sqlite3 *db, VOS_INT64 llId, 
        VOS_INT64 llNewParentId, const VOS_CHAR *pszTitleUTF8,
        const VOS_CHAR *pszFileName);

VOS_UINT32 ATP_MDB_DbDelltemRegularInfo(sqlite3 *db, VOS_INT64 llItemId);

VOS_UINT32 ATP_MDB_DbAddItemExtraInfo(sqlite3 *db, VOS_INT64 *llExId,
        const VOS_CHAR *pcTblName, const VOS_CHAR *pcValue);

const VOS_CHAR *ATP_MDB_GetMediaClassById(VOS_UINT32 ulId);

VOS_UINT32 ATP_MDB_DbGetAllItemID(sqlite3 *db);

VOS_UINT32 ATP_MDB_DbGetRoots(sqlite3 *db);

VOS_UINT32 ATP_MDB_GetMediaClassByStr(const VOS_CHAR *pcClass);

/*
 *  Properties:
 *      Independent Properties (IP): IP has zero or more DPs.
 *          IP contains no '@' symbo; IP may contains XML namespace prefix;
 *          All IPs are single valued.
 *      Dependent   Properties (DP):
 *          DP is associated with exactly one IP or directly with a CDS service class such as an object
 *
 *          name of IP consists of three parts:
 *              1. Name of the Associated IP
 *                      This part is empty when IP is associated with CDS service class
 *              2. '@'
 *              3. name conveys the relationship between DP and IP
 *
 *          Some IPs even have child IPs.
 *
 *
 *      Property Names and didl-lite representation example:
 *          dc:title     <--->       <dc:title> ... </dc:title>
 *          res          <--->       <res> ... </res>
 *          res@size  <--->       <res size="..."> ... </res>
 *          @id          <--->      <item id="..."></item>
 */
typedef enum tagATP_MDB_COL_PROPERTY_EN
{
    ATP_MDB_COL_REQUIRED        =   (1 << 0),   // Must exist for any browse
    ATP_MDB_COL_EXTRA_BUILD     =   (1 << 1),   // Need extra code to build DIDL-Lite string
    ATP_MDB_COL_IP              =   (1 << 2),   // Independent property
    ATP_MDB_COL_RES             =   (1 << 3),   // It's dependent property of res
    ATP_MDB_COL_CONTAINER_ONLY  =   (1 << 4),
    ATP_MDB_COL_HIDE			=   (1 << 5)	// Need hide property
} ATP_MDB_COL_PROPERTY_EN;

typedef enum tagATP_MDB_DLNA_COL_EN
{
    ATP_MDB_DLNA_COL_ID,
    ATP_MDB_DLNA_COL_PARENT_ID,
    ATP_MDB_DLNA_COL_PROFILE_ID,
    ATP_MDB_DLNA_COL_TITLE,
    ATP_MDB_DLNA_COL_SIZE,
    ATP_MDB_DLNA_COL_DATE,
    ATP_MDB_DLNA_COL_BITS_PER_SPL,
    ATP_MDB_DLNA_COL_BITRATE,
    ATP_MDB_DLNA_COL_DURATION_COLOR_DEPTH,  // Reused with colorDepth of picture
    ATP_MDB_DLNA_COL_NR_AUDIO_CH,
    ATP_MDB_DLNA_COL_SPL_FREQ,
    ATP_MDB_DLNA_COL_RESOLUTION,
    ATP_MDB_DLNA_COL_ACTOR,
    ATP_MDB_DLNA_COL_ALBUM,
    ATP_MDB_DLNA_COL_ARTIST,
    ATP_MDB_DLNA_COL_AUTHOR,
    ATP_MDB_DLNA_COL_CREATOR,
    ATP_MDB_DLNA_COL_DIRECTOR,
    ATP_MDB_DLNA_COL_GENRE,
    ATP_MDB_DLNA_COL_PRODUCER,
    ATP_MDB_DLNA_COL_PUBLISHER,
    
    ATP_MDB_DLNA_COL_COLOR_DEPTH,
    ATP_MDB_DLNA_COL_TRACK,

    ATP_MDB_DLNA_COL_RESTRICTED,
    ATP_MDB_DLNA_COL_SEARCHABLE,
    ATP_MDB_DLNA_COL_SEARCH_CLASS,
    ATP_MDB_DLNA_COL_WRITE_STATUS,
    ATP_MDB_DLNA_COL_DLNAMANAGED = 30,
#ifdef SUPPORT_ATP_UPNP_AV_2
    ATP_MDB_DLNA_COL_STORAGE_USED_STATUS = 31,
    ATP_MDB_DLNA_COL_DAYLIGHTSAVING = 32,
#endif
} ATP_MDB_DLNA_COL_EN;

/* 该结构体中成员名称与数据库ITEM表里字段名称保持对应 */
typedef struct tagMDB_ITEM_DETAIL_INFO_ST
{
    VOS_INT64       llItemId;
    VOS_INT64       llParentId;
    VOS_UINT32      ulProfileId;
    VOS_CHAR		acTitle[256];
    VOS_CHAR		acFileName[4096];
    VOS_UINT64      ullSize;
    VOS_CHAR		acDate[64];
    VOS_UINT32		ulBitsPerSample;
    VOS_UINT64		ullDuration;
    VOS_UINT32		nrAudioChannels;
    VOS_UINT64		ullResolution;
    VOS_UINT32		ulSampleFrequency;
    VOS_UINT32		ulBitRate;
    VOS_CHAR		acLastModify[64];
    VOS_UINT32		ulTrack;
    VOS_INT64		llACTORId;
    VOS_INT64		llALBUMId;
    VOS_INT64		llARTISTId;
    VOS_INT64		llAUTHORId;
    VOS_INT64		llCREATORId;
    VOS_INT64		llDIRECTORId;
    VOS_INT64		llGENREId;
    VOS_INT64		llPRODUCERId;
    VOS_INT64		llPUBLISHERId;
} MDB_ITEM_DETAIL_INFO_ST;

typedef struct tagATP_MDB_COLUNM_ST
{
    const VOS_CHAR          *pcColName;
    const VOS_CHAR          *pcFullName;
    VOS_UINT16              usColProperty;  // ATP_MDB_COL_PROPERTY_EN
    VOS_UINT16              usValIdx;
} ATP_MDB_COLUNM_ST;

extern const ATP_MDB_COLUNM_ST g_astDlnaCols[];
extern const VOS_UINT32 g_ulNumOfDlnaCols;
VOS_UINT32 ATP_MDB_GetFilterIdx(const VOS_CHAR *pcFilter);
VOS_UINT8 *ATP_MDB_ParseFilterString(const VOS_CHAR *pcFilter);
VOS_CHAR *ATP_MDB_BuildSortStr(const VOS_CHAR *pcSortCriteria);
VOS_UINT32 ATP_MDB_BuildMetadataForStaticContainer(
        const ATP_MDB_BROWSE_CONTAINER_ST *pstContainer,
        const VOS_CHAR                    *pcTitle,
        const VOS_CHAR                    *pcEndObjId,
        ATP_MDB_QUERY_CTX_ST              *pstQueryCtx);
VOS_UINT32 MdbLocateStaticContainerById(
        const VOS_CHAR                      *pcObjId,
        const ATP_MDB_BROWSE_CONTAINER_ST   **ppstStaticContainer,
        const VOS_CHAR                      **ppcDynamicPart);

extern sqlite3 *g_pstMdbCtx;

/*
 *
 */

extern const VOS_CHAR *g_apcObjClassStrings[];
extern const VOS_UINT32 g_ulObjClassStringLen;
VOS_BOOL ATP_MDB_AdaptIsSearchClassMatched(VOS_UINT32 ulObjType, VOS_UINT32 ulSearchClass);

VOS_UINT32 ATP_MDB_GetFileInfoById(VOS_INT64 llObjId, VOS_CHAR **ppcFullPath,
        const VOS_CHAR **ppcMimeType, const VOS_CHAR **ppcPN,
        const VOS_CHAR **ppcDLNAOP, const VOS_CHAR **ppcDLNAFLAGS);

VOS_UINT32 ATP_MDB_GetItemInfo(
        sqlite3                 *db,
        VOS_INT64               llItemId,
        VOS_INT64               *pllParentId,
        VOS_CHAR                *pcFileName,
        VOS_UINT32              ulNameLen,
        VOS_UINT32              *pulProfileId);

VOS_UINT32 ATP_MDB_GetItemDetailInfo(
        sqlite3                 *db,
        VOS_INT64               llItemId,
        MDB_ITEM_DETAIL_INFO_ST	*pstItemDetail);

VOS_UINT32 ATP_MDB_DbUpdateItem(sqlite3 *db, VOS_INT64 llId, MDB_ITEM_DETAIL_INFO_ST *pstArg);

VOS_UINT32 ATP_MDB_Copyfile(const VOS_CHAR *pszTarget, const VOS_CHAR *pszSource);

VOS_BOOL ATP_MDB_DbDLNATest(sqlite3 *db, VOS_UINT32 ulCmp);

VOS_BOOL ATP_MDB_IsFolderEmpty(VOS_INT64 llItemId);

VOS_UINT32 ATP_MDB_GetGenre(
        sqlite3                 *db,
        VOS_INT32           lGenreId,
        VOS_CHAR           *pcGenreName);

VOS_UINT32 ATP_MDB_GetAlbum(
        sqlite3                 *db,
        VOS_INT32           lAlbumId,
        VOS_CHAR           *pcAlbumName);

VOS_UINT32 ATP_MDB_GetArtist(
        sqlite3                 *db,
        VOS_INT32           lArtistId,
        VOS_CHAR           *pcArtistName);

VOS_UINT32 ATP_MDB_GetChannelAndSampleFrequency(
        VOS_INT64               llItemId,
        VOS_UINT32              *pulSampleFrequency,
        VOS_UINT32              *pulChannels);

VOS_UINT32 ATP_MDB_ReplaceRoot(
        sqlite3                  *db,
        const VOS_CHAR           *pcUSBRoot,
        const VOS_CHAR           *pcDMSRoot);

VOS_CHAR* ATP_MDB_BuildAllProtocolInfo(VOS_VOID);

VOS_UINT32 ATP_MDB_UpdateFolderFlags(sqlite3 *db,VOS_INT64 llObjId, VOS_UINT32 ulFolderFlags);
VOS_UINT32 ATP_MDB_SetFolderFlags(sqlite3 *db,VOS_INT64 llObjId, VOS_UINT32 ulFolderFlags);

#define DMS_MSGSRV_BIND_FILE_D      UPDATE_SOCKET_NAME
#define DMS_INVALID_MSG_FD_D        (-1)

typedef struct tagDMS_MSG_ST
{
    VOS_UINT32      ulMsgType;
    VOS_UINT32      ulMsgData;
    VOS_UINT32      ulDataLen;
} DMS_MSG_ST;

VOS_INT32 DMS_MsgInit(const VOS_CHAR *name, VOS_UINT32 ulMsgSrc);

#ifdef SUPPORT_ATP_DMS_UPLOAD
/*
 * 为Upload添加的接口
 */

VOS_INT32 ATP_MDB_ParseElements(ATP_MDB_CREATEOBJECT_CTX_ST *stCreateObjectCtx);

VOS_INT32 ATP_MDB_CreateObject(ATP_MDB_CREATEOBJECT_CTX_ST *stCreateObjectCtx);

VOS_INT32 ATP_MDB_DestroyObject(VOS_INT64 llObjectId);

VOS_INT32 ATP_MDB_AutoDestroyObject(VOS_VOID);

VOS_INT32 ATP_MDB_AddUploadNote(VOS_INT64 llObjectId);

VOS_INT32 ATP_MDB_DelUploadNote(VOS_INT64 llObjectId);

VOS_INT32 ATP_MDB_FindUploadNote(VOS_INT64 llObjectId);

VOS_INT32 ATP_MDB_DisableUploadNote(VOS_INT64 llObjectId);

VOS_UINT32 ATP_MDB_PNtoFix(VOS_CHAR *pcMime, VOS_CHAR *pcOrgPN, VOS_CHAR **ppcPostFix);

VOS_INT32 ATP_DMS_NOTIFY_SUBSCRIPTION(VOS_UINT32 ulProfileId);

VOS_INT32 ATP_MDB_FreeUploadList(VOS_VOID);

VOS_BOOL ATP_MDB_CheckUploadComplete(VOS_INT64 llObjectId);

#endif

#ifdef ATP_DEBUG
#define ATP_MDB_DEBUG(format, args...) {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_MDB_DEBUG(x...)
#endif

static inline int MdbSqlite3Exec(sqlite3 * db, const VOS_CHAR *zSql,
                    sqlite3_callback xCallback, void *pArg, char **pzErrMsg)
{
    int lRet = 0;
    for (;;)
    {
        lRet = sqlite3_exec(db, zSql, xCallback, pArg, pzErrMsg);
        if (lRet != 5)
        {
            break;
        }
    }
    return lRet;
}

 #endif
