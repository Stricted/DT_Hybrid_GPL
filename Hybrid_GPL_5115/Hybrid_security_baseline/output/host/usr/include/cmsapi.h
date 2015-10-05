/**\file cmsapi.h
  *corecms 公共函数头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author z00192937
  *\date  2012-2-8
*/
#ifndef __ATP_CMS_API_H__
#define __ATP_CMS_API_H__

#ifdef __cplusplus
extern "C" {
#endif

extern ATP_THREAD_SAFE VOS_UINT32 g_ulCmsErrCode;
extern VOS_BOOL g_bCmsDbInited;

typedef enum tagATP_CMS_RET_CODE_EN
{
    ATP_CMS_RET_INTERNAL_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CMS),
    ATP_CMS_RET_PARA_ERR,
    ATP_CMS_RET_APPNAME_ERR,
    ATP_CMS_RET_MEM_ERR,

    ATP_CMS_RET_INVALID_VALUE,

    // 9001

    // 9002

    // 9003

    // 9004
} ATP_CMS_RET_CODE_EN;

#define ATP_CMS_MAX_INST_NUM        (256)
/**********************h00190880*********************/

#define VDB_DB_STATUS           "/var/vdb.status"
//DTS2011083002249
#define VDB_VDB_WRITE_STATUS    "/var/vdb.write"

/**********************h00190880*********************/

/*
 *  Add and Del function proc
 */
typedef VOS_UINT32 (*CmsDelObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjID);

typedef VOS_UINT32 (*CmsCheckSingleObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);

typedef VOS_UINT32 (*CmsCheckObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);

typedef VOS_UINT32 (*CmsSetSingleObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);

typedef VOS_UINT32 (*CmsAddSetObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);

typedef VOS_UINT32 (*CmsGetObjProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                            const VOS_UINT32 *pulFocusParas,
                            VOS_UINT32 ulNumOfPara);

typedef VOS_UINT32 (*CmsGetInstProc)(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, VOS_UINT32 *pulInstValues,
                            VOS_UINT32 ulArrayLen, VOS_UINT32 *pulNumOfInst);

typedef struct tagATP_CMS_MULTI_INST_OBJ_ST
{
    VOS_UINT32          ulMaxInstNum;
    CmsCheckObjProc     pfCheckProc;
    CmsAddSetObjProc    pfAddSetProc;
    CmsDelObjProc       pfDelProc;
} ATP_CMS_MULTI_INST_OBJ_ST;

typedef struct tagATP_CMS_GET_OBJ_ST
{
    CmsGetObjProc       pfGetProc;
    CmsGetInstProc      pfGetInstProc;
} ATP_CMS_GET_OBJ_ST;

typedef struct tagATP_CMS_SET_SINGLE_OBJ_ST
{
    CmsCheckSingleObjProc   pfCheckProc;
    CmsSetSingleObjProc     pfSetSingleObjProc;
} ATP_CMS_SET_SINGLE_OBJ_ST;

typedef VOS_UINT32  (*pfDoAfterCmsInit)(VOS_VOID* pvArg);
typedef VOS_VOID    (*pfArgFree)(VOS_VOID*  pvArg);

typedef struct tagATP_CMS_AFTER_INIT_CALL
{    
    struct tagATP_CMS_AFTER_INIT_CALL*  pstNext;
    
    pfDoAfterCmsInit    pfCallBack;
    VOS_VOID*           pvArg;
    pfArgFree           pfFree;
}ATP_CMS_AFTER_INIT_CALL;

extern VOS_BOOL g_bInited;
VOS_UINT32 ATP_CMS_SigRtSigInit(VOS_VOID);
VOS_UINT32 ATP_CMS_AddAfterCmsInitCallback(pfDoAfterCmsInit pfDoAfter, 
                                           VOS_VOID*        pvArg,
                                           pfArgFree        pfFree);

VOS_UINT32 ATP_CMS_DoAfterModuleInit(VOS_VOID);

VOS_UINT32 ATP_CMS_RegGetObj(const VOS_CHAR *pcObjName, const ATP_CMS_GET_OBJ_ST *pstObj);
VOS_UINT32 ATP_CMS_RegSObj(const VOS_CHAR *pcObjName, const ATP_CMS_SET_SINGLE_OBJ_ST *pstSObj);
VOS_UINT32 ATP_CMS_RegMObj(const VOS_CHAR *pcObjName, const ATP_CMS_MULTI_INST_OBJ_ST *pstMObj);

