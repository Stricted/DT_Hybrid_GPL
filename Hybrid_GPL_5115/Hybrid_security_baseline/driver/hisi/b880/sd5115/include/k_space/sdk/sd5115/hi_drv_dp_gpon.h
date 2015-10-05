/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_gpon.h
  �� �� ��   : ����
  ��    ��   : shengjing
  ��������   : 2011��5��25��
  ����޸�   :
  ��������   : gponģ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��5��25��
    ��    ��   : shengjing
    �޸�����   : �����ļ�

  2.��    ��   : 2011��9��16��
    ��    ��   : f00125610
    �޸�����   : �޸��ļ�

******************************************************************************/
#ifndef __HI_DRV_DP_GPON_H__
#define __HI_DRV_DP_GPON_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef enum
{
    HI_DP_GPON_LOS_OCCUR_ALARM_E = 0,
    HI_DP_GPON_LOS_DIS_ALARM_E,
    HI_DP_GPON_LOF_OCCUR_ALARM_E,
    HI_DP_GPON_LOF_DIS_ALARM_E,
    HI_DP_GPON_DYINGGASP_ALARM_E,
    HI_DP_GPON_REI_COUNTER_ALARM_E,
    HI_DP_GPON_SF_ALARM_E,
    HI_DP_GPON_SD_ALARM_E,
    HI_DP_GPON_LCDG_ALARM_E,
#if defined (CONFIG_HISI_SD5115_MP_PILOT)
    HI_DP_GPON_BW_IDLE_ALARM_E,
#endif
    HI_DP_GPON_ROGUE_ONU_ALARM_E,
    HI_DP_GPON_SILENT_ONU_ALARM_E,
    HI_DP_GPON_LWI_ALARM_E,
    HI_DP_GPON_FWI_ALARM_E,
    HI_DP_GPON_EMPTY_ALARM_E,
    HI_DP_GPON_ALARM_NUM_E,
} HI_DP_GPON_ALARM_E;

#define HI_DP_GPON_LWI_EGR_BIT  HI_BIT_1
#define HI_DP_GPON_LWI_IGR_BIT  HI_BIT_0

typedef struct
{
    HI_BOOL_E   em_enable;            /* GPON����ʹ�� */
    hi_dp_tag_s st_default_tag;       /* ȱʡtag */
    HI_BOOL_E   em_eth_sync_en;
    HI_BOOL_E   em_igr_detect_en;     /* ��˿��������ʹ�� */
    HI_BOOL_E   em_egr_detect_en;     /* ���˿��������ʹ�� */
} hi_dp_gpon_attr_s;

