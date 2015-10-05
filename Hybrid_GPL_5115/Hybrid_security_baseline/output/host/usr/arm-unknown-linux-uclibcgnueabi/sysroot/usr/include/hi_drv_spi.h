/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_spi.h
  功能描述: spi驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_SPI_H__
#define __HI_DRV_SPI_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*可选择的从机*/
typedef enum
{
    HI_SPI_SLAVE_0_E = 0,                         /*从机0*/
    HI_SPI_SLAVE_1_E,                            /*从机1*/
    HI_SPI_SLAVE_2_E,
    HI_SPI_SLAVE_3_E,
    HI_SPI_SLAVE_4_E,
    HI_SPI_SLAVE_5_E,
    HI_SPI_SLAVE_6_E,
    HI_SPI_SLAVE_7_E
} HI_SPI_SLAVE_SEL_E;

/*可选的数据桢的长度*/
typedef enum
{
    HI_SPI_FRAME_LENGTH_4_BIT_E = 0,              /*4位的帧长度*/
    HI_SPI_FRAME_LENGTH_5_BIT_E,                  /*5位的帧长度*/
    HI_SPI_FRAME_LENGTH_6_BIT_E,                  /*6位的帧长度*/
    HI_SPI_FRAME_LENGTH_7_BIT_E,                  /*7位的帧长度*/
    HI_SPI_FRAME_LENGTH_8_BIT_E,                  /*8位的帧长度*/
    HI_SPI_FRAME_LENGTH_9_BIT_E,                  /*9位的帧长度*/
    HI_SPI_FRAME_LENGTH_10_BIT_E,                 /*10位的帧长度*/
    HI_SPI_FRAME_LENGTH_11_BIT_E,                 /*11位的帧长度*/
    HI_SPI_FRAME_LENGTH_12_BIT_E,                 /*12位的帧长度*/
    HI_SPI_FRAME_LENGTH_13_BIT_E,                 /*13位的帧长度*/
    HI_SPI_FRAME_LENGTH_14_BIT_E,                 /*14位的帧长度*/
    HI_SPI_FRAME_LENGTH_15_BIT_E,                 /*15位的帧长度*/
    HI_SPI_FRAME_LENGTH_16_BIT_E                  /*16位的帧长度*/
} HI_SPI_FRAME_LENGTH_E;

/*串行时钟相位, 决定数据和时钟的关系*/
typedef enum
{
    HI_SPI_CLOCK_EDGE_UP_E = 0,                    /*在片选下降沿开始传输数据，数据要在片选有效之前准备好*/
    HI_SPI_CLOCK_EDGE_DOWN_E                       /*在片选有效后的第一个时钟沿开始传输数据，数据可以在片选有效后准备好*/
} HI_SPI_CLOCK_EDGE_E;

/*空闲时候输出时钟极性*/
typedef enum
{
    HI_SPI_IDLE_CLOCK_POLARITY_LOW_E = 0,           /*SPI总线空闲状态时输出时钟为低*/
    HI_SPI_IDLE_CLOCK_POLARITY_HIGH_E               /*SPI总线空闲状态时输出时钟为高*/
} HI_SPI_IDLE_CLOCK_POLARITY_E;

/*SPI 测试模式*/
typedef enum
{
    HI_SPI_IN_LOOPBACK_E = 0,                       /*内环回测试模式*/
    HI_SPI_OUT_LOOPBACK_E                           /*外环回测试模式*/
} HI_SPI_LOOPBACK_MODE_E;

/*SPI 帧格式选择*/
typedef enum
{
    HI_SPI_MOTOR_SPI_E = 0,                          /*00：Motorola SPI帧格式；默认为此模式*/
    HI_SPI_TI_SSP_E,                                 /*01：TI SSP帧格式；*/
    HI_SPI_NATIONAL_MOCROWIRE_E                      /*10：National Microwire帧格式；*/
} HI_SPI_FRAME_FORMAT_E;

/*SPI基本属性配置*/
typedef struct
{
    uint                         ui_index; /*spi索引号，取值为[0，1],只对SD5115H有效,其他均为默认0*/
    HI_BOOL_E                    em_enable;                  /*HI_TRUE_E: spi使能 HI_FALSE_E: spi禁止*/
    HI_SPI_SLAVE_SEL_E           em_slave_select;            /*从机选择*/
    uint                         ui_baud_rate;               /*波特率, 范围：(总线时钟/65534 + 1) ~ 8000000*/
    HI_SPI_FRAME_LENGTH_E        em_frame_length;            /*数据帧单元长度*/
    HI_SPI_CLOCK_EDGE_E          em_clock_edge;              /*串行时钟相位*/
    HI_SPI_IDLE_CLOCK_POLARITY_E em_idle_clock_polarity;     /*空闲时候的时钟的极性 */
    HI_SPI_FRAME_FORMAT_E        em_frame_format;            /*帧格式类型，分为三种，1. Motor SPI格式; 2.TI SSP格式; 3.National Microwire格式，默认为 Motor SPI格式*/
} hi_spi_attr_s;

/*统计，异常记录等*/
typedef struct
{
    /*通信统计信息*/
    uint ui_read_units;                              /*读数据单元数*/
    uint ui_write_units;                             /*写数据单元数*/

    /*通信异常统计信息*/
    uint ui_read_failure_cnt;                        /*读失败计数*/
    uint ui_write_failure_cnt;                       /*写失败计数*/
    uint ui_invalid_parameter_cnt;                   /*无效参数计数*/
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

/*spi属性配置*/
extern uint hi_kernel_spi_attr_set(hi_spi_attr_s *pst_attr);
extern uint hi_kernel_spi_attr_get(hi_spi_attr_s *pst_attr);

/*获取spi模块信号量*/
extern void hi_kernel_spi_right_get(void);

/*释放spi模块信号量*/
extern void hi_kernel_spi_right_release(void);

/*spi发送接收*/
extern uint hi_kernel_spi_send(hi_spi_send_s *pst_send_s);

/*spi接收*/
extern uint hi_kernel_spi_receive(hi_spi_receive_s *pst_receive_s);

/*spi同时发送接收*/
extern uint hi_kernel_spi_send_receive(uint *ui_send_unit, uint *ui_receive_unit, uint ui_unit_num);

/*spi 统计数据读取*/
extern uint hi_kernel_spi_cnt_get(hi_spi_cnt_s *pst_cnt);

#endif /* __HI_DRV_SPI_H__ */
