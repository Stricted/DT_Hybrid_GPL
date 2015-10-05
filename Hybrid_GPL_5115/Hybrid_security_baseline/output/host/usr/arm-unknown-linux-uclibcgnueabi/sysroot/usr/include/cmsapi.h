/**\file cmsapi.h
  *corecms ��������ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
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
  ��������  : ATP_CMS_Init
  ��������  : ��ʼ��CMS�еĸ���ģ��
  �������  : ��
              ��
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : 

  �޸���ʷ      :
   1.��    ��   : 2008-12-16
     ��    ��   : z65940
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CMS_Init(VOS_VOID);

/******************************************************************************
  ��������  : ATP_CMS_UnInit
  ��������  : ȥ��ʼ��CMS�еĸ���ģ��
  �������  : 
              ��
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : 

  �޸���ʷ      :
   1.��    ��   : 2008-12-16
     ��    ��   : z65940
     �޸�����   : ��ɳ���

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
/*����*/
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


/*����*/
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
#define ATP_SYS_TIME_PART_LEN               2 //�����ж�ʱ���ַ�����":"ǰ��ĳ���

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


/*һ.Valid Function*/
/*0.Public Valid*/
/*!
  \brief  У��ֻ���������ֵ��ַ���
  \param[in]  pcBuf: ��У���ַ���
  \param[in]  isAllowZeroStart: ip��ַ�Ƿ�����0��ͷ
  \return *pcBuf�Ƿ�ǿղ�����ֻ���������ֵ��ַ���
  \retval VOS_TRUE  *pcBuf ���������ֵ��ַ���
  \retval VOS_FALSE *pcBuf Ϊ�ջ��߲����������ֵ��ַ���
*/
VOS_BOOL ATP_UTIL_ValidateThreeNumberEx(VOS_CHAR* pcBuf, VOS_BOOL isAllowZeroStart);
/*!
  \brief У���ַ����Ƿ�Ϊ�������
  \param[in]  pcNum: ��У���ַ���
  \return*pcNum �Ƿ����������
  \retval VOS_TRUE  *pcNum �����������
  \retval VOS_FALSE *pcNum �������������
*/
VOS_BOOL ATP_UTIL_ValidateNumber(VOS_CHAR* pcNum);
/*!
  \brief  �ж����������β���Ƿ�Ϸ�
  \param[in]  lNum: ���ж�β��
  \return �Ƿ�Ϸ�
  \retval VOS_TRUE  β����Ч
  \retval VOS_FALSE β�����Ϸ�
*/
VOS_BOOL ATP_UTIL_CheckNum(VOS_INT lNum);
/*!
  \brief ��֤lValue��ֵ��Χ
  \param[in]  lValue: ��У��ֵ��
  \param[in]  lMin: ��Сֵ
  \param[in]  lMax: ���ֵ
  \return ����lValue �Ƿ���[lMin, lMax] ��
  \retval VOS_TRUE   lValue ��[lMin, lMax] ��
  \retval VOS_FALSE  lValue ����[lMin, lMax] ��
*/
VOS_BOOL ATP_UTIL_ValidateNumberRange(VOS_INT32 lValue, VOS_INT32 lMin, VOS_INT32 lMax);
/*!
  \brief  ���ַ�ΧУ��
  \param[in]  *pcNum: ��У����ַ���
  \param[in]  lMin: ��У���������Сֵ��Χ
  \param[in]  lMax: ��У����������ֵ��Χ
  \return *pcNum�Ƿ���[lMin, lMax]��Χ��
   \retval VOS_TRUE  *pcNum ��[lMin, lMax]��Χ��
  \retval VOS_FALSE *pcNum ����[lMin, lMax]��Χ��
*/
VOS_BOOL ATP_UTIL_ValidateRange(VOS_CHAR *pcNum, VOS_INT lMin, VOS_INT lMax);
/*!
  \brief У���ַ�������
  \param[in]  *pszStr: ��У���ַ���
  \param[in]  ulMax: ��У���ַ�������󳤶�
  \return *pszStr�Ƿ�ǿգ��ҳ�����ulMax��Χ��
  \retval VOS_TRUE  *pszStr��Ϊ����*pszStr�ĳ�����ulMax��Χ��
  \retval VOS_FALSE *pszStrΪ�ջ�*pszStr�ĳ��Ȳ���ulMax��Χ��
*/
VOS_BOOL ATP_UTIL_ValidateLength(VOS_CHAR *pszStr, VOS_UINT ulMax);

