/** \file cfmcoremsgtypes.h
  *CFM message interface
  *\copyright  2007-2020，华为技术有限公司
  *\author      z65940
  *\version     V1.0
  *\date         2011-12-17
*/
#ifndef __ATP_CFMCORE_MSGTYPES_H__
#define __ATP_CFMCORE_MSGTYPES_H__

//!  CFM模块的名称，CFM不是独立进程，只是一个动态库，因此不要往此处发消息
#define ATP_CBBID_CFM_NAME                  "cfm"

//!  CFMA使用到的消息类型
enum ATP_CFMCORE_MSGTYPES_EN
{
    ATP_MSG_CFMCORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CFMCORE),
    ATP_E_CFGMSG_BGN = ATP_MSG_CFMCORE_START,   /*!< CFMA消息开始 */
    ATP_E_CFGMSG_CHK,       /*!< CFMA的CHECK消息类型 */
    ATP_E_CFGMSG_SET,       /*!< CFMA的SET消息类型 */
    ATP_E_CFGMSG_GET,       /*!< CFMA的GET消息类型 */
    ATP_E_CFGMSG_ADD,       /*!< CFMA的ADD消息类型 */
    ATP_E_CFGMSG_DEL,       /*!< CFMA的DEL消息类型 */
    ATP_E_CFGMSG_INSTNUM,   /*!< CFMA的GETINST消息类型 */
    ATP_E_CFGMSG_VALUECHG,  /*!< CFMA的VALUECHAGE消息类型 */
    ATP_MSG_VALUE_CHANGE = ATP_E_CFGMSG_VALUECHG,
    ATP_E_CFGMSG_END,       /*!< CFMA消息结尾 */
    ATP_MSG_CMS_DMMSG,      /*!< CFM外置参数模型消息类型 */
    ATP_MSG_CFG_UPDATE,
    ATP_MSG_CMS_START_TRANSACTION,  /*!< cms  start transaction */
    ATP_MSG_CMS_END_TRANSACTION,    /*!< cms  end transaction */
    ATP_MSG_CMS_DBITERATE_LOCK, /* cms get data lock*/
} ;
#endif // End of __ATP_CFMCORE_MSGTYPES_H__

