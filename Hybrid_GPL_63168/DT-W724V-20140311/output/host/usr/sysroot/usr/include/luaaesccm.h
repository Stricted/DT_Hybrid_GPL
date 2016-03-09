/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 ******************************************************************************/
/**\file luaaesccm.h
 *
 *\copyright 2007-2020, HUAWEI
 *\author w00200480
 *\date  2013-10-10
 */

#ifndef __LUA_AES_CCM_H_INCLUDE__
#define __LUA_AES_CCM_H_INCLUDE__

#define ATP_WEB_CCM_IV_LEN      8
#define ATP_WEB_CCM_TAG_LEN     8
#define ATP_WEB_CCM_ADATA_LEN   8
#define ATP_WEB_CCM_KEY_LEN     16

#define AES_CCM_IV_OFFSET       16
#define AES_CCM_ADATA_OFFSET    32

#define GET_CIPHER_LEN(l)       ((((l)/16)*16)+16)

extern VOS_UINT8 g_derivedKey[];

/*!
  \brief: Convert hex buffer to binary buffer.
  \param[in] hexbuf: Hex buffer pointer.
  \param[in] buflen: Hex buffer length.
  \param[out] cbuf: Output buffer pointer.
  \param[out] clen: Output buffer length.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 HexBufToBytes(const VOS_UINT8 *pucHexbuf, VOS_UINT32 ulBuflen, VOS_UINT8 **ppucCbuf, VOS_UINT32 *ulClen);

/*!
  \brief: Convert binary buffer to hex buffer.
  \param[in] bytesbuf: Binary buffer pointer.
  \param[in] buflen: Binary buffer length.
  \param[out] hbuf: Output buffer pointer.
  \param[out] hlen: Output buffer length.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 BytesBufToHex(const VOS_UINT8 *pucBytesbuf, VOS_UINT32 ulBuflen, VOS_UINT8 **ppucHbuf, VOS_UINT32 *pucHlen);

/*!
  \brief: AES CCM 128 Encrypt.
  \param[in] ccmKey: Derived Key.(128 Bits)
  \param[in] ccmNonce: Nonce value.(Initial vector. 64 Bits)
  \param[in] ccmAdata: Authentication Data.(64 Bits)
  \param[in] ccmPt: Plain text.
  \param[in] ccmPtLen: The length of plain text.
  \param[out] ccmCt: The pointer of cipher text buffer.
  \param[out] ccmCtLen: The length of cipher text.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 WebAesCcmEncrypt(const VOS_UINT8 *pucCcmKey, const VOS_UINT8 *pucCcmNonce, const VOS_UINT8 *pucCcmAdata,
        const VOS_UINT8 *pucCcmPt, VOS_INT32 ulCcmPtLen, VOS_UINT8 **ppucCcmCt, VOS_INT32 *pucCcmCtLen);

/*!
  \brief: AES CCM 128 Decrypt.
  \param[in] ccmKey: Derived Key.(128 Bits)
  \param[in] ccmNonce: Nonce value.(Initial vector. 64 Bits)
  \param[in] ccmAdata: Authentication Data.(64 Bits)
  \param[in] ccmTag: Tag value.(64 Bits.)
  \param[in] ccmCt: Cipher text.
  \param[in] ccmCtLen: The length of cipher text.
  \param[out] ccmPt: The pointer of plain text buffer.
  \param[out] ccmPtLen: The length of plain text.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 WebAesCcmDecrypt(const VOS_UINT8 *pucCcmKey, const VOS_UINT8 *pucCcmNonce, const VOS_UINT8 *pucCcmAdata,
        VOS_UINT8 *pucCcmTag, const VOS_UINT8 *pucCcmCt, VOS_INT32 ulCcmCtLen, VOS_UINT8 **ppucCcmPt, VOS_INT32 *plCcmPtLen);

/*!
  \brief: Get AES CCM 128 IV.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 WebGetAesCcmIV(VOS_VOID);

/*!
  \brief: Get AES CCM 128 Authentication data.
  \return VOS_OK/VOS_NOK.
  */
extern VOS_UINT32 WebGetAesCcmAdata(VOS_VOID);

#ifdef ATP_DEBUG

#define ATP_AESCCM_DEBUG(format, args...)  { \
    printf("[==FILE:%s,LINE:%d==]", __FILE__, __LINE__); \
    printf(format, ##args);printf("\n"); \
}

/*!
  \brief: Print buffer of pucBytesbuf of ulBuflen.
  \param[in] pucBytesbuf: Binary buffer pointer.
  \param[in] ulBuflen: Binary buffer length.
  \param[out] cTag: The buffer information cTag.
  \return.
  */
extern VOS_VOID echoBitsBufHex(const VOS_UINT8 * pucBytesbuf, VOS_UINT32 ulBuflen, VOS_CHAR *cTag);

#else
#define echoBitsBufHex(buf, len, tag)
#define ATP_AESCCM_DEBUG(format, args...)
#endif

extern VOS_UINT8 g_WebAesCcmNonce[ATP_WEB_CCM_IV_LEN];
extern VOS_UINT8 g_WebAesCcmAdata[ATP_WEB_CCM_TAG_LEN];
extern const VOS_CHAR* g_cipherTxtJsonArray[];

#endif