/*1.IP & MASK Valid*/
/*!
  \brief У��Ipv4 ��ַ
  \param[in]  *pcIpAddr: ��ҪУ��ĵ�ַ
  \return *pcIpAddr�Ƿ�ΪIp ��ַ
  \retval VOS_TRUE *pcIpAddr ��Ip ��ַ
  \retval VOS_FALSE *pcIpAddr ����Ip ��ַ
*/
VOS_BOOL ATP_UTIL_ValidateIpAddress(VOS_CHAR *pcIpAddr);
/*!
  \brief У��Ipv4 ��ַ�Ƿ�Ϊ�㲥��ַ
  \param[in]  *pIp: Ip ��ַ
  \param[in]  *pMask: ��������
  \return Ip ��ַ�Ƿ�Ϊ�㲥 ��ַ
  \retval VOS_TRUE Ip ��ַ �ǹ㲥 ��ַ
  \retval VOS_FALSE Ip ��ַ ���ǹ㲥 ��ַ
*/
VOS_BOOL ATP_UTIL_ValidateBroadcastIp(VOS_INT *pIp, VOS_INT *pMask);
/*!
  \brief У��Ipv4��ַ��ʽ
  \param[in]  *pValue: ��У���ֵ
  \return *pValue�Ƿ�ΪIp��ַ��ʽ
  \retval VOS_TRUE  *pValue ��Ip��ַ��ʽ
  \retval VOS_FALSE *pValue����Ip��ַ��ʽ
*/
VOS_BOOL ATP_UTIL_IsIpFormat(VOS_VOID *pValue);
/*!
  \brief У��Ipv4 ��ַ�Ƿ�Ϊ������ַ
  \param[in]  *pcIpAddr: ��У���Ip��ַ
  \return *pcIpAddr�Ƿ�Ϊ���� ��ַ
  \retval VOS_TRUE *pcIpAddr ������ ��ַ
  \retval VOS_FALSE *pcIpAddr �������� ��ַ
*/
VOS_BOOL ATP_UTIL_ValidateHostIpAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief У��Ipv4 ��ַ�Ƿ�Ϊ�鲥��ַ
  \param[in]  *pcIpAddr: ��У���Ip��ַ
  \return *pcIpAddr�Ƿ�Ϊ�鲥��ַ
  \retval VOS_TRUE *pcIpAddr ���鲥��ַ
  \retval VOS_FALSE *pcIpAddr �����鲥��ַ
*/
VOS_BOOL ATP_UTIL_ValidateMCastIpAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief У��Ipv4 ��ַ�Ƿ�Ϊdns��ַ��������ַ
  \param[in]  *pcIpAddr: ��У���Ip��ַ
  \return  *pcIpAddr�Ƿ�Ϊdns��ַ��������ַ
  \retval VOS_TRUE  *pcIpAddr ��dns��ַ����������ַ
  \retval VOS_FALSE *pcIpAddr ����dns��ַҲ����������ַ
*/
VOS_BOOL ATP_UTIL_ValidateDnsOrHostAddress(const VOS_CHAR *pcIpAddr);
/*!
  \brief У��Ipv4���������Ƿ�Ϸ�
  \param[in]  *pcSubnetMask: ��У�����������
  \return *pcSubnetMask�Ƿ�Ϊ��������
  \retval VOS_TRUE  *pcSubnetMask ����������
  \retval VOS_FALSE *pcSubnetMask ������������
*/
VOS_BOOL ATP_UTIL_ValidateSubnetMask(VOS_CHAR *pcSubnetMask);

/*2.MAC Valid*/
/*!
  \brief У��MAC ��ַ
  \param[in]  *pcMacAddr: ��У��MAC ��ַ
  \return *pcMacAddr�Ƿ�ΪMAC ��ַ
  \retval VOS_TRUE  *pcMacAddr��MAC ��ַ  
  \retval VOS_FALSE *pcMacAddr����MAC ��ַ
*/

