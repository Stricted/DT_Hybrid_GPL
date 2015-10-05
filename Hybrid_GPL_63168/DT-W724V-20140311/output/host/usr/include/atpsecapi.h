/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpsecapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author s00175839
  *\date  2012-1-20
*/

#ifndef __ATP_SEC_API_H
#define __ATP_SEC_API_H


#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */


#define ATP_SEC_MD5_LEN                 (16)
#define ATP_SEC_SHA1_LEN				(20)
#define ATP_SEC_SHA2_LEN                (32)
#define ATP_SEC_SHA4_LEN                (64)
#define ATP_SEC_HASHCODE_MAX_LEN        (256)

typedef enum tagATP_SEC_ERR_CODE_EN
{
    ATP_ERR_SEC_INVALID_PARA = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_SEC),
    ATP_ERR_SEC_INVALID_DATE,   // ��ʧЧ
    ATP_ERR_SEC_INVALID_ISSUR,  // issur�Ƿ�
    ATP_ERR_SEC_NOT_SUPPORT_ALG,
    ATP_ERR_SEC_INVALID_DATA,
    ATP_ERR_SEC_NO_MEMORY,
    ATP_ERR_SEC_SIGNATURE,
    ATP_ERR_SEC_KEY_GEN,
    ATP_ERR_SEC_OPEN_FILE,
    ATP_ERR_SEC_READ_FILE,
    ATP_ERR_SEC_INVALID_KEY,
    ATP_ERR_SEC_CRYPT,
    ATP_ERR_SEC_SHMEM_INIIT,
    ATP_ERR_SEC_SHMEM_HANDLE,
    ATP_ERR_SEC_SHMEM_SET,
    
} ATP_SEC_ERR_CODE_EN;

enum ATP_SEC_KEYID_EN
{
    ATP_SEC_KEY_TARGET = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_SEC), /*!< cfg file key */
    ATP_SEC_KEY_HOST,
    ATP_SEC_KEY_SSL,
};

enum ATP_SEC_SSLID_EN
{
    ATP_SEC_SSL_ROOT = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_SEC)+100,
    ATP_SEC_SSL_CWMP,
};

//!\brief �㷨ID
/*!
 *
 */
typedef enum tagATP_SEC_ALGID_EN
{
    ATP_SEC_ALGID_MD5   = 0x10000,      /*!< MD5 */ 
	ATP_SEC_ALGID_SHA1,					/*!< SHA1 */
    ATP_SEC_ALGID_SHA2,                 /*!< SHA2 */
    ATP_SEC_ALGID_SHA4,                 /*!< SHA4 */
    ATP_SEC_ALGID_DIGEST_MAX,
    
    ATP_SEC_ALGID_3DES  = 0x20000,      /*!< 3DES */
    ATP_SEC_ALGID_AES,                  /*!< AES */
    ATP_SEC_ALGID_SYMMETRIC_MAX,
    
    ATP_SEC_ALGID_RSA   = 0x40000,      /*!< RSA */
    ATP_SEC_ALGID_ASYMMETRIC_MAX,
    
    ATP_SEC_ALGID_MD5WithRSA = 0x80000, /*!< sign MD5 With RSA */
    ATP_SEC_ALGID_SHA1WithRSA,          /*!< sign SHA2 With RSA*/
    ATP_SEC_ALGID_SHA2WithRSA,          /*!< sign SHA2 With RSA*/
    ATP_SEC_ALGID_SHA4WithRSA,          /*!< sign SHA4 With RSA */
    ATP_SEC_ALGID_SIGNATURE_MAX,

    ATP_SEC_ALGID_NORMAL  = 0xC0000,      /*!�����Կ */

    ATP_SEC_ALGID_INVALID
}ATP_SEC_ALGID_EN;

#define ATP_SEC_ALGID_DIGEST_MASK       0x00010000
#define ATP_SEC_ALGID_SYMMETRIC_MASK    0x00020000
#define ATP_SEC_ALGID_ASYMMETRIC_MASK   0x00040000
#define ATP_SEC_ALGID_SIGNATURE_MASK    0x00080000


//!\brief ��Կ�洢
/*!
 *
 */
