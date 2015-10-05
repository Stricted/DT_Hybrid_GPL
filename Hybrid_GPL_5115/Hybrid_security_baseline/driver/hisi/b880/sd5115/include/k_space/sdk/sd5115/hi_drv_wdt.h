/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_wdt.h
  ��������: wdt����ͷ�ļ�
  �汾����: V1.0
  ��������: 2008��12��8��
  ��������: ��˼����������
  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_WDT_H__
#define __HI_DRV_WDT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*wdt����ģʽ*/
typedef enum
{
    HI_WDT_USER_MODE_E = 0x0,                /*�û�ģʽ���û������幷*/
    HI_WDT_AUTO_MODE_E,                      /*�Զ�ģʽ�������ڲ������幷*/
    HI_WDT_DISABLE_MODE_E,                   /*��ֹģʽ�����Ź���Ч*/
} HI_WDT_WORK_MODE_E;

/*wdt����*/
typedef struct
{
    HI_WDT_WORK_MODE_E em_mode;              /*Ĭ��Ϊ�Զ�ģʽ*/
    uint               ui_interval;          /*ϵͳ��λ���: 1-2 * (0xffff_ffff / ϵͳʱ��)*/
} hi_wdt_attr_s;

/*ͳ���쳣��¼��*/
typedef struct
{
    uint ui_invalid_parameter_cnt;           /*��Ч��������*/
} hi_wdt_cnt_s;

/*�������Բ���*/
extern uint hi_kernel_wdt_attr_set(hi_wdt_attr_s *pst_attr);
extern uint hi_kernel_wdt_attr_get(hi_wdt_attr_s *pst_attr);

/*�幷����*/
extern void hi_kernel_wdt_clear(void);

/*ͳ���쳣��¼��ȡ*/
extern uint hi_kernel_wdt_cnt_get(hi_wdt_cnt_s *pst_cnt);

#endif /*__HI_DRV_WDT_H__*/
