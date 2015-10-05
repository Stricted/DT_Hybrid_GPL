/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_kernel_drv_dp_epon.h
  Version       : Initial Draft
  Author        : z53794
  Created       : 2011/5/27
  Last Modified :
  Description   : hi_kernel_drv_dp_epon.c header file
  Function List :
  History       :
  1.Date        : 2011/5/27
    Author      : z53794
    Modification: Created file

  2.Date        : 2011/9/15
    Author      : f00125610
    Modification: Modified file

******************************************************************************/
#ifndef __HI_DRV_DP_EPON_H__
#define __HI_DRV_DP_EPON_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

/*----------------------------------------------*
* external variables                           *
*----------------------------------------------*/

/*----------------------------------------------*
* external routine prototypes                  *
*----------------------------------------------*/

/*----------------------------------------------*
* internal routine prototypes                  *
*----------------------------------------------*/

/*----------------------------------------------*
* project-wide global variables                *
*----------------------------------------------*/

/*----------------------------------------------*
* module-wide global variables                 *
*----------------------------------------------*/

/*----------------------------------------------*
* constants                                    *
*----------------------------------------------*/

/*----------------------------------------------*
* macros                                       *
*----------------------------------------------*/

/*----------------------------------------------*
* routines' implementations                    *
*----------------------------------------------*/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* ���ļ������� */
typedef enum
{
    HI_DP_EPON_TRIPLE_CHURNING_E = 0,  /* ���ؽ��� */
    HI_DP_EPON_AES_E,                  /* AES */
} HI_DP_EPON_DECIPHER_TYPE_E;

/* EPON �������� */
typedef struct
{
    HI_BOOL_E                  em_enable;       /* ʹ�ܿ��أ�Ĭ�ϴ� */
    hi_dp_tag_s                st_default_tag;  /* ȱʡTAG */
    HI_DP_EPON_DECIPHER_TYPE_E em_decipher_type;/* �������� */
    HI_BOOL_E em_single_llid_en;                   /* ��llidģʽ */
    HI_BOOL_E em_eth_sync_en;                   /* ͬ����̫ʹ�� */
} hi_dp_epon_attr_s;

typedef enum
{
    HI_DP_EPON_ROGUE_ONU_ALARM_E = 0,       /* ��åONU */
    HI_DP_EPON_SILENT_ONU_ALARM_E,          /* ������ONU */
    HI_DP_EPON_LOS_ALARM_E,                 /* ��ģ��RX ����LOS ״̬�澯 */
    HI_DP_EPON_SD_ALARM_E,                  /* ��ģ��RX �˳�LOS ״̬�澯 */
    HI_DP_EPON_DYINGGASP_ALARM_E,
    HI_DP_EPON_GATE_TIMEOUT_ALARM_E,        /* LLID0-7,ͨ���������� */
    HI_DP_EPON_ALARM_NUM_E,
} HI_DP_EPON_ALARM_E;

