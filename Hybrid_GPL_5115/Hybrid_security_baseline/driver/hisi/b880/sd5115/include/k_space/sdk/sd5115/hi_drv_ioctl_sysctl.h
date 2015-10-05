/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_ioctl_sysctl.h
  功能描述: SYSCTL IOCTL 命令字定义
  版本描述: V1.0

  创建日期: 2009年1月23日
  创建作者: 海思511x 芯片SDK 开发组

  修改记录:
******************************************************************************/

#ifndef __HI_DRV_IOCTL_SYSCTL__
#define __HI_DRV_IOCTL_SYSCTL__

#include  "sdk_autoconf.h"
#include  "hi_drv_sysctl.h"

/*********************************访问权限获取********************************************/
#define HI_SYSCTL_RIGHT_GET 0x0000
#define HI_SYSCTL_RIGHT_RELEASE 0x0001

/***********************************************************************************************/
/*********************************SYSCTL  模块********************************************/
#define HI_SYSCTL_ATTR_GET 0x0100
#define HI_SYSCTL_ATTR_SET 0x0101

#define HI_SYSCTL_STA_GET 0X0102
#define HI_SYSCTL_CNT_GET 0X0103
#define HI_SYSCTL_TEMP_GET 0X0104
#define HI_SYSCTL_CHIP_RESET 0X0105

/***********************************************************************************************/

/*for debug*/
#define HI_SYSCTL_PHYSICAL_ADDR_GET 0xf000
#define HI_SYSCTL_PHYSICAL_ADDR_SET 0xf001
#endif  /*end of */
