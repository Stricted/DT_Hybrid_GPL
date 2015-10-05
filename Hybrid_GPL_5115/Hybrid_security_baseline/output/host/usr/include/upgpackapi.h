/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>      <时间>        <问题单号>             <简要描述>
l00205975     2012/12/11    DTS2012121101724        【WEB】[B890]V100R001C994B116(router):WEB版本编译分离
========================================================================================*/
/**\file upgpackapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2011-12-27
*/
#ifndef __UPG_PACKET_API_H__
#define __UPG_PACKET_API_H__

#ifdef WIN32
#define snprintf        _snprintf
#define strncasecmp     strnicmp
#endif

//! 支持的子文件个数
#define ATP_MAX_LAYER_NUM           (16)         

//! 升级策略文件类型,只能有一个策略文件
#define ATP_UPG_TYPE_POLICY_FILE    0x00010000  
//! 升级镜像文件类型                                               
#define ATP_UPG_TYPE_IMAGE_FILE     0x00020000
//! 升级配置文件文件类型
#define ATP_UPG_TYPE_CFG_FILE       0x00030000
//! 升级文件类型掩码
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

//组播升级工厂所需配置文件
#define ATP_UPG_TYPE_IMAGE_TEMP_FACTORYCFG           (ATP_UPG_TYPE_IMAGE_FILE + 0X4)
/* DTS2012121101724 l00205975 20121211 end> */
/* 升级web模块 */
#define ATP_UPG_TYPE_IMAGE_WEB              (ATP_UPG_TYPE_IMAGE_FILE + 0x20)
/* DTS2012121101724 l00205975 20121211 end> */
#define ATP_UPG_TYPE_CFG_INI        (ATP_UPG_TYPE_CFG_FILE + 1)
#define ATP_UPG_TYPE_CFG_ERASE      (ATP_UPG_TYPE_CFG_FILE + 2)

#define ATP_UPG_INT_LEN             (4)
#define ATP_UPG_STR_LEN             (32)
#define ATP_UPG_DATE_LEN            (16)


//! PKG magic number
#define ATP_UPG_MAGIC_NUM           (0x10325476)

//!\brief Hash算法
/*!
 * 打包使用的Hash算法
 */
enum
{
    MD5WithRSA        = 0,
    SHA2WithRSA       = 1,
    SHA4WithRSA       = 2,
    HASH_MAX
};

//!\brief 签名采用RSA算法位数
/*!
 *
 */
enum
{
    RSA_BITS_2048   = 0,
    RSA_BITS_1024   = 1
};

//!\brief 每个子文件的信息
/*!
 * 
 */
typedef struct tagATP_UPG_LAYER_ST
{
    unsigned char                   aucFileType[ATP_UPG_INT_LEN];   /*!< 软件类型 */    
    unsigned char                   aucFileOffset[ATP_UPG_INT_LEN]; /*!< 偏移地址 */
    unsigned char                   aucFileLen[ATP_UPG_INT_LEN];    /*!< 软件长度 */
    unsigned char                   aucSoftVersion[ATP_UPG_STR_LEN];/*!< 软件版本号 */
} ATP_UPG_LAYER_ST;

//!\brief 签名信息
/*!
 * 打包签名信息
 */
typedef struct tagATP_UPG_SIGNINFO_ST
{    
    unsigned char                   ucHasSign;      /*!< 是否签名 */    
    unsigned char                   ucRSABits;      /*!< RSA位 */     
    unsigned char                   ucHashAlgo;     /*!< hash算法 */
    unsigned char                   ucReserved;     /*!< 保留 */
} ATP_UPG_SIGNINFO_ST;

#define ATP_UPG_SIGNATURE_ISSUER_SIZE (ATP_UPG_DATE_LEN + ATP_UPG_DATE_LEN + ATP_UPG_STR_LEN)
//!\brief 签名结构
/*!
 * 打在包尾的签名数据结构
 */ 
typedef struct tagATP_UPG_SIGNATURE_ST
{    
    unsigned char                   aucNotBefore[ATP_UPG_DATE_LEN]; /*!< not before utc yyyyMMddhhmmss */  
    unsigned char                   aucNotAfter[ATP_UPG_DATE_LEN];  /*!< not after utc yyyyMMddhhmmss */
    unsigned char                   aucIssuer[ATP_UPG_STR_LEN];     /*!< issuer */    
    unsigned char                   *pucSignature;                  /*!< 签名数据 */
} ATP_UPG_SIGNATURE_ST;


//!\brief 打包文件头格式
/*!
 *
 */
typedef struct tagATP_UPG_FILE_HADER_ST
{
    unsigned char                   aucMagicNum[ATP_UPG_INT_LEN];    /*!< 魔鬼数字 */ 
    unsigned char                   aucProductName[ATP_UPG_STR_LEN]; /*!< 产品名称 */ 
    unsigned char                   aucSoftVersion[ATP_UPG_STR_LEN]; /*!< 软件版本信息 */ 
    ATP_UPG_SIGNINFO_ST             stSignInfo;                      /*!< 签名信息 */
    unsigned char                   aucReserved[ATP_UPG_INT_LEN];    /*!< 保留 */ 
    unsigned char                   aucPkgNum[ATP_UPG_INT_LEN];      /*!< 子文件数 */ 
    ATP_UPG_LAYER_ST                astLayout[ATP_MAX_LAYER_NUM];    /*!< 每个子文件头信息 */                                                           
    unsigned char                   aucHeadCrc[ATP_UPG_INT_LEN];     /*!< CRC */ 
} ATP_UPG_FILE_HADER_ST;