/* Start of added by f00120964 for qos subnetmask function 2012-4-1*/
extern VOS_BOOL ATP_UTIL_QosValidateSubnetMask(VOS_CHAR *pcSubnetMask);
/*2.SubnetMask Valid*/
/*!
  \brief У��SubnetMask ��ַ
  \param[in]  *pcMacAddr: ��У��SubnetMask ��ַ
  \return *SubnetMask�Ƿ�ΪSubnetMask ��ַ

*/
/* End of added by f00120964 for qos subnetmask function 2012-4-1*/

VOS_BOOL ATP_UTIL_ValidateMacAddress(VOS_CHAR *pcMacAddr);

/*3.IPv6 Valid*/
/*!
  \brief У���ַ����Ƿ�ΪIpv6��ַ
  \param[in]  *pcIpv6Addr: ��У���ַ���
  \return *pcIpv6Addr�Ƿ�ΪIpv6��ַ
  \retval VOS_TRUE *pcIpv6Addr ��Ipv6��ַ
  \retval VOS_FALSE *pcIpv6Addr ��Ipv6��ַ
*/
VOS_BOOL ATP_UTIL_ValidateIpv6Address(const VOS_CHAR *pcIpv6Addr);

/*4.Domain Valid*/
/*!
  \brief У�������ĸ�ʽ�Ƿ���ȷ
          1)����ֻ������ĸ�����ֺ����ַ�'-'
          2)����������'-'��ͷ�����
          3)�������ܰ��������ַ�
          4)������չ��Ӧ���Ǿ��������
          5)�������ȷ���Ҫ��
  \param[in]  *pcDomain: ��У������
  \param[in]  ulLen: ����������������
  \return ������ʽ�Ƿ���ȷ
  \retval VOS_TRUE ������ʽ��ȷ
  \retval VOS_FALSE ������ʽ����ȷ
*/
VOS_BOOL ATP_UTIL_ValidateDomainName(VOS_CHAR *pcDomain, VOS_UINT ulLen);

/*!
  \brief У�������Ƿ�Ϸ�
          1)�����������ַ�λ��ascii�����'!'��'~'֮��
          2)�����ܳ��Ȳ�����255��������ǩ���Ȳ�����63
          3)�����б������'.'��'.'�����ٿ�ͷ��������ĩβ������'.'Ϊ�Ϸ�����
  \param[in]  *pcDomain: ��У������
  \return ������ʽ�Ƿ���ȷ
  \retval VOS_TRUE ������ʽ��ȷ
  \retval VOS_FALSE ������ʽ����ȷ
*/
VOS_BOOL ATP_UTIL_ValidateDomainNameEx(const VOS_CHAR * pcDomain);

/*!
  \brief ���˵�IPV6��ַ���ߵ�������
  \param[in]  pszValue: 
  \param[in]  acBuf: 
  \param[in]  ulSize: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_BOOL ATP_UTIL_Ipv6AddressFilter(const VOS_CHAR* pszValue, VOS_CHAR* acBuf, VOS_UINT32 ulSize);

/*!
  \brief �ַ�����IP��ַ��������ַ����У��
          1)��ipv4��ַʱ����������UtilValidateIpAddress��У��
          2)��ipv6��ַʱ����������UtilValidateIpv6Address��У��
          3)�Ȳ���ipv4��ַ�ֲ���ipv6��ַʱ����������ATP_UTIL_ValidateDomainNameEx��У��
  \param[in]  *pcValue: ��У���ַ���
  \return У���Ƿ�ͨ��
  \retval VOS_TRUE У��ͨ��
  \retval VOS_FALSE У�鲻ͨ��
*/
VOS_BOOL ATP_UTIL_ValidateIpAddressOrDomainName(const VOS_CHAR *pcValue);

/*!
  \brief �ַ�����IP��ַ��������ַ����У��
          1)��ipv4��ַʱ����������UtilValidateIpAddress��У��
          2)��ipv6��ַʱ����������UtilValidateIpv6Address��У��
          3)�Ȳ���ipv4��ַ�ֲ���ipv6��ַʱ����������ATP_UTIL_ValidateDomainNameEx��У��
  \param[in]  *pcValue: ��У���ַ���
  \return У���Ƿ�ͨ��
  \retval VOS_TRUE У��ͨ��
  \retval VOS_FALSE У�鲻ͨ��
*/
VOS_BOOL ATP_UTIL_ValidateIpAddressOrDomainNameEx(const VOS_CHAR *pcValue);

