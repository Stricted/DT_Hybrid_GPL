/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_uni.h
  �� �� ��   : ����
  ��    ��   : g00174254
  ��������   : 2011��9��21��
  ����޸�   :
  ��������   : UNI PORT ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��9��21��
    ��    ��   : g174254
    �޸�����   : �����ļ�

***************************************************************************** */

#ifndef __HI_DRV_DP_UNI_PORT_H__
#define __HI_DRV_DP_UNI_PORT_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


/* ******************************* �궨�� *********************************** */

#define HI_DP_ETH_CASCADE_PORT_NUM      4

/* ******************************* ö������ ********************************* */
/* ETH�˿�LED����˸����ģʽ */
typedef enum
{
    HI_DP_ETH_LED_ON_E = 0,    /* LED�Ƴ��� */
    HI_DP_ETH_LED_OFF_E,       /* LED�Ƴ��� */
    HI_DP_ETH_LED_BLINK_E,     /* LED����һ��Ƶ����˸ */

}HI_DP_ETH_LED_WORK_MODE_E;

typedef enum
{
    HI_DP_ETH_LED_YELLOW_E = 0,    /* ��LED�� */
    HI_DP_ETH_LED_GREEN_E,         /* ��LED�� */
    HI_DP_ETH_LED_NUM,             /* LED����Ŀ */

}HI_DP_ETH_LED_TYPE_E;

typedef enum
{
    HI_DP_ETH_LOCAL_E = 0,       /* ���ض˿� */
    HI_DP_ETH_REMOTE_E,          /* �Զ˶˿� */
    HI_DP_ETH_PORT_TYPE_NUM,    /* �˿�������Ŀ */

}HI_DP_ETH_PORT_TYPE_E;

typedef enum
{
    HI_DP_ETH_RX_E = 0,       /* ���շ��� */
    HI_DP_ETH_TX_E,           /* ���ͷ��� */
    HI_DP_ETH_DIRECTION_TYPE_NUM,    /* ����������Ŀ */

}HI_DP_ETH_DIRECTION_TYPE_E;

/* ************************* �ӽṹ, �����ṹ�б����� *********************** */


/* ��̫���˿���ЧEEE������� */
typedef struct
{
    HI_BOOL_E   em_enable;

    uint    ui_wait_time;                                                      /* �ȴ�ʱ��, ��λ8ns, Ĭ��100ms (0x00BEBC20) */
    uint    ui_sleep_time;                                                     /* ˯��ʱ��, ��λ8ns, Ĭ��20ms  (0x002625A0) */
    uint    ui_wakeup_time;                                                    /* ����ʱ��, ��λ8ns, 1000Mģʽ��, Ĭ��16.5us(0x80F)
                                                                                    100Mģʽ��, Ĭ�� 30 us(0xEA6)*/
    hi_dp_eth_eee_capability_u    u_phy_eee_cap[HI_DP_ETH_PORT_TYPE_NUM];      /* ���غͶԶ˶˿ڵ�EEE������ֻ�� */
    hi_dp_eth_eee_capability_u    u_local_eee_cap_advertise;                  /* ���ض˿�EEE�������� */
    HI_BOOL_E                   em_eee_sta[HI_DP_ETH_DIRECTION_TYPE_NUM];
}hi_dp_eth_eee_attr_s;

/* ��̫�˿�802.1X ������� */
typedef struct
{
    HI_BOOL_E   em_8021x_auth_en;       /* ��Ȩ״̬���˿ڴ򿪣���δ��Ȩ״̬���˿ڹرա� */
}hi_dp_eth_8021x_attr_s;

/* ��̫�˿�stp ������� */
typedef struct
{
    HI_DP_ETH_STP_STATUS_E  em_stp_status;      /* �˿�������״̬ */
}hi_dp_eth_stp_attr_s;

/* ETH�˿�LED����˸���� */
typedef struct
{
    HI_DP_ETH_LED_WORK_MODE_E   em_work_mode;    
    uint                        ui_freq;   /* ֻ��HI_DP_ETH_LED_BLINK_Eģʽ����Ч����λΪ100ms(�ݶ�) */
}hi_dp_eth_led_attr_s;

#if 0
/* PRBS�˿���̫���� */
typedef struct
{
    
}hi_dp_prbs_eth_attr_s;


/* PRBS����(����)���� */
typedef struct
{
    HI_BOOL_E           em_send_en;
    HI_PRBS_SEND_MODE   em_send_mode;
    uint                ui_send_len;
    uint                ui_send_ipg;
    
    uint                ui_seed;        // ��������
    
}hi_dp_prbs_send_attr_s;

/* PRBS���(����)���� */
typedef struct
{
    HI_BOOL_E   em_rcv_en;
    uint        ui_sync_period;
    
}hi_dp_prbs_rcv_attr_s;
#endif


/* ************ ���ṹ: ֱ�Ӷ������ֵĽṹ����Ϊ�����Ĳ������� ************** */


/* CPU�˿����� */
typedef struct
{
    hi_dp_tag_attr_s    st_cpu_tag_attr;    /* CPU�˿�TAG���� */

    /* TPID���޸�ͨ��VLAN sftʵ�� */
    //uint               ui_tpid_modify;          /* TPID�޸����� */

}hi_dp_uni_cpu_port_attr_s;

/* CPU�˿�ͳ�� */
typedef struct
{
    
}hi_dp_uni_cpu_port_cnt_s;

