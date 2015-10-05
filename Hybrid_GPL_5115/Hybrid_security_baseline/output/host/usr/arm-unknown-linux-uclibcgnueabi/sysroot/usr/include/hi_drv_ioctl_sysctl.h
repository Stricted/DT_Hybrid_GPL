/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_ioctl_sysctl.h
  ��������: SYSCTL IOCTL �����ֶ���
  �汾����: V1.0

  ��������: 2009��1��23��
  ��������: ��˼511x оƬSDK ������

  �޸ļ�¼:
******************************************************************************/

#ifndef __HI_DRV_IOCTL_SYSCTL__
#define __HI_DRV_IOCTL_SYSCTL__

#include  "sdk_autoconf.h"
#include  "hi_drv_sysctl.h"

/*********************************����Ȩ�޻�ȡ********************************************/
#define HI_SYSCTL_RIGHT_GET 0x0000
#define HI_SYSCTL_RIGHT_RELEASE 0x0001

/***********************************************************************************************/
/*********************************SYSCTL  ģ��********************************************/
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
