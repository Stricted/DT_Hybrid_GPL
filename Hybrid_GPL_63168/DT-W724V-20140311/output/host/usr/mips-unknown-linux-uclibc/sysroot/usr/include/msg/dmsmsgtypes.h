/**\file dmsmsgtypes.h
  *dms 需要发送给cms或者业务模块的消息
  *\copyright 2007-2020，华为技术有限公司
  *\author w00200480
  *\date  2012-2-2
*/
#ifndef __ATP_DMS_MSGTYPES_H__
#define __ATP_DMS_MSGTYPES_H__

#define ATP_CBBID_DMS_NAME "dms"
#define ATP_CBBID_SCANNER_NAME "scanner"

enum DMS_MSG_TYPE_EN
{
    ATP_MSG_DMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DMS),
    ATP_MSG_DMS_STOP,
    ATP_MSG_DMS_INIT,
    ATP_MSG_DMS_SCANNER_INIT_OK,
    ATP_MSG_DMS_SCANNER_SCAN_OK,
    ATP_MSG_DMS_SCANNER_SCAN_COMPLETE,
    ATP_MSG_DMS_SCANNER_UPDATE
} ;

#endif