typedef struct tagATP_CMS_MSG_HDL_LIST_ST
{
    struct tagATP_CMS_MSG_HDL_LIST_ST   *pstNext;
    VOS_UINT32                          ulMsgType;
    VOS_UINT32                          ulPriority;
    AtpMsgProc                          pfProc;
} ATP_CMS_MSG_HDL_LIST_ST;

#define ATP_CMS_MSG_PRIORITY_DEFAULT_D          (0x5000)
#define ATP_CMS_MSG_PRIORITY_STEP_D             (0x0100)

VOS_UINT32 ATP_CMS_RegMsgProc(VOS_UINT32 ulMsgType, AtpMsgProc pfProc,
                    VOS_UINT32 ulPriority, ATP_CMS_MSG_HDL_LIST_ST **ppstMsgHdl);

VOS_VOID ATP_CMS_UnRegMsgProc(ATP_CMS_MSG_HDL_LIST_ST *ppstMsgHdl);

VOS_VOID ATP_CMS_DoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

//!for gmsg

typedef VOS_VOID   (*PFMgntModuleGMsgProc)(VOS_VOID *pvGMsg, VOS_UINT32 ulSubType);

typedef struct tagATP_CMS_GMSG_HDL_LIST_ST
{
    struct tagATP_CMS_GMSG_HDL_LIST_ST   *pstNext;
    VOS_CHAR    pcModName[16];
    VOS_UINT32  ulMsgType;
    PFMgntModuleGMsgProc    pfProc;
} ATP_CMS_GMSG_HDL_LIST_ST;

VOS_VOID ATP_CMS_DoGMsgProc(const ATP_MSG_HEADER_ST *pvGMsg);
VOS_UINT32 ATP_CMS_RegGMsgProc(VOS_CHAR * pszModName, PFMgntModuleGMsgProc pfProc, ATP_CMS_GMSG_HDL_LIST_ST **ppstMsgHdl);
VOS_VOID ATP_CMS_UnRegGMsgProc(ATP_CMS_GMSG_HDL_LIST_ST *ppstMsgHdl);

/******************************************************************************
  函数名称  : ATP_CMS_Init
  功能描述  : 初始化CMS中的各个模块
  输入参数  : 无
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 

  修改历史      :
   1.日    期   : 2008-12-16
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CMS_Init(VOS_VOID);

/******************************************************************************
  函数名称  : ATP_CMS_UnInit
  功能描述  : 去初始化CMS中的各个模块
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 

  修改历史      :
   1.日    期   : 2008-12-16
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CMS_UnInit(VOS_VOID);
VOS_UINT32 ATP_CMS_MonitorCreate(VOS_VOID);

extern ATP_THREAD_SAFE ATP_CFM_E_IF_TYPE g_enCfgTool;

VOS_VOID ATP_CMS_AddSetMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID ATP_CMS_GetMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID ATP_CMS_GetInstMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID ATP_CMS_AddMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID ATP_CMS_DelMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

typedef VOS_VOID (* PFCmsSetMsgHooks)(VOS_VOID *pvData);

VOS_UINT32 ATP_CMS_RegSetMsgHook(PFCmsSetMsgHooks pfSetHook, VOS_VOID *pvData);

// Will not override, you can register the same function several times, and it will be executed serveral times
VOS_UINT32 ATP_CMS_RegSetMsgHookEx(PFCmsSetMsgHooks pfSetHook, VOS_VOID *pvData);
    
VOS_UINT32 ATP_CMS_RegCwmpEndSessionHook(PFCmsSetMsgHooks pfEndSessionHook, VOS_VOID *pvData);

VOS_VOID ATP_CMS_CwmpSessionEndMsgProc(const ATP_MSG_HEADER_ST *pvMsg);

VOS_UINT32 ATP_CMS_GetObj(const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32   ulOID,
                                 VOS_UINT32   **ppulObj);
/*
 *  Version utilities
 */
typedef const VOS_CHAR * (*PfAtpGetVersion)();
void ATP_CMS_GeneralPrintVersion(const VOS_CHAR *pcModName, PfAtpGetVersion pfnGetVersion);

