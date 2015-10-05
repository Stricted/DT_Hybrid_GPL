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
/* <DTS2012110301136 l00205975 20121103 begin */
#define HI_WDT_TIME_BIT_MASK                    0xFFF803FF

#define HI_WDT_CLEAR_INTER                      3  // 3s
#define HI_WDT_DEF_TIMER                        30 // 30s

#define HI_WDT_MAX_CLRTIMER                     30 // 30s ����幷����
#define HI_WDT_MAX_TIMER                        120

/*wdt����*/
typedef struct
{
    unsigned int ui_state;
    unsigned int ui_lsttime;            /*�ϴ��幷ʱ��*/
    unsigned int ui_period;             /*�幷����: 1 - 120��*/
    unsigned int ui_timeout;            /*����ʱʱ��: 1 - 120��*/
} hi_wdt_attr_s;

/*�幷����*/
extern void hi_kernel_wdt_clear(void);
/*����Ӳ����ʱ���� */
extern void hi_kernel_wdt_set(unsigned int ui_interval);
/*�����幷ʱ���� */
extern void hi_kernel_wdt_set_clearintr(unsigned int ui_interval);
/*����ع�*/
extern void hi_kernel_wdt_disable(void);
extern void hi_kernel_wdt_enable(void);
/*����Ƿ�׼ȷι��*/
extern unsigned int hi_kernel_wdt_check(void);
/* DTS2012110301136 l00205975 20121103 end> */
#endif /*__HI_DRV_WDT_H__*/
