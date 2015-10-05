/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file cfmapi.h
  *CFM library interface \n
  *     CFM�⹲���������󲿷�\n
  *     ���ݿ�ӿ�: DB�����ӿ����ڷ������ݿ⣬ÿ������cfmapi��Ľ��̵���ATP_DBInit�󶼿��Է������ݿ⡣\n
  *     CFMA�ӿ�:   CFMA�ӿ�����Ϊ���ù����ṩͳһ�����ù���ӿڣ���Ҫ��web��upnp��cwmp���á�
  *\author z65940
  *\date  2011-12-17
*/

#ifndef __CFM_API_H__
#define __CFM_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/******************************************************************************/
/*************/


#include "cfmadaptapi.h"
#include "httpapi.h"

//! CFM���ݿ��ʵ����������Ĳ��
#define ATP_CFM_MAX_TREE_DEPTH  (6)
#define ATP_CFM_MSG_RETCODE_BASE    (1024)
#define ATP_CFM_INSTNUM_FAKE_OID    (0xFFFFFFFE)

#define ATP_CFM_CFG_ROOTNAME    "InternetGatewayDeviceConfig"
#define ATP_CFM_CFG_ENDTAG        "</InternetGatewayDeviceConfig>"

#define ATP_CFM_CFGFILE_HEAD    "<?xml version=\"1.0\" ?>"
#define ATP_CFM_CFG_EXTENTIONNAME  "ATPExtentionConfig"

#define ATP_CFM_MSG_BASE    ATP_MSG_CATEGORY_DEF(ATP_MODULE_CFM)

#ifdef  SUPPORT_ATP_TR181
#define ATP_ALIAS_LEN       64
#define CWMP_MS_INSTANCEMODE_D "ManagementServer.InstanceMode"
#endif

//! CFM debug function
/*! ͨ��menuconfig��ѡ��ʹ��debugģʽ
  *  ����ͨ��make cfmcore ATP_DEBUG=1��ʹ��
  */
#ifdef ATP_DEBUG
#define CFM_DEBUG(format, args...)  {printf("CFM DEBUG============file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define CFM_DEBUG(format, args...)
#endif

//! CFM assert function, disabled when debug
//#define ATP_CFM_SAFE_RUN 1
#if defined(ATP_CFM_SAFE_RUN) && !defined(ATP_PCLINT)
#define cfmAssert(x, format, args...)   \
    if (!(x))\
    {\
        printf("CFM ASSERT>>>>>>>>file:%s, line:%d ", __FILE__, __LINE__);\
        printf(format, ##args);\
        printf("\n");\
    }
#define cfmAssertReturn(x, retval, format, args...) \
    if (!(x))\
    {\
        printf("CFM ASSERT>>>>>>>>file:%s, line:%d ", __FILE__, __LINE__);\
        printf(format, ##args);\
        printf("\n");\
        return retval;\
    }
#else
#define cfmAssert(x...)
#define cfmAssertReturn(x...)
#endif

typedef enum tagATP_E_CFGFILE_TYPE
{
    ATP_E_CFGFILE_FULLFILE,
    ATP_E_CFGFILE_PARTFILE,
    ATP_E_CFGFILE_OUTERFILE,
}ATP_E_CFGFILE_TYPE;

//!\brief CFM error code
/*!
 *    CFM Core�Ĵ�����
 */
typedef enum tagATP_E_ERR_CFM
{
    /*CFM����Ĵ�����*/
    ATP_E_ERR_CFM_PARA_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CFM),
    ATP_E_ERR_CFM_NOT_INIT,/*!< û�г�ʼ�� */
    ATP_E_ERR_CFM_OPEN_FILE,
    ATP_E_ERR_CFM_FILE_INVALID,
    ATP_E_ERR_CFM_CREATE_SHM_FAILED,
    ATP_E_ERR_CFM_LOAD_SHM_FAILED,
    ATP_E_ERR_CFM_SHM_NOT_ATTACH,
    ATP_E_ERR_CFM_INTERNAL,
    ATP_E_ERR_CFM_CFGFILE_MATCH,
    ATP_E_ERR_CFM_MEM,
    ATP_E_ERR_CFM_OID_ERR,
    ATP_E_ERR_CFM_KEYLIST_OR_OID,   //keylist����oid��Ч
    ATP_E_ERR_CFM_PARANUM_ZERO,
    ATP_E_ERR_CFM_PARA_ERR_OR_INST_ZERO,  //������Ч����ʵ������Ϊ0
    ATP_E_ERR_CFM_BUILD_CFG_FILE, 
    ATP_E_ERR_CFM_SEM_CREATE,
    ATP_E_ERR_CFM_MSG_CREATE,
    ATP_E_ERR_CFM_REG_INFO,
    ATP_E_ERR_CFM_MSG_TYPE,
    ATP_E_ERR_CFM_MSG_OPCODE,
    ATP_E_ERR_CFM_CFG_TOOL_TYPE,
    ATP_E_ERR_CFM_MSG_ITEM,
    ATP_E_ERR_CFM_MSG_RESP,
    ATP_E_ERR_CFM_MSG_GETRESP,
    ATP_E_ERR_CFM_MAINPROC_DIFF,
    ATP_E_ERR_CFM_LIB_LAOD,
    ATP_E_ERR_CFM_TOO_MANY_INSTS,
    ATP_E_ERR_CFM_INVALID_INST_NUM,
    ATP_E_ERR_CFM_DOMAIN_STRING,
    ATP_E_ERR_CFM_STRING_SHORT,
    ATP_E_ERR_CFM_OBJ_DIFF,
    ATP_E_ERR_CFM_ACCESS_DENY,
    ATP_E_ERR_CFM_NOTIFY_REJECTED,
    ATP_E_ERR_CFM_NOT_FOUND_KEY,

// for tr181 alias parameter
    ATP_E_ERR_CFM_ALIAS_NOT_FOUND,
    ATP_E_ERR_CFM_NOT_ALIAS_MODE,    
    ATP_E_ERR_CFM_OBJ_HAVE_NO_ALIAS_PARA,

    
    /* ��Ϣ��Ӧ�� */
    ATP_E_ERR_CFM_RETMSG_BGN = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CFM) + ATP_CFM_MSG_RETCODE_BASE,
    ATP_E_ERR_CFM_RETMSG_DONE,    
    ATP_E_ERR_CFM_NO_VISIT_DB,
    ATP_E_ERR_CFM_NEED_REBOOT,
    ATP_E_ERR_CFM_NO_VISIT_ANY,  //����Ҫ����FLASH�͹����ڴ�
    ATP_E_ERR_CFM_RETMSG_END,
    
    ATP_E_ERR_CFM_MAX
}ATP_E_ERR_CFM;

//!\brief CFM KeyID define
/*!
 *
 */
enum ATP_CFM_KEYID_EN
{
    ATP_CFM_KEY_EXPORT = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_CFM), /*!< cfg file key */
    ATP_CFM_KEY_FLASH,
    ATP_CFM_KEY_PARAM,
    ATP_CFM_KEY_CFGSIGN,
    ATP_CFM_KEY_DEFAULTCFG,
};


/*********************************���ݽṹ*************************************/