#if 0
const char * ATP_CMSCORE_GetVersion(void);                         
#endif

VOS_UINT32 ATP_CMS_UpdateDynAppListenFd(const VOS_CHAR * pcAppName, VOS_BOOL bStop);
/*********************h00190880********************/
typedef VOS_UINT32 (*CmsGlobalCheckPara)();
/*********************h00190880********************/

#ifdef SUPPORT_ATP_DECT_VOICE
extern void *g_pvVdbVoiceHdl;
extern void *g_pvDummyDbHdl;
#endif
/************************************************************************************************************************************/
/*Common Functions for cms*/
/*长度*/
#define ATP_SYS_BITS_MAX                    8
#define ATP_SYS_TINY_LEN                    16
#define ATP_SYS_CMD_LEN                     512
#define ATP_SYS_SMALL_LEN                   32
#define ATP_SYS_LARGE_LEN                   264
#define ATP_SYS_GIANT_LEN                   1024

/*String Safe Length*/
#define ATP_STRING_LEN_128                  132   
#define ATP_STRING_LEN_16                   20    
#define ATP_STRING_LEN_8                    12
#define ATP_STRING_LEN_2                    4

/*String Actual Length*/
#define ATP_ACTUAL_LEN_32                   32 
#define ATP_ACTUAL_LEN_16                   16


/*IP*/
#define ATP_SYS_IP_BYTE                     4
#define ATP_SYS_IP_BYTE_FIRST               0
#define ATP_SYS_IP_BYTE_SECOND              1
#define ATP_SYS_IP_BYTE_THIRD               2
#define ATP_SYS_IP_BYTE_FOUR                3
#define ATP_SYS_IP_DOT_LENGTH               19
#define ATP_SYS_IP_LENGTH                   16
#define ATP_SYS_IP_LOCAL_FIRST              127
#define ATP_SYS_IP_MAX_NUMBER               255
#define ATP_SYS_IP_MIN_NUMBER               0
#define ATP_SYS_IP_MULTI_FIRST_MAX          239
#define ATP_SYS_IP_MULTI_FIRST_MIN          224
#define ATP_SYS_IP_TBL_COL_NUM              0
#define ATP_SYS_IP_TBL_COL_MIN              2
#define ATP_SYS_IP_TBL_COL_PKTS             1
#define ATP_SYS_IP_TBL_COL_BYTES            2
#define ATP_SYS_IP_TBL_COL_TARGET           3
#define ATP_SYS_IP_TBL_COL_PROT             4
#define ATP_SYS_IP_TBL_COL_OPT              5
#define ATP_SYS_IP_TBL_COL_IN               6
#define ATP_SYS_IP_TBL_COL_OUT              7
#define ATP_SYS_IP_TBL_COL_SRC              8
#define ATP_SYS_IP_TBL_COL_DST              9
#define ATP_SYS_IP_TBL_COL_MAX              10

/*Route*/
#define ATP_ROUTE_DEF_COL                   11
#define ATP_ROUTE_DEF_COL_MIN               1
#define ATP_ROUTE_DEF_COL_FIRST             0
#define ATP_ROUTE_DEF_COL_SECOND            1
#define ATP_ROUTE_DEF_COL_THIRD             2
#define ATP_ROUTE_DEF_COL_FOUR              3
#define ATP_ROUTE_DEF_COL_FIVE              4
#define ATP_ROUTE_DEF_COL_SIX               5
#define ATP_ROUTE_DEF_COL_SEVEN             6
#define ATP_ROUTE_DEF_COL_EIGHT             7
#define ATP_ROUTE_DEF_COL_NINE              8
#define ATP_ROUTE_DEF_COL_TEN               9
#define ATP_ROUTE_DEF_COL_ELEVEN            10
#define ATP_ROUTE_DEF_FLAG                  "00000000"


/*进制*/
#define ATP_SYS_BASE_DEC                    10
#define ATP_SYS_BASE_HEX                    16

/*MAC*/
#define ATP_SYS_MAC_BYTE                    6
#define ATP_SYS_MAC_LENGTH                  18
#define ATP_SYS_MAC_MAX_NUMBER              255

/*Time*/
#define ATP_SYS_TIME_HOUR_MAX               23
#define ATP_SYS_TIME_LENGTH                 5
#define ATP_SYS_TIME_MINUTE_MAX             59
#define ATP_SYS_TIME_PART_LEN               2 //用于判断时间字符串中":"前后的长度

