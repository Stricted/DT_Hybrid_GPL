/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpseckmapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author s00175839
  *\date  2012-1-20
*/

#ifndef __ATP_SEC_KM_API_H
#define __ATP_SEC_KM_API_H

#include "strfileapi.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*!
  \brief ��Կ��Ϣ���ܺ��ɢ6�δ洢����ȡ��һ��
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK�ɹ�
*/
extern VOS_UINT32 ATP_GetInfo1(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief ��Կ��Ϣ���ܺ��ɢ6�δ洢����ȡ�ڶ���
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK�ɹ�
*/
extern VOS_UINT32 ATP_GetInfo2(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief ��Կ��Ϣ���ܺ��ɢ6�δ洢����ȡ������
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK�ɹ�
*/
extern VOS_UINT32 ATP_GetInfo3(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief ��Կ��Ϣ���ܺ��ɢ6�δ洢����ȡ���Ķ�
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK�ɹ�
*/
extern VOS_UINT32 ATP_GetInfo4(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);

/*!
  \brief ������key
  \param[in]  ulKeyID: 
  \param[in]  *key: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY �����ڴ治��
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_KM_AddKey(VOS_UINT32 ulKeyID, ATP_SEC_Key_ST *key);

/*!
  \brief ����key
  \param[in]  *pstKeyInfo: ��Կ������Ϣ
  \return 
  \retval NULL ʧ��
  \retval not NULL �½�key
*/
ATP_SEC_Key_ST* ATP_SEC_KM_CreateKey(const ATP_SEC_KeyInfo_ST *pstKeyInfo);

/*!
  \brief �ͷ�key
  \param[in]  *key: 
  \return 
*/
VOS_VOID ATP_SEC_KM_DeleteKey(ATP_SEC_Key_ST *key);

/*!
  \brief ɾ������key
  \param[in]  *key: 
  \return 
*/
VOS_VOID ATP_SEC_KM_RemoveAllKey(ATP_SEC_KeyPool_ST *keylist);


/*!
  \brief �ڹ����ڴ��д���ATP_SEC_DATA_PTR�ṹ,keyר��
  \param[in]  ulLen: 
  \return 
  \retval VOS_NULL ʧ��
*/
ATP_SEC_DATA_PTR ATP_SEC_KM_CreateKeyData(VOS_UINT32 ulLen);

/*!
  \brief �ͷ�ATP_SEC_DATA_PTR�ṹռ�ù����ڴ�
  \param[in]  pstKeyData: 
  \return 
*/
VOS_VOID  ATP_SEC_KM_DeleteKeyData(ATP_SEC_DATA_PTR pstKeyData);

/*!
  \brief key ��������
  \param[in]  key: 
  \param[in]  pstData: 
  \param[in]  ulAtrr: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_KM_AppendKeyAttribute(
                                ATP_SEC_Key_ST* key, 
                                ATP_SEC_DATA_PTR pstData, 
                                VOS_UINT32 ulAtrr);

/*!
  \brief ��ȡkey����
  \param[in]  *key: 
  \param[in]  ulAtrr: 
  \return 
  \retval VOS_NULL δ�ҵ�
*/
ATP_SEC_DATA_PTR ATP_SEC_KM_GetKeyAttribute(const ATP_SEC_Key_ST *key, VOS_UINT32 ulAtrr);

/*!
  \brief ����loadkey Driver
  \param[in]  pstKeysBuffer: 
  \param[in]  **ctx: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY �ڴ治��
  \retval VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_KM_CreateLoadKeyDriver(const ATP_SEC_DATA_PTR pstKeysBuffer, VOS_VOID **ctx);

/*!
  \brief ����key
  \param[in]  *ctx: 
  \param[in]  **keylist: 
  \return 
  \retval  VOS_OK �ɹ�
*/
VOS_UINT32 ATP_SEC_KM_LoadKey(VOS_VOID *ctx, ATP_SEC_KeyPool_ST **keylist);

/*!
  \brief 
  \param[in]  *ctx: 
  \return 
*/
VOS_VOID ATP_SEC_KM_DeleteLoadKeyDriver(VOS_VOID *ctx);

/*!
  \brief ����KeyGen context
  \param[in]  pstKeyInfo: ��Կ������Ϣ
  \param[in]  **ctx: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_SEC_KM_CreateGenKeyContext(
                                const ATP_SEC_KeyInfo_ST  *pstKeyInfo,
                                VOS_VOID            **ctx);

/*!
  \brief ������Կ
  \param[in]  *ctx: 
  \param[out]  **key: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_SEC_KM_GenarateKey(VOS_VOID *ctx, ATP_SEC_Key_ST **key);

/*!
  \brief ɾ��context
  \param[in]  *ctx: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_VOID ATP_SEC_KM_DeleteKeyGenContext(VOS_VOID *ctx);

/*!
  \brief �����ļ��洢key Driver
  \param[in]  *pucFile: 
  \param[out]  **ctx: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_SEC_KM_CreateFileStoreKeyDriver(ATP_UTIL_STRFILE_ST *pstStrFile, VOS_VOID **ctx);

/*!
  \brief �洢key
  \param[in]  *ctx: 
  \param[in]  *key: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_UINT32 ATP_SEC_KM_StoreKey(VOS_VOID *ctx, VOS_UINT32 ulKeyID, ATP_SEC_Key_ST *key);

/*!
  \brief ɾ��keyDriver
  \param[in]  *ctx: 
  \return 
  \retval VOS_OK�ɹ�
*/
VOS_VOID ATP_SEC_KM_DeleteStoreKeyDriver(VOS_VOID *ctx);

VOS_UINT32 ATP_SEC_KM_InsertKeyList(ATP_SEC_KeyPool_ST* pstKeylist);

VOS_UINT32 ATP_SEC_KM_RemoveKey(VOS_UINT32 ulKeyID);

VOS_UINT32 ATP_SEC_KM_LoadDeviceRuntimeKeys();

ATP_SEC_SSL_Cert_ST* ATP_SEC_SSL_GetCert(VOS_UINT32 ulSSLID);


#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
