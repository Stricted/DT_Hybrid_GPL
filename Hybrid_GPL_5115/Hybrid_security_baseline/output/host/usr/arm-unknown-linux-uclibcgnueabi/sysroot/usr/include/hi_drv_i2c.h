/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_i2c.h
  功能描述: i2c驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_I2C_H__
#define __HI_DRV_I2C_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*地址模式*/
typedef enum
{
    HI_I2C_ADDR_MODE_7BIT_E = 0,                         /*7bit地址长度*/
    HI_I2C_ADDR_MODE_10BIT_E                             /*10bit地址长度*/
} HI_I2C_ADDR_MODE_E;

/*I2C工作模式*/
typedef enum
{
    HI_I2C_WORK_MODE_MASTER_E = 0,                       /*主机模式*/
    HI_I2C_WORK_MODE_SLAVE_E                             /*从机模式, 暂时不支持，为以后兼容做准备*/
} HI_I2C_WORK_MODE_E;

/*I2C读写模式选择*/
typedef enum
{
    HI_I2C_NORMAL_MODE_E = 0,                           /*00：当前地址读写模式,默认为此模式*/
    HI_I2C_BURST_MODE_E                                 /*01：burst read/write 读写模式*/
} HI_I2C_RW_MODE_E;

/*I2C基本属性配置*/
typedef struct
{
    HI_BOOL_E          em_enable;                          /*HI_TRUE_E: 使能i2c  HI_FALSE_E: 关闭i2c*/
	HI_BOOL_E          em_multi_master_en;
    HI_I2C_WORK_MODE_E em_mode;                            /*主从模式设置,本版本仅支持主模式*/
    HI_I2C_ADDR_MODE_E em_addr_mode;                       /*地址模式, 主机模式: 对接从机的地址长度，从机模式: i2c作为从设备的地址长度*/
    uint               ui_baud_rate;                       /*从器件工作波特率, 1KHz ~ 400KHz, 单位KHz*/
} hi_i2c_attr_s;

/* 统计，异常记录等*/
typedef struct
{
    /* 通信统计信息 */
    uint ui_read_bytes;
    uint ui_write_bytes;

    /* 通信异常统计信息 */
    uint ui_read_failure_cnt;
    uint ui_write_failure_cnt;
    uint ui_invalid_parameter_cnt;
} hi_i2c_cnt_s;

/*i2c模块信号量的读取与释放*/
extern void hi_kernel_i2c_right_get(void);
extern void hi_kernel_i2c_right_release(void);

/*i2c属性配置*/
extern uint hi_kernel_i2c_attr_set(hi_i2c_attr_s *pst_attr);
extern uint hi_kernel_i2c_attr_get(hi_i2c_attr_s *pst_attr);

/*普通I2C接口设备读写*/
extern uint hi_kernel_i2c_read(ushort us_dev_addr, uchar uc_reg_addr, uchar *puc_data);
extern uint hi_kernel_i2c_write(ushort us_dev_addr, uchar uc_reg_addr, uchar uc_data);

/*连续I2C接口设备读写*/
extern uint hi_kernel_i2c_send(ushort us_dev_addr, uchar *puc_data, uint ui_length, HI_I2C_RW_MODE_E i2c_rw_mode);
extern uint hi_kernel_i2c_receive(ushort us_dev_addr, uchar *puc_data, uint ui_length, HI_I2C_RW_MODE_E i2c_rw_mode);
extern uint hi_kernel_i2c_send_receive(ushort us_dev_addr, uchar *puc_send_data, uint ui_send_data_num, uchar *puc_receive_data, uint ui_receive_data_num,HI_I2C_RW_MODE_E i2c_rw_mode);

/*分页I2C设备操作接口*/
extern uint hi_kernel_i2c_page_reg_read(ushort us_dev_addr, uchar uc_page_index, uchar uc_reg_addr, uchar *puc_data);
extern uint hi_kernel_i2c_page_reg_write(ushort us_dev_addr, uchar uc_page_index, uchar uc_reg_addr, uchar uc_data);

/*i2c统计数据读取*/
extern uint hi_kernel_i2c_cnt_get(hi_i2c_cnt_s *pst_cnt);

#endif /* __HI_DRV_I2C_H__ */