/*LAN*/
#define ATP_LAN_MAX                         4
#define ATP_LAN_ENET_ID	                    1
#define ATP_LAN_USB_ID		                  (ATP_LAN_ENET_ID + ATP_LAN_MAX)
#define ATP_LAN_HPNA_ID		                  (ATP_LAN_USB_ID + ATP_LAN_MAX)
#define ATP_LAN_WIRELESS_ID		              (ATP_LAN_HPNA_ID + ATP_LAN_MAX)
#define ATP_LAN_SUBNET_ID                   (ATP_LAN_WIRELESS_ID + ATP_LAN_MAX) 
#define ATP_LAN_ENET0_VNET_ID               (ATP_LAN_SUBNET_ID + ATP_LAN_MAX)
#define ATP_LAN_ENET1_VNET_ID               (ATP_LAN_ENET0_VNET_ID + ATP_LAN_MAX)

/*Time*/
#define ATP_UTIL_MSleep(x)                  usleep((x)*1000)

/*Interface*/
/* WAN type by w00165788 20120314 */
#define ATP_WAN_DSL_TYPE                    "DSL"
#define ATP_WAN_ETH_TYPE                    "Ethernet"
#define ATP_WAN_POT_TYPE                    "POTS"
#define ATP_WAN_UMTS_TYPE                  "UMTS"



/*start added by l00184769 for compare func*/
#define VOS_BIG                             1
#define VOS_SMALL                           (-1)
#define VOS_EQUAL                           0
#define VOS_ERROR_COMPARE                   (-2)
/*end added by l00184769 for compare func*/

#ifdef ATP_DEBUG
#define ATP_DEBUG_Outputf(x...)
#else
#define ATP_DEBUG_Outputf(x...)
#endif


/*一.Valid Function*/
/*0.Public Valid*/
/*!
  \brief  校验只有三个数字的字符串
  \param[in]  pcBuf: 待校验字符串
  \param[in]  isAllowZeroStart: ip地址是否允许0开头
  \return *pcBuf是否非空并且是只有三个数字的字符串
  \retval VOS_TRUE  *pcBuf 是三个数字的字符串
  \retval VOS_FALSE *pcBuf 为空或者不是三个数字的字符串
*/
VOS_BOOL ATP_UTIL_ValidateThreeNumberEx(VOS_CHAR* pcBuf, VOS_BOOL isAllowZeroStart);
/*!
  \brief 校验字符串是否为数字组成
  \param[in]  pcNum: 待校验字符串
  \return*pcNum 是否由数字组成
  \retval VOS_TRUE  *pcNum 是由数字组成
  \retval VOS_FALSE *pcNum 不是由数字组成
*/
VOS_BOOL ATP_UTIL_ValidateNumber(VOS_CHAR* pcNum);
/*!
  \brief  判断子网掩码的尾数是否合法
  \param[in]  lNum: 待判断尾数
  \return 是否合法
  \retval VOS_TRUE  尾数有效
  \retval VOS_FALSE 尾数不合法
*/
VOS_BOOL ATP_UTIL_CheckNum(VOS_INT lNum);
/*!
  \brief 验证lValue的值域范围
  \param[in]  lValue: 待校验值域
  \param[in]  lMin: 最小值
  \param[in]  lMax: 最大值
  \return 返回lValue 是否在[lMin, lMax] 内
  \retval VOS_TRUE   lValue 在[lMin, lMax] 内
  \retval VOS_FALSE  lValue 不在[lMin, lMax] 内
*/
VOS_BOOL ATP_UTIL_ValidateNumberRange(VOS_INT32 lValue, VOS_INT32 lMin, VOS_INT32 lMax);
/*!
  \brief  数字范围校验
  \param[in]  *pcNum: 待校验的字符串
  \param[in]  lMin: 待校验的数字最小值范围
  \param[in]  lMax: 待校验的数字最大值范围
  \return *pcNum是否在[lMin, lMax]范围内
   \retval VOS_TRUE  *pcNum 在[lMin, lMax]范围内
  \retval VOS_FALSE *pcNum 不在[lMin, lMax]范围内
*/
VOS_BOOL ATP_UTIL_ValidateRange(VOS_CHAR *pcNum, VOS_INT lMin, VOS_INT lMax);
/*!
  \brief 校验字符串长度
  \param[in]  *pszStr: 待校验字符串
  \param[in]  ulMax: 待校验字符串的最大长度
  \return *pszStr是否非空，且长度在ulMax范围内
  \retval VOS_TRUE  *pszStr不为空且*pszStr的长度在ulMax范围内
  \retval VOS_FALSE *pszStr为空或*pszStr的长度不在ulMax范围内
*/
VOS_BOOL ATP_UTIL_ValidateLength(VOS_CHAR *pszStr, VOS_UINT ulMax);

