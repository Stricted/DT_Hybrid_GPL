/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file nascentermsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author l68874
  *\date  2012-6-7
*/
#ifndef __ATP_NASCENTER_MSGTYPES_H__
#define __ATP_NASCENTER_MSGTYPES_H__

#include "atptypes.h"

#define ATP_CBBID_NASCENTER_NAME                  "nascenter"

#define ATP_NAS_MAX_SYNC_JOB_NUM                                 (10)
#define ATP_NAS_MAX_BACKUP_JOB_NUM                             (10)

#define ATP_NAS_CHECK_CONNT_MEDIENCENTER_TIME       (5) /*定时检查mediencenter连接状态，单位分钟，后面要改成可配置 */


#define ATP_NAS_MAX_MEDCENTER_EMAIL_LENGTH             (256)/*web 页面最长能输入255个字符*/
#define ATP_NAS_MAX_MEDCENTER_PASSWORD_LENGTH     (256)/*web 页面最长能输入255个字符*/

#define ATP_NAS_MAX_FOLDER_PATH_LENGTH                     (512+4)/*页面上是512*/

#define ATP_NAS_MAX_LOGGICVOLNAME_LENGTH                 (64)/*最大逻辑分区名长*/

/*已Mediencenter 开头 或以/Mediencenter 开头*/
#define IS_MEDIENCENTER_FLODER(pcFolder)                      ((pcFolder) == strstr((pcFolder),"Mediencenter") ||(pcFolder) == strstr((pcFolder),"/Mediencenter"))   
#define IS_HDD_FLODER(pcFolder)                                        (VOS_FALSE == IS_MEDIENCENTER_FLODER(pcFolder) ) 



typedef enum tagATP_E_NASCENTER_FAIL_REASON
{
    ATP_E_ERR_NAS_SUCCESS = 0,//=VOS_OK
    //fail reason
    ATP_E_ERR_NAS_EXECUTE_FAIL,//=VOS_NOK
    ATP_E_ERR_NAS_NOT_ACCESS,
    ATP_E_ERR_NAS_BUSY,
    ATP_E_ERR_NAS_CENTER_DISCONN,
    ATP_E_ERR_NAS_WAN_DOWN,
    ATP_E_ERR_NAS_HDD_DISCONN,
    ATP_E_ERR_NAS_JOB_STOPPED,
    ATP_E_ERR_NAS_END,
} ATP_NASCENTER_FAIL_REASON_EN;


typedef struct tagATP_NAS_MEDCENTER_USER_ST
{
    VOS_BOOL            bAllowConnect;
    VOS_CHAR            acEmail[ATP_NAS_MAX_MEDCENTER_EMAIL_LENGTH];
    VOS_CHAR            acPassword[ATP_NAS_MAX_MEDCENTER_PASSWORD_LENGTH];
}ATP_NAS_MEDCENTER_USER_ST;

enum ATP_NASCENTER_MSGTYPES_EN
{
    ATP_MSG_NAS_CONNECT_MEDIENCENTER = ATP_MSG_CATEGORY_DEF(ATP_MODULE_NASCENTER),/*msg data 中为isConnect*/
    ATP_MSG_NAS_START_JOBCENTER,   /*nas cms自己用*/
    ATP_MSG_NAS_SYNC_ADD,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_SYNC_SET,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_SYNC_ACTIVE,/*用户按了active选项，msg data 中为ulJobInstId*/
    ATP_MSG_NAS_SYNC_DEL,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_SYNC_START,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_SYNC_STOP,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_BACKUP_ADD,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_BACKUP_SET,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_BACKUP_ACTIVE,/*用户按了active选项，msg data 中为ulJobInstId*/
    ATP_MSG_NAS_BACKUP_DEL,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_BACKUP_START,/*msg data 中为ulJobInstId*//*a*/
    ATP_MSG_NAS_BACKUP_STOP,/*msg data 中为ulJobInstId*/
    ATP_MSG_NAS_RESTORE_START,/*msg data 中为ATP_NAS_RESTORE_MSG_ST*/
    ATP_MSG_NAS_RESTORE_STOP,/*msg data 中为ulJobInstId*/

    ATP_MSG_NAS_USB_MOUNT,/**/

    ATP_MSG_NAS_WAN_UP,/*msg data 中为TRUE,表up  为FALSE表Down*/
    //ATP_MSG_NAS_WAN_DOWN,/*msg data 中为ulJobInstId*/
    /*以上是nascms -> jobcente消息*/

     /*jobcente -> nascms消息*/   
    ATP_MSG_NAS_JOBCENTER_EXIT,
    /*sync->jobcenter  start*/
    ATP_MSG_NAS_SYNC_RESULT, /*msg data 中为VOS_OK表示成功，其他表失败id，不考虑失败原因，VOS_NOK表失败*/
                                                       /*ATP_MSG_HEADER_ST后紧跟的4个字节放 VOS_UINT32 ulJobInstId --为实例ID*/ 
    ATP_MSG_NAS_BACKUP_RESULT,/*msg data 中为VOS_OK表示成功，其他表失败id(ATP_NASCENTER_FAIL_REASON_EN)，不考虑失败原因，VOS_NOK表失败*/
                                                       /*ATP_MSG_HEADER_ST后紧跟的ATP_NAS_BACKUP_RESULT_ST结构*/ 
    ATP_MSG_NAS_SNTP_UPDATE_TIME,
    /*sync->jobcente  end*/
	
	ATP_MSG_NAS_INIT_SUCCESS,
};

#endif // End of __ATP_NASCENTER_MSGTYPES_H__