struct tagATP_UPG_FILE_ST;

//!\brief 解析后的子文件结构体
/*!
 *
 */
typedef struct tagATP_UPG_SUBFILE_ST
{
    unsigned int                      ulPkgType;    /*!< 解析后的PkgType */    // 
    unsigned int                      ulPkgOffset;  /*!< 解析后的PkgOffset */   // 
    unsigned int                      ulPkgLen;      /*!< 解析后的PkgLen */   // 
    char                              *pcVersion;   /*!< 软件版本号 */    // 
    char                              *pcUrl;       /*!< 下载URL */    // 
    unsigned char                     ucNeedUpdate; /*!< 是否需要升级 0：需要；1：不需要 */    
    void                              *pvArg;    /*!< 扩展指针 */     
    struct tagATP_UPG_FILE_ST       *pstParent; /*!< 父结构指针 */ 
    struct tagATP_UPG_SUBFILE_ST    *pstNext; /*!< 下一个子文件指针 */ 
} ATP_UPG_SUBFILE_ST;

//!\brief 解析后的多系统文件结构体
/*!
 * \param pcVersion:
 */
typedef struct tagATP_UPG_FILE_ST
{
    char                  *pcVersion;     /*!< 软件版本号 */
    char                  *pcProductInfo; /*!< 产品名称 */ 
    char                  *pcStatusUrl;   /*!< 状态URL */ 
    unsigned int          bUnknownFormat; /*!< 打包格式是否支持 */ 
    ATP_UPG_SIGNINFO_ST   stSignInfo;
    void                  *pvArg;         /*!< 扩展指针 */   
    ATP_UPG_SUBFILE_ST    *pstSubFiles;   /*!< 子文件列表 */   
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
  \brief 根据子文件类型获取子文件结构体
  \param[in]  ulFileType: 文件类型
  \param[in]  *pstUpgFile: 多系统结构体
  \return 
  \retval NULL 未找到
  \retval Not NULL 子文件结构体支持
*/
ATP_UPG_SUBFILE_ST *ATP_UPG_GetSubFileByType(
                                 unsigned int        ulFileType,
                                 const ATP_UPG_FILE_ST     *pstUpgFile);

/*!
  \brief 销毁多系统结构体
  \param[in]  *pstUpgFile: 待销毁的多系统结构体
  \return 
  \retval void
*/
void ATP_UPG_DestroyUpgFile(ATP_UPG_FILE_ST     *pstUpgFile);



/*!
  \brief 读取一个文件到缓冲区中
  \param[in]  *pcFileName: 待读取的文件名
  \param[in]  **ppucFileBuf: 存放文件中数据的缓冲区指针
  \param[in]  *pulFileLen: 存放文件长度的指针
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
  \brief 计算校验和
  \param[in]  *pucSrcBuf: 原始缓冲区的首地址
  \param[in]  ulSrcLen: 原始缓冲区的长度
  \param[in]  *pucDstBuf: 存放CRC计算结果的缓冲区
  \param[in]  ulDstLen: 存放CRC计算结果的缓冲区长度
  \return 
  \retval VOS_OK 成功 
  \retval VOS_NOK 失败
*/
int ATP_UPG_UtilCalcCheckSum(
                                const unsigned char     *pucSrcBuf,
                                unsigned int      ulSrcLen,
                                unsigned char     *pucDstBuf,
                                unsigned int      ulDstLen);

/*!
  \brief 解析公用频道的组播包
  \param[in]  *pucMcastHeader: 组播报文缓冲区首地址
  \param[in]  ulMcastHeaderLen: 组播报文长度
  \param[in]  **ppstUpgFile: 解析后的多系统结构体
  \return 
  \retval VOS_OK 成功 
  \retval VOS_NOK 失败
*/
unsigned int ATP_UPG_ParseMcastHeader(
                                const unsigned char     *pucMcastHeader,
                                unsigned int       ulMcastHeaderLen,
                                ATP_UPG_FILE_ST    **ppstUpgFile);

/*!
  \brief 解析打包的文件头
  \param[in]  *pucBuf: 待解析度文件内容缓冲区
  \param[in]  ulBufLen: 待解析度文件内容缓冲区长度
  \param[in]  **ppstUpgFile: 解析后的多系统文件结构体
  \return 
  \retval VOS_OK 成功 
  \retval VOS_NOK 失败
*/
unsigned int ATP_UPG_ParsePkgFileHeader(
                                const unsigned char     *pucBuf,
                                unsigned int      ulBufLen,
                                ATP_UPG_FILE_ST   **ppstUpgFile);

/*!
  \brief 校验打包的文件头
  \param[in]  *pucBuf: 待解析度文件内容缓冲区
  \param[in]  ulBufLen: 待解析度文件内容缓冲区长度
  \return 
  \retval VOS_OK 成功 
  \retval VOS_NOK 失败
*/
unsigned int ATP_UPG_VerifyPkgHeader(
                                const unsigned char  *pucBuf,
                                unsigned int         ulBufLen);

#ifdef __cplusplus
}
#endif

#endif