typedef struct
{
    /* �澯���� */
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_EPON_ALARM_NUM_E];

    /* ģ��alam �澯�ص����� */
    void (*alarm_call_back)(HI_DP_EPON_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_epon_alarm_attr_s;

typedef struct
{
    /* �澯 */
    uint  ui_alarm_times[HI_DP_EPON_ALARM_NUM_E];   /* ���澯�������� */
    uint  ui_alarm_call_back_times;                 /* �澯�ص����� */
    uint  ui_alarm_discard_times;
    ulong ul_mpcp_rx_fifo_over;        /* MPCP����FIFO�����֡�������� */
    ulong ul_data_rx_fifo_over;        /* ����֡����FIFO�����֡�������� */
    ulong ul_oam_rx_buffer_over;       /* OAM֡���ջ������ͳ�� */
    ulong ul_mac_rx_err_grant;         /* MAC���մ���Grant�������� */
    ulong ul_mac_rx_b2b_grant;         /* MAC���ձ�����Grant�������� */
    ulong ul_data_tx_fifo_und;         /* ����֡����FIFOǷ�ش�֡�������� */

    ulong ul_mac_fcs_err_less_64;      /* MAC����FCSУ���֡��֡��<64bytes�������� ��*/
    ulong ul_mac_fcs_err_64_mtu;       /* MAC����FCSУ���֡��64bytes��֡����MTU�������� ��*/
    ulong ul_mac_fcs_err_more_mtu;     /* MAC����FCSУ���֡��֡��>MTU�������� ��*/
    ulong ul_mac_fcs_err_byte;         /* MAC����FCSУ����ֽڼ����� ��*/
    ulong ul_mac_rx_all;               /* MAC��������FCS��ȷ����֡������ ��*/
    ulong ul_mac_rx_byte;              /* MAC��������FCS��ȷ�����ֽڼ����� ��*/
    ulong ul_mac_rx_all_ctrl;          /* MAC����FCS��ȷ����MAC����֡������ ��*/
    ulong ul_mac_rx_err_ctrl;          /* MAC���մ���MAC����֡��������FCS���󣩼����� ��*/
    ulong ul_mac_rx_saoff_oam;         /* MAC���յ�SA(OFF)OAM֡������ ��*/
    ulong ul_mac_tx_all;               /* MAC������������֡������ ��*/
    ulong ul_mac_tx_byte;              /* MAC�������������ֽڼ����� ��*/
    ulong ul_mac_tx_mpcp;              /* MAC��������MPCP֡������ ��*/

    ulong ul_scb_rx_byte;              /* ����SCB���������ֽڼ����� ��*/
    ulong ul_scb_rx_uc;                /* ����SCB����DA����֡������ ��*/
    ulong ul_scb_rx_mc;                /* ����SCB����DA�ಥ֡������ ��*/
    ulong ul_scb_rx_bc;                /* ����SCB����DA�㲥֡������ ��*/
    ulong ul_mac_rx_dis_gate;          /* ���н���Discovery Gate֡������ ��*/
    ulong ul_scb_rx_less_64;           /* ����SCB��������֡��OAM֡��������֡��<64bytes�� ��*/
    ulong ul_scb_rx_64;                /* ����SCB��������֡��OAM֡��������֡��=64bytes�� ��*/
    ulong ul_scb_rx_65to127;           /* ����SCB��������֡��OAM֡��������65bytes��֡����127bytes�� ��*/
    ulong ul_scb_rx_128to255;          /* ����SCB��������֡��OAM֡��������128bytes��֡����255bytes�� ��*/
    ulong ul_scb_rx_256to511;          /* ����SCB��������֡��OAM֡��������256bytes��֡����511bytes�� ��*/
    ulong ul_scb_rx_512to1023;         /* ����SCB��������֡��OAM֡��������512bytes��֡����1023bytes�� ��*/
    ulong ul_scb_rx_1024to1518;        /* ����SCB��������֡��OAM֡��������1024bytes��֡����1518bytes�� ��*/
    ulong ul_scb_rx_1519to2000;        /* ����SCB��������֡��OAM֡��������1519bytes��֡����2000bytes�� ��*/
    ulong ul_scb_rx_more_2000;         /* ����SCB��������֡��OAM֡��������֡��>2000bytes�� ��*/
    ulong ul_scb_rx_more_mtu;          /* ����SCB��������֡��OAM֡��������֡��>MTU�� ��*/
    ulong ul_scb_rx_oam;               /* ����SCB����OAM֡������ ��*/

    ulong ul_fec_tx_total_code_words;
    ulong ul_fec_rx_corrected_bytes;
    ulong ul_fec_rx_corrected_code_words;
    ulong ul_fec_rx_uncorrectable_code_words;
    ulong ul_fec_rx_total_code_words;
    ulong ul_fec_rx_seconds;

} hi_dp_epon_cnt_s;

typedef struct
{
    uint ui_llid;
    uint ui_sec;
} hi_dp_epon_llid_bandwidth_s;

typedef struct
{
    ulong ul_byte;          /* �����ֽڼ��� */
    ulong ul_uc;            /* ���͵���֡���� */
    ulong ul_mc;            /* �����鲥֡���� */
    ulong ul_bc;            /* �����鲥֡���� */
    ulong ul_report;        /* ���͵�report֡���� */
    ulong ul_less_64;       /* ����С��64�ֽ�֡���� */
    ulong ul_64;            /* ���͵���64�ֽ�֡���� */
    ulong ul_65to127;       /* ����65��127�ֽ�֡���� */
    ulong ul_128to255;      /* ����128��255�ֽ�֡���� */
    ulong ul_256to511;      /* ����256��511�ֽ�֡���� */
    ulong ul_512to1023;     /* ����512��1023�ֽ�֡���� */
    ulong ul_1024to1518;    /* ����1024��1518�ֽ�֡���� */
    ulong ul_1519to2000;    /* ����1519��2000�ֽ�֡���� */
    ulong ul_more_2000;     /* ���ʹ���2000�ֽ�֡���� */
    ulong ul_more_mtu;      /* ���ʹ���MTU֡���� */
    ulong ul_err_data;      /* ���ʹ�������֡���� */
    ulong ul_oam;           /* ����OAM֡���� */
    ulong ul_data_frag;     /* �������ݷ�Ƭ���� */
    ulong ul_oam_frag;      /* ����OAM��Ƭ���� */
} hi_dp_epon_llid_tx_cnt_s;

typedef struct
{
    ulong ul_grant;         /* ������ȷ��Grant���� */
    ulong ul_frpt_grant;    /* ������ȷ��Force Report Grant���� */
    ulong ul_grant_len_sum; /* ����Grant�����ۼ��ܺͼ��� */
    ulong ul_byte;          /* �����ֽڼ��� */
    ulong ul_uc;            /* ���յ���֡���� */
    ulong ul_mc;            /* �����鲥֡���� */
    ulong ul_bc;            /* �����鲥֡���� */
    ulong ul_gate;          /* ���յ�Gate֡���� */
    ulong ul_less_64;       /* ����С��64�ֽ�֡���� */
    ulong ul_64;            /* ���յ���64�ֽ�֡���� */
    ulong ul_65to127;       /* ����65��127�ֽ�֡���� */
    ulong ul_128to255;      /* ����128��255�ֽ�֡���� */
    ulong ul_256to511;      /* ����256��511�ֽ�֡���� */
    ulong ul_512to1023;     /* ����512��1023�ֽ�֡���� */
    ulong ul_1024to1518;    /* ����1024��1518�ֽ�֡���� */
    ulong ul_1519to2000;    /* ����1519��2000�ֽ�֡���� */
    ulong ul_more_2000;     /* ���մ���2000�ֽ�֡���� */
    ulong ul_more_mtu;      /* ���մ���MTU֡���� */
    ulong ul_oam;           /* ����OAM֡���� */
    ulong ul_otdr_gate;     /* ����OTDR GATE֡���� */
    ulong ul_cipher;        /* ����CIPHER֡���� */
} hi_dp_epon_llid_rx_cnt_s;

typedef struct
{
    uint                     ui_llid;
    hi_dp_epon_llid_rx_cnt_s st_rx;
    hi_dp_epon_llid_tx_cnt_s st_tx;
} hi_dp_epon_llid_cnt_s;
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_EPON_H__ */
