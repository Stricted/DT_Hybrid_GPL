#include <linux/netdevice.h>
#include <linux/workqueue.h>

/******************************************************************************

                  ��Ȩ���� (C), 2011-2021, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_kernel_eth0.h
  �� �� ��   : ����
  ��    ��   : y58968
  ��������   : D2011_12_15
  ����޸�   :

******************************************************************************/
#ifndef __HI_KERNEL_ETH0_H__
 #define __HI_KERNEL_ETH0_H__

 #define HI_KERNEL_NETWORK_SPEC_NUM 8

/*ħ����*/
 #define HI_KERNEL_NETWORK_MAGICAL 0x68776877
 #define HI_MAC_LEN 6


typedef struct __hi_drv_list_head
{
    struct __hi_list_head *next, *prev;
} hi_drv_list_head;

typedef enum
{
    HI_KERNEL_ETH0_VLANFLAG_UNTAG = 0,    /* untag vlan */
    HI_KERNEL_ETH0_VLANFLAG_STAG,         /* single vlan */
    HI_KERNEL_ETH0_VLANFLAG_CTAG,         /* double vlan */
} hi_kernel_eth0_vlanflag_e;

typedef struct
{
    unsigned int  ui_magic;     /*ui_magic = '5'<<24|'1'<<16|'1'<<8|'5' */ 
    unsigned char uc_groupid;   /*tx*/
    unsigned char uc_act;
    unsigned char uc_pri;
    unsigned char uc_port;        /*rx:ingress tx:engress*/

    unsigned char uc_reason;
    unsigned char uc_llid;        /*llid or tcontid*/
    unsigned char uc_resv;
    unsigned char uc_vlanflag;    /* 1: һ��HI_KERNEL_ETH0_VLANFLAG_STAG 2:����HI_KERNEL_ETH0_VLANFLAG_CTAG*/

    unsigned short us_svlan;
    unsigned short us_cvlan;

    unsigned short us_gemport;
    unsigned short us_len;
} hi_kernel_eth0_cb_s;

typedef int (*hi_kernel_eth0_dev_rxcallback)( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb );

typedef struct
{
    unsigned char  uc_dmask[HI_MAC_LEN];
    unsigned char  uc_smask[HI_MAC_LEN];
    unsigned int   ui_dipmask;
    unsigned int   ui_sipmask;
    unsigned short us_dpmask;
    unsigned short us_spmask;
    unsigned int   ui_flagmask;
} hi_kernel_network_mappingmask_s;

typedef struct
{
    void   *pv_head;
    void   *pv_data;
    uint    ui_hdrlen;
    uint    ui_ifindex;
} hi_kernel_network_data_s;

typedef struct
{
    hi_drv_list_head                st_listhead;
    unsigned int                    ui_pkttype;
    hi_kernel_network_mappingmask_s st_mask;
} hi_kernel_network_spec_s;