/*1.IP & MASK Valid*/
/*!
  \brief 校验Ipv4 地址
  \param[in]  *pcIpAddr: 需要校验的地址
  \return *pcIpAddr是否为Ip 地址
  \retval VOS_TRUE *pcIpAddr 是Ip 地址
  \retval VOS_FALSE *pcIpAddr 不是Ip 地址
*/
VOS_BOOL ATP_UTIL_ValidateIpAddress(VOS_CHAR *pcIpAddr);
/*!
  \brief 校验Ipv4 地址是否为广播地址
  \param[in]  *pIp: Ip 地址
  \param[in]  *pMask: 子网掩码
  \return Ip 地址是否为广播 地址
  \retval VOS_TRUE Ip 地址 是广播 地址
  \retval VOS_FALSE Ip 地址 不是广播 地址
*/
VOS_BOOL ATP_UTIL_ValidateBroadcastIp(VOS_INT *pIp, VOS_INT *pMask);
/*!
  \brief 校验Ipv4地址格式
  \param[in]  *pValue: 需校验的值
  \return *pValue是否为Ip地址格式
  \retval VOS_TRUE  *pValue 是Ip地址格式
  \retval VOS_FALSE *pValue不是Ip地址格式
*/
VOS_BOOL ATP_UTIL_IsIpFormat(VOS_VOID *pValue);
/*!
  \brief 校验Ipv4 地址是否为主机地址
  \param[in]  *pcIpAddr: 需校验的Ip地址
  \return *pcIpAddr是否为主机 地址
  \retval VOS_TRUE *pcIpAddr 是主机 地址
  \retval VOS_FALSE *pcIpAddr 不是主机 地址
*/
VOS_BOOL ATP_UTIL_ValidateHostIpAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief 校验Ipv4 地址是否为组播地址
  \param[in]  *pcIpAddr: 需校验的Ip地址
  \return *pcIpAddr是否为组播地址
  \retval VOS_TRUE *pcIpAddr 是组播地址
  \retval VOS_FALSE *pcIpAddr 不是组播地址
*/
VOS_BOOL ATP_UTIL_ValidateMCastIpAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief 校验Ipv4 地址是否为dns地址或主机地址
  \param[in]  *pcIpAddr: 需校验的Ip地址
  \return  *pcIpAddr是否为dns地址或主机地址
  \retval VOS_TRUE  *pcIpAddr 是dns地址或是主机地址
  \retval VOS_FALSE *pcIpAddr 不是dns地址也不是主机地址
*/
VOS_BOOL ATP_UTIL_ValidateDnsOrHostAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief 校验Ipv4子网掩码是否合法
  \param[in]  *pcSubnetMask: 需校验的子网掩码
  \return *pcSubnetMask是否为子网掩码
  \retval VOS_TRUE  *pcSubnetMask 是子网掩码
  \retval VOS_FALSE *pcSubnetMask 不是子网掩码
*/
VOS_BOOL ATP_UTIL_ValidateSubnetMask(VOS_CHAR *pcSubnetMask);

/*2.MAC Valid*/
/*!
  \brief 校验MAC 地址
  \param[in]  *pcMacAddr: 待校验MAC 地址
  \return *pcMacAddr是否为MAC 地址
  \retval VOS_TRUE  *pcMacAddr是MAC 地址  
  \retval VOS_FALSE *pcMacAddr不是MAC 地址
*/

/* Start of added by f00120964 for qos subnetmask function 2012-4-1*/
extern VOS_BOOL ATP_UTIL_QosValidateSubnetMask(VOS_CHAR *pcSubnetMask);
/*2.SubnetMask Valid*/
/*!
  \brief 校验SubnetMask 地址
  \param[in]  *pcMacAddr: 待校验SubnetMask 地址
  \return *SubnetMask是否为SubnetMask 地址

*/
/* End of added by f00120964 for qos subnetmask function 2012-4-1*/

