/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_gpio_int.h
  ��������: gpio_int����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_GPIO_INT_H__
#define __HI_DRV_GPIO_INT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*�ж�����GPIO1��4567λ*/
typedef enum
{
    HI_GPIO_INT0_E = 0,                 /*�ж�����0*/
    HI_GPIO_INT1_E,                     /*�ж�����1*/
    HI_GPIO_INT2_E,                     /*�ж�����2*/
    HI_GPIO_INT3_E,                     /*�ж�����3*/
} HI_GPIO_INT_E;

/*EX INT�жϴ���ģʽ*/
typedef enum
{
    HI_GPIO_INT_LOW_LEVEL_E = 0,         /*�͵�ƽ�����ж�*/
    HI_GPIO_INT_HIGH_LEVEL_E,            /*�ߵ�ƽ�����ж�*/
    HI_GPIO_INT_DOWN_EDGE_E,             /*�½��ش����ж�*/
    HI_GPIO_INT_UP_EDGE_E,               /*�����ش����ж�*/
} HI_GPIO_INT_MODE_E;

/*GPIO_INT�ж�����*/
typedef struct
{
    HI_BOOL_E          em_enable;        /*HI_TRUE_E: �ж�ʹ�� HI_FALSE_E: �жϽ�ֹ*/
    HI_GPIO_INT_MODE_E em_int_mode;      /*����ģʽ*/
    void (*call_back)(void);             /*�жϻص�����*/
} hi_gpio_int_attr_s;

/*ͳ�ƣ��쳣��¼*/
typedef struct
{
    uint ui_invalid_parameter_cnt;     /*������Ч��������*/
} hi_gpio_int_cnt_s;

/*�������Բ���*/
extern uint hi_kernel_gpio_int_attr_set(HI_GPIO_INT_E em_gpio_int, hi_gpio_int_attr_s *pst_attr);
extern uint hi_kernel_gpio_int_attr_get(HI_GPIO_INT_E em_gpio_int, hi_gpio_int_attr_s *pst_attr);

/*��ȡͳ���쳣��Ϣ*/
extern uint hi_kernel_gpio_int_cnt_get(hi_gpio_int_cnt_s *pst_cnt);

#endif /*__HI_DRV_GPIO_INT_H__*/