typedef enum tagATP_SEC_KeyStore_EN
{   
    ATP_SEC_STORE_NONE = 0,                 /*!< �� �� */    
    ATP_SEC_STORE_ALL,                  /*!< ��������Կ */
    ATP_SEC_STORE_PUBLICKEY,            /*!< �湫Կ */
    ATP_SEC_STORE_PRIVATEKEY            /*!< ��˽Կ */
} ATP_SEC_KeyStore_EN;

typedef enum tagATP_SEC_SIGNATURE_MODE_EN
{
    ATP_SEC_SIGNATURE_MODE_PUBLIC = 0,
    ATP_SEC_SIGNATURE_MODE_PRIVATIE = 1
}ATP_SEC_SIGNATURE_MODE_EN;

//!\brief ��Կ����
/*!
 *
 */
typedef enum tagATP_SEC_KeyAttribute_EN
{
    ATP_SEC_KEY_ATTR_KEYID      = 0x000000001,
    ATP_SEC_KEY_ATTR_ALG        = 0x000000002,
    ATP_SEC_KEY_ATTR_SIZE       = 0x000000004,
    ATP_SEC_KEY_ATTR_STORE      = 0x000000008,    
    ATP_SEC_KEY_ATTR_PUBLIC     = 0x000000010,
    ATP_SEC_KEY_ATTR_PRIVATE    = 0x000000020,
    ATP_SEC_KEY_ATTR_SYMMETRIC  = 0x000000040,
    
    ATP_SEC_KEY_ATTR_CBC        = 0x000010000,
    ATP_SEC_KEY_ATTR_IV         = 0x000020000,
    ATP_SEC_KEY_ATTR_N          = 0x000040000,
    ATP_SEC_KEY_ATTR_E          = 0x000080000,
    ATP_SEC_KEY_ATTR_D          = 0x000100000,
    ATP_SEC_KEY_ATTR_P          = 0x000200000,
    ATP_SEC_KEY_ATTR_Q          = 0x000400000,
    ATP_SEC_KEY_ATTR_DP         = 0x000800000,
    ATP_SEC_KEY_ATTR_DQ         = 0x001000000,
    ATP_SEC_KEY_ATTR_QP         = 0x002000000,

    ATP_SEC_KEY_ATTR_NORMAL     = 0x004000000,
    
    ATP_SEC_KEY_ATTR_MAX
}ATP_SEC_KeyAttribute_EN;

//!\brief ���ݴ洢�ṹ
/*!
 *
 */
typedef struct tagATP_SEC_DATA_ST
{
    VOS_UINT32 ulLength;
    VOS_UINT8  *pucData;
}ATP_SEC_DATA;

typedef ATP_SEC_DATA * ATP_SEC_DATA_PTR;

//!\brief ��Կdata list�ṹ
/*!
 *
 */
typedef struct tagATP_SEC_KeyData_ST
{
    struct tagATP_SEC_KeyData_ST* pstNext;
    VOS_UINT32       attribute;
    ATP_SEC_DATA_PTR key;
}ATP_SEC_KeyData_ST;

//!\brief ��Կ��Ϣ
/*!
 *
 */
typedef struct tagATP_SEC_KeyInfo_ST
{
    ATP_SEC_ALGID_EN    enAlgId;            /*!< �㷨ID */
    VOS_UINT32          ulKeySize;          /*!< ��Կ����bytes */
    ATP_SEC_KeyStore_EN enKeyStore;         /*!< ��Կ�Ƿ�洢 */
}ATP_SEC_KeyInfo_ST;

typedef struct tagATP_SEC_Key_ST
{
    ATP_SEC_KeyInfo_ST      stKeyInfo;
    ATP_SEC_KeyData_ST      *pstKeyDataList;
}ATP_SEC_Key_ST;

typedef struct tagATP_SEC_KeyPool_ST
{
    struct tagATP_SEC_KeyPool_ST*   pstNext;
    
    VOS_UINT32                      ulKeyID;  /*!< ��Կ�����ĸ�ģ�� ��ATP_MODLE��Ӧ */
    ATP_SEC_Key_ST                  *pstKey;
}ATP_SEC_KeyPool_ST;

typedef struct tagATP_SEC_SSL_Cert_ST
{
    struct tagATP_SEC_SSL_Cert_ST*  pstNext;
    VOS_UINT32                      ulSSLID;
    VOS_UINT32                      ulKeyID;
    const  VOS_CHAR                 *pcCAPath;
    const  VOS_CHAR                 *pcPubCertPath;
    const  VOS_CHAR                 *pcKeyPath;
}ATP_SEC_SSL_Cert_ST;

