/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_dma.h
  功能描述: dma驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_DMA_H__
#define __HI_DRV_DMA_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*DMA通道数量*/
#define HI_DMA_CHANNEL_NUM 16

/*DMA通道*/
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

/*数据位宽*/
typedef enum
{
    HI_DMA_DATA_8BIT_E = 0x0,                          /*数据宽度为8bit*/
    HI_DMA_DATA_16BIT_E,                               /*数据宽度为16bit*/
    HI_DMA_DATA_32BIT_E,                               /*数据宽度为32bit,外设的数据宽度必须为32BIT, 有效位为低xx位（xx是外设数据宽度）*/
    HI_DMA_DATA_64BIT_E,                               /*数据宽度为64bit*/
} HI_DMA_DATA_WIDTH_E;

/*DMA传输数据方式*/
typedef enum
{
    HI_DMA_TRANS_M2M_FLOW_CTRL_IS_DMA_E = 0x0,         /*存储器到存储器，DMA作流控制器*/
    HI_DMA_TRANS_M2P_FLOW_CTRL_IS_DMA_E,               /*存储器到外设，DMA作流控制器*/
    HI_DMA_TRANS_P2M_FLOW_CTRL_IS_DMA_E,               /*外设到存储器，DMA作流控制器*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_DMA_E,               /*外设到外设，DMA作流控制器*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_DEST_E,              /*外设到外设，目标外设作流控制器, 此方式不支持*/
    HI_DMA_TRANS_M2P_FLOW_CTRL_IS_DEST_E,              /*存储器到外设，目标外设作流控制器, 此方式不支持*/
    HI_DMA_TRANS_P2M_FLOW_CTRL_IS_SRC_E,               /*外设到存储器，源外设作流控制器, 此方式不支持*/
    HI_DMA_TRANS_P2P_FLOW_CTRL_IS_SRC_E,               /*外设到外设，源外设作流控制器, 此方式不支持*/
} HI_DMA_TRANS_MODE_E;

/*BEGIN:Addded by zhouqingsong/2012/2/14  外设请求号与5113相比，有变化*/
/*外设的请求号, 当有一端为外设时候需要配置外设请求号, 目前只支持下面几种外设*/
typedef enum
{
    HI_DMA_SPI_RX_REQ_E = 0x0,                  /*SPI 接收通道DMA 传输请求*/
    HI_DMA_SPI_TX_REQ_E,                        /*SPI 发送通道DMA 传输请求*/
    HI_DMA_I2C_M_RX_REQ_E,                      /*I2C Master接收通道DMA 传输请求*/
    HI_DMA_I2C_M_TX_REQ_E,                      /*I2C Master发送通道DMA 传输请求*/
    HI_DMA_I2C_S_RX_REQ_E,                      /*I2C Slave接收通道DMA 传输请求*/
    HI_DMA_I2C_S_TX_REQ_E,                      /*I2C Slave发送通道DMA 传输请求*/
    HI_DMA_UART1_RX_REQ_E,                      /*UART1 接收通道DMA 传输请求*/
    HI_DMA_UART1_TX_REQ_E,                      /*UART1 发送通道DMA 传输请求*/
    HI_DMA_UART0_RX_REQ_E,                      /*UART0 接收通道DMA 传输请求*/
    HI_DMA_UART0_TX_REQ_E,                      /*UART0 发送通道DMA 传输请求*/
    HI_DMA_SCI_RX_REQ_E,                        /*SCI 接收通道DMA 传输请求*/
    HI_DMA_SCI_TX_REQ_E,                        /*SCI 发送通道DMA 传输请求*/
} HI_DMA_PERIPHERAL_REQ_E;
/*END:Addded by zhouqingsong/2012/2/14  外设请求号与5113相比，有变化*/

/*通道状态*/
typedef enum
{
    HI_DMA_CHANNEL_IDLE_E = 0x0,                     /*通道空闲*/
    HI_DMA_CHANNEL_BUSY_E,                           /*通道忙*/
} HI_DMA_CHANNEL_STA_E;

/*DMA通道属性*/
typedef struct
{
    HI_DMA_CHANNEL_E        em_channel;              /*DMA通道号*/
    HI_BOOL_E               em_enable;               /*通道使能开关*/
    HI_DMA_TRANS_MODE_E     em_trans_mode;           /*DMA传输数据的方式*/
    HI_DMA_DATA_WIDTH_E     em_src_width;            /*源数据宽度*/
    HI_DMA_DATA_WIDTH_E     em_dest_width;           /*目的数据宽度*/
    uint                    ui_src_burst;            /*源burst长度, 1 - 16*/
    uint                    ui_dest_burst;           /*目的burst长度, 1 - 16*/
    HI_DMA_PERIPHERAL_REQ_E em_src_req;              /*源外设请求号，当DMA传输请求的数据源为外设时设置*/
    HI_DMA_PERIPHERAL_REQ_E em_dest_req;             /*目标外设请求号，当DMA传输请求的目标为外设时设置*/
    void (*call_back)(uint ui_sta);                  /*DMA通道中断回调函数, ui_sta是dma传输状态，1: dma传输错误，0：dma传输完成*/
} hi_dma_channel_attr_s;

/*DMA状态*/
typedef struct
{
    HI_BOOL_E            em_enable;                                /*DMA使能状态*/
    HI_DMA_CHANNEL_STA_E a_em_channel_sta[HI_DMA_CHANNEL_NUM];     /*通道状态*/
} hi_dma_sta_s;

/*获取DMA通道属性, 需要先指名操作那个通道*/
extern uint hi_kernel_dma_channel_attr_get(hi_dma_channel_attr_s *pst_attr);

/*配置DMA通道属性，需要先指名操作那个通道*/
extern uint hi_kernel_dma_channel_attr_set(hi_dma_channel_attr_s *pst_attr);

/*启动通道传送数据*/
extern uint hi_kernel_dma_channel_trans(HI_DMA_CHANNEL_E em_channel, uint ui_src_addr, uint ui_dest_addr,
                                        uint ui_trans_size);

/*获取通道的状态*/
extern uint hi_kernel_dma_sta_get(hi_dma_sta_s *pst_sta);

/*强制禁止dma通道*/
extern uint hi_kernel_dma_channel_disable(HI_DMA_CHANNEL_E em_channel);

#endif /* __HI_DRV_DMA_H__ */
