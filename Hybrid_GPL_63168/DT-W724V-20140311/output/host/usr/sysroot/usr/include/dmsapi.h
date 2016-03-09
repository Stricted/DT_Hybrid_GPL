#ifndef __ATP_DMS_API_H__
#define __ATP_DMS_API_H__

#include "atptypes.h"
#include "ixml.h"
#include "upnp.h"

#define ATP_DMS_UUID_CONST_D_V1              "00e0fc37-2626-2828-"
#define ATP_DMS_CDS_SERVICE_ID      "urn:upnp-org:serviceId:ContentDirectory"

#ifdef SUPPORT_ATP_UPNP_AV_2
#define ATP_DMS_UUID_CONST_D_V2              "00e0fc37-2727-2828-"
#endif

//#ifndef SUPPORT_ATP_UPNP_AV_2
#define SORTCPAS "dc:title,dc:creator,upnp:artist,upnp:actor,upnp:author,upnp:genre,upnp:album,"\
"upnp:originalTrackNumber,dc:date,upnp:class"
//#else
//#define SORTCPAS "dc:title,dc:creator,dc:date,res@size"
//#endif

typedef enum tagATP_DMS_RET_CODE_EN
{
    ATP_DMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_DMSAPI),
    ATP_DMS_RET_MEM_ERR,
    ATP_DMS_RET_SYS_ERR,
    ATP_DMS_RET_ACTION_FORMAT_ERR,
    ATP_DMS_RET_ACTION_ARG_VALUE_ERR,
    ATP_DMS_RET_INTERNAL_ERR,
} ATP_DMS_RET_CODE_EN;

typedef struct tagATP_DMS_HANDLE_ST
{
    VOS_BOOL            bInited;
    VOS_UINT32          ulLocalPort;
    UpnpDevice_Handle   hStkHdl;
    //for support IPV6
    UpnpDevice_Handle   hStkHdlV6;
#ifdef SUPPORT_ATP_UPNP_AV_2
    UpnpDevice_Handle   hStkHdl_AV2;
    UpnpDevice_Handle   hStkHdlV6_AV2;
#endif
} ATP_DMS_HANDLE_ST;

typedef struct tagATP_DMS_MMR_RegistDevice
{
    VOS_UINT8 ucProtocolVersion;
    VOS_UINT8 ucMessageType;
    VOS_UINT8 usSerialNumber[16];
    VOS_UINT32 ulCertificateLength;
}ATP_DMS_MMR_REGISTDEVICE;

extern ATP_DMS_HANDLE_ST g_stDmsHdl;
extern const VOS_CHAR *g_pcTopSoap;
extern VOS_CHAR g_acSystemUpdateID[8];

#define ATP_DMS_PORT_D      (10287)

//extern const struct UpnpVirtualDirCallbacks g_stVirtualDir;

VOS_UINT32 ATP_DMS_UpnpStart(const VOS_CHAR *pcIntf, VOS_UINT32  ulPort);
VOS_VOID ATP_DMS_UpnpStop(VOS_VOID);

VOS_CHAR *ATP_DMS_GetDeviceDescription(VOS_VOID);
#ifdef SUPPORT_ATP_UPNP_AV_2
VOS_CHAR *ATP_DMS_GetDeviceDescription_v2(VOS_VOID);
#endif
VOS_INT32 ATP_DMS_CtrlEvtProc(Upnp_EventType type, void *event,
        void *cookie);

VOS_UINT32 ATP_DMS_GetArgValueFromRequest(
        struct Upnp_Action_Request  *request,
        const VOS_CHAR              *pcArgName,
        const VOS_CHAR              **ppcArgValue);

VOS_UINT32 ATP_DMS_GetArgUi4ValueFromRequest (
        struct Upnp_Action_Request  *request,
        const VOS_CHAR              *pcArgName,
        VOS_UINT32                  *pulArgValue);


/*
 *  Services and Actions manager
 */

typedef struct tagATP_DMS_SERVICE_ST ATP_DMS_SERVICE_ST;
typedef VOS_UINT32 (*DmsActionProc)(struct Upnp_Action_Request *request, const ATP_DMS_SERVICE_ST *pstSrv);

