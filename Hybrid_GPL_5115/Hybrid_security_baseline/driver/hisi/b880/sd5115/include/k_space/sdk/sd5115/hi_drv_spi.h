/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_spi.h
  ��������: spi����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_SPI_H__
#define __HI_DRV_SPI_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*��ѡ��Ĵӻ�*/
typedef enum
{
    HI_SPI_SLAVE_0_E = 0,                         /*�ӻ�0*/
    HI_SPI_SLAVE_1_E,                            /*�ӻ�1*/
    HI_SPI_SLAVE_2_E,
    HI_SPI_SLAVE_3_E,
    HI_SPI_SLAVE_4_E,
    HI_SPI_SLAVE_5_E,
    HI_SPI_SLAVE_6_E,
    HI_SPI_SLAVE_7_E
} HI_SPI_SLAVE_SEL_E;

/*��ѡ��������ĳ���*/
typedef enum
{
    HI_SPI_FRAME_LENGTH_4_BIT_E = 0,              /*4λ��֡����*/
    HI_SPI_FRAME_LENGTH_5_BIT_E,                  /*5λ��֡����*/
    HI_SPI_FRAME_LENGTH_6_BIT_E,                  /*6λ��֡����*/
    HI_SPI_FRAME_LENGTH_7_BIT_E,                  /*7λ��֡����*/
    HI_SPI_FRAME_LENGTH_8_BIT_E,                  /*8λ��֡����*/
    HI_SPI_FRAME_LENGTH_9_BIT_E,                  /*9λ��֡����*/
    HI_SPI_FRAME_LENGTH_10_BIT_E,                 /*10λ��֡����*/
    HI_SPI_FRAME_LENGTH_11_BIT_E,                 /*11λ��֡����*/
    HI_SPI_FRAME_LENGTH_12_BIT_E,                 /*12λ��֡����*/
    HI_SPI_FRAME_LENGTH_13_BIT_E,                 /*13λ��֡����*/
    HI_SPI_FRAME_LENGTH_14_BIT_E,                 /*14λ��֡����*/
    HI_SPI_FRAME_LENGTH_15_BIT_E,                 /*15λ��֡����*/
    HI_SPI_FRAME_LENGTH_16_BIT_E                  /*16λ��֡����*/
} HI_SPI_FRAME_LENGTH_E;

/*����ʱ����λ, �������ݺ�ʱ�ӵĹ�ϵ*/
typedef enum
{
    HI_SPI_CLOCK_EDGE_UP_E = 0,                    /*��Ƭѡ�½��ؿ�ʼ�������ݣ�����Ҫ��Ƭѡ��Ч֮ǰ׼����*/
    HI_SPI_CLOCK_EDGE_DOWN_E                       /*��Ƭѡ��Ч��ĵ�һ��ʱ���ؿ�ʼ�������ݣ����ݿ�����Ƭѡ��Ч��׼����*/
} HI_SPI_CLOCK_EDGE_E;

/*����ʱ�����ʱ�Ӽ���*/
typedef enum
{
    HI_SPI_IDLE_CLOCK_POLARITY_LOW_E = 0,           /*SPI���߿���״̬ʱ���ʱ��Ϊ��*/
    HI_SPI_IDLE_CLOCK_POLARITY_HIGH_E               /*SPI���߿���״̬ʱ���ʱ��Ϊ��*/
} HI_SPI_IDLE_CLOCK_POLARITY_E;

/*SPI ����ģʽ*/
typedef enum
{
    HI_SPI_IN_LOOPBACK_E = 0,                       /*�ڻ��ز���ģʽ*/
    HI_SPI_OUT_LOOPBACK_E                           /*�⻷�ز���ģʽ*/
} HI_SPI_LOOPBACK_MODE_E;

/*SPI ֡��ʽѡ��*/
typedef enum
{
    HI_SPI_MOTOR_SPI_E = 0,                          /*00��Motorola SPI֡��ʽ��Ĭ��Ϊ��ģʽ*/
    HI_SPI_TI_SSP_E,                                 /*01��TI SSP֡��ʽ��*/
    HI_SPI_NATIONAL_MOCROWIRE_E                      /*10��National Microwire֡��ʽ��*/
} HI_SPI_FRAME_FORMAT_E;

/*SPI������������*/
typedef struct
{
    uint                         ui_index; /*spi�����ţ�ȡֵΪ[0��1],ֻ��SD5115H��Ч,������ΪĬ��0*/
    HI_BOOL_E                    em_enable;                  /*HI_TRUE_E: spiʹ�� HI_FALSE_E: spi��ֹ*/
    HI_SPI_SLAVE_SEL_E           em_slave_select;            /*�ӻ�ѡ��*/
    uint                         ui_baud_rate;               /*������, ��Χ��(����ʱ��/65534 + 1) ~ 8000000*/
    HI_SPI_FRAME_LENGTH_E        em_frame_length;            /*����֡��Ԫ����*/
    HI_SPI_CLOCK_EDGE_E          em_clock_edge;              /*����ʱ����λ*/
    HI_SPI_IDLE_CLOCK_POLARITY_E em_idle_clock_polarity;     /*����ʱ���ʱ�ӵļ��� */
    HI_SPI_FRAME_FORMAT_E        em_frame_format;            /*֡��ʽ���ͣ���Ϊ���֣�1. Motor SPI��ʽ; 2.TI SSP��ʽ; 3.National Microwire��ʽ��Ĭ��Ϊ Motor SPI��ʽ*/
} hi_spi_attr_s;

/*ͳ�ƣ��쳣��¼��*/
typedef struct
{
    /*ͨ��ͳ����Ϣ*/
    uint ui_read_units;                              /*�����ݵ�Ԫ��*/
    uint ui_write_units;                             /*д���ݵ�Ԫ��*/

    /*ͨ���쳣ͳ����Ϣ*/
    uint ui_read_failure_cnt;                        /*��ʧ�ܼ���*/
    uint ui_write_failure_cnt;                       /*дʧ�ܼ���*/
    uint ui_invalid_parameter_cnt;                   /*��Ч��������*/
} hi_spi_cnt_s;

#define HI_SPI_UNIT_NUM 100  /* Added by fufenglian */

typedef struct
{
    uint ui_send_unit[HI_SPI_UNIT_NUM];
    uint ui_unit_num;
} hi_spi_send_s;

typedef struct
{
    uint ui_receive_unit[HI_SPI_UNIT_NUM];
    uint ui_unit_base;
    uint ui_unit_num;
} hi_spi_receive_s;

/*spi��������*/
extern uint hi_kernel_spi_attr_set(hi_spi_attr_s *pst_attr);
extern uint hi_kernel_spi_attr_get(hi_spi_attr_s *pst_attr);

/*��ȡspiģ���ź���*/
extern void hi_kernel_spi_right_get(void);

/*�ͷ�spiģ���ź���*/
extern void hi_kernel_spi_right_release(void);

/*spi���ͽ���*/
extern uint hi_kernel_spi_send(hi_spi_send_s *pst_send_s);

/*spi����*/
extern uint hi_kernel_spi_receive(hi_spi_receive_s *pst_receive_s);

/*spiͬʱ���ͽ���*/
extern uint hi_kernel_spi_send_receive(uint *ui_send_unit, uint *ui_receive_unit, uint ui_unit_num);

/*spi ͳ�����ݶ�ȡ*/
extern uint hi_kernel_spi_cnt_get(hi_spi_cnt_s *pst_cnt);

#endif /* __HI_DRV_SPI_H__ */
