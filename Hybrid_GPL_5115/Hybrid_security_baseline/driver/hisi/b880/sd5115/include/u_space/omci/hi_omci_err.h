/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_err.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_10_27

******************************************************************************/
#ifndef __HI_OMCI_ERR_H__
#define __HI_OMCI_ERR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* OMCI模块错误码 */
typedef enum 
{
    HI_RET_OMCI_BASE                     = ( HI_SRCMODULE_APPS_OMCI ),
    HI_RET_OMCI_SQL_RETBASE_E            = ( HI_RET_OMCI_BASE | 0x100 ),/* SQL错误码基地址 */
    HI_RET_OMCI_SEND_ERR_E               = ( HI_RET_OMCI_BASE | 0x001 ),/* OMCI发送失败 */
    HI_RET_OMCI_UNKNOWN_MSG_E            = ( HI_RET_OMCI_BASE | 0x002 ),/* 未知的消息类型*/
    HI_RET_OMCI_REPEAT_PACKET_E          = ( HI_RET_OMCI_BASE | 0x003 ),/* 重复OMCI报文消息 */
    HI_RET_OMCI_ALARM_EXIST_E            = ( HI_RET_OMCI_BASE | 0x004 ),/* 告警已经存在 */
    HI_RET_OMCI_OPEN_DBBASE_E            = ( HI_RET_OMCI_BASE | 0x005 ),/* 打开数据库错误*/
    HI_RET_OMCI_READ_DATABSE_E           = ( HI_RET_OMCI_BASE | 0x006 ),/* 读数据库失败 */
    HI_RET_OMCI_WRITER_DATABSE_E         = ( HI_RET_OMCI_BASE | 0x007 ),/* 写数据库失败 */
    HI_RET_OMCI_RULE_NOT_SUPPORTED_E     = ( HI_RET_OMCI_BASE | 0x008 ),/* 不支持此项规则定义的动作 */
    HI_RET_OMCI_SQL_NODATA_E             = ( HI_RET_OMCI_BASE | 0x009 ),/* 没有数据*/
    HI_RET_OMCI_SQL_TABLE_UNEXISTED_E    = ( HI_RET_OMCI_BASE | 0x00A ),/* 表项不存在*/
    HI_RET_OMCI_SQL_TABLE_EXISTED_E      = ( HI_RET_OMCI_BASE | 0x00B ),/* 表项已存在*/
    HI_RET_OMCI_SQL_NORIGHT_E            = ( HI_RET_OMCI_BASE | 0x00C ),/* 没有权限*/
    HI_RET_OMCI_MIB_DONT_UPDATEDB_E      = ( HI_RET_OMCI_BASE | 0x00D ),/* 不需要刷新数据库*/
    HI_RET_OMCI_UPGRADE_FILEPATH_E       = ( HI_RET_OMCI_BASE | 0x00E ),/* 文件路径或名称错误*/
}hi_omci_err_e;

#define HI_OMCI_SQL_RET(ret)  (HI_RET_OMCI_SQL_RETBASE_E | ret )


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_ERR_H__ */
