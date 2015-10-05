/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_special_pkt.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2009��2��3��
  ����޸�   :
  ��������   : SPECIAL PKTģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��2��3��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

***************************************************************************** */


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus  */


#ifndef __HI_DRV_DP_SPECIAL_PKT_H__
#define __HI_DRV_DP_SPECIAL_PKT_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

 
/* ��֧��CAR���Ƶ����ⱨ�ĵ�������Ŀ */
#define HI_DP_UNSUPPORT_CAR_NUM  4
/* ֧��CAR���Ƶ����ⱨ�ĵ�������Ŀ */
#define HI_DP_SUPPORT_CAR_NUM    12
 
#define HI_DP_POINT_TYPE_NUM  4
 #define HI_DP_PPPOE_POINT_TYPE_NUM  3

 
typedef struct  
{

    /* 4 �ֿ�ָ�������ⱨ�Ķ��� */
    uint    ui_point_type[4];

    /* 3���Զ���PPPoE�����ϲ�Э������ */
    uint    ui_pppoe_point_type[HI_DP_PPPOE_POINT_TYPE_NUM];
    
}hi_dp_special_pkt_attr_s;

 typedef struct
{
    HI_DP_SPECIAL_PKT_TYPE_E    em_type;    /* �������ͣ���Ϊ������� */

    HI_BOOL_E    em_security_en;     /* �Ƿ�μӰ�ȫģ���smac�����vlan��� */
    HI_BOOL_E    em_high_arb;        /* �����ȴ��� */
    HI_BOOL_E    em_car_en[HI_DP_ETH_PORT_NUM + 1];   /* ��̫��+PON�ڣ������Ƿ����car */ 
#ifdef  CONFIG_HISI_SD5115_TYPE_T
    HI_DP_TRANS_ACT_E  em_act[HI_DP_ETH_PORT_NUM + 2];   /* ��̫��+PON+CPU������ת����Ϊ */
#else
    HI_DP_TRANS_ACT_E  em_act[HI_DP_ETH_PORT_NUM + 1];   /* ��̫��+PON������ת����Ϊ */
#endif
}hi_dp_special_pkt_trans_table_item_s;
 

typedef struct  
{
    uint ui_drop;               /* �����ı��ĸ���ͳ��                    */
    uint ui_tocpu;              /* �����CPU�ı��ĸ���ͳ��               */
    uint ui_thru;               /* ͸������  */
   uint cnt[HI_DP_CAP_PKT_CFG_SIZE];


}hi_dp_special_pkt_cnt_s;



#endif /* __HI_DRV_DP_SPECIAL_PKT_H__  */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus  */



