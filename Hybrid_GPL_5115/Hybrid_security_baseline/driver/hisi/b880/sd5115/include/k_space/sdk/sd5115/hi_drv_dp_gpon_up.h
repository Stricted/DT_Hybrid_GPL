/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_gpon_up.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : GPON UP ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��24��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

  2.��    ��   : 2011��9��16��
    ��    ��   : f00125610
    �޸�����   : �޸��ļ�
***************************************************************************** */

#ifndef __HI_DRV_DP_GPON_UP_H__
#define __HI_DRV_DP_GPON_UP_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_DP_GPON_UP_GEMPORT_CNT_NUM 128
#define HI_GPON_ALLOCID_INVALID 0xffff
#define HI_GPON_EXCLUSIVE_TCONT 0x80

/* ��ģ�鷢�����ģʽ */
typedef enum
{
    HI_DP_GPON_UP_TX_AUTO_E = 0,   /* �߼��Զ����� */
    HI_DP_GPON_UP_TX_ENABLE_E,     /* ������ */
    HI_DP_GPON_UP_TX_DISABLE_E,     /* ������ */
} HI_DP_GPON_UP_TX_MODE_E;

/* Dying Gasp ���Ϳ��ƣ������Ӳ�� */
typedef enum
{
    HI_DP_SOFT_E = 0,
    HI_DP_HARDWARE_E,
} HI_DP_DYINGGASP_SEND_MODE_E;

typedef enum
{
    HI_DP_GPON_UP_DBRU_32B_E = 0,   /* 32�ֽ� */
    HI_DP_GPON_UP_DBRU_48B_E,       /* 48�ֽ� */
    HI_DP_GPON_UP_DBRU_64B_E,       /* 64�ֽ� */
    HI_DP_GPON_UP_DBRU_96B_E,       /* 96�ֽ� */
    HI_DP_GPON_UP_DBRU_128B_E,      /* 128�ֽ� */
    HI_DP_GPON_UP_DBRU_192B_E,      /* 192�ֽ� */
    HI_DP_GPON_UP_DBRU_256B_E,      /* 256�ֽ� */
    HI_DP_GPON_UP_DBRU_384B_E,      /* 384�ֽ� */
    HI_DP_GPON_UP_DBRU_UINT_NUM_E,
} HI_DP_GPON_UP_DBRU_UNIT_E;

typedef struct
{
    uint ui_preamble_type1_bits;        /* TYPE1����ǰ���볤�� */
    uint ui_preamble_type2_bits;        /* TYPE2����ǰ���볤�� */
    uint ui_preamble_type3_bytes;       /* TYPE3����ǰ���볤�� */
    uint ui_preamble_type3_pattern;     /* TYPE3����ǰ������ʽ */
    uint ui_delimiter;                  /* �������ʽ */
} hi_dp_gpon_up_plou_s;

typedef struct
{
    uint ui_pre_eqd;       /* ��ʼ����ʱ */
    uint ui_eqd;           /* ONU�������õĵ�Ч��ʱ�ֽ����� */
    uint ui_eqd_dec;
    uint ui_eqd_inc;
    HI_BOOL_E em_eqd_precision;
} hi_dp_gpon_up_eqd_s;

/* OMCI ������� */
typedef struct
{
    HI_BOOL_E em_cut_en;           /* OMCI��Ƭʹ�� */
    uint      ui_tcont;            /* OMCI���͵�TCONT���� */
    uint      ui_gemport;          /* CPU����OMCI��Ԫ��Port ID */
} hi_dp_gpon_up_omci_attr_s;

/* ����������� */
typedef struct
{
    HI_LEVEL_E              em_enable_level; /* ����������ʹ�ܵ�ƽ */
    HI_LEVEL_E              em_sd_level;/* ��ģ�鷢���ƽ */
    HI_DP_GPON_UP_TX_MODE_E em_tx_mode; /* ������� */
    uint ui_tx_vld_ahead;               /* ��ģ�鷢��ʹ���ź���ǰʱ������ã���λbyte */

    uint ui_laser_on_delay;             /* ��PON MACָʾ��ģ��رշ��⵽��ģ��رշ���״̬���ӳ� */
    uint ui_laser_off_delay;            /* ��PON MACָʾ��ģ�鷢�⵽��ģ���������״̬���ӳ� */
} hi_dp_gpon_up_laser_attr_s;

/* gpon up ������������ */
typedef struct
{
    hi_dp_gpon_up_plou_s      st_plou;       /* Preamble/Delimiter */
    uint                      ui_onu_id;     /* ONU-ID */
    HI_DP_GPON_UP_DBRU_UNIT_E em_dbru_unit;  /* DBRU�ϱ����� */

    /* ��ʱ�������� */
    uint                ui_fixed_delay;  /* ONU����̶���ʱ */
    uint                ui_random_delay; /* �����ʱ���üĴ����������ʱ����ֵ��������� */
    uint                ui_tx_delay;     /* ���з���������Ч�źŵ���ʱ������ */
    hi_dp_gpon_up_eqd_s st_eqd;

    HI_BOOL_E em_dying_gasp_tx_en;   /* �߼��Ƿ���Dyinggasp */
    hi_dp_gpon_up_laser_attr_s st_tx_attr;  /* ���������⣬���Ͳ���������� */
    hi_dp_gpon_up_omci_attr_s  st_omci;
} hi_dp_gpon_up_attr_s;

/* ����GEMPORT ӳ������ṹ */
typedef struct
{
    uint ui_sfid;             /* ������ID, ���� */
    uint ui_gemport;          /* ӳ��GEMPORT ID */
    uint ui_tcont;            /* ӳ��TCONT ID   */
} hi_dp_gpon_up_map_table_item_s;

/* ����TCOUNT ����ṹ */
typedef struct
{
    uint ui_tcont;               /* TCOUNT  ID 0~8, 0-7��ʾ��ͨtcont��8ֻ�ܳ���OMCI */
    uint      ui_alloc_id;       /* allocid */
    HI_BOOL_E em_valid;
} hi_dp_gpon_up_tcont_table_item_s;

typedef struct
{
    uint ui_ploam_tx;       /* ��������PLOAM֡���� */
    uint ui_omci_tx;        /* ��������OMCI֡���� */
} hi_dp_gpon_up_cnt_s;


typedef struct 
{
}hi_dp_gpon_up_sta_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_GPON_UP_H__ */
