/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<�޸���>      <ʱ��>        <���ⵥ��>             <��Ҫ����>
l00205975     2012/12/11    DTS2012121101724        ��WEB��[B890]V100R001C994B116(router):WEB�汾�������
========================================================================================*/
/**\file upgpackapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author s00175839
  *\date  2011-12-27
*/
#ifndef __UPG_PACKET_API_H__
#define __UPG_PACKET_API_H__

#ifdef WIN32
#define snprintf        _snprintf
#define strncasecmp     strnicmp
#endif

//! ֧�ֵ����ļ�����
#define ATP_MAX_LAYER_NUM           (16)         

//! ���������ļ�����,ֻ����һ�������ļ�
#define ATP_UPG_TYPE_POLICY_FILE    0x00010000  
//! ���������ļ�����                                               
#define ATP_UPG_TYPE_IMAGE_FILE     0x00020000
//! ���������ļ��ļ�����
#define ATP_UPG_TYPE_CFG_FILE       0x00030000
//! �����ļ���������
#define ATP_UPG_TYPE_MASK           0xFFFF0000

#define ATP_UPG_TYPE_IMAGE_FS               (ATP_UPG_TYPE_IMAGE_FILE + 0X1)
#define ATP_UPG_TYPE_IMAGE_KERNEL           (ATP_UPG_TYPE_IMAGE_FILE + 0X2)
#define ATP_UPG_TYPE_IMAGE_TAG              (ATP_UPG_TYPE_IMAGE_FILE + 0X3)
#define ATP_UPG_TYPE_IMAGE_DECT             (ATP_UPG_TYPE_IMAGE_FILE + 0X5)
#define ATP_UPG_TYPE_IMAGE_BOOTLOAD         (ATP_UPG_TYPE_IMAGE_FILE + 0x11)
#define ATP_UPG_TYPE_IMAGE_MIDDLEWARE       (ATP_UPG_TYPE_IMAGE_FILE + 0x100)
#define ATP_UPG_TYPE_IMAGE_MODULE           (ATP_UPG_TYPE_IMAGE_FILE + 0x101)
#define ATP_UPG_TYPE_IMAGE_RESERVED         (ATP_UPG_TYPE_IMAGE_FILE + 0x110)
#define ATP_UPG_TYPE_IMAGE_CURRENT_CONFIG   (ATP_UPG_TYPE_IMAGE_FILE + 0x111)
#define ATP_UPG_TYPE_IMAGE_FIXED_PARAM      (ATP_UPG_TYPE_IMAGE_FILE + 0x1000)

#define ATP_UPG_TYPE_IMAGE_SD5115_BOOT      (ATP_UPG_TYPE_IMAGE_FILE + 0x201)
#define ATP_UPG_TYPE_IMAGE_LTE              (ATP_UPG_TYPE_IMAGE_FILE + 0x210)

//�鲥�����������������ļ�
#define ATP_UPG_TYPE_IMAGE_TEMP_FACTORYCFG           (ATP_UPG_TYPE_IMAGE_FILE + 0X4)
/* DTS2012121101724 l00205975 20121211 end> */
/* ����webģ�� */
#define ATP_UPG_TYPE_IMAGE_WEB              (ATP_UPG_TYPE_IMAGE_FILE + 0x20)
/* DTS2012121101724 l00205975 20121211 end> */
#define ATP_UPG_TYPE_CFG_INI        (ATP_UPG_TYPE_CFG_FILE + 1)
#define ATP_UPG_TYPE_CFG_ERASE      (ATP_UPG_TYPE_CFG_FILE + 2)

#define ATP_UPG_INT_LEN             (4)
#define ATP_UPG_STR_LEN             (32)
#define ATP_UPG_DATE_LEN            (16)


//! PKG magic number
#define ATP_UPG_MAGIC_NUM           (0x10325476)

//!\brief Hash�㷨
/*!
 * ���ʹ�õ�Hash�㷨
 */
enum
{
    MD5WithRSA        = 0,
    SHA2WithRSA       = 1,
    SHA4WithRSA       = 2,
    HASH_MAX
};

//!\brief ǩ������RSA�㷨λ��
/*!
 *
 */
enum
{
    RSA_BITS_2048   = 0,
    RSA_BITS_1024   = 1
};

//!\brief ÿ�����ļ�����Ϣ
/*!
 * 
 */