typedef struct tagATP_SEC_KM_Info_ST
{
    ATP_SEC_Key_ST                  *pstInitKey;    /*!������Կ��Կ */
    ATP_SEC_SSL_Cert_ST             *pstSslCertList;
    ATP_SEC_KeyPool_ST              *pstKeyPool;
}ATP_SEC_KM_Info_ST;

typedef VOS_UINT32 (*PFATPSecLoadStaticKeysProc)(ATP_SEC_DATA_PTR *pstData);
extern PFATPSecLoadStaticKeysProc g_pfSecLoadStaticKeysProc;

#include "atpseckmapi.h"

/**********************************************************************
 *                                   UTIL
**********************************************************************/
/*!
  \brief mallocָ�����Ȼ���
  \param[in]  ulLength: ���ݳ���
  \return 
  \retval VOS_NULL mallocʧ��
*/
ATP_SEC_DATA_PTR ATP_SEC_CreateData(VOS_UINT32 ulLength);

/*!
  \brief �ͷ�ATP_SEC_DATA_PTR
  \param[in]  ptData: 
  \return 
  \retval VOID
*/
VOS_VOID ATP_SEC_DeleteData(ATP_SEC_DATA_PTR ptData);

/*!
  \brief �ļ����ݶ���ATP_SEC_DATA
  \param[in]  *pcFile: 
  \param[in]  *ptData: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_ReadFileToData(const VOS_CHAR *pcFile, ATP_SEC_DATA_PTR *ptData);


/**********************************************************************
 *                                   Digest
**********************************************************************/

/*!
  \brief  ����HASHֵ����Context
  \param[in]  enAlg: �㷨ID
  \param[out]  **ctx:  context  
  \return 
  \retval VOS_OK �ɹ�
  \retval ATP_ERR_SEC_NOT_SUPPORT_ALG �㷨��֧��
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
*/
VOS_UINT32 ATP_SEC_CreateDigestContext(
                                ATP_SEC_ALGID_EN    enAlg, 
                                VOS_VOID            **ctx);


/*!
  \brief ����HASHֵ
  \param[in]  *ctx: 
  \param[in]  *pucData: 
  \param[in]  ulLen: 
  \param[in]  **pucDigest: 
  \param[in]  *pulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_SEC_DigestData(
                                VOS_VOID                    *ctx, 
                                const VOS_UINT8             *pucData,
                                VOS_UINT32                  ulLen,
                                VOS_UINT8                   **pucDigest,
                                VOS_UINT32                  *pulLen);

/*!
  \brief ����HASHֵ
  \param[in]  *ctx: context
  \param[in]  ptData: 
  \param[out]  *ptDigest: HASHֵ
  \return 
  \retval VOS_OK �ɹ�
  \retval ATP_ERR_SEC_INVALID_PARA ��������
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
*/
VOS_UINT32 ATP_SEC_DigestDataEx(
                                VOS_VOID                    *ctx, 
                                const ATP_SEC_DATA_PTR      ptData, 
                                ATP_SEC_DATA_PTR            *ptDigest);


/*!
  \brief �����ַ���HASHֵ
  \param[in]  *ctx: 
  \param[in]  *pstSrc: �����ַ���
  \param[in]  **pstDst: ���hashֵ
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_SEC_DigestString(VOS_VOID    *ctx,
                                const VOS_CHAR *pstSrc,
                                VOS_CHAR **pstDst);

/*!
  \brief �ͷ�context
  \param[in]  *ctx: 
  \return 
*/
VOS_VOID ATP_SEC_DeleteContext(VOS_VOID *ctx);


/**********************************************************************
 *                                   symmetric & asymmetric
**********************************************************************/
/*!
  \brief �����ԳƼ���ctx
  \param[in]  enDigestAlg: �㷨ID
  \param[in]  *key: ��Կ
  \param[out]  **ctx: 
  \return 
  \retval VOS_OK �ɹ�
  \retval ATP_ERR_SEC_NOT_SUPPORT_ALG �㷨��֧��
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
*/
VOS_UINT32 ATP_SEC_CreateSymmetricContext(
                                ATP_SEC_ALGID_EN            enAlg, 
                                const ATP_SEC_Key_ST        *key,
                                VOS_VOID                    **ctx);