VOS_BOOL ATP_UTIL_ValidateMacAddress(VOS_CHAR *pcMacAddr);

/*3.IPv6 Valid*/
/*!
  \brief 校验字符串是否为Ipv6地址
  \param[in]  *pcIpv6Addr: 待校验字符串
  \return *pcIpv6Addr是否为Ipv6地址
  \retval VOS_TRUE *pcIpv6Addr 是Ipv6地址
  \retval VOS_FALSE *pcIpv6Addr 非Ipv6地址
*/
VOS_BOOL ATP_UTIL_ValidateIpv6Address(const VOS_CHAR *pcIpv6Addr);

/*4.Domain Valid*/
/*!
  \brief 校验域名的格式是否正确
          1)域名只包括字母、数字和连字符'-'
          2)域名不能用'-'开头或结束
          3)域名不能包括特殊字符
          4)域名扩展名应该是经过定义的
          5)域名长度符合要求
  \param[in]  *pcDomain: 待校验域名
  \param[in]  ulLen: 允许的最大域名长度
  \return 域名格式是否正确
  \retval VOS_TRUE 域名格式正确
  \retval VOS_FALSE 域名格式不正确
*/
VOS_BOOL ATP_UTIL_ValidateDomainName(VOS_CHAR *pcDomain, VOS_UINT ulLen);

/*!
  \brief 校验域名是否合法
          1)域名包括的字符位于ascii编码的'!'和'~'之间
          2)域名总长度不超过255，各个标签长度不超过63
          3)域名中必须包含'.'，'.'不能再开头，可以在末尾，单个'.'为合法域名
  \param[in]  *pcDomain: 待校验域名
  \return 域名格式是否正确
  \retval VOS_TRUE 域名格式正确
  \retval VOS_FALSE 域名格式不正确
*/
VOS_BOOL ATP_UTIL_ValidateDomainNameEx(const VOS_CHAR * pcDomain);

/*!
  \brief 过滤掉IPV6地址两边的中括号
  \param[in]  pszValue: 
  \param[in]  acBuf: 
  \param[in]  ulSize: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_BOOL ATP_UTIL_Ipv6AddressFilter(const VOS_CHAR* pszValue, VOS_CHAR* acBuf, VOS_UINT32 ulSize);

/*!
  \brief 字符串的IP地址和域名地址联合校验
          1)是ipv4地址时，必须满足UtilValidateIpAddress的校验
          2)是ipv6地址时，必须满足UtilValidateIpv6Address的校验
          3)既不是ipv4地址又不是ipv6地址时，必须满足ATP_UTIL_ValidateDomainNameEx的校验
  \param[in]  *pcValue: 待校验字符串
  \return 校验是否通过
  \retval VOS_TRUE 校验通过
  \retval VOS_FALSE 校验不通过
*/
VOS_BOOL ATP_UTIL_ValidateIpAddressOrDomainName(const VOS_CHAR *pcValue);

/*!
  \brief 字符串的IP地址和域名地址联合校验
          1)是ipv4地址时，必须满足UtilValidateIpAddress的校验
          2)是ipv6地址时，必须满足UtilValidateIpv6Address的校验
          3)既不是ipv4地址又不是ipv6地址时，必须满足ATP_UTIL_ValidateDomainNameEx的校验
  \param[in]  *pcValue: 待校验字符串
  \return 校验是否通过
  \retval VOS_TRUE 校验通过
  \retval VOS_FALSE 校验不通过
*/
VOS_BOOL ATP_UTIL_ValidateIpAddressOrDomainNameEx(const VOS_CHAR *pcValue);

/*5.Time Valid*/
/*!
  \brief 校验时间字符串是否合法
  \param[in]  pszTime: 待校验时间字符串
  \return *pszTime是否非空并且合法
  \retval VOS_TRUE  *pszTime 有效
  \retval VOS_FALSE *pszTime为空或不合法
*/
VOS_BOOL ATP_UTIL_ValidateTime(VOS_CHAR* pszTime);