typedef struct tagATP_UPG_LAYER_ST
{
    unsigned char                   aucFileType[ATP_UPG_INT_LEN];   /*!< ������� */    
    unsigned char                   aucFileOffset[ATP_UPG_INT_LEN]; /*!< ƫ�Ƶ�ַ */
    unsigned char                   aucFileLen[ATP_UPG_INT_LEN];    /*!< ������� */
    unsigned char                   aucSoftVersion[ATP_UPG_STR_LEN];/*!< ����汾�� */
} ATP_UPG_LAYER_ST;

//!\brief ǩ����Ϣ
/*!
 * ���ǩ����Ϣ
 */
typedef struct tagATP_UPG_SIGNINFO_ST
{    
    unsigned char                   ucHasSign;      /*!< �Ƿ�ǩ�� */    
    unsigned char                   ucRSABits;      /*!< RSAλ */     
    unsigned char                   ucHashAlgo;     /*!< hash�㷨 */
    unsigned char                   ucReserved;     /*!< ���� */
} ATP_UPG_SIGNINFO_ST;

#define ATP_UPG_SIGNATURE_ISSUER_SIZE (ATP_UPG_DATE_LEN + ATP_UPG_DATE_LEN + ATP_UPG_STR_LEN)
//!\brief ǩ���ṹ
/*!
 * ���ڰ�β��ǩ�����ݽṹ
 */ 
typedef struct tagATP_UPG_SIGNATURE_ST
{    
    unsigned char                   aucNotBefore[ATP_UPG_DATE_LEN]; /*!< not before utc yyyyMMddhhmmss */  
    unsigned char                   aucNotAfter[ATP_UPG_DATE_LEN];  /*!< not after utc yyyyMMddhhmmss */
    unsigned char                   aucIssuer[ATP_UPG_STR_LEN];     /*!< issuer */    
    unsigned char                   *pucSignature;                  /*!< ǩ������ */
} ATP_UPG_SIGNATURE_ST;


//!\brief ����ļ�ͷ��ʽ
/*!
 *
 */
typedef struct tagATP_UPG_FILE_HADER_ST
{
    unsigned char                   aucMagicNum[ATP_UPG_INT_LEN];    /*!< ħ������ */ 
    unsigned char                   aucProductName[ATP_UPG_STR_LEN]; /*!< ��Ʒ���� */ 
    unsigned char                   aucSoftVersion[ATP_UPG_STR_LEN]; /*!< ����汾��Ϣ */ 
    ATP_UPG_SIGNINFO_ST             stSignInfo;                      /*!< ǩ����Ϣ */
    unsigned char                   aucReserved[ATP_UPG_INT_LEN];    /*!< ���� */ 
    unsigned char                   aucPkgNum[ATP_UPG_INT_LEN];      /*!< ���ļ��� */ 
    ATP_UPG_LAYER_ST                astLayout[ATP_MAX_LAYER_NUM];    /*!< ÿ�����ļ�ͷ��Ϣ */                                                           
    unsigned char                   aucHeadCrc[ATP_UPG_INT_LEN];     /*!< CRC */ 
} ATP_UPG_FILE_HADER_ST;

struct tagATP_UPG_FILE_ST;

//!\brief ����������ļ��ṹ��
/*!
 *
 */
typedef struct tagATP_UPG_SUBFILE_ST
{
    unsigned int                      ulPkgType;    /*!< �������PkgType */    // 
    unsigned int                      ulPkgOffset;  /*!< �������PkgOffset */   // 
    unsigned int                      ulPkgLen;      /*!< �������PkgLen */   // 
    char                              *pcVersion;   /*!< ����汾�� */    // 
    char                              *pcUrl;       /*!< ����URL */    // 
    unsigned char                     ucNeedUpdate; /*!< �Ƿ���Ҫ���� 0����Ҫ��1������Ҫ */    
    void                              *pvArg;    /*!< ��չָ�� */     
    struct tagATP_UPG_FILE_ST       *pstParent; /*!< ���ṹָ�� */ 
    struct tagATP_UPG_SUBFILE_ST    *pstNext; /*!< ��һ�����ļ�ָ�� */ 
} ATP_UPG_SUBFILE_ST;

//!\brief ������Ķ�ϵͳ�ļ��ṹ��
/*!
 * \param pcVersion:
 */
typedef struct tagATP_UPG_FILE_ST
{
    char                  *pcVersion;     /*!< ����汾�� */
    char                  *pcProductInfo; /*!< ��Ʒ���� */ 
    char                  *pcStatusUrl;   /*!< ״̬URL */ 
    unsigned int          bUnknownFormat; /*!< �����ʽ�Ƿ�֧�� */ 
    ATP_UPG_SIGNINFO_ST   stSignInfo;
    void                  *pvArg;         /*!< ��չָ�� */   
    ATP_UPG_SUBFILE_ST    *pstSubFiles;   /*!< ���ļ��б� */   
} ATP_UPG_FILE_ST;

