/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_gpon_dn.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : GPON DN ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��24��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

  2.��    ��   : 2011��9��16��
    ��    ��   : f00125610
    �޸�����   : �޸��ļ�

***************************************************************************** */

#ifndef __HI_DRV_DP_GPON_DN_H__
#define __HI_DRV_DP_GPON_DN_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_DP_GPON_KEY_LEN 4   /* ��Կ��8��16bit */

/* gpon dn ������������ */
typedef struct
{
    /* �����������޸�Ϊ������С����ֵ���ã���Ҫ��SDK��ؿ�����Ա��ͨ */
    uint       ui_key_exchange_time;            /* ������Կ�л�ʱ�� */
    uint       ui_key_a[HI_DP_GPON_KEY_LEN];    /* A����Կ[0]��Ӧ���16λ��[7]��Ӧ���16λ */
    uint       ui_key_b[HI_DP_GPON_KEY_LEN];    /* B����Կ[0]��Ӧ���16λ��[7]��Ӧ���16λ */
    uint       ui_sd_thr;                       /* SD�澯���� */
    uint       ui_sf_thr;                       /* SF�澯���� */
    HI_LEVEL_E em_los_level;                    /* LOS ��ƽ */

    uint              ui_ber_interval;          /* ���������ֽڼ�� */
    HI_DP_TRANS_ACT_E em_umc_act;               /* δ֪�鲥���˻���͸�� */
    HI_BOOL_E         em_eapol_en;              /* PON�ڵ�EAPOL����ʹ�� */

    void (*ploam_call_back)(hi_ploam_message_s * st_ploam);
    void (*omci_call_back)(hi_omci_message_s * st_omci);
} hi_dp_gpon_dn_attr_s;

/* TypeB�Ż����� */
typedef enum
{
    HI_DP_GPON_DN_EQD_ADJ = 0,   /* EQD������Ϣ�߼����� */
    HI_DP_GPON_UP_RANGE_TIME_E,  /* range time��Ϣ�߼����� */
    HI_DP_GPON_DN_POP_UP_E,      /* �㲥POP UP��Ϣ�߼����� */
} HI_DP_GPON_DN_TYPEB_OP_E;

typedef struct
{
    HI_DP_GPON_DN_TYPEB_OP_E em_op;
    HI_BOOL_E                em_enable;
} hi_dp_gpon_dn_typeb_op_s;

/* ����GEMPORT ���� */
typedef enum
{
    HI_DP_GPON_DN_OMCI_PORT_E = 0,   /* ���ڳ���OMCI��Port */
    HI_DP_GPON_DN_PRBS_PORT_E,      /* ���ڳ���PRBS���ĵ�Port */
    HI_DP_GPON_DN_ETH_PORT_E,        /* ���ڳ��ط��鲥��̫֡��Port */
    HI_DP_GPON_DN_MC_PORT_E,        /* ���ڳ����鲥��Port */
    HI_DP_GPON_DN_PORT_TYPE_NUM_E,
} HI_DP_GPON_DN_PORT_TYPE_E;

/* ����GEMPORT ����ṹ */
typedef struct
{
    uint                      ui_gemport; /* gemport id, �������� 0 - 4095 */
    uint                      ui_pri_remarking; /* �������ȼ�remarking����0 - 7   */
    HI_DP_GPON_DN_PORT_TYPE_E em_type;    /* �˿�����                       */
    HI_BOOL_E                 em_aes_en;  /* �Ƿ����                       */
    HI_BOOL_E                 em_local;   /* �Ƿ񱾵�port                   */
} hi_dp_gpon_dn_gemport_table_item_s;

typedef struct
{
    uint ui_stop_time;             /* Stop time��ֵ��                */
    uint ui_start_time;            /* Start time��ֵ��               */
    uint ui_flag;                  /* FLAG��ֵ��                     */
    uint ui_alloc_id;              /* ALLOC_ID��ֵ��                 */
    uint ui_eof;                   /* ��ǰ֡BWMAP������־��          */
    uint ui_sof;                   /* ��ǰ֡BWMAP��ʼ��־��          */
} hi_dp_gpon_dn_bwmap_table_item_s;

#define HI_DP_GPON_DN_BWMAP_TABLE_SIZE 256

typedef struct
{
    /* 5115�޸�: st_bwmap_table_item-->st_item */
    hi_dp_gpon_dn_bwmap_table_item_s st_item[HI_DP_GPON_DN_BWMAP_TABLE_SIZE];
} hi_dp_gpon_dn_bwmap_table_s;

typedef struct
{
    uint ui_ploam_proc;             /* PLOAM֡������� */
    uint ui_ploam_callback;         /* ���ûص��������� */
    uint ui_ploam_discard;          /* PLOAM֡��ص�����Ϊ�մ������������ */
    uint ui_omci_proc;              /* OMCI֡������� */
    uint ui_omci_callback;          /* ���ûص��������� */
    uint ui_omci_input;             /* OMCI��Ӽ��� */
    uint ui_omci_output;            /* OMCI���Ӽ��� */
    uint ui_omci_discard;           /* OMCI֡��������������� */
    uint ui_omci_over_run;          /* OMCI֡һ���հ�����ָ������FIFO��Ȼ�б��ļ��� */
} hi_dp_gpon_dn_cnt_s;

/* Ӳ����ǰ����ʹ�õ���Կλ�� */
typedef enum
{
    HI_DP_GPON_DN_KEY_A_E = 0,
    HI_DP_GPON_DN_KEY_B_E,
} HI_DP_GPON_DN_KEY_E;

typedef struct
{
    HI_DP_GPON_DN_KEY_E em_current_key;   /* ָʾ��ǰʹ�õ�����ԿA������ԿB */
    HI_BOOL_E           em_fec_en;        /* ����֡FEC���뿪�� */
    HI_BOOL_E           em_key_exchange_en; /* ������Կ�л�ʹ��,ע��CPU������0,Ӳ���л�����Զ���0 */

    HI_DP_ALARM_STATE_E em_los;
    HI_DP_ALARM_STATE_E em_lof;
} hi_dp_gpon_dn_sta_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_GPON_DN_H__ */