// ���ù�������ֵ�����ATP���������ɹ����е�ֵһ��(ref: ATP V100R003C01 ������.xls)
typedef enum tagATP_CFM_E_IF_TYPE
{
    ATP_CFM_E_IF_TYPE_BGN           = 0,
    ATP_CFM_E_IF_TYPE_CWMP_ADMIN    = 1 << 0,
    ATP_CFM_E_IF_TYPE_CLI_ADMIN     = 1 << 1,
    ATP_CFM_E_IF_TYPE_WEB_USER      = 1 << 2,
    ATP_CFM_E_IF_TYPE_WEB_ADMIN     = 1 << 3,
    ATP_CFM_E_IF_TYPE_TR064_ADMIN   = 1 << 4,
    ATP_CFM_E_IF_TYPE_CLI_USER      = 1 << 5,

    ATP_CFM_E_IF_TYPE_UPNP_DM       = 1 << 6,

    // Add more config tool types
	
	// halp allow access by tr069 and web
    ATP_CFM_E_IF_TYPE_HALP_CFG= 1 << 7,   
    ATP_CFM_E_IF_TYPE_CMS           = 1 << 14,
    ATP_CFM_E_IF_TYPE_OTHER         = 1 << 15,
    ATP_CFM_E_IF_TYPE_END           = 0xFFFF   /* ��಻�ܳ���16�����ù��� */
} ATP_CFM_E_IF_TYPE;

typedef enum tagATP_CFM_E_OPER_TYPE // ������������Ϣ����һ��
{   
    ATP_CFM_E_OPER_TYPE_GET = ATP_E_CFGMSG_GET,
    ATP_CFM_E_OPER_TYPE_CHK = ATP_E_CFGMSG_CHK,        
    ATP_CFM_E_OPER_TYPE_SET = ATP_E_CFGMSG_SET,
    ATP_CFM_E_OPER_TYPE_DEL = ATP_E_CFGMSG_DEL,
    ATP_CFM_E_OPER_TYPE_ADD = ATP_E_CFGMSG_ADD,
    ATP_CFM_E_OPER_TYPE_GETINST = ATP_E_CFGMSG_INSTNUM,
    
    ATP_CFM_E_OPER_TYPE_BUTT,
} ATP_CFM_E_OPER_TYPE;

typedef enum tagATP_CFM_VALUETYPE_EN
{
    ATP_CFM_VALUETYPE_UNKNOWN = 0,
    ATP_CFM_VALUETYPE_STRING,
    ATP_CFM_VALUETYPE_NUMBER,
    
    ATP_CFM_VALUETYPE_EXTEND
} ATP_CFM_VALUETYPE_EN;


typedef struct tagATP_CFM_TOOL_NAME_ST
{
    VOS_UINT32          ulCfmLevel;
    VOS_CHAR           *ulCfmToolName;
} ATP_CFM_TOOL_NAME_ST;


/**
 *  CFM main proc app
 */
typedef struct tagATP_CFM_MAINPROC_MAP
{
    const VOS_CHAR *pcMainProc;
    //VOS_UINT32 ulIndex;
    VOS_UINT32 ulIfType;
}ATP_CFM_MAINPROC_MAP;

typedef VOS_UINT32 (*AtpCfmExportProc)(VOS_UINT8 **ppucBuffer, VOS_UINT32 *pulLen);
typedef VOS_UINT32 (*AtpCfmImportProc)(VOS_UINT8 *pucBuffer, VOS_UINT32 ulLen);

typedef struct tagATP_CFM_Export_ST
{
    const VOS_CHAR              *pcName;
    AtpCfmExportProc            pfExport;
    AtpCfmImportProc            pfImport;
}ATP_CFM_Export_ST;

typedef VOS_UINT32 (*AtpCfmExportHook)(ATP_UTIL_STRFILE_ST *pstStrFile);

extern AtpCfmExportHook g_pfCfmExportHook;

//!\brief CFMȫ�������ļ�
/*!
 * ʹ�����ù�����̵�cfmȫ�ֽṹ���ýṹ��ֵ��ATP_CFM_ExtConfig��ʼ���������п��޸ĸ�ֵ��
 * �ṹ��Աֵ�޸Ĳ�Ӱ���������̡�
 */
typedef struct tagATP_CFM_GLOBAL_CONFIG
{
    const VOS_CHAR              *pszMapFile;
    const VOS_CHAR              *pszDBSaveFile;
    const VOS_CHAR              **apcMainProcList;

    VOS_UINT16                  usCfgToolType;
    VOS_UINT16                  usExtendNameCfgTool;
    VOS_UINT32                  ulShareMemSize;  //!!!һ���������Ϊ128k
    //todo: cache path
} ATP_CFM_GLOBAL_CONFIG;

//!  CFM NumberOfEntries utility
/*!
 * CFM�п����Զ�����TR098����ģ����NumberOfEntries�Ĵ���
*/
typedef struct tagATP_CFM_ENTRYNUMBER_ST
{
    VOS_UINT32 ulOID;    /*!< ulOID para value = ulAddOID1 obj number + ulAddOID2 obj number */
    VOS_UINT32 ulAddOID1;/*!< test */
    VOS_UINT32 ulAddOID2;/*!< 0 for ignore, not used now */
}ATP_CFM_ENTRYNUMBER_ST;


#ifdef SUPPORT_ATP_CFM_VOICE_NODE_MAP
typedef struct tagATP_CFM_VOICE_PARA_MAP_ST
{
    VOS_UINT32  ulFromOID;
    /* node InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.X_OutgoSuffixInfo.{i}.
     * mapped to X_CommonConfig.OutgoSuffixInfo.{i}. , the last 1 {i} need copy, so ulInstLevel = 1;
     */
    VOS_BOOL    bCopyLastInstNum;
    VOS_UINT32  ulToOID;
    
}ATP_CFM_VOICE_PARA_MAP_ST;
#endif



/********************************����ת���ӿں���************************************/
const VOS_VOID *ATP_UTIL_Str2TypedValue(const VOS_CHAR *pszStrVal, VOS_UINT32 ulOID);

const VOS_CHAR *ATP_UTIL_TypedValue2Str(const VOS_VOID*     pvValue, 
                                        VOS_UINT32          ulOID, 
                                        const VOS_CHAR      acDigitBuf[ATP_DIGIT_STR_MAX_LEN_D],
                                        VOS_UINT32          ulDigitLen);


/*********************************ȫ�ֱ���*************************************/
extern ATP_THREAD_SAFE VOS_UINT32 g_ulCfmErr; //����Ψһ�Ĵ������¼

extern const VOS_UINT32 *g_aulRecycleOids;

extern VOS_BOOL g_bRestoreFactory; // �Ƿ�ָ��������ã����ָ�������Բ���������

/********************************������ӿ�************************************/
/******************************************************************************
  ��������  : ATP_CFM_GetVersion
  ��������  : ��ȡAPI����汾
  �������  : 
              1.  pszVer: �汾��Ϣ�ַ���
              2.  ulVerLen: �汾��Ϣ�ַ�������
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : VOS_OK/Err Code

  �޸���ʷ      :
   1.��    ��   : 2008-11-26
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
const VOS_CHAR * ATP_CFM_GetVersion(void);

/********************************CFM������ӿ�*********************************/
/******************************************************************************
  ��������  : ATP_DBMain
  ��������  : CFM��ʼ�����(������)
  �������  : 
              
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-26
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
#if 0
VOS_UINT32 ATP_DBMain();
#endif

/*!
  \brief �������ݿ�ĳ�ʼ��
  \param[in]  enIfType: enIfType ���ù�������
  \param[in]  bReInit:    �Ƿ���Ҫ���³�ʼ�������ڴ棬���ǣ���VOS_TRUE
  \return ����ATP_E_ERR_CFM���͵Ĵ�����
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_DBInit(ATP_CFM_E_IF_TYPE enIfType, VOS_BOOL bReInit);

/*!
  \brief   CFM�����ڴ�ж��(ȥ��ʼ��)
  \return ����ATP_E_ERR_CFM���͵Ĵ�����
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_DBDestroy(VOS_VOID);

/******************************************************************************
  ��������  : ATP_DBSave
  ��������  : ���ݿⱣ��flash
  �������  : 
              ��
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-26
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBSave(VOS_VOID);

/*!
  \brief �ѹ����ڴ��е����ݿ�д��Flash��
  \param[in]  pcFileToFlush:    �����ڴ����ݿ����л�����ʱ��xml�ļ�·��
  \return ATP_E_ERR_CFM���͵Ĵ�����
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_DBFlush(ATP_UTIL_STRFILE_ST *pstStrFile);

VOS_UINT32 ATP_DBCheckFile(ATP_UTIL_STRFILE_ST *pstStrFile);

#define ATP_CFM_INITKEYLIST {0, 0, 0, 0, 0, 0}

/********************************************************************************
 * ��������  : ATP_DBExportPart
 * ��������  : �������������ļ�
 * �������  : 
 *             1.  ulKeyList[ATP_CFM_MAX_TREE_DEPTH]:
 *             2.  ulOID:
 *             3.  pszExpFile:
 * ���ú���  : 
 * ��������  : 
 * �������  : ��
 * �� �� ֵ  : ��
 *
 * �޸���ʷ      :
 *  1.��    ��   : 2008-12-1
 *    ��    ��   : p42047
 *    �޸�����   : ��ɳ���
******************************************************************************/
VOS_UINT32 ATP_DBExportCfgOfObj(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                   VOS_UINT32        ulOID, 
                                   ATP_UTIL_STRFILE_ST *pstStrFile);