/*!
  \brief �����ǶԳƼ���context
  \param[in]  enDigestAlg: �㷨ID
  \param[in]  *key: ��Կ
  \param[out]  **ctx: 
  \return 
  \retval VOS_OK �ɹ�
  \retval ATP_ERR_SEC_NOT_SUPPORT_ALG �㷨��֧��
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
*/
VOS_UINT32 ATP_SEC_CreateAsymmetricContext(
                                ATP_SEC_ALGID_EN            enAlg, 
                                const ATP_SEC_Key_ST        *key,
                                VOS_VOID                    **ctx);

/*!
  \brief ����
  \param[in]  *ctx: 
  \param[in]  *pucSrc: ����
  \param[in]  ulLen: 
  \param[in]  *pucDst: ����
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_EncryptData(
                                VOS_VOID                    *ctx, 
                                VOS_UINT8                   *pucSrc, 
                                VOS_UINT32                   ulLen, 
                                VOS_UINT8                   **ppucDst,
                                VOS_UINT32                  *pulDstLen);

/*!
  \brief ����
  \param[in]  *ctx: 
  \param[in]  pstSrc: 
  \param[in]  *pstDst: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_EncryptDataEx(
                                VOS_VOID                    *ctx,
                                const ATP_SEC_DATA_PTR      pstSrc,
                                ATP_SEC_DATA_PTR            *pstDst);

/*!
  \brief ����
  \param[in]  *ctx: 
  \param[in]  *pucSrc: ����
  \param[in]  ulLen: 
  \param[in]  *pucDst: ���ģ�pucDst������pucSrc��ַ��ͬ
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_DecryptData(
                                VOS_VOID                    *ctx, 
                                VOS_UINT8                   *pucSrc, 
                                VOS_UINT32                  ulLen, 
                                VOS_UINT8                   *pucDst);

/*!
  \brief ����
  \param[in]  *ctx: 
  \param[in]  pstSrc: 
  \param[in]  *pstDst: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_DecryptDataEx(
                                VOS_VOID                    *ctx, 
                                const ATP_SEC_DATA_PTR      pstSrc,
                                ATP_SEC_DATA_PTR            *pstDst);



/**********************************************************************
 *                                   signature
**********************************************************************/
/*!
  \brief ����ǩ��context
  \param[in]  enAlgId: �㷨ID
  \param[in]  *pstKeyInfo: ��Կ
  \param[in]  **ctx: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_CreateSignatureContext(
                                ATP_SEC_ALGID_EN            enAlgId, 
                                const ATP_SEC_Key_ST        *key,
                                VOS_VOID                    **ctx);

/*!
  \brief ǩ��
  \param[in]  *ctx: 
  \param[in]  ulMode: 0 public 1 private
  \param[in]  ptData: ��ǩ������ 
  \param[out]  *ptSignature: ǩ��
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_SignData(
                                VOS_VOID                    *ctx,
                                VOS_UINT32                  ulMode,
                                const ATP_SEC_DATA_PTR      ptData, 
                                ATP_SEC_DATA_PTR            *ptSignature);

/*!
  \brief У��ǩ��
  \param[in]  *ctx: 
  \param[in]  ulMode: 0 public 1 private
  \param[in]  ptData: 
  \return 
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_VerifyData(
                                VOS_VOID                    *ctx, 
                                VOS_UINT32                  ulMode,
                                const ATP_SEC_DATA_PTR      ptData);


/**********************************************************************
 *                                   key manage
**********************************************************************/
/*!
  \brief ATP_SEC_KM_Init ��ʼ��
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_SEC_KM_Init(VOS_VOID);

/*!
  \brief get ��Կ
  \param[in]  ulKeyID: 
  \param[in]  **key: 
  \return 
  \retval VOS_NULL δ�ҵ�
*/
ATP_SEC_Key_ST* ATP_SEC_KM_GetKey(VOS_UINT32 ulKeyID);

/*!
  \brief ����KM
  \return 
  \retval 
*/
VOS_VOID ATP_SEC_KM_Destroy(VOS_VOID);

#ifdef ATP_DEBUG
#define ATP_SEC_DEBUG(format, args...)  {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
void ATP_SEC_PrintData(ATP_SEC_DATA_PTR data);
#else
#define ATP_SEC_DEBUG(x...)
#define ATP_SEC_PrintData(data);
#endif


#ifdef __cplusplus
}
#endif /* __cpluscplus */


#endif