typedef struct tagATP_DMS_ACTION_ST
{
    const VOS_CHAR      *pcName;
    DmsActionProc       pfProc;
} ATP_DMS_ACTION_ST;

struct tagATP_DMS_SERVICE_ST
{
    const VOS_CHAR              *pcSrvId;
    const VOS_CHAR              *pcSrvType;
    const VOS_CHAR              *pcDescName;
    const VOS_CHAR              *pcDesc;
    VOS_UINT32                  ulDescLen;
    const ATP_DMS_ACTION_ST     *pstActions;
    const VOS_CHAR        **VariableName;
    const VOS_CHAR        **VariableStrVal;
    VOS_UINT32       VariableCount;
};

extern const ATP_DMS_SERVICE_ST g_astDmsSrvs_v1[];
extern const VOS_UINT16         g_usDmsNumOfSrvs_v1;
#ifdef SUPPORT_ATP_UPNP_AV_2
extern const ATP_DMS_SERVICE_ST g_astDmsSrvs_v2[];
extern const VOS_UINT16         g_usDmsNumOfSrvs_v2;
#endif

VOS_UINT32 ATP_DMS_GetSrvAndActionByReq(
        const struct Upnp_Action_Request    *request,
        const ATP_DMS_SERVICE_ST            **service,
        const ATP_DMS_ACTION_ST             **action,
        void* cookie
        );

VOS_UINT32 ATP_DMS_GetSrvDescByFileName(
        const VOS_CHAR              *filename,
        const VOS_CHAR              **ppszSrvDesc,
        VOS_UINT32                  *pulDescLen);

/*
 *  Virtual directory and Virtual path utilities
 */

#define ATP_DMS_VIRTUALDIR_NAME_D           "/web/"
#define ATP_DMS_VIRTUALDIR_NAME2_D          "/web"

#define ATP_DMS_DESC_PATH_NAME_D            "desc/"
#define ATP_DMS_MDB_PATH_NAME_D             "mdb/"
#define ATP_DMS_ICON_PATH_NAME_D            "icon/"
#define ATP_DMS_UPLOAD_PATH_NAME_D          "upload/"

#define ATP_DMS_UPLOAD_CONTAINER			"DLNA.ORG_AnyContainer"

typedef enum tagATP_DMS_VIRDIR_TYPE_EN
{
    ATP_DMS_VIRDIR_FILE_LOCAL,
    ATP_DMS_VIRDIR_FILE_MEMORY
} ATP_DMS_VIRDIR_TYPE_EN;

typedef struct tagATP_DMS_VIRDIR_FILE_ST
{
    int                     fd;
    VOS_VOID                *pvCookie;
} ATP_DMS_VIRDIR_FILE_ST;

typedef struct tagATP_DMS_VIRDIR_MEMORY_ST
{
    off_t                   len;
    char                    *contents;
} ATP_DMS_VIRDIR_MEMORY_ST;

typedef struct tagATP_DMS_VIRDIR_ST
{
    char                            *fullpath;
    off_t                           pos;
    ATP_DMS_VIRDIR_TYPE_EN          type;

    union
    {
        ATP_DMS_VIRDIR_FILE_ST      local;
        ATP_DMS_VIRDIR_MEMORY_ST    memory;
    } detail;
} ATP_DMS_VIRDIR_ST;

typedef VOS_INT32 (*DmsVirtualPathGetInfo)(const VOS_CHAR *pcFileName, struct File_Info *info);
typedef ATP_DMS_VIRDIR_ST *(*DmsVirtualPathOpen)(const VOS_CHAR *pcFileName);

typedef struct tagATP_DMS_VIRPATH_ST
{
    const VOS_CHAR          *pcPathName;
    enum UpnpOpenFileMode   enAllowedMode;
    DmsVirtualPathGetInfo   pfGetInfo;
    DmsVirtualPathOpen      pfOpen;
} ATP_DMS_VIRPATH_ST;

extern const ATP_DMS_VIRPATH_ST g_astDmsVirPaths[];
extern const VOS_UINT16 g_usDmsNumOfVirPaths;

