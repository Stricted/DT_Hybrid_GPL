/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_gpio.h
  ��������: gpio����ͷ�ļ�
  �汾����: V1.0
  ��������: 2008��12��8��
  ��������: ��˼����������
  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_GPIO_H__
#define __HI_DRV_GPIO_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*һ��GPIO�ܽ�*/
typedef enum
{
    HI_GPIO_BIT0_E = 0,
    HI_GPIO_BIT1_E,
    HI_GPIO_BIT2_E,
    HI_GPIO_BIT3_E,
    HI_GPIO_BIT4_E,
    HI_GPIO_BIT5_E,
    HI_GPIO_BIT6_E,
    HI_GPIO_BIT7_E,
    HI_GPIO_BIT8_E,
    HI_GPIO_BIT9_E,
    HI_GPIO_BIT10_E,
    HI_GPIO_BIT11_E,
    HI_GPIO_BIT12_E,
    HI_GPIO_BIT13_E,
    HI_GPIO_BIT14_E,
    HI_GPIO_BIT15_E,
    HI_GPIO_BIT16_E,
    HI_GPIO_BIT17_E,
    HI_GPIO_BIT18_E,
    HI_GPIO_BIT19_E,
    HI_GPIO_BIT20_E,
    HI_GPIO_BIT21_E,
    HI_GPIO_BIT22_E,
    HI_GPIO_BIT23_E,
    HI_GPIO_BIT24_E,
    HI_GPIO_BIT25_E,
    HI_GPIO_BIT26_E,
    HI_GPIO_BIT27_E,
    HI_GPIO_BIT28_E,
    HI_GPIO_BIT29_E,
    HI_GPIO_BIT30_E,
    HI_GPIO_BIT31_E,
} HI_GPIO_BIT_E;

/*GPIO��*/

/*
˵����
 ��SD5115V100-MPW-ASIC�У�ֻ�õ���HI_GPIO_PORT0_E -- HI_GPIO_PORT3_E��4��GPIO��
,��һ��GPIO��ӦGPIO_0-GPIO_31���ڶ���GPIO��ӦGPIO_32-GPIO_63, ������GPIO��ӦGPIO64-GPIO_95,
 ���У���GPIO_64��GPIO_75��12��IO��Ŀǰ������Ϊ��ͨ��������ڣ�������GPIO��ӦGPIO96-GPIO_106��11��IO�ڡ�
 */
typedef enum
{
    HI_GPIO_PORT0_E = 0,
    HI_GPIO_PORT1_E,
    HI_GPIO_PORT2_E,
    HI_GPIO_PORT3_E,
    HI_GPIO_PORT4_E,    //��ʱû��
    HI_GPIO_PORT5_E,    //��ʱû��
    HI_GPIO_PORT6_E,    //��ʱû��
    HI_GPIO_PORT7_E,    //��ʱû��
} HI_GPIO_PORT_E;

/*GPIO����ģʽ*/
typedef enum
{
    HI_GPIO_WORK_MODE_INPUT_E = 0,                     /*����ģʽ*/
    HI_GPIO_WORK_MODE_OUTPUT_E,                        /*���ģʽ*/
    HI_GPIO_WORK_MODE_HARDWARE_E,                      /*���⹦��ģʽ��Ӳ��ģʽ�ܽŸ���*/
} HI_GPIO_WORK_MODE_E;

/*GPIOһλ������*/
typedef struct
{
    HI_GPIO_WORK_MODE_E em_mode;                      /*GPIOһλ�Ĺ���ģʽ*/
} hi_gpio_bit_attr_s;

/*GPIO�˿�����*/
typedef struct
{
    HI_BOOL_E em_enable;                              /*GPIO�˿�ʹ�����ԣ�HI_TRUE_E: GPIO�˿�ʱ�Ӵ򿪡�HI_FALSE_E: GPIO�˿�ʱ�ӹر�*/
} hi_gpio_port_attr_s;

/*ͳ���쳣��¼*/
typedef struct
{
    uint ui_invalid_parameter_cnt;                    /*������Ч�����Ĵ���*/
} hi_gpio_cnt_s;

/*GPIOλ�������Բ���*/
extern uint hi_kernel_gpio_bit_attr_get(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, hi_gpio_bit_attr_s *pst_attr);
extern uint hi_kernel_gpio_bit_attr_set(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, hi_gpio_bit_attr_s *pst_attr);

/*GPIO�˿���������,��������Ҫ��Ҫ�ر�GPIO�˿�ʱ�ӣ��ᵼ�¸���GPIO��ص�λ���ܹ���*/
extern uint hi_kernel_gpio_port_attr_get(HI_GPIO_PORT_E em_port, hi_gpio_port_attr_s *pst_attr);
extern uint hi_kernel_gpio_port_attr_set(HI_GPIO_PORT_E em_port, hi_gpio_port_attr_s *pst_attr);

/*GPIO�˿���� ����GPIO�˿�(0~7bit)ȫ������Ϊ���ģʽʱ, ͨ���˽ӿ������������*/
extern uint hi_kernel_gpio_write(HI_GPIO_PORT_E em_port, uint uc_data);

/*GPIO�˿����� ����GPIO�˿�(0~7bit)ȫ������Ϊ����ģʽʱ, ͨ���˽ӿڵõ���������*/
extern uint hi_kernel_gpio_read(HI_GPIO_PORT_E em_port, uint *puc_data);

/*GPIO��ƽ����, ����GPIO����Ϊ���ģʽʱ,ͨ���˽ӿ����������ƽ*/
extern uint hi_kernel_gpio_bit_write(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, HI_LEVEL_E em_level);

/*GPIO�����ƽ��ȡ������GPIO����Ϊ����ʱ,��ͨ���˽ӿڵõ������ƽ*/
extern uint hi_kernel_gpio_bit_read(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, HI_LEVEL_E *pem_level);

/*ȡͳ���쳣����*/
extern uint hi_kernel_gpio_cnt_get(hi_gpio_cnt_s *pst_cnt);

#endif /* __HI_DRV_GPIO_H__ */
