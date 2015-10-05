/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾
 
******************************************************************************
  �ļ�����: hi_drv_timer.h
  ��������: timer����ͷ�ļ� 
  �汾����: V1.0
  ��������: 2008��12��8��
  ��������: ��˼����������    
  �޸ļ�¼: 
******************************************************************************/
#ifndef __HI_DRV_TIMER_H__
#define __HI_DRV_TIMER_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*timerѡ��, ֻ���timer2��timer3*/
typedef enum
{
    HI_TIMER2_E = 0,                                   /*timer2*/
    HI_TIMER3_E,                                       /*timer3*/
} HI_TIMER_E;

/*timer tick*/
typedef enum
{
    HI_TICK_1MS_E = 0x0,                               /*1ms*/
    HI_TICK_2MS_E,                                     /*2ms*/
    HI_TICK_5MS_E,                                     /*5ms*/
    HI_TICK_10MS_E,                                    /*10ms*/
    HI_TICK_20MS_E,                                    /*20ms*/
    HI_TICK_50MS_E,                                    /*50ms*/
    HI_TICK_100MS_E,                                   /*100ms*/
    HI_TICK_US_E,                                      /*΢�뼶��ʱ����������ui_usecָ��timer����, ʵ�ʵ����жϴ�����Ϊui_period * ui_usec ΢���*/
} HI_TIMER_TICK_E;

/*ÿ��timer��������������*/
typedef struct
{
    HI_BOOL_E em_enable;                               /*HI_TRUE_E: timerʹ�� HI_FALSE_E: timer��ֹ*/
    HI_TIMER_TICK_E em_tick;                           /*timer����*/
    uint ui_period;                                    /*em_tickʱ��������Ĵ�����Ҳ���жϷ����Ĵ�����, ��СֵΪ1*/
    uint ui_usec;                                      /*΢�뼶���¶�ʱ���ڣ�em_tickΪHI_TICK_US_E�²���Ч��ȡֵ��1~1000000*/
    void (*call_back)(void);                           /*�жϻص�����*/
} hi_timer_attr_s;

/*�쳣ͳ������*/
typedef struct
{
    uint ui_invalid_parameter_cnt;                     /*��Ч����ͳ��*/
} hi_timer_cnt_s;

/*timer���Բ���*/
extern uint hi_kernel_timer_attr_set(HI_TIMER_E em_timer, hi_timer_attr_s *pst_attr);
extern uint hi_kernel_timer_attr_get(HI_TIMER_E em_timer, hi_timer_attr_s *pst_attr);

/*��ǰtick����, �����һ��tick�Ƿ���һ���жϣ�����һ��em_tick��ʱ�䣬�˽ӿڸ��û�����ע���жϴ�����*/
/*ͨ����ѯ��ǰ��tickֵ���ﵽԤ����ֵʱ���û��Լ�����*/
extern uint hi_kernel_timer_cur_tick_get(HI_TIMER_E em_timer, uint *pui_cur_tick);
extern uint hi_kernel_timer_cur_tick_set(HI_TIMER_E em_timer, uint ui_cur_tick);

/*�쳣ͳ�����ݻ�ȡ*/
extern uint hi_kernel_timer_cnt_get(hi_timer_cnt_s *pst_cnt);

#endif /*__HI_DRV_TIMER_H__*/