/******************************************************************************
  ��������  : ATP_DBExport
  ��������  : �������������ļ�
  �������  : 
              1. path:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-12-1
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBExport(ATP_UTIL_STRFILE_ST *pstStrFile);

/******************************************************************************
  ��������  : ATP_DBSetCurrUser
  ��������  : �������õ�ǰʹ��CFM API������ù��ߣ��ýӿڿ���һ�������ж�ε��á�
  �������  : 
              1.  enIfType:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-28
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBSetCurrUser(ATP_CFM_E_IF_TYPE enIfType);

/******************************************************************************
  ��������  : ATP_DBGetCurrUser
  ��������  : ��ȡ��ǰʹ��CFM API������ù�������
  �������  : 
              
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-12-1
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBGetCurrUser(VOS_VOID);

/********************************DB������ӿ�*********************************/
VOS_UINT32 ATP_DBGetObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulOID,
                        VOS_UINT32 **ppulObj,
                        VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBGetObjByName(const VOS_CHAR *pcObjName,
                        VOS_UINT32 **ppulObj,
                        VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBSetObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                               VOS_UINT32       ulOID,
                               const VOS_UINT32 *pulObj);

VOS_UINT32 ATP_DBSetObjByName(const VOS_CHAR *pcObjName,
                               const VOS_UINT32 *pulObj);

/******************************************************************************
  ��������  : ATP_DBAddObj
  ��������  : ��Ӷ���
  �������  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  ulOID:
              4.  hParaList:
              5.  ulPreSetFlag:
              6.  pulInstanceId:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���
//�Ƿ�ȷʵ��ҪԤ���
******************************************************************************/
VOS_UINT32 ATP_DBAddObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulOID,
                        const VOS_UINT32 *pulObjValues,
                        VOS_UINT32 *pulInstanceId);

VOS_UINT32 ATP_DBAddObjByName(const VOS_CHAR *pcObjName,
                        const VOS_UINT32 *pulObjValues,
                        VOS_UINT32 *pulInstanceId);

/******************************************************************************
  ��������  : ATP_DBDelObj
  ��������  : 
  �������  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  ulOID:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBDelObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                               VOS_UINT32 ulOID);

VOS_UINT32 ATP_DBDelObjByName(const VOS_CHAR *pcObjName);

// ��ȡ����ʵ����
VOS_UINT32 ATP_DBGetNumOfObjs(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulOID, VOS_UINT32 *pulNum,
                              VOS_UINT32 **ppulInstArray);

VOS_UINT32 ATP_DBGetNumOfObjsByName(const VOS_CHAR *pcObjName, VOS_UINT32 *pulNum,
                              VOS_UINT32 **ppulInstArray);

VOS_UINT32 ATP_DBGetObjectBySeqNum(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulOID,
                                VOS_UINT32 ulSeqNum,
                                VOS_UINT32 **ppulObj,
                                VOS_UINT32 *puInstNo,
                                VOS_UINT32 *pulNumOfParas,
                                VOS_UINT32 ulNewKeyList[ATP_CFM_MAX_TREE_DEPTH]);

/******************************************************************************
  ��������  : ATP_DBGetPara
  ��������  : 
  �������  : 
              1.  ulKeyList:
              2.  ulOID:
              3.  *pValue:
              4.  ulValueLen:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBGetPara(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulOID,
                                VOS_VOID *pValue,
                                VOS_UINT32 ulValueLen);

/******************************************************************************
  ��������  : ATP_DBGetPara
  ��������  : 
  �������  : 
              1.  ulKeyList:
              2.  ulOID:
              3.  *pValue:
              4.  ulValueLen:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBGetParaByName(const VOS_CHAR *pcParaName, 
                                VOS_VOID *pValue,
                                VOS_UINT32 ulValueLen);

/******************************************************************************
  ��������  : ATP_DBSetPara
  ��������  : 
  �������  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  hParaList:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBSetPara(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                VOS_UINT32 ulParaCMO,
                                const VOS_VOID *pvData);

VOS_UINT32 ATP_DBSetParaByName(const VOS_CHAR *pcPara, const VOS_VOID *pvValue);

VOS_UINT32 ATP_DBDomainToKeyListIgnorePrefix(const VOS_CHAR* pszDomain, 
                                 VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32* pulOID);

VOS_UINT32 ATP_DBDomainToKeyList(const VOS_CHAR* pszDomain, 
                                        VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32* pulOID);

VOS_UINT32 ATP_DBDomainToKeyListEx(const VOS_CHAR* pszDomain, 
                                 VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 *pulOID, VOS_UINT32 *pulKeyListDepth);

#ifdef  SUPPORT_ATP_TR181

typedef    struct tagCFM_ALIAS_PARA_CMO_VALUE_ST
{
    struct tagCFM_ALIAS_PARA_CMO_VALUE_ST        *pstNext;
    VOS_UINT32        ulOID;
    VOS_UINT32        ulLeftSquareBracketsPostion;
    VOS_UINT32        ulRightSquareBracketsPostion;    
    VOS_CHAR        *pcOldValue;
    VOS_CHAR        *pcNewValue;
}CFM_ALIAS_PARA_CMO_VALUE_ST;

typedef struct tagCFM_LIST_ST
{
    struct tagCFM_LIST_ST        *pstNext;

    VOS_UINT32                        ulOID;
    VOS_UINT32                        ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
} CFM_LIST_ST;

typedef struct tagCFM_AUTO_CREATE_INSTANCE_ST
{
    struct tagCFM_AUTO_CREATE_INSTANCE_ST    *pstNext;    
    CFM_LIST_ST    *pstAutoCreatedInst;
}CFM_AUTO_CREATE_INSTANCE_ST;

typedef struct tagCFM_STRONG_REF_PARA_NAME_ST
{
    struct tagCFM_STRONG_REF_PARA_NAME_ST        *pstNext;
    VOS_UINT32    ulOID;
    VOS_CHAR    *pcParaDomainName;

}CFM_STRONG_REF_PARA_NAME_ST;

typedef struct tagDBITERATE_PARA_ST
{
    CFM_STRONG_REF_PARA_NAME_ST        *pstParaList;
} DBITERATE_PARA_ST;


/* ref parameter
*        values: A.B.[cpe-1].,A.C.[cpe-2].D.[cpe-1].
*                            |---> DomainName          |--->DomainName
*                  |--->OID,OldValue          |---->OID,OldValue
*                                                                   |---->OID,OldValue
*/

