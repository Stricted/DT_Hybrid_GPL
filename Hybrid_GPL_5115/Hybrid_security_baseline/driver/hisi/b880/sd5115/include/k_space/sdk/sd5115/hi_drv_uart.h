/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_uart.h
  功能描述: uart驱动头文件
  版本描述: V1.0
  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组
  修改记录:
******************************************************************************/
#ifndef __HI_DRV_UART_H__
#define __HI_DRV_UART_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*uart端口*/
typedef enum
{
    HI_UART0_0_E = 0x0,                          /*uart0*/
    HI_UART0_1_E,                                 /* 5115 ASIC-S and FPGA only one UART0, 5115 ASIC-H UART0_0 UART0_1 */
    HI_UART1_0_E,                                /*uart1*/
    HI_UART1_1_E,                                /* 5115 ASIC-S and FPGA only one UART1, 5115 ASIC-H UART1_0 UART1_1 */
} HI_UART_E;

/*uart波特率*/
typedef enum
{
    HI_UART_BAUD_RATE_9600_E = 0x0,            /*9600bit/s*/
    HI_UART_BAUD_RATE_38400_E,                 /*38400bit/s*/
    HI_UART_BAUD_RATE_57600_E,                 /*57600bit/s*/
    HI_UART_BAUD_RATE_115200_E,                /*115200bit/s*/
    HI_UART_BAUD_RATE_187500_E,                /*187500bit/s*/
} HI_UART_BAUD_RATE_E;

/*uart数据位长度, 注意枚举的值顺序不可以变，枚举值与硬件寄存器值一一对应*/
typedef enum
{
    HI_UART_DATA_BIT5_E = 0x0,                  /*5bit数据位*/
    HI_UART_DATA_BIT6_E,                        /*6bit数据位*/
    HI_UART_DATA_BIT7_E,                        /*7bit数据位*/
    HI_UART_DATA_BIT8_E,                        /*8bit数据位*/
} HI_UART_DATA_BIT_E;

/*uart停止位长度，注意枚举的值顺序不可以变，枚举值与硬件寄存器值一一对应*/
typedef enum
{
    HI_UART_STOP_BIT1_E = 0x0,                  /*数据停止位为1bit*/
    HI_UART_STOP_BIT2_E,                        /*数据停止位为2bit, 当数据位选择5bit时候，停止位是1.5bit*/
} HI_UART_STOP_BIT_E;

/*uart校验*/
typedef enum
{
    HI_UART_PARITY_NO_E = 0x0,                   /*无校验*/
    HI_UART_PARITY_ODD_E,                        /*奇校验*/
    HI_UART_PARITY_EVEN_E,                       /*偶校验*/
} HI_UART_PARITY_E;

/*uart测试模式*/
typedef enum
{
    HI_UART_IN_LOOPBACK_E = 0x0,                 /*内环回测试模式*/
    HI_UART_OUT_LOOPBACK_E,                      /*外环回测试模式*/
} HI_UART_LOOPBACK_MODE_E;

/*uart基本属性配置*/
typedef struct
{
    HI_BOOL_E           em_enable;                /*HI_TRUE_E: uart使能   HI_FALSE_E: uart关闭*/
    HI_UART_E           em_uart;                  /*uart端口*/
    HI_UART_BAUD_RATE_E em_baud_rate;             /*uart波特率*/
    HI_UART_DATA_BIT_E  em_data_bit;              /*数据位配置*/
    HI_UART_STOP_BIT_E  em_stop_bit;              /*停止位配置*/
    HI_UART_PARITY_E    em_parity;                /*校验配置*/
} hi_uart_attr_s;

/*统计异常记录*/
typedef struct
{
    /*通信统计信息*/
    uint ui_read_bytes;                           /*读字节数统计*/
    uint ui_write_bytes;                          /*写字节数统计*/

    /*通信异常统计信息*/
    uint ui_read_failure_cnt;                     /*读失败次数统计*/
    uint ui_write_failure_cnt;                    /*写失败次数统计*/
    uint ui_invalid_parameter_cnt;                /*输入无效参数次数统计*/
} hi_uart_cnt_s;

/*配置属性操作*/
extern uint hi_kernel_uart_attr_set(hi_uart_attr_s *pst_attr);
extern uint hi_kernel_uart_attr_get(hi_uart_attr_s *pst_attr);

/*通信接口*/
extern uint hi_kernel_uart_send_ready(void);
extern uint hi_kernel_uart_send(uchar *puc_send_buffer, uint ui_length);
extern uint hi_kernel_uart_receive_ready(void);
extern uint hi_kernel_uart_receive(uchar *puc_receive_buffer, uint ui_length, uint *pui_received_length);

/*统计信息*/
extern uint hi_kernel_uart_cnt_get(hi_uart_cnt_s *pst_cnt);

#endif /*__HI_DRV_UART_H__*/