// Max number returned for browse and search
extern const VOS_UINT32 g_ulMaxResultCnt;

const ATP_DMS_VIRPATH_ST *ATP_DMS_VirtualPathFind(const VOS_CHAR *pcFileName);

/*
 *  Service description file path proc
 */
VOS_INT32 ATP_DMS_GetDescFileInfo(const VOS_CHAR *pcFileName, struct File_Info *info);
ATP_DMS_VIRDIR_ST *ATP_DMS_OpenDescFile(const VOS_CHAR *pcFileName);

VOS_INT32 ATP_DMS_GetMediaFileInfo(const VOS_CHAR *pcFileName, struct File_Info *info);
ATP_DMS_VIRDIR_ST *ATP_DMS_OpenMediaFile(const VOS_CHAR *pcFileName);

VOS_INT32 ATP_DMS_GetIconFileInfo(const VOS_CHAR *pcFileName, struct File_Info *info);
ATP_DMS_VIRDIR_ST *ATP_DMS_OpenIconFile(const VOS_CHAR *pcFileName);

#ifdef SUPPORT_ATP_DMS_UPLOAD
VOS_INT32 ATP_DMS_GetUploadFileInfo(const VOS_CHAR *pcFileName, struct File_Info *info);
ATP_DMS_VIRDIR_ST *ATP_DMS_OpenUploadFile(const VOS_CHAR *pcFileName);
#endif

/*
 *  ATP provided actions
 */

VOS_UINT32 ATP_DMS_ActionBrowse(
        struct Upnp_Action_Request  *request,
        const ATP_DMS_SERVICE_ST    *pstSrv);

VOS_UINT32 ATP_DMS_ActionSearch(
        struct Upnp_Action_Request  *request,
        const ATP_DMS_SERVICE_ST    *pstSrv);

#ifdef SUPPORT_ATP_DMS_UPLOAD
VOS_UINT32 ATP_DMS_ActionCreateObject(
        struct Upnp_Action_Request  *request,
        const ATP_DMS_SERVICE_ST    *pstSrv);

VOS_UINT32 ATP_DMS_ActionDestroyObject(
        struct Upnp_Action_Request  *request,
        const ATP_DMS_SERVICE_ST    *pstSrv);

/*VOS_UINT32 ATP_DMS_ActionGetDlnaUploadProfiles( 
    struct Upnp_Action_Request *request,
    const ATP_DMS_SERVICE_ST *pstSrv);
*/
#endif

const VOS_CHAR * ATP_DMS_GetVersion(void);

VOS_INT ATP_DMS_NOTIFY_UPDATE(VOS_CHAR *value);

int DmsVirtualDirGetInfo(const char *filename, struct File_Info *info);
UpnpWebFileHandle DmsVirtualDirOpen(const char *filename, enum UpnpOpenFileMode mode);
int DmsVirtualDirRead(UpnpWebFileHandle fh, char *buf, size_t buflen);
int DmsVirtualDirWrite(UpnpWebFileHandle fh, char *buf, size_t buflen);
int DmsVirtualDirSeek(UpnpWebFileHandle fh, off_t offset, int origin);
VOS_INT32 DmsVirtualDirClose(UpnpWebFileHandle fh);

#define ATP_DLNA_PROCESS_PARA_FILE "/var/dlnapara"
#define ATP_DMS_PARA_MIN_NB         4
#define ATP_SCANNER_PARA_MIN_NB     3

typedef struct tagATP_DLNA_PROCESS_PARA_ST
{
    VOS_INT8   processName[8];
    //union
    //{
        VOS_UINT16 ulDlnaCertification;
        VOS_UINT16 dhcpStAndDelExistDB;
        //VOS_UINT16 mdbPathLen;
    //} uField;
    //VOS_INT8   dhcpStatus;
    //VOS_INT8   needDelExistDB;
    VOS_UINT32  paraLen;
    VOS_INT8   buf[0];
} ATP_DLNA_PROCESS_PARA_ST;

#ifdef ATP_DEBUG
#define ATP_DMS_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_DMS_DEBUG(x...)
#endif


#endif

