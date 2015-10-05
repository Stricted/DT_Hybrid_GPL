/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_i2c.h
  ��������: i2c����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_I2C_H__
#define __HI_DRV_I2C_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*��ַģʽ*/
typedef enum
{
    HI_I2C_ADDR_MODE_7BIT_E = 0,                         /*7bit��ַ����*/
    HI_I2C_ADDR_MODE_10BIT_E                             /*10bit��ַ����*/
} HI_I2C_ADDR_MODE_E;

/*I2C����ģʽ*/
typedef enum
{
    HI_I2C_WORK_MODE_MASTER_E = 0,                       /*����ģʽ*/
    HI_I2C_WORK_MODE_SLAVE_E                             /*�ӻ�ģʽ, ��ʱ��֧�֣�Ϊ�Ժ������׼��*/
} HI_I2C_WORK_MODE_E;

/*I2C��дģʽѡ��*/
typedef enum
{
    HI_I2C_NORMAL_MODE_E = 0,                           /*00����ǰ��ַ��дģʽ,Ĭ��Ϊ��ģʽ*/
    HI_I2C_BURST_MODE_E                                 /*01��burst read/write ��дģʽ*/
} HI_I2C_RW_MODE_E;

/*I2C������������*/
typedef struct
{
    HI_BOOL_E          em_enable;                          /*HI_TRUE_E: ʹ��i2c  HI_FALSE_E: �ر�i2c*/
	HI_BOOL_E          em_multi_master_en;
    HI_I2C_WORK_MODE_E em_mode;                            /*����ģʽ����,���汾��֧����ģʽ*/
    HI_I2C_ADDR_MODE_E em_addr_mode;                       /*��ַģʽ, ����ģʽ: �ԽӴӻ��ĵ�ַ���ȣ��ӻ�ģʽ: i2c��Ϊ���豸�ĵ�ַ����*/
    uint               ui_baud_rate;                       /*����������������, 1KHz ~ 400KHz, ��λKHz*/
} hi_i2c_attr_s;

/* ͳ�ƣ��쳣��¼��*/
typedef struct
{
    /* ͨ��ͳ����Ϣ */
    uint ui_read_bytes;
    uint ui_write_bytes;

    /* ͨ���쳣ͳ����Ϣ */
    uint ui_read_failure_cnt;
    uint ui_write_failure_cnt;
    uint ui_invalid_parameter_cnt;
} hi_i2c_cnt_s;

/*i2cģ���ź����Ķ�ȡ���ͷ�*/
extern void hi_kernel_i2c_right_get(void);
extern void hi_kernel_i2c_right_release(void);

/*i2c��������*/
extern uint hi_kernel_i2c_attr_set(hi_i2c_attr_s *pst_attr);
extern uint hi_kernel_i2c_attr_get(hi_i2c_attr_s *pst_attr);

/*��ͨI2C�ӿ��豸��д*/
extern uint hi_kernel_i2c_read(ushort us_dev_addr, uchar uc_reg_addr, uchar *puc_data);
extern uint hi_kernel_i2c_write(ushort us_dev_addr, uchar uc_reg_addr, uchar uc_data);

/*����I2C�ӿ��豸��д*/
extern uint hi_kernel_i2c_send(ushort us_dev_addr, uchar *puc_data, uint ui_length, HI_I2C_RW_MODE_E i2c_rw_mode);
extern uint hi_kernel_i2c_receive(ushort us_dev_addr, uchar *puc_data, uint ui_length, HI_I2C_RW_MODE_E i2c_rw_mode);
extern uint hi_kernel_i2c_send_receive(ushort us_dev_addr, uchar *puc_send_data, uint ui_send_data_num, uchar *puc_receive_data, uint ui_receive_data_num,HI_I2C_RW_MODE_E i2c_rw_mode);

/*��ҳI2C�豸�����ӿ�*/
extern uint hi_kernel_i2c_page_reg_read(ushort us_dev_addr, uchar uc_page_index, uchar uc_reg_addr, uchar *puc_data);
extern uint hi_kernel_i2c_page_reg_write(ushort us_dev_addr, uchar uc_page_index, uchar uc_reg_addr, uchar uc_data);

/*i2cͳ�����ݶ�ȡ*/
extern uint hi_kernel_i2c_cnt_get(hi_i2c_cnt_s *pst_cnt);

#endif /* __HI_DRV_I2C_H__ */
