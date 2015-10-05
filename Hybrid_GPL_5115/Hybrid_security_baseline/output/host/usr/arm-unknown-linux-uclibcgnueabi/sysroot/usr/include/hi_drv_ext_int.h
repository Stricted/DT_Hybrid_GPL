/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_ext_int.h
  功能描述: ext_int驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_EXT_INT_H__
#define __HI_DRV_EXT_INT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*-----------------------------------------------------*/
/* 5115HGIC分配 */
#define HI_GIC_IRQ_START                32
#define HI_GIC_DDRC_ERR                 HI_GIC_IRQ_START + 28   /* mddrc错误中断                            */
#define HI_GIC_ZSP2ARM_INT1             HI_GIC_IRQ_START + 29
#define HI_GIC_ZSP2ARM_INT2             HI_GIC_IRQ_START + 30
#define HI_GIC_COMMTX                   HI_GIC_IRQ_START + 32
#define HI_GIC_COMMRX                   HI_GIC_IRQ_START + 33
#define HI_GIC_L2_INT_COMB              HI_GIC_IRQ_START + 34
#define HI_GIC_L2_SPC_INT0              HI_GIC_IRQ_START + 35
#define HI_GIC_L2_SPC_INT1              HI_GIC_IRQ_START + 36
#define HI_GIC_ZSP2ARM_INT0             HI_GIC_IRQ_START + 37   /* ZSP上报A9中断0                          */
#define HI_GIC_USB_OHCI                 HI_GIC_IRQ_START + 38   /* OHCI中断                                          */
#define HI_GIC_USB_EHCI                 HI_GIC_IRQ_START + 39   /* EHCI中断                                           */
#define HI_GIC_SFC                      HI_GIC_IRQ_START + 40   /* SFC中断                                            */
#define HI_GIC_NANDC_S                  HI_GIC_IRQ_START + 41   /* NANDC中断                                       */
#define HI_GIC_SSI                      HI_GIC_IRQ_START + 42   /* SPI中断                                             */
#define HI_GIC_I2C_S                    HI_GIC_IRQ_START + 43
#define HI_GIC_I2C_M                    HI_GIC_IRQ_START + 44
#define HI_GIC_UART0                    HI_GIC_IRQ_START + 45   /* UART0中断                                        */
#define HI_GIC_UART1                    HI_GIC_IRQ_START + 46   /* UART1中断                                        */
#define HI_GIC_TIMER_0_1                HI_GIC_IRQ_START + 47   /* TIMER01中断                                     */
#define HI_GIC_TIMER_2_3                HI_GIC_IRQ_START + 48   /* TIMER23中断                                     */
#define HI_GIC_GPIO_0                   HI_GIC_IRQ_START + 49   /* GPIO0中断                                         */
#define HI_GIC_GPIO_1                   HI_GIC_IRQ_START + 50   /* GPIO1中断                                         */
#define HI_GIC_GPIO_2                   HI_GIC_IRQ_START + 51   /* GPIO2中断                                         */
#define HI_GIC_GPIO_3                   HI_GIC_IRQ_START + 52   /* GPIO3中断                                         */
#define HI_GIC_GPIO_4                   HI_GIC_IRQ_START + 53   /* GPIO4中断                                         */
#define HI_GIC_GPIO_5                   HI_GIC_IRQ_START + 54
#define HI_GIC_GPIO_6                   HI_GIC_IRQ_START + 55
#define HI_GIC_GPIO_7                   HI_GIC_IRQ_START + 56
#define HI_GIC_SDIO                     HI_GIC_IRQ_START + 57
#define HI_GIC_DMAC                     HI_GIC_IRQ_START + 58   /* DMAC中断                                          */
#define HI_GIC_PCIE0_A                  HI_GIC_IRQ_START + 59   /* PCIe0传统中断a                            */
#define HI_GIC_PCIE0_B                  HI_GIC_IRQ_START + 60   /* PCIe0传统中断b                            */
#define HI_GIC_PCIE0_C                  HI_GIC_IRQ_START + 61   /* PCIe0传统中断c                            */
#define HI_GIC_PCIE0_D                  HI_GIC_IRQ_START + 62   /* PCIe0传统中断d                            */
#define HI_GIC_PCIE1_A                  HI_GIC_IRQ_START + 63   /* PCIe1传统中断a                            */
#define HI_GIC_PCIE1_B                  HI_GIC_IRQ_START + 64   /* PCIe1传统中断b                            */
#define HI_GIC_PCIE1_C                  HI_GIC_IRQ_START + 65   /* PCIe1传统中断c                            */
#define HI_GIC_PCIE1_D                  HI_GIC_IRQ_START + 66   /* PCIe1传统中断d                            */
#define HI_GIC_PMU                      HI_GIC_IRQ_START + 67
#define HI_GIC_SCI                      HI_GIC_IRQ_START + 68   /* SCI中断                                              */
#define HI_GIC_PCIE0_LINK_DOWN          HI_GIC_IRQ_START + 69   /* PCIe0链路断开中断                  */
#define HI_GIC_PCIE1_LINK_DOWN          HI_GIC_IRQ_START + 70   /* PCIe1链路断开中断                  */
#define HI_GIC_PCIE0_PM                 HI_GIC_IRQ_START + 71   /* PCIe0低功耗退出中断            */
#define HI_GIC_PCIE1_PM                 HI_GIC_IRQ_START + 72   /* PCIe1低功耗退出中断            */
#define HI_GIC_PIE_INT0                 HI_GIC_IRQ_START + 100  /* 5115的PIE中断号 */
#define HI_GIC_EPON_INT                 HI_GIC_IRQ_START + 101
#define HI_GIC_GPON_INT                 HI_GIC_IRQ_START + 102
#define HI_GIC_LSW_INT                  HI_GIC_IRQ_START + 103
#define HI_GIC_MDIO0                    HI_GIC_IRQ_START + 104  /* MDIO0中断                                       */
#define HI_GIC_GE_DYG_INT               HI_GIC_IRQ_START + 105
#define HI_GIC_PHY0_INT                 HI_GIC_IRQ_START + 106
#define HI_GIC_PHY1_INT                 HI_GIC_IRQ_START + 107
#define HI_GIC_PHY2_INT                 HI_GIC_IRQ_START + 108
#define HI_GIC_PHY3_INT                 HI_GIC_IRQ_START + 109
#define HI_GIC_ROGUE_ONU_INT            HI_GIC_IRQ_START + 110
#define HI_GIC_SILENT_ONU_INT           HI_GIC_IRQ_START + 111
#define HI_GIC_UNI_MAC_INT              HI_GIC_IRQ_START + 112
#define HI_GIC_ZSI_INT                  HI_GIC_IRQ_START + 113
#define HI_GIC_EXT_INTSCR0              HI_GIC_IRQ_START + 114  /* 外部中断0 */
#define HI_GIC_EXT_INTSCR1              HI_GIC_IRQ_START + 115  /* 外部中断1 */
#define HI_GIC_EXT_INTSCR2              HI_GIC_IRQ_START + 116  /* 外部中断2 */
#define HI_GIC_EXT_INTSCR3              HI_GIC_IRQ_START + 117  /* 外部中断3 */
#define HI_GIC_NNI_MAC_INT              HI_GIC_IRQ_START + 118
#define HI_GIC_LP_INT                   HI_GIC_IRQ_START + 119
#define HI_GIC_IRQS                     HI_GIC_IRQ_START + 128
/*-----------------------------------------------------*/

/*外部中断管脚号*/
typedef enum
{
    HI_EXT_INT0_E = 0x0,        /*外部pin管脚中断0*/
    HI_EXT_INT1_E,              /*外部pin管脚中断1*/
    HI_EXT_INT2_E,              /*外部pin管脚中断2*/
    HI_EXT_INT3_E,              /*外部pin管脚中断3*/
} HI_EXT_INT_E;

/*外部pin管脚属性*/
typedef struct
{
    HI_BOOL_E  em_enable;       /*中断使能开关*/
    HI_LEVEL_E em_level;        /*中断触发方式*/
    void (*call_back)(void);    /*中断回调函数*/
} hi_ext_int_attr_s;

/*获取属性*/
extern uint hi_kernel_ext_int_attr_get(HI_EXT_INT_E em_int, hi_ext_int_attr_s *pst_attr);

/*配置属性*/
extern uint hi_kernel_ext_int_attr_set(HI_EXT_INT_E em_int, hi_ext_int_attr_s *pst_attr);

#endif /* __HI_DRV_EXT_INT_H__ */
