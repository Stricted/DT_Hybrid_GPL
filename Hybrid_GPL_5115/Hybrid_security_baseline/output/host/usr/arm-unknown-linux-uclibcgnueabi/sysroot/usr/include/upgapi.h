#ifndef __UPG_API_H__
#define __UPG_API_H__

#include "upgtypes.h"
#include "upgpackapi.h"
#include "upginiapi.h"

#define ATP_UPG_NEED_REBOOT_BIT_D       (0x40)

/* error num for trans tools */
#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

/* �ļ����䷽�� */
typedef enum tagATP_UPG_TRANS_DIR_EN
{
    ATP_TRANS_DIR_UPLOAD,
    ATP_TRANS_DIR_DOWNLOAD,
    ATP_TRANS_DIR_BOTH,
    ATP_TRANS_DIR_CHECK
} ATP_UPG_TRANS_DIR_EN;

/* �ļ����䷽�� */
typedef enum tagATP_UPG_STATE_EN
{
    ATP_UPG_BEFORE_TRANS,
    ATP_UPG_AFTER_TRANS,
    ATP_UPG_ERR_TRANS
} ATP_UPG_STATE_EN;

typedef struct tagATP_UPG_CTX_ST
{
    ATP_UPG_FILE_ST         *pstPkgList;
    ATP_UPG_INI_SECTION_ST  *pstIniSection;
    unsigned char           *pcImageBuff;
    unsigned int             ulLen;
    const char              *pcCfgTool;
    const char              *pcVersion;   
}ATP_UPG_CTX_ST;

/* ���ļ������� */
typedef unsigned int (* AtpSubFileUpgProc)(const ATP_UPG_CTX_ST* pstUpgCtx);

typedef unsigned int (* AtpUpgFlashErrorHook)(
                                    const ATP_UPG_CTX_ST* pstUpgCtx,
                                    unsigned int ulRet);
extern AtpUpgFlashErrorHook g_pfUpgFlashErrorHook;

/* ��������� */
typedef unsigned int (* AtpTransItemProc)(
                                ATP_UPG_STATE_EN    enTranState,
                                const char      *pcCfgTool,
                                const char      *pcFilePath,
                                const char            *apcArg[],
                                unsigned int          ulCurArgLen);

/* ����Ԥ������ */
typedef unsigned int (* AtpUpgPreProc)(
                                 char      *pcCfgTool,
                                ATP_UPG_FILE_ST     *pstPkgList);

/* ������ṹ�� */
typedef struct tagATP_UPG_TRANS_ITEM_ST
{
     char                  *pcItemName;
     char                  *pcFilePath;
    AtpTransItemProc                pfProc;
    unsigned int                      ulDir;      // ATP_UPG_TRANS_DIR_EN
    unsigned int                      ulMaxSize;

    void                        *pvSubProcs;    // Sub proc list

    struct tagATP_UPG_TRANS_ITEM_ST *pstNext;
} ATP_UPG_TRANS_ITEM_ST;

/* ���乤�߽ṹ�� */
typedef struct tagATP_UPG_TRANS_TOOL_ST
{
     char                  *pcExtCliName;
     char                  *pcProtocol;
    unsigned short                      usTransDir;
    unsigned short                      usDftPort;
    struct tagATP_UPG_TRANS_TOOL_ST *pstNext;
} ATP_UPG_TRANS_TOOL_ST;

#define UPG_MAX_PATH_LEN        (256)

