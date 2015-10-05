/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_dma.h
  ��������: dma����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_DMA_H__
#define __HI_DRV_DMA_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*DMAͨ������*/
#define HI_DMA_CHANNEL_NUM 16

/*DMAͨ��*/
typedef enum
{
    HI_DMA_CHANNEL0_E = 0x0,
    HI_DMA_CHANNEL1_E,
    HI_DMA_CHANNEL2_E,
    HI_DMA_CHANNEL3_E,
    HI_DMA_CHANNEL4_E,
    HI_DMA_CHANNEL5_E,
    HI_DMA_CHANNEL6_E,
    HI_DMA_CHANNEL7_E,
    HI_DMA_CHANNEL8_E,
    HI_DMA_CHANNEL9_E,
    HI_DMA_CHANNEL10_E,
    HI_DMA_CHANNEL11_E,
    HI_DMA_CHANNEL12_E,
    HI_DMA_CHANNEL13_E,
    HI_DMA_CHANNEL14_E,
    HI_DMA_CHANNEL15_E,
} HI_DMA_CHANNEL_E;

/*����λ��*/
typedef enum
{
    HI_DMA_DATA_8BIT_E = 0x0,                          /*���ݿ��Ϊ8bit*/
    HI_DMA_DATA_16BIT_E,                               /*���ݿ��Ϊ16bit*/
    HI_DMA_DATA_32BIT_E,                               /*���ݿ��Ϊ32bit,��������ݿ�ȱ���Ϊ32BIT, ��ЧλΪ��xxλ��xx���������ݿ�ȣ�*/
    HI_DMA_DATA_64BIT_E,                               /*���ݿ��Ϊ64bit*/
} HI_DMA_DATA_WIDTH_E;

/*DMA�������ݷ�ʽ*/
typedef enum
{
    HI_DMA_TRANS_M2M_FLOW_CTRL_IS_DMA_E = 0x0,         /*�洢�����洢����DMA����������*/
    HI_DMA_TRANS_M2P_FLOW_CTRL_IS_DMA_E,               /*�洢�������裬DMA����������*/
    HI_DMA_TRANS_P2M_FLOW_CTRL_IS_DMA_E,               /*���赽�洢����DMA����������*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_DMA_E,               /*���赽���裬DMA����������*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_DEST_E,              /*���赽���裬Ŀ����������������, �˷�ʽ��֧��*/
    HI_DMA_TRANS_M2P_FLOW_CTRL_IS_DEST_E,              /*�洢�������裬Ŀ����������������, �˷�ʽ��֧��*/
    HI_DMA_TRANS_P2M_FLOW_CTRL_IS_SRC_E,               /*���赽�洢����Դ��������������, �˷�ʽ��֧��*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_SRC_E,               /*���赽���裬Դ��������������, �˷�ʽ��֧��*/
} HI_DMA_TRANS_MODE_E;

/*BEGIN:Addded by zhouqingsong/2012/2/14  �����������5113��ȣ��б仯*/
/*����������, ����һ��Ϊ����ʱ����Ҫ�������������, Ŀǰֻ֧�����漸������*/
typedef enum
{
    HI_DMA_SPI_RX_REQ_E = 0x0,                  /*SPI ����ͨ��DMA ��������*/
    HI_DMA_SPI_TX_REQ_E,                        /*SPI ����ͨ��DMA ��������*/
    HI_DMA_I2C_M_RX_REQ_E,                      /*I2C Master����ͨ��DMA ��������*/
    HI_DMA_I2C_M_TX_REQ_E,                      /*I2C Master����ͨ��DMA ��������*/
    HI_DMA_I2C_S_RX_REQ_E,                      /*I2C Slave����ͨ��DMA ��������*/
    HI_DMA_I2C_S_TX_REQ_E,                      /*I2C Slave����ͨ��DMA ��������*/
    HI_DMA_UART1_RX_REQ_E,                      /*UART1 ����ͨ��DMA ��������*/
    HI_DMA_UART1_TX_REQ_E,                      /*UART1 ����ͨ��DMA ��������*/
    HI_DMA_UART0_RX_REQ_E,                      /*UART0 ����ͨ��DMA ��������*/
    HI_DMA_UART0_TX_REQ_E,                      /*UART0 ����ͨ��DMA ��������*/
    HI_DMA_SCI_RX_REQ_E,                        /*SCI ����ͨ��DMA ��������*/
    HI_DMA_SCI_TX_REQ_E,                        /*SCI ����ͨ��DMA ��������*/
} HI_DMA_PERIPHERAL_REQ_E;
/*END:Addded by zhouqingsong/2012/2/14  �����������5113��ȣ��б仯*/

/*ͨ��״̬*/
typedef enum
{
    HI_DMA_CHANNEL_IDLE_E = 0x0,                     /*ͨ������*/
    HI_DMA_CHANNEL_BUSY_E,                           /*ͨ��æ*/
} HI_DMA_CHANNEL_STA_E;

/*DMAͨ������*/
typedef struct
{
    HI_DMA_CHANNEL_E        em_channel;              /*DMAͨ����*/
    HI_BOOL_E               em_enable;               /*ͨ��ʹ�ܿ���*/
    HI_DMA_TRANS_MODE_E     em_trans_mode;           /*DMA�������ݵķ�ʽ*/
    HI_DMA_DATA_WIDTH_E     em_src_width;            /*Դ���ݿ��*/
    HI_DMA_DATA_WIDTH_E     em_dest_width;           /*Ŀ�����ݿ��*/
    uint                    ui_src_burst;            /*Դburst����, 1 - 16*/
    uint                    ui_dest_burst;           /*Ŀ��burst����, 1 - 16*/
    HI_DMA_PERIPHERAL_REQ_E em_src_req;              /*Դ��������ţ���DMA�������������ԴΪ����ʱ����*/
    HI_DMA_PERIPHERAL_REQ_E em_dest_req;             /*Ŀ����������ţ���DMA���������Ŀ��Ϊ����ʱ����*/
    void (*call_back)(uint ui_sta);                  /*DMAͨ���жϻص�����, ui_sta��dma����״̬��1: dma�������0��dma�������*/
} hi_dma_channel_attr_s;

/*DMA״̬*/
typedef struct
{
    HI_BOOL_E            em_enable;                                /*DMAʹ��״̬*/
    HI_DMA_CHANNEL_STA_E a_em_channel_sta[HI_DMA_CHANNEL_NUM];     /*ͨ��״̬*/
} hi_dma_sta_s;

/*��ȡDMAͨ������, ��Ҫ��ָ�������Ǹ�ͨ��*/
extern uint hi_kernel_dma_channel_attr_get(hi_dma_channel_attr_s *pst_attr);

/*����DMAͨ�����ԣ���Ҫ��ָ�������Ǹ�ͨ��*/
extern uint hi_kernel_dma_channel_attr_set(hi_dma_channel_attr_s *pst_attr);

/*����ͨ����������*/
extern uint hi_kernel_dma_channel_trans(HI_DMA_CHANNEL_E em_channel, uint ui_src_addr, uint ui_dest_addr,
                                        uint ui_trans_size);

/*��ȡͨ����״̬*/
extern uint hi_kernel_dma_sta_get(hi_dma_sta_s *pst_sta);

/*ǿ�ƽ�ֹdmaͨ��*/
extern uint hi_kernel_dma_channel_disable(HI_DMA_CHANNEL_E em_channel);

#endif /* __HI_DRV_DMA_H__ */
