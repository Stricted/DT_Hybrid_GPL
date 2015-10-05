/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_uart.h
  ��������: uart����ͷ�ļ�
  �汾����: V1.0
  ��������: 2008��12��8��
  ��������: ��˼����������
  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_UART_H__
#define __HI_DRV_UART_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*uart�˿�*/
typedef enum
{
    HI_UART0_0_E = 0x0,                          /*uart0*/
    HI_UART0_1_E,                                 /* 5115 ASIC-S and FPGA only one UART0, 5115 ASIC-H UART0_0 UART0_1 */
    HI_UART1_0_E,                                /*uart1*/
    HI_UART1_1_E,                                /* 5115 ASIC-S and FPGA only one UART1, 5115 ASIC-H UART1_0 UART1_1 */
} HI_UART_E;

/*uart������*/
typedef enum
{
    HI_UART_BAUD_RATE_9600_E = 0x0,            /*9600bit/s*/
    HI_UART_BAUD_RATE_38400_E,                 /*38400bit/s*/
    HI_UART_BAUD_RATE_57600_E,                 /*57600bit/s*/
    HI_UART_BAUD_RATE_115200_E,                /*115200bit/s*/
    HI_UART_BAUD_RATE_187500_E,                /*187500bit/s*/
} HI_UART_BAUD_RATE_E;

/*uart����λ����, ע��ö�ٵ�ֵ˳�򲻿��Ա䣬ö��ֵ��Ӳ���Ĵ���ֵһһ��Ӧ*/
typedef enum
{
    HI_UART_DATA_BIT5_E = 0x0,                  /*5bit����λ*/
    HI_UART_DATA_BIT6_E,                        /*6bit����λ*/
    HI_UART_DATA_BIT7_E,                        /*7bit����λ*/
    HI_UART_DATA_BIT8_E,                        /*8bit����λ*/
} HI_UART_DATA_BIT_E;

/*uartֹͣλ���ȣ�ע��ö�ٵ�ֵ˳�򲻿��Ա䣬ö��ֵ��Ӳ���Ĵ���ֵһһ��Ӧ*/
typedef enum
{
    HI_UART_STOP_BIT1_E = 0x0,                  /*����ֹͣλΪ1bit*/
    HI_UART_STOP_BIT2_E,                        /*����ֹͣλΪ2bit, ������λѡ��5bitʱ��ֹͣλ��1.5bit*/
} HI_UART_STOP_BIT_E;

/*uartУ��*/
typedef enum
{
    HI_UART_PARITY_NO_E = 0x0,                   /*��У��*/
    HI_UART_PARITY_ODD_E,                        /*��У��*/
    HI_UART_PARITY_EVEN_E,                       /*żУ��*/
} HI_UART_PARITY_E;

/*uart����ģʽ*/
typedef enum
{
    HI_UART_IN_LOOPBACK_E = 0x0,                 /*�ڻ��ز���ģʽ*/
    HI_UART_OUT_LOOPBACK_E,                      /*�⻷�ز���ģʽ*/
} HI_UART_LOOPBACK_MODE_E;

/*uart������������*/
typedef struct
{
    HI_BOOL_E           em_enable;                /*HI_TRUE_E: uartʹ��   HI_FALSE_E: uart�ر�*/
    HI_UART_E           em_uart;                  /*uart�˿�*/
    HI_UART_BAUD_RATE_E em_baud_rate;             /*uart������*/
    HI_UART_DATA_BIT_E  em_data_bit;              /*����λ����*/
    HI_UART_STOP_BIT_E  em_stop_bit;              /*ֹͣλ����*/
    HI_UART_PARITY_E    em_parity;                /*У������*/
} hi_uart_attr_s;

/*ͳ���쳣��¼*/
typedef struct
{
    /*ͨ��ͳ����Ϣ*/
    uint ui_read_bytes;                           /*���ֽ���ͳ��*/
    uint ui_write_bytes;                          /*д�ֽ���ͳ��*/

    /*ͨ���쳣ͳ����Ϣ*/
    uint ui_read_failure_cnt;                     /*��ʧ�ܴ���ͳ��*/
    uint ui_write_failure_cnt;                    /*дʧ�ܴ���ͳ��*/
    uint ui_invalid_parameter_cnt;                /*������Ч��������ͳ��*/
} hi_uart_cnt_s;

/*�������Բ���*/
extern uint hi_kernel_uart_attr_set(hi_uart_attr_s *pst_attr);
extern uint hi_kernel_uart_attr_get(hi_uart_attr_s *pst_attr);

/*ͨ�Žӿ�*/
extern uint hi_kernel_uart_send_ready(void);
extern uint hi_kernel_uart_send(uchar *puc_send_buffer, uint ui_length);
extern uint hi_kernel_uart_receive_ready(void);
extern uint hi_kernel_uart_receive(uchar *puc_receive_buffer, uint ui_length, uint *pui_received_length);

/*ͳ����Ϣ*/
extern uint hi_kernel_uart_cnt_get(hi_uart_cnt_s *pst_cnt);

#endif /*__HI_DRV_UART_H__*/
