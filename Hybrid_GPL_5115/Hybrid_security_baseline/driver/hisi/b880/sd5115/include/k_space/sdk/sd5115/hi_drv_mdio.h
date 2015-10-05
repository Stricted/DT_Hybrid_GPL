/******************************************************************************
  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
 �ļ�����: hi_drv_mdio.h
 ��������: mdio����ͷ�ļ�
 �汾����: V1.0

 ��������: 2009��1��23��
 ��������: ��˼SD511x оƬSDK������

 �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_MDIO_H__
#define __HI_DRV_MDIO_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"
#ifdef CONFIG_HISI_SD5115_AF_FPGA

/*���PHY������*/
 #define HI_MDIO_PHY_NUM 6    //G174254: ����
#endif

/*������, ˳����������*/
typedef enum
{
    HI_MDIO_BAUD_RATE_2M5_E = 0,        /* �����ʣ�2.5MHz  */
    HI_MDIO_BAUD_RATE_1M25_E,           /* �����ʣ�1.25MHz */
    HI_MDIO_BAUD_RATE_5M_E,             /* �����ʣ�5M      */
    HI_MDIO_BAUD_RATE_10M_E,            /* �����ʣ�10MHz   */
} HI_MDIO_BAUD_RATE_E;

/*mdio ������������*/
typedef struct
{
    HI_BOOL_E           em_enable;     //G174254: ����
    HI_MDIO_BAUD_RATE_E em_baud_rate;   /*������*/
} hi_mdio_attr_s;

/*ͳ�ƣ��쳣��¼��*/
typedef struct
{
    /*ͨ��ͳ����Ϣ*/
    uint ui_read_cnt;                                        /*������ͳ��*/
    uint ui_write_cnt;                                       /*д����ͳ��*/

    /*ͨ���쳣ͳ����Ϣ*/
    uint ui_read_failure_cnt;                                /*��ʧ�ܼ���*/
    uint ui_write_failure_cnt;                               /*дʧ�ܼ���*/
    uint ui_invalid_parameter_cnt;                           /*��Ч��������*/
} hi_mdio_cnt_s;

/*�������Բ���*/
extern uint hi_kernel_mdio_attr_set(hi_mdio_attr_s *pst_attr);
extern uint hi_kernel_mdio_attr_get(hi_mdio_attr_s *pst_attr);

/*��ͨ��д����, �����ù���ģʽΪ��ͨģʽ���ʹ��, phy��ַ��phy�Ĵ�����ַ����5bit*/
extern uint hi_kernel_mdio_write(uint ui_phy_addr, uint ui_reg_addr, ushort us_data);
extern uint hi_kernel_mdio_read(uint ui_phy_addr, uint ui_reg_addr, ushort *pus_data);

/*ͳ����Ϣ*/
extern uint hi_kernel_mdio_cnt_get(hi_mdio_cnt_s *pst_cnt);

#endif /*__HI_DRV_MDIO_H__*/
