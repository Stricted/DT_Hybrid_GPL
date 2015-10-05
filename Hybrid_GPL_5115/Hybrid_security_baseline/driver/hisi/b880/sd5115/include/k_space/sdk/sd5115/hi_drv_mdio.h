/******************************************************************************
  版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
 文件名称: hi_drv_mdio.h
 功能描述: mdio驱动头文件
 版本描述: V1.0

 创建日期: 2009年1月23日
 创建作者: 海思SD511x 芯片SDK开发组

 修改记录:
******************************************************************************/
#ifndef __HI_DRV_MDIO_H__
#define __HI_DRV_MDIO_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"
#ifdef CONFIG_HISI_SD5115_AF_FPGA

/*外接PHY的数量*/
 #define HI_MDIO_PHY_NUM 6    //G174254: 待定
#endif

/*波特率, 顺序与代码相关*/
typedef enum
{
    HI_MDIO_BAUD_RATE_2M5_E = 0,        /* 波特率：2.5MHz  */
    HI_MDIO_BAUD_RATE_1M25_E,           /* 波特率：1.25MHz */
    HI_MDIO_BAUD_RATE_5M_E,             /* 波特率：5M      */
    HI_MDIO_BAUD_RATE_10M_E,            /* 波特率：10MHz   */
} HI_MDIO_BAUD_RATE_E;

/*mdio 基本属性配置*/
typedef struct
{
    HI_BOOL_E           em_enable;     //G174254: 待定
    HI_MDIO_BAUD_RATE_E em_baud_rate;   /*波特率*/
} hi_mdio_attr_s;

/*统计，异常记录等*/
typedef struct
{
    /*通信统计信息*/
    uint ui_read_cnt;                                        /*读次数统计*/
    uint ui_write_cnt;                                       /*写次数统计*/

    /*通信异常统计信息*/
    uint ui_read_failure_cnt;                                /*读失败计数*/
    uint ui_write_failure_cnt;                               /*写失败计数*/
    uint ui_invalid_parameter_cnt;                           /*无效参数计数*/
} hi_mdio_cnt_s;

/*配置属性操作*/
extern uint hi_kernel_mdio_attr_set(hi_mdio_attr_s *pst_attr);
extern uint hi_kernel_mdio_attr_get(hi_mdio_attr_s *pst_attr);

/*普通读写操作, 在配置工作模式为普通模式情况使用, phy地址和phy寄存器地址都是5bit*/
extern uint hi_kernel_mdio_write(uint ui_phy_addr, uint ui_reg_addr, ushort us_data);
extern uint hi_kernel_mdio_read(uint ui_phy_addr, uint ui_reg_addr, ushort *pus_data);

/*统计信息*/
extern uint hi_kernel_mdio_cnt_get(hi_mdio_cnt_s *pst_cnt);

#endif /*__HI_DRV_MDIO_H__*/
