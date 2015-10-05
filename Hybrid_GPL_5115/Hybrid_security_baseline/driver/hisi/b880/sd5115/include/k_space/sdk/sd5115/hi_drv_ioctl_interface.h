/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_ioctl_interface.h
  功能描述: INTERFACE IOCTL 命令字定义
  版本描述: V1.0

  创建日期: 2009年1月23日
  创建作者: 海思511x 芯片SDK 开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_IOCTL_INTERFACE_H__
#define __HI_DRV_IOCTL_INTERFACE_H__

#include  "sdk_autoconf.h"
#include "hi_drv_mdio.h"
#include "hi_drv_gpio.h"
#include "hi_drv_spi.h"
#include "hi_drv_i2c.h"

/*********************************访问权限获取*********************************/
#define HI_INTERFACE_RIGHT_GET 0x0000
#define HI_INTERFACE_RIGHT_RELEASE 0x0001

/******************************************************************************/

/*************************************MDIO*************************************/
#define HI_INTERFACE_MDIO_ATTR_GET 0x0100
#define HI_INTERFACE_MDIO_ATTR_SET 0x0101
#define HI_INTERFACE_MDIO_WRITE 0x0102
#define HI_INTERFACE_MDIO_READ 0x0103
#define HI_INTERFACE_MDIO_CNT_GET 0x0104
#define HI_INTERFACE_MDIO_DUMP 0x0105

/*************************************GPIO*************************************/
#define HI_INTERFACE_GPIO_BIT_ATTR_GET 0x0106
#define HI_INTERFACE_GPIO_BIT_ATTR_SET 0x0107
#define HI_INTERFACE_GPIO_PORT_ATTR_GET 0x0108
#define HI_INTERFACE_GPIO_PORT_ATTR_SET 0x0109
#define HI_INTERFACE_GPIO_WRITE 0x010A
#define HI_INTERFACE_GPIO_READ 0x010B
#define HI_INTERFACE_GPIO_BIT_WRITE 0x010C
#define HI_INTERFACE_GPIO_BIT_READ 0x010D
#define HI_INTERFACE_GPIO_CNT_GET 0x010E
#define HI_INTERFACE_GPIO_DUMP 0x010F

/*************************************SPI*************************************/
#define HI_INTERFACE_SPI_RIGHT_GET 0x0110
#define HI_INTERFACE_SPI_RIGHT_RELEASE 0x0111
#define HI_INTERFACE_SPI_SEND 0x0112
#define HI_INTERFACE_SPI_RECEIVE 0x0113
#define HI_INTERFACE_SPI_SEND_RECEIVE 0x0114
#define HI_INTERFACE_SPI_TEST 0x0115
#define HI_INTERFACE_SPI_ATTR_GET 0x0116
#define HI_INTERFACE_SPI_ATTR_SET 0x0117
#define HI_INTERFACE_SPI_CNT_GET 0x0118
#define HI_INTERFACE_SPI_DUMP 0x0119

#define HI_INTERFACE_I2C_ATTR_SET 0x011A
#define HI_INTERFACE_I2C_ATTR_GET 0x011B
#define HI_INTERFACE_I2C_READ 0x011C
#define HI_INTERFACE_I2C_WRITE 0x011D
#define HI_INTERFACE_I2C_SEND 0x011E
#define HI_INTERFACE_I2C_RECEIVE 0x011F

typedef struct
{
    unsigned char  us_dev_addr;
    unsigned char *puc_data;
    unsigned int   ui_length;
} hi_i2c_send_receive_info_s;

typedef struct
{
    uint *ui_send_unit;
    uint *ui_receive_unit;
    uint  ui_unit_num;
} hi_spi_send_receive_info_s;
#endif  /*end of */
