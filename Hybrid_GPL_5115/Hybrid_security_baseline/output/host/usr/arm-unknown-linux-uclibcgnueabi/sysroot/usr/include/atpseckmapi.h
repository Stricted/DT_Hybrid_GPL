/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpseckmapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
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
  \brief 密钥信息加密后分散6段存储，获取第一段
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK成功
*/
extern VOS_UINT32 ATP_GetInfo1(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief 密钥信息加密后分散6段存储，获取第二段
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK成功
*/
extern VOS_UINT32 ATP_GetInfo2(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief 密钥信息加密后分散6段存储，获取第三段
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK成功
*/
extern VOS_UINT32 ATP_GetInfo3(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);
/*!
  \brief 密钥信息加密后分散6段存储，获取第四段
  \param[in]  **pucBuffer: 
  \param[in]  *pulLen: 
  \return 
  \retval VOS_OK成功
*/
extern VOS_UINT32 ATP_GetInfo4(const VOS_UINT8 **pucBuffer, VOS_UINT32 *pulLen);

/*!
  \brief 插入新key
  \param[in]  ulKeyID: 
  \param[in]  *key: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY 共享内存不足
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_SEC_KM_AddKey(VOS_UINT32 ulKeyID, ATP_SEC_Key_ST *key);

/*!
  \brief 创建key
  \param[in]  *pstKeyInfo: 密钥基本信息
  \return 
  \retval NULL 失败
  \retval not NULL 新建key
*/
ATP_SEC_Key_ST* ATP_SEC_KM_CreateKey(const ATP_SEC_KeyInfo_ST *pstKeyInfo);

/*!
  \brief 释放key
  \param[in]  *key: 
  \return 
*/
VOS_VOID ATP_SEC_KM_DeleteKey(ATP_SEC_Key_ST *key);

/*!
  \brief 删除所有key
  \param[in]  *key: 
  \return 
*/
VOS_VOID ATP_SEC_KM_RemoveAllKey(ATP_SEC_KeyPool_ST *keylist);


/*!
  \brief 在共享内存中创建ATP_SEC_DATA_PTR结构,key专用
  \param[in]  ulLen: 
  \return 
  \retval VOS_NULL 失败
*/
ATP_SEC_DATA_PTR ATP_SEC_KM_CreateKeyData(VOS_UINT32 ulLen);

/*!
  \brief 释放ATP_SEC_DATA_PTR结构占用共享内存
  \param[in]  pstKeyData: 
  \return 
*/
VOS_VOID  ATP_SEC_KM_DeleteKeyData(ATP_SEC_DATA_PTR pstKeyData);

/*!
  \brief key 增加属性
  \param[in]  key: 
  \param[in]  pstData: 
  \param[in]  ulAtrr: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY 内存不足
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_SEC_KM_AppendKeyAttribute(
                                ATP_SEC_Key_ST* key, 
                                ATP_SEC_DATA_PTR pstData, 
                                VOS_UINT32 ulAtrr);

/*!
  \brief 获取key属性
  \param[in]  *key: 
  \param[in]  ulAtrr: 
  \return 
  \retval VOS_NULL 未找到
*/
ATP_SEC_DATA_PTR ATP_SEC_KM_GetKeyAttribute(const ATP_SEC_Key_ST *key, VOS_UINT32 ulAtrr);

/*!
  \brief 创建loadkey Driver
  \param[in]  pstKeysBuffer: 
  \param[in]  **ctx: 
  \return 
  \retval ATP_ERR_SEC_NO_MEMORY 内存不足
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_SEC_KM_CreateLoadKeyDriver(const ATP_SEC_DATA_PTR pstKeysBuffer, VOS_VOID **ctx);

/*!
  \brief 加载key
  \param[in]  *ctx: 
  \param[in]  **keylist: 
  \return 
  \retval  VOS_OK 成功
*/
VOS_UINT32 ATP_SEC_KM_LoadKey(VOS_VOID *ctx, ATP_SEC_KeyPool_ST **keylist);

/*!
  \brief 
  \param[in]  *ctx: 
  \return 
*/
VOS_VOID ATP_SEC_KM_DeleteLoadKeyDriver(VOS_VOID *ctx);

/*!
  \brief 创建KeyGen context
  \param[in]  pstKeyInfo: 密钥基本信息
  \param[in]  **ctx: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_SEC_KM_CreateGenKeyContext(
                                const ATP_SEC_KeyInfo_ST  *pstKeyInfo,
                                VOS_VOID            **ctx);

/*!
  \brief 生成密钥
  \param[in]  *ctx: 
  \param[out]  **key: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_SEC_KM_GenarateKey(VOS_VOID *ctx, ATP_SEC_Key_ST **key);

/*!
  \brief 删除context
  \param[in]  *ctx: 
  \return 
  \retval VOS_OK成功
*/
VOS_VOID ATP_SEC_KM_DeleteKeyGenContext(VOS_VOID *ctx);

/*!
  \brief 创建文件存储key Driver
  \param[in]  *pucFile: 
  \param[out]  **ctx: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_SEC_KM_CreateFileStoreKeyDriver(ATP_UTIL_STRFILE_ST *pstStrFile, VOS_VOID **ctx);

/*!
  \brief 存储key
  \param[in]  *ctx: 
  \param[in]  *key: 
  \return 
  \retval VOS_OK成功
*/
VOS_UINT32 ATP_SEC_KM_StoreKey(VOS_VOID *ctx, VOS_UINT32 ulKeyID, ATP_SEC_Key_ST *key);

/*!
  \brief 删除keyDriver
  \param[in]  *ctx: 
  \return 
  \retval VOS_OK成功
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
