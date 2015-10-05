/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file upginiapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2011-12-29
*/
/** config.ini file format 
  *  
  * [type] #16进制
  * main_offset= #16进制
  * main_len=
  * slave_offset=
  * slave_len=
*/
#ifndef __UPG_INI_API_H
#define __UPG_INI_API_H

#define ATP_UPG_INI_SECTION_GLOBAL_NAME   "global"
#define ATP_UPG_INI_SECTION_GLOBAL_TYPE   0xffffffff
#define ATP_UPG_INI_SECTION_START         '['
#define ATP_UPG_INI_SECTION_END           ']'

#define ATP_UPG_INI_GLOBAL_KEY_BASE       "base"
#define ATP_UPG_INI_GLOBAL_KEY_SECTOR     "sector"
#define ATP_UPG_INI_GLOBAL_KEY_BOARD_ID     "board_id"
#define ATP_UPG_INI_GLOBAL_KEY_DOUBLE_SYSTEM    "system_type"
#define ATP_UPG_INI_GLOBAL_KEY_MULTIUPGTYPE    "multi_upg_start_type"
#define ATP_HGW_SOFTVERSION    "hgw_softversion"
#define ATP_HGW_SOFTVERSION_WEB    "hgw_softversion_web"

#define ATP_UPG_INI_TYPE_KEY_MAIN_OFFSET  "main_offset"
#define ATP_UPG_INI_TYPE_KEY_MAIN_LEN     "main_size"
#define ATP_UPG_INI_TYPE_KEY_SLAVE_OFFSET "slave_offset"
#define ATP_UPG_INI_TYPE_KEY_SLAVE_LEN    "slave_size"

#define ATP_UPG_INI_KEY_LEN        32
#define ATP_UPG_INI_VALUE_LEN      32

typedef enum
{
    INI_MAIN_PARAM,
    INI_SLAVE_PARAM
}ATP_UPG_INI_PARAM_EN;

//!\brief key-value对
/*!
 *
 */
typedef struct tagATP_UPG_INI_ITEM_ST
{
    char key[ATP_UPG_INI_KEY_LEN];
    char value[ATP_UPG_INI_VALUE_LEN];
    struct tagATP_UPG_INI_ITEM_ST* pstNext;
    struct tagATP_UPG_INI_SECTION_ST* pstParent;
}ATP_UPG_INI_ITEM_ST;

typedef struct tagATP_UPG_INI_SECTION_ST
{
    unsigned int ulType;
    unsigned int ulItemNum;
    ATP_UPG_INI_ITEM_ST *pstItemList;
    struct tagATP_UPG_INI_SECTION_ST *pstNext;
}ATP_UPG_INI_SECTION_ST;

typedef struct tagATP_UPG_INI_FILE_ST
{
    unsigned int ulSectionNum;
    ATP_UPG_INI_SECTION_ST *pstSectionList;
}ATP_UPG_INI_FILE_ST;

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

unsigned int ATP_UPG_INI_GetLine(const unsigned char     *pucBuf,
                               unsigned int          ulBufLen,
                               unsigned int          *pulHasReadLen,
                               char                *acLine,
                               unsigned int          ulLen);

/*!
  \brief 解析Ini文件
  \param[in]  *pucBuf: ini文件缓存
  \param[in]  ulBufLen: 缓存大小
  \param[out]  **ppstIniFile: 返回ini结构体
  \return 
  \retval VOS_OK 成功
  \retval VOS_OK 失败
*/
unsigned int ATP_UPG_INI_ParseIniFile(
                                const unsigned char     *pucBuf,
                                unsigned int            ulBufLen,
                                ATP_UPG_INI_FILE_ST     **ppstIniFile);

/*!
  \brief 销毁Ini文件结构
  \param[in]  *pstIniFile: ini结构体
  \return 
  \retval void
*/
void ATP_UPG_INI_DestroyIniFile(ATP_UPG_INI_FILE_ST *pstIniFile);

/*!
  \brief get Ini文件section
  \param[in]  *pstIniFile: ini结构体
  \param[in]  ulType: 类型
  \return 
  \retval NULL 未找到section
  \retval not NULL ATP_UPG_INI_SECTION_ST* section结构体
*/
ATP_UPG_INI_SECTION_ST* ATP_UPG_INI_GetSectionByType(
                                const ATP_UPG_INI_FILE_ST *pstIniFile,
                                unsigned int ulType);

/*!
  \brief get Ini value
  \param[in]  *pstIniFile: ini结构体
  \param[in]  ulType:  类型
  \param[in]  *key: 键值
  \return 
  \retval NULL未找到值
  \retval not NULL 返回key对应value
*/
char* ATP_UPG_INI_GetValue(const ATP_UPG_INI_FILE_ST *pstIniFile, unsigned int ulType, const char *key);

/*!
  \brief get section value
  \param[in]  *pstSection: section结构体
  \param[in]  *key: 键值
  \return 
  \retval NULL未找到值
  \retval not NULL 返回key对应value

*/
char* ATP_UPG_INI_GetValueInSection(const ATP_UPG_INI_SECTION_ST *pstSection, const char *key);

/*!
  \brief get section offset & len
  \param[in]  *pstIniSection: section结构体
  \param[in]  enMainorSlave: main or slave
  \param[out]  *pulOffset: 偏移
  \param[out]  *pulLen: 长度
  \return 
  \retval VOS_OK 获取成功
  \retval VOS_OK 获取失败
*/
unsigned int ATP_UPG_INI_GetOffsetParam(const ATP_UPG_INI_SECTION_ST *pstIniSection, 
                                    ATP_UPG_INI_PARAM_EN enMainorSlave, 
                                    unsigned int *pulOffset,
                                    unsigned int *pulLen);
#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