typedef struct tagCFM_DOMAIN_PARSE_ST
{
    struct tagCFM_DOMAIN_PARSE_ST    *pstNext;
    VOS_CHAR                        *pcDomainName;
    CFM_ALIAS_PARA_CMO_VALUE_ST            *pstAliasParaOV;    
}CFM_DOMAIN_PARSE_ST;


typedef struct tagCFM_REF_PARA_NAME_VALUE_ST
{
    struct tagCFM_REF_PARA_NAME_VALUE_ST *pstNext;
    VOS_CHAR        *pcParaDomainName;
    VOS_CHAR        *pcParaInstName;
    VOS_CHAR        *pcOldParaValues;// A.B.[cpe-1].,A.C.[wan].     length 1024
    CFM_DOMAIN_PARSE_ST            *pstDomainParse;                
}CFM_REF_PARA_NAME_VALUE_ST;

typedef struct tagCFM_REF_PARA_NAME_ST
{
    struct tagCFM_REF_PARA_NAME_ST  *pstNext;
    VOS_CHAR        *pcParaName;        // form ACS
    VOS_CHAR        *pcParaInstName;    // convert to Instance Number Mode
}CFM_REF_PARA_NAME_ST;

// ����Device.ManagementServer.InstanceMode ȡֵ�ĺ궨�� add by l00163811
#define CFM_INSTANCEMODE_NUM_D                "InstanceNumber"
#define CFM_INSTANCEMODE_ALIAS_D              "InstanceAlias"

#define CWMP_MS_ALIASBASEDADDRESSING_D      "ManagementServer.AliasBasedAddressing"

VOS_UINT32 ATP_DBAliasToInstanceDomain(const VOS_CHAR* pcAliasDomainName, 
										VOS_CHAR *pcInstDomainName, 
										VOS_UINT32 ulLen);
VOS_UINT32 ATP_DBGetAliasParaValueFromPath(const VOS_CHAR *pcAliasDomainName,VOS_CHAR *pcParaAliasValue);    
VOS_BOOL   ATP_AliasIsAvailable(const VOS_CHAR *pszAliasName);
VOS_BOOL   ATP_DBGetInstIdByAliasName(const VOS_CHAR *pszDomainName, const VOS_CHAR *pszAliasName, VOS_UINT32 *pulInstNum);
VOS_UINT32 ATP_DBSetAliasParaByDomainName(const VOS_CHAR *pszDomainName, VOS_UINT32 ulInstID, const VOS_CHAR *pcAliasValue);

VOS_UINT32 ATP_DBSetAliasInAddObj(const VOS_CHAR *pszDomainName, VOS_UINT32 *pulKeyList, VOS_UINT32 ulOID, VOS_UINT32 ulInstID, VOS_UINT32 ulInstDepth);

VOS_UINT32 ATP_DBCreateInstanceIfNotExist(const VOS_CHAR *pszDomainName, CFM_AUTO_CREATE_INSTANCE_ST **pstCreatedInstList);
VOS_UINT32 ATP_DBDelObjAutoAdded(CFM_AUTO_CREATE_INSTANCE_ST *pstAutoCreatedList);
VOS_UINT32 ATP_DBReleaseAutoCreatedList(CFM_AUTO_CREATE_INSTANCE_ST *pstAutoCreatedList);