/******************************************************************************
  ��������  : ATP_UPG_ParseURL
  ��������  : ����URL��ַ
  �������  :
              1. pcUrl      :   �����URL�ַ���
              2. pcProtocol:  ��������Э���ַ���������Ϊ�գ���Ϊ�գ��򳤶�ΪUPG_MAX_PATH_LEN
              3. pcAdrr     :   ������������ַ�Ļ�����������UPG_MAX_PATH_LEN
              4. pcRemote   :   ���Զ��·���Ļ�����������UPG_MAX_PATH_LEN
              5. usPort     :   �˿ڣ�Ĭ��ֵΪ0
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_TRUE  :   �����ɹ�
              VOS_FALSE :   ����ʧ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_ParseURL(  const char                *pcUrl,
                                char                *pcProtocol,
                                char                *pcAdrr,
                                char                *pcRemote,
                                unsigned short      *pusPort);

/******************************************************************************
  ��������  : ATP_UPG_RegTransTool
  ��������  : ע�ᴫ�乤��
  �������  :
              1. pcExeName  :   ���乤������
              2. pcProName  :   ���乤����֧�ֵ�Э������
              3. eTransDir  :   ���乤��֧�ֵĴ��䷽��
              4. usDftPort  :   ���乤�ߵ�Ĭ�϶˿�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-19
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_RegTransTool(
                        const char              *pcExeName,
                        const char              *pcProName,
                        ATP_UPG_TRANS_DIR_EN    eTransDir,
                        unsigned short          usDftPort);

/******************************************************************************
  ��������  : ATP_UPG_RegTransItem
  ��������  : ע�ᴫ����
  �������  :
              1. pcItemName :   ����������
              2. pcFileName :   ������ı����ļ�����
              3. eDir       :   ������ķ���
              4. pfUpgProc  :   ������Ĵ�����
              5. ulMaxSize  :   ���������󳤶ȣ�0��ʾû������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-19
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_RegTransItem(
                        const char              *pcItemName,
                        const char              *pcFileName,
                        ATP_UPG_TRANS_DIR_EN    eDir,
                        AtpTransItemProc        pfUpgProc,
                        unsigned int            ulMaxSize);

/******************************************************************************
  ��������  : ATP_UPG_SetDftUpgProc
  ��������  : ����Ĭ������������
  �������  : 
              1.  ulFileType    :   Ĭ�ϵ��ļ�����
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-10-14
     ��    ��   : z65940
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_SetDftUpgProc(unsigned int  ulFileType);

/******************************************************************************
  ��������  : ATP_UPG_RegSubFile
  ��������  : ע�����ļ�
  �������  :
              1. ulFileType :   ���ļ�����
              2. pcFileName :   ���ļ�������·��
              3. bDftProc   :   �����ļ��Ƿ�ΪĬ�ϵ������ļ�
                                �����ص��ļ�����ATP������ļ���ʽʱ����Ҫʹ��Ĭ��
                                �������������������˴�ָ���Ƿ���Ϊ�Ǹ����ļ�
                                VOS_TRUE:�ǣ�VOS_FALSE:��
              4. ulMaxSize  :   ���ļ�������С
              5. pfUpgProc  :   ���ļ��Ĵ�����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_RegSubFile(
                        unsigned int            ulFileType,
                        unsigned int            bDftProc,
                        AtpSubFileUpgProc       pfUpgProc);

/******************************************************************************
  ��������  : ATP_UPG_GetProcByType
  ��������  : �������ļ����ͻ�ȡ�䴦����
  �������  :
              1. ulSubType  :   ���ļ�����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              NULL      : û���ҵ����ļ�������
              Not NULL  : ���ļ�������

  �޸���ʷ      :
   1.��    ��   : 2008-6-25
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
AtpSubFileUpgProc ATP_UPG_GetProcByType(
                                    unsigned int      ulSubType);

/******************************************************************************
  ��������  : ATP_UPG_StartTransItem
  ��������  : ��ʼһ�δ���
  �������  :
              1. pcItemName : ����������ƣ�������Ϊ��
              2. pcCfgTool  : �����乤�ߵ����ƣ�����Ϊ��
              3. pcUrl      : Զ��URL������Ϊ��
              4. pcLocalIp  : �󶨱���IP��Ϊ�ձ�ʾ����
              5. pcUserName : ����ʱ��֤���û���������Ϊ��
              6. pcPassWord : ����ʱ��֤�����룬����Ϊ��
              7. enDir      : ���䷽���ϴ���������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_StartTransItem(
                            const char          *pcItemName,
                            const char          *pcCfgTool,
                            const char          *pcUrl,
                            const char          *pcLocalIp,
                            const char          *pcUserName,
                            const char          *pcPassWord,
                            ATP_UPG_TRANS_DIR_EN    enDir);

/******************************************************************************
  ��������  : ATP_UPG_UpgradeFromHeaderFile
  ��������  : �Ӷ�ϵͳ�ļ�ͷ�����ÿ�����ļ�������
              ���а汾֧���������͵��ļ�ͷ:
              1����ϵͳ����ļ����ļ�ͷ
              2���鲥����ͷ
  �������  :
              1. pcBaseCmd      :   ���ض�ϵͳ����ļ�ͷ�������У�
                                    ���Ϊ�գ���ʾ�鲥����
              2. pcLocalIp      :   ����󶨵ı���IP������Ϊ�գ�ֻ���鲥ʱʹ��
              3. pcFileHeader   :   �ļ�ͷ��·����������Ϊ��
              4. pcCfgTool      :   ���ù������ƣ�����Ϊ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
unsigned int ATP_UPG_UpgradeFromHeaderFile(
                        const char               *apcBaseArgs[],
                        unsigned int        ulBaseArgCnt,
                        const char               *pcLocalIp,
                        const char               *pcFileHeader,
                        const char               *pcCfgTool);

unsigned int ATP_UPG_IsPkgTypeExist(
                                const ATP_UPG_FILE_ST   *pstPkgList,
                                unsigned int              ulPkgType);

ATP_UPG_SUBFILE_ST *ATP_UPG_SubFileFindByType(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

unsigned int ATP_UPG_SubFileMoveToTheLast(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

unsigned int ATP_UPG_SubFileMoveToTheFirst(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

const char * ATP_UPG_GetVersion(void);

unsigned int ATP_UPG_StartTransItemEx(
                            const char          *pcItemName,
                            const char          *pcCfgTool,
                            const char          *pcUrl,
                            const char          *pcLocalIp,
                            const char          *pcUserName,
                            const char          *pcPassWord,
                            ATP_UPG_TRANS_DIR_EN    enDir,
                            const char          *pcHostName);


unsigned int ATP_UPG_Upgrade(unsigned char *pcImage, unsigned int ulLen, const char *pcCfgTool);

#ifdef ATP_DEBUG
#define ATP_UPG_DEBUG(format, args...)  {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#ifdef WIN32
#define ATP_UPG_DEBUG printf
#else
#define ATP_UPG_DEBUG(x...)
#endif
#endif

#endif