/* GE�˿����� */
typedef struct
{
    uint                              ui_index;
    HI_BOOL_E                         em_enable;

    HI_DP_ETH_MII_MODE_E              em_mii_mode;                                 /* MII/GMII/RMII/SEDERS�� */
//#if (defined (CONFIG_HISI_SD5115_AF_ASIC) && defined (CONFIG_HISI_SD5115_TYPE_H))
    HI_DP_RGMII_LINE_SEQ_E            em_line_seq;                                 /* RGMII1������ */
//#endif
    HI_DP_ETH_WORK_MODE_E             em_work_mode;                                /* ����ģʽ */
    hi_dp_eth_work_capability_u       u_work_mode_cap;                             /* ����ģʽ���� */
    hi_dp_eth_auto_neg_capability_u   u_auto_neg_cap;                              /* ��Э�̵����� */
    hi_dp_eth_phy_capability_u        u_phy_cap;
    HI_DP_ETH_LOOP_MODE_E             em_loop_mode;                                /* ����ģʽ */
    hi_dp_eth_led_attr_s              st_led_attr[HI_DP_ETH_LED_NUM];

    hi_dp_tag_attr_s                  st_tag_attr;                                 /* TAG������� */
    
    hi_dp_eth_eee_attr_s              st_eee_attr;                                 /* EEE��Ч���� */
    hi_dp_eth_fctl_attr_s             st_fctl_attr;                                 /* �������� */
    hi_dp_eth_8021x_attr_s            st_8021x_attr;                                /* 802.1X  ������� */
    hi_dp_eth_stp_attr_s              st_stp_attr;                                  /* �˿�������������� */
    
}hi_dp_uni_eth_port_attr_s;


/* GE�˿�ͳ�� */
typedef struct
{
    uint     ui_index;

    /* ALL_MAC_reg */
    ulong    ul_rx;
    ulong    ul_rx_uc;
    ulong    ul_rx_mc;
    ulong    ul_rx_bc;
    ulong    ul_rx_pause;
    ulong    ul_rx_unk_ctrl;
    ulong    ul_rx_under_min;
    ulong    ul_rx_min_63;
    ulong    ul_rx_64;
    ulong    ul_rx_65_127;
    ulong    ul_rx_128_255;
    ulong    ul_rx_256_511;
    ulong    ul_rx_512_1023;
    ulong    ul_rx_1024_1518;
    ulong    ul_rx_1519_max;
    ulong    ul_rx_over_max;
    ulong    ul_rx_undersize;
    ulong    ul_rx_fragement;
    ulong    ul_rx_over_size;
    ulong    ul_rx_jabber;
    ulong    ul_rx_fcs_err;
    ulong    ul_rx_align_err;
    ulong    ul_rx_fifo_overrun;
    ulong    ul_ch_tag_err;
    ulong    ul_rx_byte_ok;
    ulong    ul_rx_byte_err;
    
    ulong    ul_tx;
    ulong    ul_tx_uc;
    ulong    ul_tx_mc;
    ulong    ul_tx_bc;
    ulong    ul_tx_pause;
    ulong    ul_tx_min_63;
    ulong    ul_tx_64;
    ulong    ul_tx_65_127;
    ulong    ul_tx_128_255;
    ulong    ul_tx_256_511;
    ulong    ul_tx_512_1023;
    ulong    ul_tx_1024_1518;
    ulong    ul_tx_1519_max;
    ulong    ul_tx_over_max;
    ulong    ul_tx_crc_err;
    ulong    ul_tx_single_col;
    ulong    ul_tx_multi_col;
    ulong    ul_tx_excess_col;
    ulong    ul_tx_late_col;
    ulong    ul_tx_deferral_trans;
    ulong    ul_tx_crs_lost;
    ulong    ul_tx_abort;
    ulong    ul_tx_fifo_underrun;
    ulong    ul_tx_byte_ok;
    ulong    ul_tx_byte_err;
    
    /* ... */
    
}hi_dp_uni_eth_port_cnt_s;


typedef struct
{
    uint                        ui_index;
    HI_DP_ETH_LINK_STA_E        em_link_sta;
    HI_DP_ETH_WORK_MODE_E       em_work_mode;
    
}hi_dp_uni_eth_port_sta_s;

#if 0
/* PRBS */
typedef struct
{
    hi_dp_prbs_eth_attr_s       st_prbs_eth_attr;
    hi_dp_prbs_send_attr_s      st_prbs_tx_attr;
    hi_dp_prbs_send_attr_s      st_prbs_rx_attr;
    
}hi_dp_uni_prbs_port_attr_s;

typedef struct
{
    
}hi_dp_uni_prbs_port_cnt_s;

typedef struct
{
    
}hi_dp_uni_prbs_port_sta_s;
#endif

typedef struct
{
    /* оƬ�̶���GE0��Ϊ�����˿ڣ�ֻ��1�������˿� */
    
    HI_BOOL_E     em_enable;        /* ����ģʽ���� */
    uint          ui_tpid;          /* ����ģʽ��, ���ļ���tagʶ������ֵ */
    HI_BOOL_E     em_double_tag_en; /* ���㼶��TAGʹ�� */

    /* �����˿�tci��Ϣ��������������ӵ�pri&cfi&vid��, ���vid���˿ڵ�ӳ�� */
    hi_dp_tci_s   st_tag_map[HI_DP_ETH_CASCADE_PORT_NUM];

} hi_dp_uni_cascade_attr_s;

/* BEGIN: Added by w00180499, 2012/3/12 */
typedef struct
{
    unsigned int ui_phy_addr;
    unsigned int ui_reg_addr;
    unsigned short us_data;
} hi_inner_phy_reg_info_s;
/* END:   Added by w00180499, 2012/3/12 */

/* ************************************************************************** */

#endif /* */

