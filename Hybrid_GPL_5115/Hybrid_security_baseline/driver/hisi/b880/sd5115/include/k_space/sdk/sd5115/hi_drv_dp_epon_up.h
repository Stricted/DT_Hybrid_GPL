/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_epon_up.h
  �� �� ��   : ����
  ��    ��   : ��ΰȨ
  ��������   : 2009��7��13��
  ����޸�   :
  ��������   : EPON�����û�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��7��13��
    ��    ��   : ��ΰȨ
    �޸�����   : �����ļ�

  2.��    ��   : 2011��9��16��
    ��    ��   : f00125610
    �޸�����   : �޸��ļ�
***************************************************************************** */

#ifndef __HI_DRV_DP_EPON_UP_H__
#define __HI_DRV_DP_EPON_UP_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* ----------------------------------------------*
 * �ⲿ����˵��                                 *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * ȫ�ֱ���                                     *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * ģ�鼶����                                   *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * ��������                                     *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * �궨��                                       *
 *---------------------------------------------- */



typedef enum
{
    HI_DP_EPON_UP_QUEUE0_E = 0,
    HI_DP_EPON_UP_QUEUE1_E,
    HI_DP_EPON_UP_QUEUE2_E,
    HI_DP_EPON_UP_QUEUE3_E,
    HI_DP_EPON_UP_QUEUE4_E,
    HI_DP_EPON_UP_QUEUE5_E,
    HI_DP_EPON_UP_QUEUE6_E,
    HI_DP_EPON_UP_QUEUE7_E,
    HI_DP_EPON_UP_QUEUE_NUM,
}HI_DP_EPON_UP_QUEUE_E;


/* ��ģ�鷢�����ģʽ */
typedef enum
{
    HI_DP_EPON_UP_TX_AUTO_E  = 0,   /* �߼��Զ����� */
    HI_DP_EPON_UP_TX_ENABLE_E ,     /* ������ */
    HI_DP_EPON_UP_TX_DISABLE_E,     /* ������ */
}HI_DP_EPON_UP_TX_MODE_E;


/* ���������⣬����������� */
typedef struct
{
    HI_LEVEL_E em_enable_level;              /* ����������ʹ����Ч��ƽ*/
    HI_LEVEL_E em_sd_level;                  /* ��ģ�������TX SD�źŵļ��ԡ�*/
    HI_DP_EPON_UP_TX_MODE_E em_tx_mode;      /* �������ģʽ*/    

    uint ui_ctrl_delay;                 /* Laser Controlʱ���ӳٵ������á���λΪ16ns */ 
    uint ui_laser_on_delay;             /* ��PON MACָʾ��ģ��رշ��⵽��ģ��رշ���״̬���ӳ� */
    uint ui_laser_off_delay;            /* ��PON MACָʾ��ģ�鷢�⵽��ģ���������״̬���ӳ� */
}hi_dp_epon_up_laser_attr_s;


/* MPCPע������ */
typedef struct
{
    HI_BOOL_E em_reg_en;         /* ע��ʹ�� */
    uint ui_reg_llid_index;      /* Ҫע���LLID���� */
    uint ui_dis_sync_time;       /* Discovery Grant ��ͬ��ʱ�� */
    uint ui_dis_random_dly;      /* Discovery Grant �������ʱ */
    uint ui_sync_time;           /* Normal Grant ��ͬ��ʱ�� */
}hi_dp_epon_up_register_attr_s;


/* �������� */
typedef struct
{
    HI_BOOL_E em_fec_en[HI_DP_EPON_LLID_NUM];        /* FEC����ʹ�� */
#if defined(CONFIG_HISI_SD5115_TYPE_T)
    HI_BOOL_E em_fec_adapt_en;                       /* 5115T add: ����FEC����Ӧʹ�� */
#endif
    uint  ui_ipg;                                    /* ���з���֡��� ��λΪ1BYTEʱ�䣬Ĭ��Ϊ12 */
    hi_dp_epon_up_laser_attr_s st_tx_attr;           /* ���������⣬���Ͳ���������� */
    HI_BOOL_E em_dying_gasp_tx_en;                   /* �߼��Ƿ���Dyinggasp */
    hi_dp_epon_up_register_attr_s st_reg_attr;       /* MPCPע������ */

}hi_dp_epon_up_attr_s;


/* ����GEMPORT ӳ������ṹ */
typedef struct
{
    uint ui_sfid;           /* ������ID, ���� */
    uint ui_llid;           /* LLID */
}hi_dp_epon_up_map_table_item_s;


#define HI_DP_EPON_THRESH_NUM   3

typedef struct
{
    HI_BOOL_E em_enable;
    uint      ui_report;    /* Reportֵֻ�� */
    ushort    us_thresh[HI_DP_EPON_THRESH_NUM];
    ushort    reserved;
}hi_dp_epon_up_report_queue_s;


/* Report�����������ݽṹ */
typedef struct
{
    uint ui_llid;
    uint ui_qsets_num;      /* ���м�����  1 - 4 */
    hi_dp_epon_up_report_queue_s st_queue[HI_DP_EPON_UP_QUEUE_NUM];
}hi_dp_epon_up_report_table_item_s;


/* ״̬ */
typedef struct
{

}hi_dp_epon_up_sta_s;


/* ͳ����Ϣ */
typedef struct
{
    ulong ul_tx_mpcp;
    ulong ul_tx_oam;
}hi_dp_epon_up_cnt_s;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_EPON_UP_H__ */