/*二.common call functions*/
/*1.convert & get & set Function*/
/*!
  \brief  把IPv4字符串转换成数组
  \param[in]  *pszIpAddrValue: Ip字符串
  \param[out]  *pSplitValue: 转换后的数组
  \return 是否转换成功
  \retval VOS_TRUE  转换成功
  \retval VOS_FALSE 转换失败
*/
VOS_BOOL ATP_UTIL_IpAddrSplit(VOS_CHAR *pszIpAddrValue, VOS_INT *pSplitValue);

/*2.InterfaceName Function*/
/*!
  \brief  根据ID，获取接口名
  \param[in]  lInterId: 接口名ID
  \param[out]  *pcInterName: 获取到的接口名
  \return 是否获取接口名成功
  \retval VOS_TRUE  获取接口名成功
  \retval VOS_FALSE 获取接口名失败
*/
VOS_INT ATP_UTIL_GetInterfaceNameById(VOS_INT  lInterId, VOS_CHAR *pcInterName);

/*3.Module Function*/
/*!
  \brief 校验模块是否已经插入
  \param[in]  *pcModName: 待校验模块名
  \return 是否模块已插入
  \retval VOS_TRUE   模块已插入
  \retval VOS_FALSE  模块未插入
*/
VOS_BOOL ATP_UTIL_IsModuleInserted(VOS_CHAR *pcModName);
/*!
  \brief 插入模块
  \param[in]  *pcModName: 待插入模块名
  \param[in]  *pcModulePath: 模块路径
  \return 是否 插入成功
  \retval VOS_TRUE  插入成功
  \retval VOS_FALSE 插入失败
*/
VOS_BOOL ATP_UTIL_InsertModule(VOS_CHAR *pcModName, VOS_CHAR *pcModulePath);

typedef VOS_UINT32 (*AtpStatusChangeNotifyProc)(VOS_UINT32 ulServiceType, VOS_VOID *pvNotifyData);

/*!
  \brief Status change response interface
  \param[in]  ulServiceType: 
  \param[in]  pfNotifyProc: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_STATUS_RegNotifyProc(
                                VOS_UINT32 ulServiceType,
                                AtpStatusChangeNotifyProc pfNotifyProc);

/*!
  \brief Status change notifier interface
  \param[in]  ulServiceType: 
  \param[in]  *pvNotifyData: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_STATUS_ChangeNotify(
                                VOS_UINT32 ulServiceType, 
                                VOS_VOID *pvNotifyData);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval 非0 成功 0 失败
*/

VOS_BOOL ATP_UTIL_IsIpv4Format(const VOS_CHAR *pcValue);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval  VOS_OK成功  VOS_FALSE失败
*/

VOS_BOOL ATP_UTIL_IsIpv4FormatRepair(const VOS_CHAR *pcValue);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval  VOS_OK成功  VOS_FALSE失败
*/

VOS_BOOL ATP_UTIL_IsGlobalIpv6Address(const VOS_CHAR *pcValue);

/*!
  \brief 清除所有资源
  \return 
  \retval VOS_OK成功
*/
VOS_VOID ATP_STATUS_DestroyAllNotify();

VOS_INT32 ATP_UTIL_CompareIpv6Address(const struct in6_addr* pstAddrNew, const struct in6_addr* pstAddrOld);


/******************************************************************************
  函数名称：WanValidateNumberAlpha
  功能描述：校验字符串是否是a-z，A-Z, 0-9
  输入参数：pszValue 
  输出参数：无
  返 回 值：合法VOS_TRUE，非法VOS_FALSE
  其它说明：
******************************************************************************/
VOS_BOOL ATP_UTIL_ValidateNumberAlpha(const VOS_CHAR* pszValue);
VOS_BOOL  ATP_UTIL_ValidateIpv6AddressEx(const VOS_CHAR * pcIpv6Addr);

/************************************************************************************************************************************/
#ifdef ATP_DEBUG
#define ATP_CMS_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_CMS_DEBUG(x...)
#endif

#define ATP_CMS_RELEASE(format, args...)     {ATP_MSG_Printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);ATP_MSG_Printf(format, ##args);}

#ifdef SUPPORT_ATP_TRACE_CODE_DEBUG
#define ATP_CMS_TRACE(format, args...)     {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_CMS_TRACE(x...)
#endif

#ifdef __cplusplus
}
#endif

#endif

