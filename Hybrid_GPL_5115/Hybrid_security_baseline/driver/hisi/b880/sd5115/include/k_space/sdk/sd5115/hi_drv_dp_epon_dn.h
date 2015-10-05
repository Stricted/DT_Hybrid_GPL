/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_epon_dn.h
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

  2.��    ��   : 2011��9��15��
    ��    ��   : f00125610
    �޸�����   : 5115�޸�
***************************************************************************** */

#ifndef __HI_DRV_DP_EPON_DN_H__
#define __HI_DRV_DP_EPON_DN_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* �������� */
typedef struct
{
    HI_BOOL_E         em_eapol_en;                  /* PON�ڵ�EAPOL����ʹ�� */
    HI_BOOL_E         em_fec_en;                    /* ����FEC����ʹ�� */
    HI_LEVEL_E        em_los_level;                 /* LOS ��Ч��ƽ */
    HI_DP_TRANS_ACT_E em_umc_act;                   /* �鲥���˻���͸�� */
    uint              ui_gate_timeout;              /* Gate��ʱ���� */
    uint              ui_rtt_adj;                   /* RTT ����ʱ�䲹�� */

    void (*mpcp_call_back)(hi_mpcp_message_s * st_mpcp);
    void (*oam_call_back)(hi_oam_message_s * st_oam);
} hi_dp_epon_dn_attr_s;

#define HI_DP_EPON_KEY_LEN 4   /* 5115����: ��Կ���ȣ���128bit */

/* ֧�����ּ��ܷ�ʽ���޸���Կ���ȡ�*/
typedef struct
{
    uint ui_key0[HI_DP_EPON_KEY_LEN];  /* 5115�޸�: ��ԿID 0 */
    uint ui_key1[HI_DP_EPON_KEY_LEN];  /* 5115�޸�: ��ԿID 1 */
} hi_dp_epon_dn_key_s;


#define HI_DP_EPON_INVALID_LLID 0x7fff

/* ����llid ���˱���ע������Ҳ���ѯ�ñ� */
typedef struct
{
    uint                ui_index;      /* ����  0-7, TΪ0-15 */
    HI_BOOL_E           em_registered; /* ע��״̬ */
    HI_BOOL_E           em_mod_en;     /* 5115����: MOD����ʹ�� */
    uint                ui_llid;       /* ע���LLID */
    hi_mac_s            st_mac;        /* ʹ�õ�MAC��ַ */
    hi_dp_epon_dn_key_s st_key;        /* 5115�޸�: ֧�����ּ��ܷ�ʽ */
} hi_dp_epon_dn_llid_table_item_s;

typedef struct
{
    HI_DP_ALARM_STATE_E em_ftm_los;
    HI_BOOL_E           em_synchronized;                   /* ֡ͬ��״̬ */
    uint                ui_key_index[HI_DP_EPON_LLID_NUM]; /* ��LLID�����������֡�ļ�����Կ���� */
    HI_BOOL_E           em_cipher_st[HI_DP_EPON_LLID_NUM]; /* ��LLID�����������֡�ļ���״̬ */
    HI_BOOL_E em_fec_up_st[HI_DP_EPON_LLID_NUM];           /* 5115T add: ��LLID����FEC����״̬ */
    HI_BOOL_E em_fec_dn_st[HI_DP_EPON_LLID_NUM];           /* 5115T add: ��LLID����FEC����״̬ */
} hi_dp_epon_dn_sta_s;

/* ͳ����Ϣ */
typedef struct
{
    ulong ul_mpcp_call_back_cnt;
    ulong ul_mpcp_call_back_dsicard_cnt;
    ulong ul_mpcp_err_discard_cnt;
    ulong ul_rx_mpcp_cnt;

    ulong ul_oam_call_back_cnt;
    ulong ul_oam_call_back_dsicard_cnt;
    ulong ul_oam_err_discard_cnt;
    ulong ul_rx_oam_cnt;
} hi_dp_epon_dn_cnt_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_EPON_DN_H__ */
