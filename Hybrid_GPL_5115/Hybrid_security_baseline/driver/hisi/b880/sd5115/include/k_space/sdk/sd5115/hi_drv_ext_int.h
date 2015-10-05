/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_ext_int.h
  ��������: ext_int����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_EXT_INT_H__
#define __HI_DRV_EXT_INT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*-----------------------------------------------------*/
/* 5115HGIC���� */
#define HI_GIC_IRQ_START                32
#define HI_GIC_DDRC_ERR                 HI_GIC_IRQ_START + 28   /* mddrc�����ж�                            */
#define HI_GIC_ZSP2ARM_INT1             HI_GIC_IRQ_START + 29
#define HI_GIC_ZSP2ARM_INT2             HI_GIC_IRQ_START + 30
#define HI_GIC_COMMTX                   HI_GIC_IRQ_START + 32
#define HI_GIC_COMMRX                   HI_GIC_IRQ_START + 33
#define HI_GIC_L2_INT_COMB              HI_GIC_IRQ_START + 34
#define HI_GIC_L2_SPC_INT0              HI_GIC_IRQ_START + 35
#define HI_GIC_L2_SPC_INT1              HI_GIC_IRQ_START + 36
#define HI_GIC_ZSP2ARM_INT0             HI_GIC_IRQ_START + 37   /* ZSP�ϱ�A9�ж�0                          */
#define HI_GIC_USB_OHCI                 HI_GIC_IRQ_START + 38   /* OHCI�ж�                                          */
#define HI_GIC_USB_EHCI                 HI_GIC_IRQ_START + 39   /* EHCI�ж�                                           */
#define HI_GIC_SFC                      HI_GIC_IRQ_START + 40   /* SFC�ж�                                            */
#define HI_GIC_NANDC_S                  HI_GIC_IRQ_START + 41   /* NANDC�ж�                                       */
#define HI_GIC_SSI                      HI_GIC_IRQ_START + 42   /* SPI�ж�                                             */
#define HI_GIC_I2C_S                    HI_GIC_IRQ_START + 43
#define HI_GIC_I2C_M                    HI_GIC_IRQ_START + 44
#define HI_GIC_UART0                    HI_GIC_IRQ_START + 45   /* UART0�ж�                                        */
#define HI_GIC_UART1                    HI_GIC_IRQ_START + 46   /* UART1�ж�                                        */
#define HI_GIC_TIMER_0_1                HI_GIC_IRQ_START + 47   /* TIMER01�ж�                                     */
#define HI_GIC_TIMER_2_3                HI_GIC_IRQ_START + 48   /* TIMER23�ж�                                     */
#define HI_GIC_GPIO_0                   HI_GIC_IRQ_START + 49   /* GPIO0�ж�                                         */
#define HI_GIC_GPIO_1                   HI_GIC_IRQ_START + 50   /* GPIO1�ж�                                         */
#define HI_GIC_GPIO_2                   HI_GIC_IRQ_START + 51   /* GPIO2�ж�                                         */
#define HI_GIC_GPIO_3                   HI_GIC_IRQ_START + 52   /* GPIO3�ж�                                         */
#define HI_GIC_GPIO_4                   HI_GIC_IRQ_START + 53   /* GPIO4�ж�                                         */
#define HI_GIC_GPIO_5                   HI_GIC_IRQ_START + 54
#define HI_GIC_GPIO_6                   HI_GIC_IRQ_START + 55
#define HI_GIC_GPIO_7                   HI_GIC_IRQ_START + 56
#define HI_GIC_SDIO                     HI_GIC_IRQ_START + 57
#define HI_GIC_DMAC                     HI_GIC_IRQ_START + 58   /* DMAC�ж�                                          */
#define HI_GIC_PCIE0_A                  HI_GIC_IRQ_START + 59   /* PCIe0��ͳ�ж�a                            */
#define HI_GIC_PCIE0_B                  HI_GIC_IRQ_START + 60   /* PCIe0��ͳ�ж�b                            */
#define HI_GIC_PCIE0_C                  HI_GIC_IRQ_START + 61   /* PCIe0��ͳ�ж�c                            */
#define HI_GIC_PCIE0_D                  HI_GIC_IRQ_START + 62   /* PCIe0��ͳ�ж�d                            */
#define HI_GIC_PCIE1_A                  HI_GIC_IRQ_START + 63   /* PCIe1��ͳ�ж�a                            */
#define HI_GIC_PCIE1_B                  HI_GIC_IRQ_START + 64   /* PCIe1��ͳ�ж�b                            */
#define HI_GIC_PCIE1_C                  HI_GIC_IRQ_START + 65   /* PCIe1��ͳ�ж�c                            */
#define HI_GIC_PCIE1_D                  HI_GIC_IRQ_START + 66   /* PCIe1��ͳ�ж�d                            */
#define HI_GIC_PMU                      HI_GIC_IRQ_START + 67
#define HI_GIC_SCI                      HI_GIC_IRQ_START + 68   /* SCI�ж�                                              */
#define HI_GIC_PCIE0_LINK_DOWN          HI_GIC_IRQ_START + 69   /* PCIe0��·�Ͽ��ж�                  */
#define HI_GIC_PCIE1_LINK_DOWN          HI_GIC_IRQ_START + 70   /* PCIe1��·�Ͽ��ж�                  */
#define HI_GIC_PCIE0_PM                 HI_GIC_IRQ_START + 71   /* PCIe0�͹����˳��ж�            */
#define HI_GIC_PCIE1_PM                 HI_GIC_IRQ_START + 72   /* PCIe1�͹����˳��ж�            */
#define HI_GIC_PIE_INT0                 HI_GIC_IRQ_START + 100  /* 5115��PIE�жϺ� */
#define HI_GIC_EPON_INT                 HI_GIC_IRQ_START + 101
#define HI_GIC_GPON_INT                 HI_GIC_IRQ_START + 102
#define HI_GIC_LSW_INT                  HI_GIC_IRQ_START + 103
#define HI_GIC_MDIO0                    HI_GIC_IRQ_START + 104  /* MDIO0�ж�                                       */
#define HI_GIC_GE_DYG_INT               HI_GIC_IRQ_START + 105
#define HI_GIC_PHY0_INT                 HI_GIC_IRQ_START + 106
#define HI_GIC_PHY1_INT                 HI_GIC_IRQ_START + 107
#define HI_GIC_PHY2_INT                 HI_GIC_IRQ_START + 108
#define HI_GIC_PHY3_INT                 HI_GIC_IRQ_START + 109
#define HI_GIC_ROGUE_ONU_INT            HI_GIC_IRQ_START + 110
#define HI_GIC_SILENT_ONU_INT           HI_GIC_IRQ_START + 111
#define HI_GIC_UNI_MAC_INT              HI_GIC_IRQ_START + 112
#define HI_GIC_ZSI_INT                  HI_GIC_IRQ_START + 113
#define HI_GIC_EXT_INTSCR0              HI_GIC_IRQ_START + 114  /* �ⲿ�ж�0 */
#define HI_GIC_EXT_INTSCR1              HI_GIC_IRQ_START + 115  /* �ⲿ�ж�1 */
#define HI_GIC_EXT_INTSCR2              HI_GIC_IRQ_START + 116  /* �ⲿ�ж�2 */
#define HI_GIC_EXT_INTSCR3              HI_GIC_IRQ_START + 117  /* �ⲿ�ж�3 */
#define HI_GIC_NNI_MAC_INT              HI_GIC_IRQ_START + 118
#define HI_GIC_LP_INT                   HI_GIC_IRQ_START + 119
#define HI_GIC_IRQS                     HI_GIC_IRQ_START + 128
/*-----------------------------------------------------*/

/*�ⲿ�жϹܽź�*/
typedef enum
{
    HI_EXT_INT0_E = 0x0,        /*�ⲿpin�ܽ��ж�0*/
    HI_EXT_INT1_E,              /*�ⲿpin�ܽ��ж�1*/
    HI_EXT_INT2_E,              /*�ⲿpin�ܽ��ж�2*/
    HI_EXT_INT3_E,              /*�ⲿpin�ܽ��ж�3*/
} HI_EXT_INT_E;

/*�ⲿpin�ܽ�����*/
typedef struct
{
    HI_BOOL_E  em_enable;       /*�ж�ʹ�ܿ���*/
    HI_LEVEL_E em_level;        /*�жϴ�����ʽ*/
    void (*call_back)(void);    /*�жϻص�����*/
} hi_ext_int_attr_s;

/*��ȡ����*/
extern uint hi_kernel_ext_int_attr_get(HI_EXT_INT_E em_int, hi_ext_int_attr_s *pst_attr);

/*��������*/
extern uint hi_kernel_ext_int_attr_set(HI_EXT_INT_E em_int, hi_ext_int_attr_s *pst_attr);

#endif /* __HI_DRV_EXT_INT_H__ */