/*gponģ��澯���� */
typedef struct
{
    /* �澯����*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_GPON_ALARM_NUM_E];

    /* ģ��alam �澯�ص�����*/
    void (*alarm_call_back)(HI_DP_GPON_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_gpon_alarm_attr_s;

/* 5115�޸�: ͳ�����޸� */
typedef struct
{
    uint      ui_index;  /* 0~127,оƬ��֧��128��GEMPORT��ͳ�� */
    uint      ui_gemport;/* ָ�����ҵ�gemport_id      */
    HI_BOOL_E em_valid;

    ulong ul_up_frame;   /* ָ��gemport��������̫֡�� */
    ulong ul_up_gem;     /* ָ��gemport������GEM֡��  */
    ulong ul_up_byte;    /* ָ��gemport�������ֽ���   */

    ulong ul_dn_frame;   /* ָ��gemport��������̫֡�� */
    ulong ul_dn_gem;     /* ָ��gemport������GEM֡��  */
    ulong ul_dn_byte;    /* ָ��gemport�������ֽ���   */
} hi_dp_gpon_gemport_cnt_table_item_s;

typedef struct
{
    /* �澯 */
    uint ui_alarm_times[HI_DP_GPON_ALARM_NUM_E];    /* ���澯�������� */
    uint ui_alarm_call_back_times;                  /* �澯�ص����� */
    uint ui_alarm_discard_times;

    ulong ul_tx_ploam;         /* PLOAMU��Ϣ���ʹ���ͳ�Ƽ�������*/
    ulong ul_tx_ploam_nvld;    /* NO_MESSAGE��Ϣ��PLOAMu���ʹ�����������*/
    ulong ul_tx_dbru_m0;           /* DBRUģʽ0�ķ����ܴ���ͳ�Ƽ�������*/
    ulong ul_tx_dbru_m1;           /* DBRUģʽ1�ķ����ܴ���ͳ�Ƽ�������*/
    ulong ul_tx_dbru_spec;     /* ָ��TCONT DBRU�ķ����ܴ���ͳ�Ƽ�������*/
    ulong ul_tx_omci;          /* OMCI�����ܰ�����������*/
    ulong ul_tx_gem;              /* ����GEM����ͳ�Ƹ�����������*/
    ulong ul_tx_gem_cut;           /* ����GEM��Ƭͳ�Ƽ�������*/
    ulong ul_tx_idle;          /* ���з���IDLE֡���ֽ�����������*/
    ulong ul_tx_burst;         /* ����burst���͸���ͳ�Ƽ�������*/
    ulong ul_tx_eth_byte;       /* PON��������̫���ֽ�������ͳ�Ƽ�������*/
    ulong ul_tx_uc;            /* PON��������̫�����ܸ���ͳ�Ƽ�������*/
    ulong ul_tx_mc;            /* PON��������̫�鲥�ܸ���ͳ�Ƽ�������*/
    ulong ul_tx_bc;           /* PON��������̫�㲥�ܸ���ͳ�Ƽ�������*/
    ulong ul_tx_eth;           /* PON��������̫�ܸ���ͳ�Ƽ�������*/
    ulong ul_bip_err;          /* BIP����ͳ�Ƽ�������*/
    ulong ul_range_req;       /* RANGE�����������*/
    ulong ul_sn_req;              /* SN�����������*/
    ulong ul_ploam_right;      /* ��ȷ��ploam��Ϣͳ�ơ�*/
    ulong ul_ploam_wrong;      /* CRC�����ploam��Ϣͳ�ơ�*/
    ulong ul_ploam;           /* ���д�ploam�����ڶ�����CPU��ploam��Ϣͳ�ƣ�����CRC�����ploam��Ϣ��*/
    ulong ul_ploam_full_drop;   /* ��������͹���״̬���յ��ı���ploam��Ϣ��Ŀͳ�ơ�*/
    ulong ul_rei;                 /* ͳ�Ƽ���ڵ�BIP����ͳ�ơ�*/
    ulong ul_bwmap_crc_err;    /* ����BWMAP CRCУ�����ͳ�Ƽ�������*/
    ulong ul_fec_us_switch;    /* ����FEC�л�ͳ�Ƽ�������*/
    ulong ul_fec_decode_cw;    /* FEC���ֽ����������*/
    ulong ul_fec_err_cw;          /* FEC���������������*/
    ulong ul_fec_cor_cw;          /* FEC���־����������*/
    ulong ul_fec_uncor_cw;     /* FEC����δ�����������*/
    ulong ul_fec_cor_byte;    /* FEC����BYTE����������*/
    ulong ul_fec_seconds;     /* FEC Seconds�Ǵ���ͳ�ƣ�ÿ1���Ӳ�ѯһ�Σ��������Uncorrectable code words����ͳ��ֵ����1��*/
    ulong ul_fec_ds_switch;   /* ����FEC�л�ͳ�Ƽ�������*/
    ulong ul_rx_pon_byte;     /* ����PON���ֽ�����ͳ�Ƽ�������*/
    ulong ul_rx_gem;           /* ���ձ���GEM����ͳ�Ƽ�������*/
    ulong ul_gem_eth;        /* ���з��鲥ETH GEM������������ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_gem_mc;             /* �����鲥����GEM������������ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_gem_omci;       /* OMCI GEMͳ�Ƽ�������*/
    ulong ul_gem_prbs;       /* PRBS GEMͳ�ƼĴ�����*/
    ulong ul_aes_omci;       /* ������Ҫ���ܵ�OMCI GEM��������*/
    ulong ul_aes_prbs;       /* ������Ҫ���ܵ�PRBS GEM��������*/
    ulong ul_aes_mc;            /* ������Ҫ���ܵ��鲥GEM��������ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_aes_eth;       /* ������Ҫ���ܵķ��鲥ETH GEM��������ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_aes_blk;       /* �������ݿ�ͳ�Ƽ�������*/
    ulong ul_key_switch;     /* ������Կ�л��ɹ�����ͳ�Ƽ�������*/
    ulong ul_rec_eth;        /* ����������ķ��鲥ETH GEM֡��������PTIΪ0����ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_rec_mc;            /* ������������鲥����GEM֡��������PTIΪ0����ע���鲥�ͷ��鲥�жϻ�׼ΪPORTID�������͡�*/
    ulong ul_rec_omci;      /*OMCI���飨PTI=0��GEMͳ�Ƽ�������*/
    ulong ul_rec_prbs;      /*PRBS ���飨PTI=0��GEMͳ�ƼĴ�����*/
    ulong ul_short_err;     /*��̫���̹���ͳ�Ƽ�������*/
    ulong ul_long_err;      /*��̫��������ͳ�Ƽ�������*/
    ulong ul_rx_mc;        /*�����鲥���ļ��������鲥ͨ��DA��һ���ֽڵ����bitʶ�𣩡�*/
    ulong ul_rx_uc;        /*���е������ļ�����������ͨ��DA��һ���ֽڵ����bitʶ�𣩡�*/
    ulong ul_rx_bc;        /*���й㲥���ļ�����������ͨ��DA��һ���ֽ�ʶ�𣩡�*/
    ulong ul_rx_eth;           /*������̫����ͳ�Ƽ�������*/
    ulong ul_fcs_err;       /*FCSУ�����ͳ�Ƽ�������*/
    ulong ul_gem_lof;       /*GEM��״̬��ʧ��ͳ�Ƽ�������*/
    ulong ul_gem_drop;      /*����GEM����ͳ�Ƽ����������д����������㣬ԭ��GEM֡PLI���������С����1�ֽڡ���*/
    ulong ul_hec_cor;       /*HEC 1bit����ͳ�Ƽ�������*/
    ulong ul_rx_omci;       /*OMCI���հ���ͳ�Ƽ�������*/
    ulong ul_omci_crc_err;   /*OMCI����������CRCУ�����ͳ�Ƽ�������*/
    ulong ul_omci_full_drop; /*OMCI��������������ͳ�Ƽ�������*/
#if defined(CONFIG_HISI_SD5115_TYPE_T)
    ulong ul_tx_less_64;      /* 5115T add: ����ETH���ĳ���С��64B�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_64;           /* 5115T add: ����ETH���ĳ��ȵ���64B�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_65to127;      /* 5115T add: ����ETH���ĳ���[65B,127B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_128to255;     /* 5115T add: ����ETH���ĳ���[128B,255B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_256to511;     /* 5115T add: ����ETH���ĳ���[256B,511B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_512to1023;    /* 5115T add: ����ETH���ĳ���[512B,1023B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_1024to1518;   /* 5115T add: ����ETH���ĳ���[1024B,1518B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_1519tomtu;    /* 5115T add: ����ETH���ĳ���[1519B,MTU]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_tx_more_mtu;     /* 5115T add: ����ETH���ĳ��ȴ���MTU�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_bwmap_right;     /* 5115T add: ����BWMAP�Ϸ���ͳ�Ƽ�������*/
    ulong ul_bwmap_wrong;     /* 5115T add: ����BWMAP�Ƿ���ͳ�Ƽ�������*/
    ulong ul_ploam_bc;        /* 5115T add: �㲥ploam��Ϣ��������*/
    ulong ul_ploam_uc;        /* 5115T add: ����ploam��Ϣ��������*/
    ulong ul_rx_less_64;      /* 5115T add: ����ETH���ĳ���С��64B�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_64;           /* 5115T add: ����ETH���ĳ��ȵ���64B�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_65to127;      /* 5115T add: ����ETH���ĳ���[65B,127B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_128to255;     /* 5115T add: ����ETH���ĳ���[128B,255B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_256to511;     /* 5115T add: ����ETH���ĳ���[256B,511B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_512to1023;    /* 5115T add: ����ETH���ĳ���[512B,1023B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_1024to1518;   /* 5115T add: ����ETH���ĳ���[1024B,1518B]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_1519tomtu;    /* 5115T add: ����ETH���ĳ���[1519B,MTU]�ı��ĸ���ͳ�Ƽ�������*/
    ulong ul_rx_more_mtu;     /* 5115T add: ����ETH���ĳ��ȴ���MTU�ı��ĸ���ͳ�Ƽ�������*/
#endif
} hi_dp_gpon_cnt_s;

typedef struct
{
    uint ui_spcnt_curr;   /* ��ǰ֡�ĳ�֡������ֵ */
} hi_dp_gpon_spcnt_s;

typedef struct
{
    HI_DP_ALARM_STATE_E em_los;             /* ��ǰLOS״̬ */
    HI_DP_ALARM_STATE_E em_lof;             /* ��ǰLOF״̬ */
    HI_BOOL_E em_igr_empty;                 /* ��˿�����״̬ */
    HI_BOOL_E em_egr_empty;                 /* ���˿�����״̬ */    
} hi_dp_gpon_sta_s;


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_GPON_H__ */
