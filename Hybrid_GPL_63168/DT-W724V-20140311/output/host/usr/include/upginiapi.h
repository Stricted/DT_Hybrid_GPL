/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file upginiapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author s00175839
  *\date  2011-12-29
*/
/** config.ini file format 
  *  
  * [type] #16����
  * main_offset= #16����
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

//!\brief key-value��
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
  \brief ����Ini�ļ�
  \param[in]  *pucBuf: ini�ļ�����
  \param[in]  ulBufLen: �����С
  \param[out]  **ppstIniFile: ����ini�ṹ��
  \return 
  \retval VOS_OK �ɹ�
  \retval VOS_OK ʧ��
*/
unsigned int ATP_UPG_INI_ParseIniFile(
                                const unsigned char     *pucBuf,
                                unsigned int            ulBufLen,
                                ATP_UPG_INI_FILE_ST     **ppstIniFile);

/*!
  \brief ����Ini�ļ��ṹ
  \param[in]  *pstIniFile: ini�ṹ��
  \return 
  \retval void
*/
void ATP_UPG_INI_DestroyIniFile(ATP_UPG_INI_FILE_ST *pstIniFile);

/*!
  \brief get Ini�ļ�section
  \param[in]  *pstIniFile: ini�ṹ��
  \param[in]  ulType: ����
  \return 
  \retval NULL δ�ҵ�section
  \retval not NULL ATP_UPG_INI_SECTION_ST* section�ṹ��
*/
ATP_UPG_INI_SECTION_ST* ATP_UPG_INI_GetSectionByType(
                                const ATP_UPG_INI_FILE_ST *pstIniFile,
                                unsigned int ulType);

/*!
  \brief get Ini value
  \param[in]  *pstIniFile: ini�ṹ��
  \param[in]  ulType:  ����
  \param[in]  *key: ��ֵ
  \return 
  \retval NULLδ�ҵ�ֵ
  \retval not NULL ����key��Ӧvalue
*/
char* ATP_UPG_INI_GetValue(const ATP_UPG_INI_FILE_ST *pstIniFile, unsigned int ulType, const char *key);

/*!
  \brief get section value
  \param[in]  *pstSection: section�ṹ��
  \param[in]  *key: ��ֵ
  \return 
  \retval NULLδ�ҵ�ֵ
  \retval not NULL ����key��Ӧvalue

*/
char* ATP_UPG_INI_GetValueInSection(const ATP_UPG_INI_SECTION_ST *pstSection, const char *key);

/*!
  \brief get section offset & len
  \param[in]  *pstIniSection: section�ṹ��
  \param[in]  enMainorSlave: main or slave
  \param[out]  *pulOffset: ƫ��
  \param[out]  *pulLen: ����
  \return 
  \retval VOS_OK ��ȡ�ɹ�
  \retval VOS_OK ��ȡʧ��
*/
unsigned int ATP_UPG_INI_GetOffsetParam(const ATP_UPG_INI_SECTION_ST *pstIniSection, 
                                    ATP_UPG_INI_PARAM_EN enMainorSlave, 
                                    unsigned int *pulOffset,
                                    unsigned int *pulLen);
#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