VOS_UINT32 ATP_DB_ALIAS_IterateStrongRefPara(DBITERATE_PARA_ST     *pstArg);
VOS_UINT32 ATP_DB_ALIAS_ParaAppendList(const VOS_UINT32 *pulKeyList, VOS_UINT32 ulOID, const VOS_CHAR *pcAliasNew);
VOS_UINT32 ATP_DB_ALIAS_FreeAliasParaList();
VOS_UINT32 ATP_DB_ALIAS_CmpAndUpdateRefList(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_ParseParaValue(VOS_CHAR *pcParaValue, CFM_DOMAIN_PARSE_ST **ppstList);
VOS_UINT32 ATP_DB_ALIAS_RefNameListToRefSTList(CFM_STRONG_REF_PARA_NAME_ST *pstRefParaName, CFM_REF_PARA_NAME_VALUE_ST **pstList);
VOS_UINT32 ATP_DB_ALIAS_ShowCVList();
VOS_UINT32 ATP_DB_ALIAS_ShowRefPVList(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_GetNewRefParaValuesAndSave(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_GetStrongRefParaList();
VOS_UINT32 ATP_DB_ALIAS_SetRefParamValue();
VOS_UINT32 ATP_DBGetDomainNameByAliasMode(const VOS_CHAR *pcInstDomainName, VOS_CHAR *pcAliasDomainName);
VOS_BOOL   ATP_DB_ALIAS_IsAliasBasedAddressing();
VOS_BOOL   ATP_DB_ALIAS_IsAliasMode();
VOS_UINT32 ATP_DB_ALIAS_AliasParaInvalidOrExist(const VOS_CHAR *pcDomainName, const VOS_CHAR *pcValue);
VOS_VOID ATP_DB_ReleaseAutoCreateKO(ATP_UTIL_LIST_ST *pstItem);
VOS_UINT32 ATP_DB_ALIAS_GetDomainNameByAliasMode(const VOS_CHAR *pcDomainNameFromACS, 
                                                 const VOS_CHAR *pcDomainNameInst,
                                                 VOS_CHAR *pcDomainNameDest);
VOS_UINT32 ATP_DB_ALIAS_GetRefValuesByAliasMode(const VOS_CHAR *pcDomainName,
                                                VOS_UINT32 ulOID,
                                                const VOS_CHAR *pcOldValues,
                                                VOS_CHAR *pcNewValues);

VOS_VOID ATP_DB_ALIAS_SetRpcAppendRefList(const VOS_CHAR *pcParaName);
VOS_VOID ATP_DB_ALIAS_SetFlagForACSSet();
VOS_VOID ATP_DB_ALIAS_FreeRefListElement(ATP_UTIL_LIST_ST *pstItem);

#endif

VOS_UINT32 ATP_DBKeyListToDomain(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulOID,
                                        VOS_CHAR* pszDomain,
                                        VOS_UINT32 ulDomainLen);

VOS_UINT32 ATP_DBKeyListToDomainEx(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 ulOID,
                                 VOS_CHAR* pszDomain,
                                 VOS_UINT32 ulDomainLen,
                                 VOS_BOOL  bDecreaseByOne);

VOS_UINT32 ATP_DBGetInstDepthOfOID(VOS_UINT32 ulOID);

VOS_UINT32 ATP_DBObjMove(
                    const VOS_UINT32    ulSourceKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                    VOS_UINT32          ulSourceObj,
                    const VOS_UINT32    ulDestKeyList[ATP_CFM_MAX_TREE_DEPTH],
                    VOS_UINT32          *pulNewObjInstId);

VOS_UINT32 ATP_DBGetParaOIDByParaName(VOS_UINT32 ulObjOID, 
                                             const VOS_CHAR* pszParaName);


typedef struct tagATP_PARA_ATTR_VAL_ST
{
    VOS_UINT16            usAccessBits;    /* �����б�*/
    VOS_UINT8            ucNotifyType;    /* �����ı�֪ͨ */
    VOS_UINT8            ucReserve;        /* ���� */
} ATP_PARA_ATTR_VAL_ST;

VOS_UINT32 ATP_CFM_ExtGetAccListItem(const VOS_CHAR *pcAccList);

VOS_UINT32 ATP_CFM_ExtGetAccListStrings(VOS_UINT32 ulAccList, const VOS_CHAR *apcNames[], VOS_UINT32 ulBufLen);
#ifdef SUPPORT_ATP_DT_CONFIGNAME
VOS_UINT32 ATP_CFM_GetExportFileName(VOS_CHAR *pcBuf, VOS_UINT32 ulBufLen);
#endif

/******************************************************************************
  ��������  : ATP_CFM_SetParaProfile
  ��������  : 
  �������  : 
              1.  ulKeyList:
              2.  hParaList:
              3.  pstParaProfile:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
typedef enum tagATP_CFM_ATTR_CHG_TYPE
{
    ATP_CFM_ATTR_CHG_ACC_LIST = 1,
    ATP_CFM_ATTR_CHG_NOTIFY = 2,
    ATP_CFM_ATTR_CHG_RESERVE = 4
} ATP_CFM_ATTR_CHG_TYPE;
VOS_UINT32 ATP_CFM_SetAttrOfCMO(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                       VOS_UINT32                ulOID,
                                       ATP_CFM_ATTR_CHG_TYPE    enChgType,
                                       const ATP_PARA_ATTR_VAL_ST    *pstParaAttr);

typedef struct tagATP_CFM_ATTR_SET_ST
{
    struct tagATP_CFM_ATTR_SET_ST *     pstNext;
    VOS_UINT32              ulOID;
    VOS_UINT32              ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    ATP_CFM_ATTR_CHG_TYPE   enChgType;
    ATP_PARA_ATTR_VAL_ST    stAttr;
} ATP_CFM_ATTR_SET_ST;

VOS_UINT32 ATP_CFM_SetAttrOfCMOEx(ATP_CFM_ATTR_SET_ST *pstSetAttrList);


/******************************************************************************
  ��������  : ATP_CFM_GetParaProfile
  ��������  : 
  �������  : 
              1.  hKeyList:
              2.  hParaList:
              3.  pstParaProfile:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFM_GetAttrOfCMO(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                      VOS_UINT32    ulOID,
                                      ATP_PARA_ATTR_VAL_ST *pstParaAttr);

VOS_UINT32 ATP_CFM_SetHideBitsOfCMO(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                                    VOS_UINT32                ulOID,
                                                    VOS_UINT32               ulHideBits);

/*************/

/*****************************������Ϣ�����ӿ�*********************************/

//ppulSetValues[idx_of_cmo()]

/*
 *    CMS����Ϣ�ӿ�
 */
VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsg(const VOS_VOID *pvMsg, VOS_BOOL bAdd, VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulOldValues,
                                       VOS_UINT32 **ppulSetValues, VOS_UINT32 **ppulChgParas,
                                       VOS_UINT32 *pulNumOfParas, VOS_UINT32 *pulNextObjId);

VOS_UINT32 ATP_CFM_GetObjectFromOtherMsg(const VOS_VOID *pvMsg, VOS_UINT32 aulInstList[ATP_CFM_MAX_TREE_DEPTH],
                                         VOS_UINT32 *pulObjID);

VOS_UINT32 ATP_CFM_GetObjectFromGetMsg(const VOS_VOID *pvMsg, VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulGetValues,
                                       VOS_UINT32 **ppbFocusParas,
                                       VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_CFM_GetObjectFromDelMsg(const VOS_VOID *pvMsg, VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 *pulNextObjId);

VOS_UINT32 ATP_CFM_ResponseInstArrayMsg(const VOS_VOID *pvMsg, 
                                    VOS_UINT32 ulMsgType,
                                    const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                    VOS_UINT32 ulOID, 
                                    const VOS_UINT32 *pulInstArray, 
                                    VOS_UINT32 ulInstNum);

VOS_UINT32 ATP_CFM_ResponseMsgRetCode(VOS_VOID *pvMsg, VOS_UINT32 ulErrOID, VOS_UINT32 ulErrCode);

VOS_UINT32 ATP_CFM_ResponseGetMsg(VOS_VOID *pvMsg, const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                  VOS_UINT32 ulOID, const VOS_UINT32 *pulFocusParas, const VOS_UINT32 *pulGetValues);

VOS_UINT32 ATP_CFM_ResponseSetMsg(VOS_VOID *pvMsg, VOS_VOID *pvRespMsg, VOS_UINT32 ulRespCode);

VOS_UINT32 ATP_CFM_InitResponseMsg(const ATP_MSG_HEADER_ST *pstMsg, VOS_UINT32 ulErrCode, VOS_BOOL *pbSetMsg);

VOS_UINT32 ATP_CFM_AppendErrorCode(const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID,
                                                                      VOS_UINT32 ulErrCode, const VOS_CHAR *pcErrStr);
VOS_UINT32 ATP_CFM_ErrorCode(VOS_UINT32 ulDefaultErrorCode);
VOS_UINT32 ATP_CFM_GetErrorCodeString(VOS_VOID *pstErrortMsg,  VOS_CHAR * pcErrorString, VOS_UINT32 ulStringLen);

VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsgForAll(const VOS_VOID *pvMsg, VOS_BOOL bSet);

VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsgEx(VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulOldValues,
                                       VOS_UINT32 **ppulSetValues, VOS_UINT32 **ppulChgParas,
                                       VOS_UINT32 *pulNumOfParas, VOS_UINT32 *pulNextObjId, VOS_BOOL bCheck);

VOS_VOID ATP_CFM_FreeObjList(VOS_VOID);

VOS_VOID ATP_CFM_ChangeInstIdByNow(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH]);

typedef VOS_BOOL (*CfmObjCompareProc)(VOS_UINT32 ulDBObjKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                                                        const VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulDBValues, VOS_VOID *pvHook);

VOS_BOOL ATP_CFM_IsObjectExist(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulObjID,
                const VOS_UINT32 *pulSetValues, const VOS_UINT32 ulInstNum, CfmObjCompareProc pfProc, VOS_VOID *pvHook);

/*
 *    ���ù��߲���Ϣ�ӿ�
 */
VOS_UINT32 ATP_CFM_CfgMsgChgType(VOS_VOID * pvMsg, VOS_UINT32 ulMsgType);

VOS_UINT32 ATP_CFM_CfgMsgHeadBuilder(VOS_VOID **ppvMsg,
                         VOS_UINT32 ulMsgType,
                         const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32 ulOID);
VOS_UINT32 ATP_CFM_CfgMsgAppendPara(VOS_VOID **ppvMsg,
                         const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32         ulOID,
                         const VOS_VOID     *pvValue);

VOS_UINT32 ATP_CFM_CfgMsgAppendParaEx(VOS_VOID **ppvMsg,
                         const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32         ulOID,
                         const VOS_VOID     *pvValue,
                         VOS_UINT32        ulValueType);

ATP_CFM_E_IF_TYPE ATP_CFM_GetCfgToolOfMsg(const VOS_VOID* pvMsg);

VOS_UINT32 ATP_CFM_GetObjNumOfMsg(const VOS_VOID* pvMsg, VOS_UINT32* pulObjNum);
VOS_UINT32 ATP_CFM_GetParaNumOfMsg(const VOS_VOID* pvMsg,
                                   VOS_UINT32 ulObjIndex,
                                   VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                   VOS_UINT32* pulParaNum);

VOS_UINT32 ATP_CFM_GetParaOfMsgBySeqNum(const VOS_VOID *pvMsg,
                                        VOS_UINT32 ulObjIndex, // ��0��ʼ
                                        VOS_UINT32 ulSeqNum, // ��0��ʼ
                                        VOS_UINT32 *pulOID,
                                        VOS_VOID **ppValue);

VOS_UINT32 ATP_CFM_GetParaOfMsgByOID(const VOS_VOID *pvMsg,
                                     VOS_UINT32 ulObjIndex, // ��0��ʼ
                                     VOS_UINT32 ulOID,
                                     VOS_VOID ** ppValue);
VOS_UINT32 ATP_CFM_GetParaOfMsgByOIDEx(const VOS_VOID *pvMsg,
                                     VOS_UINT32 ulObjIndex, // ��0��ʼ
                                     VOS_UINT32 ulOID,
                                     VOS_VOID ** ppValue,
                                     VOS_UINT32 ulValueType);

VOS_UINT32 ATP_CFM_MsgFree(VOS_VOID* pvCfmMsgHead);

// this api just for debug

#ifdef ATP_DEBUG
VOS_VOID ATP_CFM_PrintMsgEx(const VOS_VOID* pvCfmMsg, const VOS_CHAR* pszFile, VOS_UINT32 ulLine);
#define ATP_CFM_PrintMsg(pvCfmMsg) ATP_CFM_PrintMsgEx((pvCfmMsg), __FILE__, __LINE__)
#else
#define ATP_CFM_PrintMsg(x...)
#endif

/*****************************������Ϣ�����ӿ�*********************************/
/*
 *    OBJ Interfaces
 */
VOS_BOOL ATP_CFM_CompareObj(VOS_UINT32 ulOID,
                            const VOS_UINT32 *pulSrcObj,
                            const VOS_UINT32 *pulDstObj);

VOS_VOID ATP_CFM_FreeObj(VOS_UINT32 ulOID,
                         VOS_UINT32 *pulObj);

VOS_VOID ATP_CFM_FreeObjByName(const VOS_CHAR *pcObjName,
                         VOS_UINT32 *pulObj);

VOS_UINT32 ATP_CFM_CopyObjEx(VOS_UINT32 ulOID, VOS_UINT32 *pulDstObj,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 *ATP_CFM_CopyObj(VOS_UINT32 ulOID,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 *ATP_CFM_CopyObjByName(const VOS_CHAR *pcObjName,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 ATP_CFM_SetObjStrValue(VOS_UINT32 ulParaIndex,
                                   VOS_UINT32 *pulObj,
                                   const VOS_CHAR *pcNewValue);

VOS_UINT32 ATP_CFM_SetObjUintLongValue(VOS_UINT32 ulParaIndex,
                                   VOS_UINT32 *pulObj,
                                   const VOS_UINT64 ullNewValue);

VOS_BOOL ATP_CFM_IsKeyListValid(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulCMO);

VOS_BOOL ATP_CFM_IsKeyListSame(const VOS_UINT32 ulKeyList1[ATP_CFM_MAX_TREE_DEPTH],
                                        const VOS_UINT32 ulKeyList2[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulCMO);

VOS_BOOL ATP_CFM_IsAttrNeedNotify(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                      VOS_UINT32    ulOID);

VOS_UINT32 ATP_CFM_GetObjParaCmo(VOS_UINT32 ulCMO, VOS_UINT32 ulIdx, VOS_UINT32 *pulParaCmo);


#ifdef ATP_DEBUG
VOS_VOID ATP_CFM_PrintObj(VOS_UINT32 ulOID,
                          const VOS_UINT32 *pulObjValues,
                          VOS_UINT32* pulNumOfParas);
#else
#define ATP_CFM_PrintObj(x...)
#endif


/*****************************������Ϣ�����ӿ�*********************************/

/******************************************************************************
  ��������  : ATP_CFMA_GetRetInfo
  ��������  : 
  �������  : 
              1.  hCfgListHandle:
              2.  penretCode:
              3.  pulErrOID:
              4.  pszRetInfo:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
#if 0
VOS_UINT32 ATP_CFMA_GetRetInfo(ATP_CFM_E_MSG_RETCODE        *penRetCode,
                                      VOS_UINT32                *pulErrOID);
#endif

/* �����ÿ��CFMA�ӿڵĳ�ʱʱ�䶼����Ctx�У�ֻ��Ҫ��ʼ��һ�Σ�����ÿ�ζ����ò��� */

/******************************************************************************
  ��������  : ATP_CFMA_AddObj
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  *pulInstanceID:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_AddObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                  VOS_UINT32        ulOID,
                                  const VOS_UINT32  *pulObjValues,
                                  VOS_UINT32        *pulInstanceID,
                                  VOS_VOID          **ppvResMsg);

VOS_UINT32 ATP_CFMA_AddObjByMsg(const VOS_VOID* pvCfmMsg, VOS_VOID **ppvResMsg, VOS_UINT32 * pulInstanceID);

#define IS_CFMA_ADD_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NO_VISIT_ANY == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

#if 0
VOS_UINT32 ATP_CFMA_AddObjByName(const VOS_CHAR *pcObjName, VOS_UINT32 *pulInstanceID);
#endif

/******************************************************************************
  ��������  : ATP_CFMA_DelObj
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_DelObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID);

VOS_UINT32 ATP_CFMA_DelObjByName(const VOS_CHAR *pcDelObj);

VOS_UINT32 ATP_CFMA_DelObjByMsg(const VOS_VOID* pvCfmMsg);

#define IS_CFMA_DEL_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NO_VISIT_ANY == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

/******************************************************************************
  ��������  : ATP_CFMA_GetPara
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_GetPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

VOS_UINT32 ATP_CFMA_GetObj(const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32   ulOID,
                                 VOS_UINT32   **ppulObj,
                                 VOS_UINT32   *pulNumOfParas,
                                 VOS_UINT32   ulTimeout);

#define IS_CFMA_GET_OK(x)   (VOS_OK == (x))

/******************************************************************************
  ��������  : ATP_CFMA_CheckPara
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_CheckPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

/******************************************************************************
  ��������  : ATP_CFMA_CheckParaAcc
  ��������  : 
  �������  : 
              1.  pvSrcMsg:
              2.  ulTimeout:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2012-09-18
     ��    ��   : l00163811
     �޸�����   : ��ɳ���

******************************************************************************/

VOS_UINT32 ATP_CFMA_CheckParaAcc(const VOS_VOID *pvSrcMsg, VOS_BOOL bReadAccess, VOS_BOOL bCheckPara, VOS_UINT32 ulTimeout);

/******************************************************************************
  ��������  : ATP_CFMA_SetPara
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_SetPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

#define IS_CFMA_SET_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

/******************************************************************************
  ��������  : ATP_CFMA_GetObjInstNum
  ��������  : 
  �������  : 
              1.  CfgListHandle:
              2.  *pulInstNum:
              3.  hInsSeqNumList:
              4.  ulWaitTime:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-27
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_GetObjInstNum(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                         VOS_UINT32 ulOID, 
                                         VOS_UINT32** ppulInstArray,  //�ò������ⲿʹ��free�ӿ�ֱ���ͷ�
                                         VOS_UINT32* pulInstNum);

/******************************************************************************
  ��������  : ATP_CFMA_SetLockFlag
  ��������  : ��CFMA��������������
  �������  : 
              1.  usLockMask: ����λ����
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-11-29
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFMA_SetLockFlag(VOS_BOOL bLock, VOS_BOOL bRead);

/*****************************************************************************/
VOS_UINT32 ATP_DBGetDftObject(VOS_UINT32 ulOID, 
                              VOS_UINT32 **ppulDftValues, 
                              VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBGetDftObjectByName(const VOS_CHAR *pcObjName,
                              VOS_UINT32 **ppulDftValues, 
                              VOS_UINT32 *pulNumOfParas);

/******************************************************************************
  ��������  : ATP_DBSetDftObject
  ��������  : Set Obj default value and save db (if bSaveDB == VOS_TRUE)
  �������  : 
              1.  ulKeyList[ATP_CFM_MAX_TREE_DEPTH]:
              2.  ulOID:
              3.  bSaveDB:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2009-2-18
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_DBSetDftObject(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                              VOS_UINT32 ulOID, 
                              VOS_BOOL bSaveDB);

const VOS_CHAR *ATP_DBGetNameOfOID(VOS_UINT32 ulOID);
#if 0
VOS_UINT32 ATP_CFM_AppendInstNumToKeyList(VOS_UINT32 aulInstList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulObjId, VOS_UINT32 ulInstId);
#endif
typedef enum tagATP_CFM_INSTALLOC_TYPE_EN
{
    ATP_CFM_INSTALLOC_NOCYCLE,
    ATP_CFM_INSTALLOC_RECYCLE,
} ATP_CFM_INSTALLOC_TYPE_EN;

VOS_UINT32 ATP_DBPreGetObjInstID(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID,
                                 VOS_UINT32 *pulInstId);

typedef VOS_UINT32 (* PFCfmDBIterator)(
                            VOS_UINT32                  ulOID,
                            const VOS_CHAR              *pcDomainName,
                            const VOS_VOID              *pvValue,
                            const ATP_PARA_ATTR_VAL_ST  *pstAttr,
                            VOS_VOID                    *pvArg);
typedef enum tagATP_DB_ITERATE_TYPE_EN
{
    ATP_DB_ITERATE_VALUE    =   1,
    ATP_DB_ITERATE_ATTR     =   2,
} ATP_DB_ITERATE_TYPE_EN;

VOS_UINT32 ATP_CFMA_IterateEx(const VOS_CHAR *pcDomainName,
                            VOS_VOID *pvArg, PFCfmDBIterator pfIterator, VOS_UINT32 ulSearchDepth,
                            ATP_DB_ITERATE_TYPE_EN  enIterType);

VOS_UINT32 ATP_CFMA_Iterate(const VOS_CHAR *pcDomainName,
                            VOS_VOID *pvArg, PFCfmDBIterator pfIterator, VOS_BOOL bNextLevel,
                            ATP_DB_ITERATE_TYPE_EN  enIterType);

VOS_UINT16 ATP_CFMA_GetWrb(
	VOS_UINT32                  ulOID);



typedef VOS_UINT32 (* PFCfmTTreeIterator)(
                            VOS_UINT32                  ulOID,
                            VOS_VOID                    *pvArg);

VOS_UINT32 ATP_TTree_Iterate(VOS_UINT32 ulOID,
                            VOS_VOID *pvArg, PFCfmTTreeIterator pfIterator, VOS_UINT32 ulSearchDepth);

VOS_UINT32 ATP_TTree_BuildName(VOS_UINT32 ulOID, const VOS_CHAR *pcSeperator, VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen);
VOS_UINT32 ATP_TTree_ParseName(const VOS_CHAR *pcName, const VOS_CHAR *pcSeperator, VOS_UINT32 *pulOID);

#define ATP_TTREE_BBF_SEPERATOR_D       "{i}"
#define ATP_TTREE_UPNP_DM_SEPERATOR_D   "#"

VOS_UINT32 ATP_TTree_BuildName(VOS_UINT32 ulOID, const VOS_CHAR *pcSeperator, VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen);
VOS_UINT32 ATP_TTree_ParseName(const VOS_CHAR *pcName, const VOS_CHAR *pcSeperator, VOS_UINT32 *pulOID);

VOS_UINT32 ATP_CFM_CfgMsgAppendDiffPara(VOS_VOID **ppvMsg,
                             const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                             VOS_UINT32 ulObjId,
                             const VOS_UINT32 *pulOldObj,
                             const VOS_UINT32 *pulNewObj);

/*
  * ���亯���ӿ�
  */
/******************************************************************************
  ��������  : ATP_CFM_ExtConfig
  ��������  : ����ӿڣ�ȫ��������Ϣ
  �������  : 
              1.  pstGlbConfig:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2009-1-8
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFM_ExtConfig(ATP_CFM_GLOBAL_CONFIG* pstGlbConfig);

/******************************************************************************
  ��������  : ATP_CFM_ExtWriteCfgFile
  ��������  : ����ӿڣ��������ļ�д��FLASH
  �������  : 
              1.  pszCfgFile:
              2.  ulCfgFileType:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2009-1-8
     ��    ��   : p42047
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CFM_ExtWriteCfgFile(ATP_UTIL_STRFILE_ST *pstStrFile, VOS_UINT32 ulCfgFileType);

VOS_UINT32 ATP_CFM_ExtReadCfgFile(ATP_UTIL_STRFILE_ST *pstStrFile);

VOS_BOOL ATP_CFM_ExtIsCmoNeedEncrypt(VOS_UINT32 ulCmo);

VOS_CHAR *ATP_CFM_ExtEncodeParaValue(VOS_UINT32 ulCmo, const VOS_CHAR *pcParaValue);

VOS_CHAR *ATP_CFM_ExtDecodeParaValue(VOS_UINT32 ulCmo, const VOS_CHAR *pcParaValue);

/*!
  \brief configure tools lock
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK lock ok, other return current locked tools
*/
VOS_UINT32 ATP_CFM_Lock(VOS_VOID);

VOS_UINT32 ATP_CFM_Unlock(VOS_VOID);

VOS_UINT32 ATP_CFM_Getlock(VOS_VOID);

/*
 *      Global variable
 */
typedef enum tagATP_GVAR_IDX_EN
{
    ATP_UTIL_GVAR_TR069_DATA,
    ATP_UTIL_GVAR_TR069_VAL_CHG_LIST,
    ATP_UTIL_GVAR_TR064_DATA
} ATP_GVAR_IDX_EN;

#define ATP_CFM_ATTR_ACCLIST        "AccList"
#define ATP_CFM_ATTR_NOTIFY         "Notify"
#define ATP_CFM_ATTR_RESERVE        "Resv"
#define ATP_CFM_ATTR_HIDE_BITS      "HideBits"

extern const VOS_UINT16 g_usHidePwdCfgTools;
extern VOS_BOOL g_bCheckSetPara;

/*
 *  General Message Interface
 */
typedef struct tagATP_GMSG_HEAD
{
    VOS_UINT32          ulMagicNum;      // ħ����
    VOS_UINT32          ulAllSize;       // ��Ϣ���ܴ�С
    VOS_UINT32          ulFreeSize;      // ��Ϣ��ʣ��ռ�
    VOS_UINT32          ulErrCode;       // ������
    VOS_UINT32          ulIntfType;      // ���ù�������
    VOS_UINT32          ulGMsgSubCode;   // ���ò�����

    VOS_CHAR            acModName[16];
} ATP_GMSG_HEAD;

typedef enum tagATP_GMSG_SUBTYPE_EN
{
    ATP_GMSG_SUBTYPE_GET,
    ATP_GMSG_SUBTYPE_ASYNC_SET,
    ATP_GMSG_SUBTYPE_SYNC_SET
} ATP_GMSG_SUBTYPE_EN;

VOS_UINT32 ATP_GMSG_Builder(VOS_VOID **ppvGMsg, const VOS_CHAR  *pcModName, VOS_UINT32 ulSubType);

VOS_UINT32 ATP_GMSG_AppendNV(VOS_VOID **ppvGMsg, const VOS_CHAR *pcName, const VOS_CHAR *pcValue);

const VOS_CHAR *ATP_GMSG_GetHeadInfo(const VOS_VOID *pvGMsg, VOS_UINT32 *pulSubType);

const VOS_CHAR *ATP_GMSG_GetValueByName(const VOS_VOID *pvGMsg, const VOS_CHAR *pcName);

VOS_VOID ATP_GMSG_Free(VOS_VOID *pvGMsg);

#ifdef ATP_DEBUG
VOS_VOID ATP_GMSG_Print(VOS_VOID *pvGMsg);
#else
#define ATP_GMSG_Print(x...)
#endif

VOS_UINT32 ATP_GMSG_Send(VOS_VOID *pvGMsg);

VOS_UINT32 ATP_GMSG_SyncSend(VOS_VOID *pvGMsg, VOS_VOID **ppvRespMsg, VOS_UINT32 ulTimeout);

VOS_UINT32 ATP_GMSG_Reply(VOS_VOID *pvGMsg, VOS_UINT32 ulErrCode, VOS_VOID *pvRespGMsg);

VOS_UINT32 ATP_GMSG_ReplyRetCode(VOS_VOID *pvGMsg, VOS_UINT32 ulErrCode);

/*
 *  Value type utilities
 */
VOS_BOOL ATP_CFM_UtilChar2Ulong(const VOS_CHAR *pcValue, VOS_UINT32 *pulValue);

VOS_BOOL ATP_CFM_UtilChar2Long(const VOS_CHAR *pcValue, VOS_INT32 *plValue);

VOS_BOOL ATP_CFM_UtilConvertValueType(
                        const VOS_CHAR       *pcValue,
                        VOS_VOID             **pvValue,
                        VOS_UINT32           ulValueType);

VOS_UINT32 ATP_CFM_NotifyValueChange(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                             VOS_UINT32 ulOid, const VOS_VOID *pvValue);

VOS_UINT32 ATP_CFM_ChangeInstId(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                       VOS_UINT32        ulOID,
                                       VOS_UINT32       ulNewInstId);
VOS_UINT32 ATP_UTIL_EncodePath(const VOS_CHAR *pcSrc, VOS_CHAR *pcDst, VOS_UINT32 ulLen);
VOS_UINT32 ATP_UTIL_DecodePath(const VOS_CHAR *pcSrc, VOS_CHAR *pcDst, VOS_UINT32 ulLen);

typedef VOS_UINT32 (*PFCFMUtilDataProc)(const VOS_UINT8 *pucSrc, VOS_UINT32 ulSrcLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);

typedef struct tagATP_CFM_DataProc_ST
{
    PFCFMUtilDataProc pfDataProc;
}ATP_CFM_DataProc_ST;

VOS_UINT32 ATP_CFM_UtilDataProcs(
                                         const VOS_UINT8            *pucSrc, 
                                         VOS_UINT32                 ulSrcLen, 
                                         VOS_UINT8                  **ppucDst, 
                                         VOS_UINT32                 *pulDstLen,
                                         const ATP_CFM_DataProc_ST *pstDataProcs,
                                         VOS_UINT32                 ulArrayLen);

#ifdef SUPPORT_ATP_LIBZ
/*!
  \brief ѹ��
  \param[in]  *pucStr: 
  \param[in]  ulStrLen: 
  \param[in]  **ppucDst: 
  \param[in]  *pulDstLen: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_UTIL_Compress(const VOS_UINT8 *pucStr, VOS_UINT32 ulStrLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);

/*!
  \brief ��ѹ
  \param[in]  *pucStr: 
  \param[in]  ulStrLen: 
  \param[in]  **ppucDst: 
  \param[in]  *pulDstLen: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_UTIL_Uncompress(const VOS_UINT8 *pucStr, VOS_UINT32 ulStrLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);
#endif

typedef VOS_UINT32 (*PFCfmGetDomainValuesHook)(VOS_VOID *pvHandle, VOS_UINT32 ulOID, VOS_VOID *pvValue);

VOS_UINT32 ATP_CFM_CfgGetDomainValues(
                            VOS_CHAR * const    acDomainNames[],
                            VOS_UINT32          ulNum,
                            PFCfmGetDomainValuesHook pfHook,
                            VOS_VOID            *pvHandle);


typedef VOS_BOOL (* PFCfmEndParseHook)(VOS_UINT32 ulOID, const VOS_CHAR *pcEndBuf);
extern PFCfmEndParseHook g_pfEndParseHook;

// For DM Message
typedef struct tagATP_DMMSG_HEAD
{
    VOS_UINT32          ulMagicNum;      // ħ����
    VOS_UINT32          ulAllSize;       // ��Ϣ���ܴ�С
    VOS_UINT32          ulFreeSize;      // ��Ϣ��ʣ��ռ�
    VOS_UINT32          ulErrCode;       // ������
    VOS_UINT32          ulIntfType;      // ���ù�������
    VOS_UINT32          ulMsgSubCode;   // ���ò�����
} ATP_DMMSG_HEAD;

typedef struct tagATP_DMMSG_ITEM
{
    VOS_UINT32          ulIdentity;
    VOS_UINT32          ulNameLen;
    VOS_UINT32          ulValueLen;
    const VOS_CHAR      *pcName;
    const VOS_CHAR      *pcValue;
} ATP_DMMSG_ITEM;

typedef enum tagATP_DMMSG_TYPE_EN
{
    ATP_DMMSG_TYPE_UNKNOWN,
    ATP_DMMSG_GET_NAMES,
    ATP_DMMSG_GET_VALUES,
    ATP_DMMSG_GET_ATTRS,
    ATP_DMMSG_SET_VALUES,
    ATP_DMMSG_SET_ATTRS,
    ATP_DMMSG_ADD_OBJ,
    ATP_DMMSG_DEL_OBJ,
    ATP_DMMSG_REBOOT,
    ATP_DMMSG_RESET,
    ATP_DMMSG_DOWNLOAD,
    ATP_DMMSG_UPLOAD,
    ATP_DMMSG_GET_DEVICE_STATUS,
    ATP_DMMSG_GET_DOWNLOAD_STATUS,
    //...
    ATP_DMMSG_CONFIG_SERVICE,
    ATP_DMMSG_GET_DEVICE_INFO,                  //��ȡ�豸�İ汾����Ϣ,����ҵ���Զ����ź�����
} ATP_DMMSG_TYPE_EN;

VOS_UINT32 ATP_DMMSG_Builder(VOS_VOID **ppvDMMsg, const VOS_CHAR *pcRecvApp, VOS_UINT32 ulSubType);
VOS_UINT32 ATP_DMMSG_AppendItem(VOS_VOID **ppvDMMsg, VOS_UINT32 ulIdentity,
                            const VOS_CHAR *pcName, const VOS_VOID *pvValue, VOS_UINT32 ulValueLength);
VOS_UINT32 ATP_DMMSG_GetErrCode(const VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_GetSubType(const VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_GetItem(const VOS_VOID *pvDMMsg, ATP_DMMSG_ITEM *pstItem, VOS_VOID **ppvPos);
VOS_VOID ATP_DMMSG_Free(VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_Send(VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_SyncSend(VOS_VOID *pvDMMsg, VOS_VOID **ppvRespMsg, VOS_UINT32 ulTimeout);
VOS_UINT32 ATP_DMMSG_ReplyRetCode(VOS_VOID *pvDMMsg, VOS_UINT32 ulErrCode);
VOS_UINT32 ATP_DMMSG_Reply(VOS_VOID *pvDMMsg, VOS_UINT32 ulErrCode, VOS_VOID *pvRespDMMsg);
VOS_UINT32 ATP_DMMSG_ReplyEx(const VOS_CHAR *pcDstApp, VOS_UINT32 ulErrCode, VOS_VOID *pvRespDMMsg);
// check if need tr181 defined
#ifdef SUPPORT_ATP_TR181
VOS_VOID ATP_DB_ALIAS_FreeRefList();
#endif

//#ifdef ATP_DEBUG
#if 1
VOS_VOID ATP_DMMSG_Print(VOS_VOID *pvDMMsg);
#endif

#ifdef ATP_DEBUG
#define ATP_CFM_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_CFM_DEBUG(x...)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif  /* __CFM_API_H__ */