typedef struct
{
    /*����car��̬����*/
    uint ui_car_up_lasttime;      /*�ϴμ�������ʱ��*/
    uint ui_car_up_token;         /*��ǰ������,ÿ����һ�����ļ�һ,��ģ����ֵ�Ƚ�*/
    uint ui_car_stat_up_green;    /*ͳ�Ƽ���*/
    uint ui_car_stat_up_yellow;   /*ͳ�Ƽ���*/
    uint ui_car_stat_up_red;      /*ͳ�Ƽ���*/
    /*����car��̬����*/
    uint ui_car_dn_lasttime;      /*�ϴμ�������ʱ��*/
    uint ui_car_dn_token;         /*��ǰ������,ÿ����һ�����ļ�һ,��ģ����ֵ�Ƚ�*/
    uint ui_car_stat_dn_green;    /*ͳ�Ƽ���*/
    uint ui_car_stat_dn_yellow;   /*ͳ�Ƽ���*/
    uint ui_car_stat_dn_red;      /*ͳ�Ƽ���*/
    /*����pq��̬����*/
    uint ui_pq_up_lasttime;      /*�ϴμ�������ʱ��*/
    uint ui_pq_up_token_pri0;
    uint ui_pq_up_token_pri1;
    uint ui_pq_up_token_pri2;
    uint ui_pq_up_token_pri3;
    uint ui_pq_up_token_pri4;
    uint ui_pq_up_token_pri5;
    uint ui_pq_up_token_pri6;
    uint ui_pq_up_token_pri7;

    /*����pqͳ��*/
    uint ui_pq_up_stat_pri0;
    uint ui_pq_up_stat_pri1;
    uint ui_pq_up_stat_pri2;
    uint ui_pq_up_stat_pri3;
    uint ui_pq_up_stat_pri4;
    uint ui_pq_up_stat_pri5;
    uint ui_pq_up_stat_pri6;
    uint ui_pq_up_stat_pri7;

    /*����pq��̬����*/
    uint ui_pq_dn_lasttime;      /*�ϴμ�������ʱ��*/
    uint ui_pq_dn_token_pri0;
    uint ui_pq_dn_token_pri1;
    uint ui_pq_dn_token_pri2;
    uint ui_pq_dn_token_pri3;
    uint ui_pq_dn_token_pri4;
    uint ui_pq_dn_token_pri5;
    uint ui_pq_dn_token_pri6;
    uint ui_pq_dn_token_pri7;

    /*����pqͳ��*/
    uint ui_pq_dn_stat_pri0;
    uint ui_pq_dn_stat_pri1;
    uint ui_pq_dn_stat_pri2;
    uint ui_pq_dn_stat_pri3;
    uint ui_pq_dn_stat_pri4;
    uint ui_pq_dn_stat_pri5;
    uint ui_pq_dn_stat_pri6;
    uint ui_pq_dn_stat_pri7;

    /*misc*/
    uint ui_napt_stat;
} hi_drv_network_traffic_stat_s;

typedef struct
{
    /*��������*/
    unsigned char  ui_mode;        /*0:ϵͳ���ݵ���CPU����60%����������
                                   1:ϵͳ���ݾ������������е���*/
    unsigned char  uc_cpurate;     /*0..100 �ٷֱ�*/
    unsigned short us_resv;
    unsigned int   ui_token;      /*��Ҫϵͳ��ʵ�������״̬�µ�ϵͳ������ת������,��λ ��/ÿ��*/
    /*��̬����*/
    unsigned int ui_drop;
    unsigned int ui_peektoken;
    unsigned int ui_curtoken;
    unsigned int ui_lasttime;
} hi_drv_network_unattack_s;

