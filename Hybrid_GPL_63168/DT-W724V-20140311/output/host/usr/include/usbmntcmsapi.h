/**\file usbmntcmsapi.h
  *usbmount cms外部API提供
  *\copyright 2007-2020，华为技术有限公司
  *\author y00183935
  *\date  2012-4-5
*/
#ifndef __USBMNTCMSAPI_H__
#define __USBMNTCMSAPI_H__


typedef struct tagATP_USBSTORAGECMS_MONITOR_MOUNTINFO_ST
{
    VOS_INT32                                   lAllMountedDiskSubareaNum;
    VOS_INT32                                   lRecentlyMountDiskSubareaNum;
    VOS_INT32                                   lRecentlyUmountDiskSubareaNum;
} ATP_USBSTORAGECMS_MONITOR_MOUNTINFO_ST;


typedef enum tagATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_EN
{
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_START       = 0,
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_FTPD        = ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_START,
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_SAMBA,
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_TR140,
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_DMS,
    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_SERVICE,

    ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_END,
} ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_EN;


//!...........................................外部api接口start.................................................
#if defined(__cplusplus)
extern "C" {
#endif
VOS_UINT32 ATP_USBMount_Init(VOS_VOID);

/*!
  \brief 注册usbmount disksubarea refresh 回调函数
  \param[in]  pfProc: 回调函数
  \param[in]  enPriority: 优先级
  \param[in]  pvNotifyHook: 参数
  \return VOS_OK 表示命令执行成功，其它表示不成功
*/
VOS_UINT32 ATP_USBStorage_Mount_DiskSubarea_Refresh_NotifyRegister( AtpNotifierProc pfProc, ATP_USBSTORAGE_MOUNTREFRESH_NOTIFY_PRIORITY_EN enPriority, VOS_VOID *pvNotifyHook );

#if defined(__cplusplus)
}
#endif
//!...........................................外部api接口end.................................................

#endif