/* start modify of upg led for DT W724V by y00179387 */
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
#define WEB_UPGRADE_LED_COUNTER     (410)  // the average upg time is 3'25"
#define UPNP_UPGRADE_LED_COUNTER    (150)
#endif
/* end modify of upg led for DT W724V by y00179387 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
  \brief �������ļ����ͻ�ȡ���ļ��ṹ��
  \param[in]  ulFileType: �ļ�����
  \param[in]  *pstUpgFile: ��ϵͳ�ṹ��
  \return 
  \retval NULL δ�ҵ�
  \retval Not NULL ���ļ��ṹ��֧��
*/
ATP_UPG_SUBFILE_ST *ATP_UPG_GetSubFileByType(
                                 unsigned int        ulFileType,
                                 const ATP_UPG_FILE_ST     *pstUpgFile);

/*!
  \brief ���ٶ�ϵͳ�ṹ��
  \param[in]  *pstUpgFile: �����ٵĶ�ϵͳ�ṹ��
  \return 
  \retval void
*/
void ATP_UPG_DestroyUpgFile(ATP_UPG_FILE_ST     *pstUpgFile);



/*!
  \brief ��ȡһ���ļ�����������
  \param[in]  *pcFileName: ����ȡ���ļ���
  \param[in]  **ppucFileBuf: ����ļ������ݵĻ�����ָ��
  \param[in]  *pulFileLen: ����ļ����ȵ�ָ��
  \return 
  \retval void
*/
#ifndef BUILD_BOOTLOADER 
unsigned int ATP_UPG_UtilGetFileToBuffer(
                                const char          *pcFileName,
                                unsigned char       **ppucFileBuf,
                                unsigned int        *pulFileLen);
#endif
/*!
  \brief ����У���
  \param[in]  *pucSrcBuf: ԭʼ���������׵�ַ
  \param[in]  ulSrcLen: ԭʼ�������ĳ���
  \param[in]  *pucDstBuf: ���CRC�������Ļ�����
  \param[in]  ulDstLen: ���CRC�������Ļ���������
  \return 
  \retval VOS_OK �ɹ� 
  \retval VOS_NOK ʧ��
*/
int ATP_UPG_UtilCalcCheckSum(
                                const unsigned char     *pucSrcBuf,
                                unsigned int      ulSrcLen,
                                unsigned char     *pucDstBuf,
                                unsigned int      ulDstLen);

/*!
  \brief ��������Ƶ�����鲥��
  \param[in]  *pucMcastHeader: �鲥���Ļ������׵�ַ
  \param[in]  ulMcastHeaderLen: �鲥���ĳ���
  \param[in]  **ppstUpgFile: ������Ķ�ϵͳ�ṹ��
  \return 
  \retval VOS_OK �ɹ� 
  \retval VOS_NOK ʧ��
*/
unsigned int ATP_UPG_ParseMcastHeader(
                                const unsigned char     *pucMcastHeader,
                                unsigned int       ulMcastHeaderLen,
                                ATP_UPG_FILE_ST    **ppstUpgFile);

/*!
  \brief ����������ļ�ͷ
  \param[in]  *pucBuf: ���������ļ����ݻ�����
  \param[in]  ulBufLen: ���������ļ����ݻ���������
  \param[in]  **ppstUpgFile: ������Ķ�ϵͳ�ļ��ṹ��
  \return 
  \retval VOS_OK �ɹ� 
  \retval VOS_NOK ʧ��
*/
unsigned int ATP_UPG_ParsePkgFileHeader(
                                const unsigned char     *pucBuf,
                                unsigned int      ulBufLen,
                                ATP_UPG_FILE_ST   **ppstUpgFile);

/*!
  \brief У�������ļ�ͷ
  \param[in]  *pucBuf: ���������ļ����ݻ�����
  \param[in]  ulBufLen: ���������ļ����ݻ���������
  \return 
  \retval VOS_OK �ɹ� 
  \retval VOS_NOK ʧ��
*/
unsigned int ATP_UPG_VerifyPkgHeader(
                                const unsigned char  *pucBuf,
                                unsigned int         ulBufLen);

#ifdef __cplusplus
}
#endif

#endif