typedef struct  __hi_kernel_network_vif_s
{
    struct net_device                *pst_dev;         /*pointer to the net device*/
    struct net_device_stats st_stats;    /*the stats of the device*/
    struct net_device_stats st_devstats; /*the stats of the real device.e.g:eth0*/
    struct napi_struct napi;
    struct work_struct reset_task;
    struct __hi_kernel_network_vif_s *pst_fathervif;
    hi_drv_list_head st_rootlisthead;  /*next level node dev of sub interface*/
    hi_drv_list_head st_listhead;       /*this node*/
    hi_drv_list_head st_mappinglisthead;   /* �¹��������õ���ӳ��*/
    uint             ui_magical; /*����Ƿ�Ϊ����ӿڴ����Ĺ���ʵ�����Ϊ */

    /*ӳ������*/
    uint ui_mappingtype;   /*��Ϊ����ڵ�/�ӿ���Ч,�����Ǳ��뱻����*/

    unsigned char uc_matchmode;      /*ƥ��ģʽ HI_KERNEL_VIF_TRAFFIC_UNMATCH_FILTER*/
    unsigned char uc_mac[HI_MAC_LEN];

    unsigned char uc_vlanflag;  /*vlan������*/
    unsigned char uc_priority;       /*�ӿڵ������ȼ�0��..7��,ȱʡ0*/
    unsigned char uc_broadcast; /*�Ƿ�㲥�������ӽӿ�*/
    unsigned char uc_resv; /*����*/

    uint   ui_vlanmode;  /*����Ϊ����ӿ�ʱ,ϵͳֻ�ܹ�ʹ��TRUNK-0*/
    ushort us_defvlan;   /*ȱʡ0Ϊû�м���*/
    ushort us_deftpid;   /*ȱʡ0x8100*/

    /*�ӿ����õĻ�������*/
    uint ui_pridns;
    uint ui_secdns;
    uint ui_wins;

    void *                          pv_hashhead_mapping; /*hash��ͷ�ڵ�,����ƥ���������*/
    void *                          pv_hashhead_spec; /*hash��ͷ�ڵ�,����ƥ���������*/
    hi_kernel_network_mappingmask_s st_mask;

    /*�ӿ����õĹ�������*/
    unsigned char uc_type;          /*0:����ڵ�/1:�ӿ�/2:�ӽӿ�*/
    unsigned char uc_status;        /*�ӿ�״̬0:�ر�1:����*/
    unsigned char uc_actionmode;        /*��������ת������ģʽ,ȱʡ0:
                                        0:��Linux���ת��
                                        1:�������ת�� �� Linuxת��
                                        2:Ӳ������ת�� �� Linuxת��
                                        3:Ӳ������ת�� ���������ת�� �� Linuxת��
                                        4:����ӿ�֮���Ž�ת��
                                         */
    unsigned char uc_actionflag;        /*��������ת��������,ȱʡ0:
                                        0:ת����Linux;
                                        1:ת�����������*/

    unsigned char uc_enable8021x;   /*8021xʹ��0:�ر�1:ʹ��*/
    unsigned char uc_unkowncast;        /*δ֪��������ʽ1:ת��0:����*/
    unsigned char uc_unkownmultcast;    /*δ֪�鲥����ʽ1:ת��0:����*/
    unsigned char uc_unkownbroadcast;   /*δ֪�㲥����ʽ1:ת��0:����*/

    /*�ӿڶ�̬����*/
    uint ui_activetime;    /*�ӿڼ���ʱ��*/

    /*����ģ��*/
    unsigned char uc_car_up_indx;          /*��������ģ������,ȱʡ0*/
    unsigned char uc_car_dn_indx;          /*��������ģ������,ȱʡ0*/
    unsigned char uc_pq_up_indx;          /*��������ģ������,ȱʡ0*/
    unsigned char uc_pq_dn_indx;          /*��������ģ������,ȱʡ0*/

    /*����ͳ��*/
    hi_drv_network_traffic_stat_s st_trafficstat;

    /*������*/
    hi_drv_network_unattack_s st_unattack;
    hi_kernel_network_spec_s  st_sepc[HI_KERNEL_NETWORK_SPEC_NUM];

    int (*pv_vifrecvcallback)(hi_kernel_eth0_cb_s * pst_cb, struct sk_buff *pst_skb,
                              struct __hi_kernel_network_vif_s *pst_vif);
    int (*pv_vifxmitcallback)(struct sk_buff *pst_skb,
                              struct __hi_kernel_network_vif_s *pst_vif);
    int (*pv_vifnaptcallback)(hi_kernel_network_data_s * puc_data, void *pst_cb, unsigned char uc_actionflag);
} hi_kernel_network_vif_s;

/*****************************************************************************/
extern struct net_device * hi_kernel_eth0_dev_create( char *ifname );
extern void                hi_kernel_eth0_dev_fillvif( hi_kernel_network_vif_s *pst_vif );
extern void                hi_kernel_eth0_dev_destroy( struct net_device *pst_dev );

extern void                hi_kernel_eth0_dev_removetag( struct sk_buff *pst_skb, ushort *pus_len);
extern int                 hi_kernel_eth0_dev_tolinux( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb,
                                                       struct net_device *pst_dev );
extern struct sk_buff *    hi_kernel_eth0_dev_skb(                                   hi_kernel_eth0_cb_s *pst_cb,
                                                                                     unsigned char *    puc_data,
                                                                                     struct net_device *pst_dev );

extern int                 hi_kernel_eth0_tx_specialsend( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb );
extern int                 hi_kernel_eth0_tx_send( struct sk_buff *pst_skb );
extern void                hi_kernel_eth0_dev_rxhook_reg( hi_kernel_eth0_dev_rxcallback pv_rx_callback );
extern void                hi_kernel_eth0_dev_rxhook_unreg( void );
extern void                hi_kernel_eth0_dev_cb_enable(void);
extern void                hi_kernel_eth0_dev_cb_disable(void);

extern hi_kernel_eth0_dev_rxcallback g_pv_rx_pkt_callback;

/*****************************************************************************/

#endif  /* __HI_KERNEL_ETH0_H__ */