/*5.Time Valid*/
/*!
  \brief У��ʱ���ַ����Ƿ�Ϸ�
  \param[in]  pszTime: ��У��ʱ���ַ���
  \return *pszTime�Ƿ�ǿղ��ҺϷ�
  \retval VOS_TRUE  *pszTime ��Ч
  \retval VOS_FALSE *pszTimeΪ�ջ򲻺Ϸ�
*/
VOS_BOOL ATP_UTIL_ValidateTime(VOS_CHAR* pszTime);

/*��.common call functions*/
/*1.convert & get & set Function*/
/*!
  \brief  ��IPv4�ַ���ת��������
  \param[in]  *pszIpAddrValue: Ip�ַ���
  \param[out]  *pSplitValue: ת���������
  \return �Ƿ�ת���ɹ�
  \retval VOS_TRUE  ת���ɹ�
  \retval VOS_FALSE ת��ʧ��
*/
VOS_BOOL ATP_UTIL_IpAddrSplit(VOS_CHAR *pszIpAddrValue, VOS_INT *pSplitValue);

/*2.InterfaceName Function*/
/*!
  \brief  ����ID����ȡ�ӿ���
  \param[in]  lInterId: �ӿ���ID
  \param[out]  *pcInterName: ��ȡ���Ľӿ���
  \return �Ƿ��ȡ�ӿ����ɹ�
  \retval VOS_TRUE  ��ȡ�ӿ����ɹ�
  \retval VOS_FALSE ��ȡ�ӿ���ʧ��
*/
VOS_INT ATP_UTIL_GetInterfaceNameById(VOS_INT  lInterId, VOS_CHAR *pcInterName);

/*3.Module Function*/
/*!
  \brief У��ģ���Ƿ��Ѿ�����
  \param[in]  *pcModName: ��У��ģ����
  \return �Ƿ�ģ���Ѳ���
  \retval VOS_TRUE   ģ���Ѳ���
  \retval VOS_FALSE  ģ��δ����
*/
VOS_BOOL ATP_UTIL_IsModuleInserted(VOS_CHAR *pcModName);
/*!
  \brief ����ģ��
  \param[in]  *pcModName: ������ģ����
  \param[in]  *pcModulePath: ģ��·��
  \return �Ƿ� ����ɹ�
  \retval VOS_TRUE  ����ɹ�
  \retval VOS_FALSE ����ʧ��
*/
VOS_BOOL ATP_UTIL_InsertModule(VOS_CHAR *pcModName, VOS_CHAR *pcModulePath);

typedef VOS_UINT32 (*AtpStatusChangeNotifyProc)(VOS_UINT32 ulServiceType, VOS_VOID *pvNotifyData);

/*!
  \brief Status change response interface
  \param[in]  ulServiceType: 
  \param[in]  pfNotifyProc: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_STATUS_RegNotifyProc(
                                VOS_UINT32 ulServiceType,
                                AtpStatusChangeNotifyProc pfNotifyProc);

/*!
  \brief Status change notifier interface
  \param[in]  ulServiceType: 
  \param[in]  *pvNotifyData: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_STATUS_ChangeNotify(
                                VOS_UINT32 ulServiceType, 
                                VOS_VOID *pvNotifyData);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval ��0 �ɹ� 0 ʧ��
*/

VOS_BOOL ATP_UTIL_IsIpv4Format(const VOS_CHAR *pcValue);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval  VOS_OK�ɹ�  VOS_FALSEʧ��
*/

VOS_BOOL ATP_UTIL_IsIpv4FormatRepair(const VOS_CHAR *pcValue);

/*!
  \brief 
  \param[in]  *pcValue: 
  \return 
  \retval  VOS_OK�ɹ�  VOS_FALSEʧ��
*/

VOS_BOOL ATP_UTIL_IsGlobalIpv6Address(const VOS_CHAR *pcValue);

/*!
  \brief ���������Դ
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_VOID ATP_STATUS_DestroyAllNotify();

VOS_INT32 ATP_UTIL_CompareIpv6Address(const struct in6_addr* pstAddrNew, const struct in6_addr* pstAddrOld);


/******************************************************************************
  �������ƣ�WanValidateNumberAlpha
  ����������У���ַ����Ƿ���a-z��A-Z, 0-9
  ���������pszValue 
  �����������
  �� �� ֵ���Ϸ�VOS_TRUE���Ƿ�VOS_FALSE
  ����˵����
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

