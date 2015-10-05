/** \file cfmcoremsgtypes.h
  *CFM message interface
  *\copyright  2007-2020����Ϊ�������޹�˾
  *\author      z65940
  *\version     V1.0
  *\date         2011-12-17
*/
#ifndef __ATP_CFMCORE_MSGTYPES_H__
#define __ATP_CFMCORE_MSGTYPES_H__

//!  CFMģ������ƣ�CFM���Ƕ������̣�ֻ��һ����̬�⣬��˲�Ҫ���˴�����Ϣ
#define ATP_CBBID_CFM_NAME                  "cfm"

//!  CFMAʹ�õ�����Ϣ����
enum ATP_CFMCORE_MSGTYPES_EN
{
    ATP_MSG_CFMCORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CFMCORE),
    ATP_E_CFGMSG_BGN = ATP_MSG_CFMCORE_START,   /*!< CFMA��Ϣ��ʼ */
    ATP_E_CFGMSG_CHK,       /*!< CFMA��CHECK��Ϣ���� */
    ATP_E_CFGMSG_SET,       /*!< CFMA��SET��Ϣ���� */
    ATP_E_CFGMSG_GET,       /*!< CFMA��GET��Ϣ���� */
    ATP_E_CFGMSG_ADD,       /*!< CFMA��ADD��Ϣ���� */
    ATP_E_CFGMSG_DEL,       /*!< CFMA��DEL��Ϣ���� */
    ATP_E_CFGMSG_INSTNUM,   /*!< CFMA��GETINST��Ϣ���� */
    ATP_E_CFGMSG_VALUECHG,  /*!< CFMA��VALUECHAGE��Ϣ���� */
    ATP_MSG_VALUE_CHANGE = ATP_E_CFGMSG_VALUECHG,
    ATP_E_CFGMSG_END,       /*!< CFMA��Ϣ��β */
    ATP_MSG_CMS_DMMSG,      /*!< CFM���ò���ģ����Ϣ���� */
    ATP_MSG_CFG_UPDATE,
    ATP_MSG_CMS_START_TRANSACTION,  /*!< cms  start transaction */
    ATP_MSG_CMS_END_TRANSACTION,    /*!< cms  end transaction */
    ATP_MSG_CMS_DBITERATE_LOCK, /* cms get data lock*/
} ;
#endif // End of __ATP_CFMCORE_MSGTYPES_H__

