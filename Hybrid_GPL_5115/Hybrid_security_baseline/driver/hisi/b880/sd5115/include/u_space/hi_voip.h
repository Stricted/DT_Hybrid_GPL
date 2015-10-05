/******************************************************************************

                  ��Ȩ���� (C), 2011-2021, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_voip.h
  �� �� ��   : ����
  ��    ��   : y58968
  ��������   : D2011_12_15
  ����޸�   :

******************************************************************************/
#ifndef __HI_VOIP_H__
#define __HI_VOIP_H__


#ifdef __cplusplus
extern "C" {
#endif


/*******************************
       ö��&�궨��
*******************************/


/*SD511xоƬ���ͨ����*/
#define HI_VOIP_MAX_CHANNEL_PER_CHIP   4  

/*SD511xоƬ���֧�ֻ�����*/
#define HI_VOIP_MAX_CONF_PER_CHIP      2 

#define HI_VOIP_MAX_G711_CHAN_NUM      HI_VOIP_MAX_CHANNEL_PER_CHIP

/* ��������͵Ķ��� */
#define	HI_VOIP_TONE_DTMF_0	   	0   /* DTMF����0 */
#define	HI_VOIP_TONE_DTMF_1	   	1
#define	HI_VOIP_TONE_DTMF_2	   	2
#define	HI_VOIP_TONE_DTMF_3	   	3
#define	HI_VOIP_TONE_DTMF_4	   	4
#define	HI_VOIP_TONE_DTMF_5	   	5
#define	HI_VOIP_TONE_DTMF_6	   	6
#define	HI_VOIP_TONE_DTMF_7	   	7
#define	HI_VOIP_TONE_DTMF_8	   	8
#define	HI_VOIP_TONE_DTMF_9	   	9
#define	HI_VOIP_TONE_DTMF_10   	10
#define	HI_VOIP_TONE_DTMF_11   	11
#define	HI_VOIP_TONE_DTMF_12   	12
#define	HI_VOIP_TONE_DTMF_13   	13
#define	HI_VOIP_TONE_DTMF_14   	14
#define	HI_VOIP_TONE_DTMF_15   	15
#define	HI_VOIP_TONE_ESI        16   /* 1375 + 2002HZ */
#define	HI_VOIP_TONE_ESR        17   /* 1529 + 2225HZ */
#define	HI_VOIP_TONE_390HZ	   	18
#define	HI_VOIP_TONE_2225HZ	   	19
#define	HI_VOIP_TONE_1270HZ	   	20	
#define	HI_VOIP_TONE_1400HZ	   	21   
#define	HI_VOIP_TONE_2250HZ	   	22   
#define	HI_VOIP_TONE_980HZ	   	23   
#define	HI_VOIP_TONE_1650HZ	   	24   
#define	HI_VOIP_TONE_1300HZ	   	25   
#define	HI_VOIP_TONE_CT         26   /*section1: 1300Hz + section2: silence*/
#define	HI_VOIP_TONE_CNG        27   /*section1: 1100Hz + section2: silence*/                     
#define HI_VOIP_TONE_ANS        32   /* 2100Hz */
#define HI_VOIP_TONE_ANSBAR     33   /* 2100Hz����λ��ת */                 
#define HI_VOIP_TONE_ANI        36
#define HI_VOIP_TONE_1800HZ     46
#define HI_VOIP_TONE_V21H       47
#define HI_VOIP_TONE_CM_FUN0    48   /* V21L�źţ�callfunction�������ǣ�000*/
#define HI_VOIP_TONE_CM_FUN1    49   /* V21L�źţ�callfunction�������ǣ�001*/
#define HI_VOIP_TONE_CM_FUN2    50   /* V21L�źţ�callfunction�������ǣ�002*/
#define HI_VOIP_TONE_CM_FUN3    51   /* V21L�źţ�callfunction�������ǣ�003*/
#define HI_VOIP_TONE_CM_FUN4    52   /* V21L�źţ�callfunction�������ǣ�004*/
#define HI_VOIP_TONE_CM_FUN5    53   /* V21L�źţ�callfunction�������ǣ�005*/
#define HI_VOIP_TONE_CM_FUN6    54   /* V21L�źţ�callfunction�������ǣ�006*/
#define HI_VOIP_TONE_CM_FUN7    55   /* V21L�źţ�callfunction�������ǣ�007*/
#define HI_VOIP_TONE_INVALID    63   /* �����ź��������ϱ�*/

#define DTMF_NUMBER_SEQUENCE_MAX_LEN       32

#define HI_VOIP_TONEFREQMAX   4   /*���ĵ�����*/
#define HI_VOIP_RHYTHMMAX     8   /*���Ľ�������*/

#define HI_VOIP_CID_FSK_MSG_PAYLOAD_LEN 220
#define HI_VOIP_CT_SIMPLETONE_MAX       8   /*ÿ�� Composed Tone ��������Simple Tone����Ŀ*/
#define HI_VOIP_MAX_PARAM_NUM           36
#define HI_VOIP_DOWNBUF_DATA_SIZE       1598    /* ������������С */

#define LENGTH_OF_DATE_BUF              5
#define LENGTH_OF_TIME_BUF              5
#define LENGTH_OF_CALLING_NUMBER        32
#define LENGTH_OF_CALLER_NAME           64

/* 2833���Ĵ���ģʽ */
typedef enum
{
    HI_VOIP_TRANS_NONE_E = 0,    /* none : ����*/
    HI_VOIP_TRANS_INBAND_E,      /* ���ڴ���:͸�� */
    HI_VOIP_TRANS_OUTBAND_E      /* ���⴫��:2833 */    
} hi_voip_2833transmode_e;

/* �����з���ö�� */
typedef enum
{
    HI_VOIP_DIRECTION_NONE_E,        /* none */
    HI_VOIP_DIRECTION_FROM_TDM_E,     /* ���ˣ�TDM�� */
    HI_VOIP_DIRECTION_FROM_IP_E,      /* Զ�ˣ�IP�� */
    HI_VOIP_DIRECTION_BOTH_E         /* IP/TDM˫�� */
} hi_voip_direction_e;

typedef enum
{
    HI_VOIP_RTCP_STOP, 
    HI_VOIP_RTCP_START  			
} hi_voip_rtcpctl_e;

/* 2833 play mode */
typedef enum
{
    HI_VOIP_MUTE2833_E = 0,   /* ���� */
	HI_VOIP_PLAY2833_E = 1    /* ���� */
} hi_voip_2833playmode_e;

/* �����ö�� */
typedef enum
{
    HI_VOIP_CODEC_G711A_E = 0,      /* G.711 A�� */    
    HI_VOIP_CODEC_G711MU_E = 1,     /* G.711 Mu�� */   
    HI_VOIP_CODEC_G729AB_E = 2,     /* G.729 */     
    HI_VOIP_CODEC_G72353K_E = 3,    /* G.723 5.3K Ԥ��,��֧��*/    
    HI_VOIP_CODEC_G72363K_E = 4,    /* G.723 6.3K Ԥ��,��֧��*/    
    HI_VOIP_CODEC_ILBC_E = 5,       /* iLBC Ԥ��,��֧��*/      
    HI_VOIP_CODEC_VBDCLEAR_E = 6,   /* VBD Clear Ԥ��,��֧��*/    
    HI_VOIP_CODEC_G722_E = 7,       /* G.722 */     
    HI_VOIP_CODEC_T38_E = 0x5a,     /* T38ʱ��Codec��һ��ħ������ */   
    HI_VOIP_CODEC_NUM_E = 8         /* �ܱ��������Ǳ�������*/
} hi_voip_codec_e;

/* �����Ա����ö�� */
typedef enum
{
    HI_VOIP_CONF_DIRECTION_TDM_E = 0,  /* TDM, it means the user of local media gateway */
    HI_VOIP_CONF_DIRECTION_IP_E        /* IP, it means the user of remote media gateway */   
}hi_voip_conf_direction_e;

/*�����ҳ�Ա״̬�޸�ö��*/
typedef enum{
	HI_VOIP_CONF_USER_SIMPLEHOLD_E = 0,  
	HI_VOIP_CONF_USER_ANNHOLD_E = 1,
	HI_VOIP_CONF_USER_UNHOLD_E = 2,
	HI_VOIP_CONF_USER_MUTE_E = 3,
	HI_VOIP_CONF_USER_UNMUTE_E = 4
} hi_voip_conf_userctrl_e;

typedef enum
{
    HI_VOIP_ECM_DISABLE_E = 0,/*��ECMģʽ*/
    HI_VOIP_ECM_ENABLE_E       /*ECMģʽ */  
}hi_voip_ecmmode_e;

/*t38 ��֧�ֵ������������ */
typedef enum
{
    HI_VOIP_FAX_RATE_NO_LIMIT_E   = 0,    /* ����*/
    HI_VOIP_FAX_RATE_V27_2400_E   = 1,    /*V27 2,400bps */
    HI_VOIP_FAX_RATE_V27_4800_E   = 2,    /*V27 4,800bps */
    HI_VOIP_FAX_RATE_V29_7200_E   = 3,    /* V29 7,200bps */
    HI_VOIP_FAX_RATE_V29_9600_E   = 4,    /* V29 9,600bps */
    HI_VOIP_FAX_RATE_V17_7200_E   = 5,    /* V17 7,200bps */
    HI_VOIP_FAX_RATE_V17_9600_E   = 6,    /* V17 9,600bps */
    HI_VOIP_FAX_RATE_V17_12000_E  = 7,    /* V17 12,000bps */
    HI_VOIP_FAX_RATE_V17_14400_E  = 8     /* V17 14,400bps */
}hi_voip_t38_bitrate_e;

/*t38 ����ģʽ */
typedef enum
{
    HI_VOIP_T38EC_REDUNDANCY_E   = 0,  /* �����ģʽ*/
    HI_VOIP_T38EC_FEC_E          = 1   /*ǰ�����ģʽ��Ŀǰ��֧�� */
}hi_voip_t38_ec_e;

/*t38 ����Э�� */
typedef enum
{
    HI_VOIP_T38EC_TCPTL_E   = 0,    /*TCPTLģʽ��Ŀǰ��֧��*/
    HI_VOIP_T38EC_UDPTL_E   = 1     /*UDPTLģʽ*/
}hi_voip_t38_transpro_e;

/*VBD ģʽ */
typedef enum
{
    HI_VOIP_VBD_PASSMODE_E   = 0,  /*͸��ģʽ*/
    HI_VOIP_VBD_CLEARMODE_E  = 1   /*CLEARģʽ*/
}hi_voip_vbd_mode_e;

typedef enum
{
    HI_VOIP_VBD_ALAW_E   = 0,    /*A��*/
    HI_VOIP_VBD_ULAW_E   = 1     /*U��*/
}hi_voip_vbd_g711law_e;

typedef enum
{
    HI_VOIP_EVENT_TONE_DETECTE_E = 0,    /* ��⵽�ź��� */
    HI_VOIP_EVENT_VBD_END_E,             /* VBD���� */
    HI_VOIP_EVENT_REMOTE_FAX_START_E,    /* Զ�˴��濪ʼ */
    HI_VOIP_EVENT_2833_E,                /* announcement���Ž��� */
    HI_VOIP_EVENT_FSK_END_E,             /* FSK�������� */
    HI_VOIP_EVENT_DTMF_END_E,            /* DTMF�������� */
    HI_VOIP_EVENT_TG_END_E,              /* ����������*/
    HI_VOIP_EVENT_T38_END_E,             /* T38 �������*/
    HI_VOIP_EVENT_NO_HEARTBEAT_E,        /* DSP ����ֹͣ*/
    HI_VOIP_EVENT_COREDUMP_E,            /* COREDUMP�ϱ�*/
    HI_VOIP_EVENT_PKT_E,                 /*RTP����*/
    
    HI_VOIP_EVENT_RTCP_STAT_E = 0X101,   /*RTCP��ѯ���ϱ����*/
    HI_VOIP_EVENT_JB_STAT_E,             /*JB��ѯ���ϱ����*/
    HI_VOIP_EVENT_T38_STAT_E
}hi_voip_event_e;

typedef enum
{
    HI_VOIP_PLC_DISABLE, 
    HI_VOIP_PLC_ENABLE  			
} hi_voip_plcctl_e;

typedef enum
{
    HI_VOIP_PKT_RTP_E = 0,   /* RTP packet*/
    HI_VOIP_PKT_RTCP_E,      /* RTCP packet*/
    HI_VOIP_PKT_T38_E   ,     /* T.38 packet*/    
    HI_VOIP_PKT_TRACE_E      /* Trace packet*/    
} hi_voip_pkt_e;

/* <DTS2013071102608  z00159386  20130725  begin */
typedef enum
{
    HI_VOIP_POTS_FREQ_8KHZ = 0,      /*POTS 8K����ģʽ*/
    HI_VOIP_POTS_FREQ_16KHZ          /*POTS 16K����ģʽ */  
}hi_voip_pots_sampling_freq_e;
/* DTS2013071102608  z00159386  20130725  end> */

/*******************************
       �ṹ�嶨��
*******************************/

/* �¼��ϱ��ṹ */
typedef struct
{
    hi_ushort16    us_event_type;    /*�¼�����*/
    hi_ushort16    us_chan_id;       /*ͨ����*/
    hi_ushort16    us_direction;     /*����*/
    hi_ushort16    us_reserved;      /*����*/
} hi_voip_report_event_s;

/* ����������Ϣ�ṹ */
typedef struct
{
    hi_ushort16    us_pkt_type;     /*��������*/
    hi_ushort16    us_pkt_len;      /*���ĳ���*/
}hi_voip_pkt_des_s;

/* ������¼��ϱ���Ϣ */
typedef struct {
    hi_ushort16  us_tone_type;     /*�ź�������*/
    hi_short16   s_level;          /*����*/
    hi_ushort16  auc_reserved[4];  /*����*/
}hi_voip_tonedetect_info_s;

typedef struct
{
    hi_uchar8     uc_event;            /*  �¼�  */
    hi_uchar8     b_volume6 : 6;       /*  ����  */
    hi_uchar8     b_resv : 1;          /*  ��������0  */
    hi_uchar8     b_end : 1;           /*  ������־  */
    hi_ushort16   us_duration;      
}hi_voip_event_2833_s;

typedef struct
{
    hi_ushort16  us_calltime;          /*����ʱ�� */
    hi_ushort16  us_tx_pagenum;        /*���͵�ҳ�� */
    hi_ushort16  us_rx_pagenum;        /*���յ�ҳ��*/
    hi_ushort16  us_faxrate;           /*���յĴ����ٶ�*/
    hi_uint32    ui_tx_ifppktnum;      /*���͵�IFP����*/
    hi_uint32    ui_tx_ifpbytenum;     /*���͵�IFP�� ���ֽ���*/
    hi_uint32    ui_rx_ifppktnum;      /*���յ�IFP����*/
    hi_uint32    ui_rx_ifpbytenum;     /*���յ�IFP�� ���ֽ���*/
} hi_voip_t38_stat_s;

typedef struct
{
    hi_uint32   ui_psent;        /* Sent packet count */
    hi_uint32   ui_osent;        /* Sent octets count */
    hi_uint32   ui_precv;        /* Recvive packet count */
    hi_uint32   ui_orecv;        /* Recvive octets count */
    hi_uint32   ui_rssrc;        /* Data source */
    hi_uint32   ui_lost;         /* Receivers packet lost */
    hi_uint32   ui_jitter;        /* Receivers interarrival jitter */
    hi_uint32   ui_rtd;            /* Round trip delay*/
    hi_uint32   ui_discarded_packets; /*����jitter buffer������µĶ�����*/
} hi_voip_report_rtcpstat_s;

typedef struct
{
    hi_ushort16  us_jbcur_size;           /* JB��ʼֵ */ 
    hi_ushort16  us_jbcur_dep;            /* JB��ǰ��� */
    hi_ushort16  us_min_dep;              /* ��СJB��ȣ��������䣬��λms */
    hi_ushort16  us_max_dep;              /* ���JB��ȣ��������䣬��λms */
    hi_ushort16  us_jitteradj_min;        /* JB����������ֹ�����Сֵ */   
    hi_ushort16  us_jitteradj_max;        /* JB����������ֹ������ֵ */

    hi_ushort16  us_jitterearly_num;      /* JB�絽���� */   
    hi_ushort16  us_jitterlate_num;       /* JB�ٵ����� */
} hi_voip_report_jbstat_s;

/*IP/UDPͷ�����ýṹ*/
typedef struct
{
    hi_char8  uc_reserved[3];        /* ����*/                 
    hi_char8  uc_ipfilterenable;     /* ԴIP��UDP�˿ںŹ���ʹ�� */  
    hi_uint32 ui_localip;            /* ����IP*/
    hi_uint32 ui_remoteip;           /* Զ��IP*/
    hi_ushort16 us_localrtpport;     /* ����RTP�˿ں� */
    hi_ushort16 us_remotertpport;    /* Զ��RTP�˿ں� */
    hi_ushort16 us_localrtcpport;    /* ����RTCP�˿ں� */
    hi_ushort16 us_remotertcpport;   /* Զ��RTCP�˿ں� */
    hi_ushort16 us_localudptlport;   /* ����UDPTL�˿ں� */
    hi_ushort16 us_remoteudptlport;  /* Զ��UDPTL�˿ں� */
} hi_voip_ipudphdr_s;

/*�û�RTCP���ýṹ��*/
typedef struct 
{
    hi_ushort16 us_rtcpinterval;     /*RTCP�������ڣ���λΪ��*/
    hi_uchar8   uc_rtcpevtenable;    /*�¼��ϱ�ʹ��*/
} hi_voip_rtcp_s;

/*ҵ��ӿ�EC���ýṹ��*/
typedef struct
{
    hi_ushort16 us_ecenable;        /*ECʹ��*/
    hi_ushort16 us_erl;             /*ERL*/
    hi_ushort16 us_clear;           /*��ռĴ�����־*/
    hi_ushort16 us_hpenable;        /*��ͨ�˲���ʹ��*/
    hi_ushort16 us_nlpenable;       /*NLPʹ��*/
    hi_ushort16 us_agcenable;       /*AGCʹ��*/
    hi_ushort16 us_cngenable;       /*CNGʹ��*/
    hi_ushort16 us_hreset;          /*Hϵ����λ*/
    hi_ushort16 us_hfreeze;         /*Hϵ������*/
} hi_voip_ec_s;

/*ҵ��ӿ�AGC���ýṹ��*/
typedef struct{
    hi_short16 s_agc_targetlevel_tdm;      /*����VQE��ʱ��������Ŀ��ֵ*/
    hi_short16 s_agc_slewgainup_tdm;       /* TDM���AGC�����΢���ٶ����ֵ*/
    hi_short16 s_agc_slewgaindown_tdm;     /* TDM���AGC�����΢���ٶ���Сֵ*/
    hi_short16 s_agc_targetlevel_ip;       /*����VQE��ʱ��������Ŀ��ֵ*/
    hi_short16 s_agc_slewgainup_ip;        /* IP���AGC�����΢���ٶ����ֵ*/
    hi_short16 s_agc_slewgaindown_ip;      /* IP���AGC�����΢���ٶ���Сֵ*/
} hi_voip_agc_s;

/*�����������ṹ*/
typedef struct 
{
    hi_short16 ToneFreq;  /*������Ƶ��*/
    hi_short16 ToneAmp;   /*����������*/                 
} hi_voip_tone_profile_s;

/*�����Ͷ���ṹ*/
typedef struct 
{
    hi_short16 TPIndex;       /*����������*/
    hi_short16 OnTime;        /*������ʱ��*/
    hi_short16 OffTime;       /*����Ĭʱ��*/
    hi_short16 Reserved;      /*����*/                
} hi_voip_beat_type_s;

typedef struct
{
    hi_ushort16 us_mixflag;     /*������־ 0: ��������1: ��ʾ����*/
    hi_ushort16 us_reserved;    /*����*/ 
    hi_ushort16 us_rhythmnum;   /*���ĵ���Ч���� 1~8*/                                
    hi_ushort16 us_loop;        /*���ŵ�ѭ��������ʵ�ʲ��Ŵ���Ϊloop+1��
                                  0xffffffff��ʾ����ѭ������*/
    hi_voip_tone_profile_s ast_toneinfo[HI_VOIP_TONEFREQMAX];   /*����Ϣ�ṹ��*/
    hi_voip_beat_type_s    ast_rhythminfo[HI_VOIP_RHYTHMMAX];   /*��������*/                
} hi_voip_playtoneinfo_s;

typedef struct
{
    hi_char8   auc_name[32];        /*��������*/
    hi_voip_playtoneinfo_s   st_playtoneinfo;
}hi_voip_simpletonecfg_s;

typedef struct
{
    hi_char8   a_uc_name[32];              /*������ */
    hi_uint32  ui_loop;                   /* ���ŵ�ѭ������ */
    hi_uint32  ui_alternatvoicepath;      /*ѭ����ɴ����������¼�����λ10ms */                 
    hi_uint32  ui_cnt;                    /*����������simple tone�� */  
    hi_uint32  a_ui_tones[HI_VOIP_CT_SIMPLETONE_MAX];               /*����������simple tone ���ͣ�VOIP������˳�򲥷���Щ��*/
} hi_voip_composedtoneinfo_s;

typedef struct 
{
    hi_uint32  ui_maxbitrate;         /*�����������*/
    hi_uint32  ui_udpec;              /*UDP �������ģʽ��Ŀǰֻ֧���������ʽ*/
    hi_uint32  ui_ratemng;            /*�������ʹ���ģʽ*/
    hi_uint32  ui_transpro;           /*t38 ��ʹ�õĴ���Э�飬Ŀǰֻ֧��UDPTL*/
    hi_uint32  ui_udpmaxbuffersize;   /*������Ŀǰ��֧��, �������buffersize*/
    hi_uint32  ui_t38datagramsize;    /*������Ŀǰ��֧��, ���������鳤��*/
    hi_uint32  ui_t38version;         /*��������֧�����ã�Ŀǰʹ�ð汾2*/
    hi_uint32  ui_bitopt;             /*������Ŀǰ��֧��, ת��ͼ������ʽ����*/
}hi_voip_t38_params_s;

typedef struct 
{
    hi_ushort16  us_vbdmode;      /*��������*/
    hi_ushort16  us_aulaw;        /*���͵�ƽ*/
    hi_ushort16  us_tdmlevel;     /*��Ϣ����*/
    hi_ushort16  us_iplevel;      /*ͨ��ռ�ó���*/
    hi_ushort16  us_pritimer;     /*˫������������ʱ��*/
    hi_ushort16  us_sectimer;     /*˫�������Ĵμ�����*/   
 } hi_voip_vbd_params_s;

typedef struct
{
    hi_uint32  ui_hlevel;         /*��Ƶ��ƽֵ   */
    hi_uint32  ui_llevel;         /*��Ƶ��ƽֵ  */
    hi_uint32  ui_play_time;      /*OnTimeʱ������λms */
    hi_uint32  ui_pause_time;     /*OffTime ʱ������λms */
    hi_uint32  ui_dtmfnum;        /*��Ҫ���ŵ�DTMF����*/
    hi_char8   ac_dtmf[DTMF_NUMBER_SEQUENCE_MAX_LEN];     /* 0��9, *, #, A~D  */
} hi_voip_dtmf_params_s;

/*FSK���ýṹ*/
typedef struct 
{
    hi_ushort16 us_mod_type;       /*��������*/
    hi_ushort16 us_tx_level;       /*���͵�ƽ*/
    hi_ushort16 us_msg_length;     /*��Ϣ����*/
    hi_ushort16 us_seize_length;   /*ͨ��ռ�ó���*/
    hi_ushort16 us_mark_length;    /*��־����*/
    hi_ushort16 us_crc_stopbitnum; /*FSKУ��λ��ֹͣλ������1�ĸ�����*/
    hi_char8  ac_msg_payload[HI_VOIP_CID_FSK_MSG_PAYLOAD_LEN];  /*��Ϣ����+2byte�����ͺͳ���+2byte�����ֽ� */
} hi_voip_fsk_params_s;

typedef struct 
{
    hi_uint32  ui_srcip;    /*ԴIP*/
    hi_uint32  ui_desip;    /*Ŀ��IP*/
    hi_uint32  ui_srcport;  /*Դ�˿ں�*/
    hi_uint32  ui_desport;  /*Ŀ�Ķ˿ں�*/
}hi_voip_ipport_s; 

typedef struct{
    hi_ushort16 us_omnipotence;  /*���ܲ���*/
    hi_ushort16 us_chanid;       /*ͨ����*/ 
    hi_ushort16 us_paranum;      /*��������*/
    hi_ushort16 aus_ParaSeq[HI_VOIP_MAX_PARAM_NUM]; /*��������*/   
}hi_voip_omnipotent_s;

typedef struct 
{
    hi_uchar8 aucDump[1024];  /*dump����*/
} hi_voip_coredump_s; 

/* �¼��ϱ��ṹ */
typedef struct
{
    hi_uint32  ui_pkt_type;    /*������*/
    hi_uchar8  auc_pkt_body[1598];   /*������*/
} hi_voip_report_pkt_s;

/****************************
     ��������
****************************/
extern hi_uint32 hi_voip_init(hi_uchar8 *puc_fwpath, hi_void *pv_phy_add);

extern hi_uint32 hi_voip_deinit(hi_void);

extern hi_uint32 hi_voip_cfg_simpletone(hi_uint32 ui_toneindex, hi_voip_simpletonecfg_s * pst_simpletonecfg);

extern hi_uint32 hi_voip_cfg_composedtone(hi_uint32 ui_index, hi_voip_composedtoneinfo_s *pst_info);

extern hi_uint32 hi_voip_create_connection( hi_uint32 ui_chanid, hi_uint32 ui_codec );

extern hi_uint32 hi_voip_destroy_connection (hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_cfg_ipudpheader(hi_uint32 ui_chanid, hi_voip_ipudphdr_s  * pst_ipudpheader);

extern hi_uint32 hi_voip_cfg_rtpttime(hi_uint32 ui_chanid, hi_uint32 ui_ptime);

extern hi_uint32 hi_voip_cfg_rtpsn(hi_uint32 ui_chanid, hi_ushort16 us_seqnum);

extern hi_uint32 hi_voip_start_tone(hi_uint32  ui_chanid, hi_uint32 ui_toneindex, hi_uint32  ui_direct);

extern hi_uint32 hi_voip_start_composedtone(hi_uint32  ui_chanid, hi_uint32 ui_toneindex, hi_uint32  ui_direct );

extern hi_uint32 hi_voip_stop_tone(hi_uint32  ui_chanid,hi_uint32  ui_direct);

extern hi_uint32 hi_voip_cfg_codec(hi_uint32  ui_chanid,  hi_uint32  ui_codec);

/* <DTS2013071102608  z00159386  20130725  begin */
extern hi_uint32 hi_voip_cfg_pots_sampling_freq(hi_uint32 ui_chanid, hi_int32 ui_pots_sampling_freq);
/* DTS2013071102608  z00159386  20130725  end> */
extern hi_uint32 hi_voip_cfg_pt(hi_uint32  ui_chanid, hi_uint32 ui_codec, hi_uint32 ui_pt);

extern hi_uint32 hi_voip_cfg_rtcp(hi_uint32 ui_chanid, hi_voip_rtcp_s *pst_rtcp);

extern hi_uint32 hi_voip_cfg_rtcpctl(hi_uint32 ui_chanid, hi_uint32 ui_rtcp_enable);

extern hi_uint32 hi_voip_cfg_pktdirec(hi_uint32 ui_chanid, hi_uint32 ui_direc);

extern hi_uint32 hi_voip_cfg_dtmf_transmode(hi_uint32 ui_chanid, hi_uint32 ui_mode);

extern hi_uint32 hi_voip_cfg_rtp2833playmode(hi_uint32 ui_chanid, hi_uint32 ui_mode);

extern hi_uint32 hi_voip_cfg_rtp2833payloadtype(hi_uint32 ui_chanid, hi_uint32 ui_rx_payload, hi_uint32 ui_tx_payload);

extern hi_uint32 hi_voip_cfg_faxmodem_transmode (hi_uint32 ui_chanid, hi_uint32 ui_transmode);

extern hi_uint32 hi_voip_cfg_gain(hi_uint32 ui_chanid, hi_int32 i_gainrx, hi_int32 i_gaintx);

extern hi_uint32 hi_voip_cfg_agc(hi_uint32 ui_chanid, hi_uint32 ui_agcdirec, hi_voip_agc_s *pst_cfgagc);

extern hi_uint32 hi_voip_cfg_ec(hi_uint32 ui_chanid, hi_voip_ec_s *pst_cfgec);

extern hi_uint32 hi_voip_cfg_vadcngsc(hi_uint32 ui_chanid, hi_uint32 ui_cng, hi_uint32 ui_vad, hi_uint32 ui_sc);

extern hi_uint32 hi_voip_cfg_jb(hi_uint32 ui_chanid, hi_uint32 ui_static , hi_uint32 ui_minsize,
                                hi_uint32 ui_maxsize, hi_uint32 ui_initsize );

extern hi_uint32 hi_voip_cfg_tonedetect(hi_uint32 ui_chanid, hi_uint32 ui_direction, hi_uint32 a_ui_mask[]);

extern hi_uint32 hi_voip_log_setcfg( hi_uint32 ui_maskdbgflag,hi_uchar8 uc_printflag);

extern hi_uint32 hi_voip_klog_setcfg( hi_uint32 ui_maskdbgflag,hi_uchar8 uc_printflag);

extern hi_uint32 hi_voip_print_chipstate( hi_void );

extern hi_uint32 hi_voip_print_channstate( hi_uint32 ui_chanid );

extern hi_uint32 hi_voip_query_rtcpstat(hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_query_jbstat(hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_create_conference(hi_uint32 ui_confid);

extern hi_uint32 hi_voip_add_conference_user(hi_uint32 ui_chanid, hi_uint32 ui_confid, hi_uint32 ui_direction);

extern hi_uint32 hi_voip_modify_conference_user(hi_uint32 ui_chanid, hi_uint32 ui_confid, hi_uint32 ui_confuserctrl);

extern hi_uint32 hi_voip_del_conference_user(hi_uint32 ui_chanid, hi_uint32 ui_confid,hi_uint32 ui_direction);

extern hi_uint32 hi_voip_delete_conference( hi_uint32 ui_confid);

extern hi_uint32 hi_voip_cfg_t38fax(hi_uint32 ui_chanid, hi_uint32 ui_ecmmode, hi_uint32 ui_framemode);

extern hi_uint32 hi_voip_cfg_t38params(hi_uint32 ui_chanid, hi_voip_t38_params_s *pst_params);

extern hi_uint32 hi_voip_start_t38(hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_stop_t38(hi_uint32 ui_chanid, hi_uint32 ui_codec);

extern hi_uint32 hi_voip_query_t38_stat(hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_start_vbd(hi_uint32 ui_chanid, hi_voip_vbd_params_s *pst_params);

extern hi_uint32 hi_voip_stop_vbd(hi_uint32 ui_chanid, hi_uint32 ui_codec);

extern hi_uint32 hi_voip_play_dtmf(hi_uint32  ui_chanid, hi_uint32 ui_direction, hi_voip_dtmf_params_s *pst_dtmf_param);

extern hi_uint32 hi_voip_stop_dtmf(hi_uint32  ui_chanid, hi_uint32 ui_direction);

extern hi_uint32 hi_voip_send_fsk(hi_uint32  ui_chanid, hi_voip_fsk_params_s *pst_fsk_param);

extern hi_uint32 hi_voip_stop_fsk(hi_uint32  ui_chanid);

extern hi_uint32 hi_voip_cfg_event_mask(hi_uint32  ui_chanid, hi_uint32 ui_direction, hi_uint32  a_ui_mask[2]);

extern hi_uint32 hi_voip_start_rmttrace(hi_uint32 ui_chanid, hi_voip_ipport_s *pst_params);

extern hi_uint32 hi_voip_stop_rmttrace(hi_uint32 ui_chanid);

extern hi_uint32 hi_voip_enter_coredump(hi_void);

extern hi_uint32 hi_voip_query_coredump(hi_void);

extern hi_uint32 hi_voip_cfg_timeslot(hi_uint32 ui_chanid,  hi_uint32 ui_slot);

extern hi_uint32 hi_voip_cfg_tos_dscp(hi_uchar8 uc_dscp);

extern hi_uint32 hi_voip_cfg_tos_precedence(hi_uchar8 uc_precedence);

extern hi_uint32 hi_voip_cfg_plc_ctl(hi_uint32 ui_chanid, hi_uint32 ui_plc);

extern hi_uint32 hi_voip_query_sendmsg(hi_void);

extern hi_uint32 hi_voip_clear_sendmsg(hi_void);

extern hi_uint32 hi_voip_sendto_dsp(hi_ushort16 us_chanid, hi_voip_pkt_e em_pkt_type, hi_void *pv_data, hi_uint32 ui_len);

extern hi_int32 hi_voip_eventinit(hi_void);

extern hi_int32 hi_voip_eventrecv(hi_void *pv_data, hi_int32 i_len );

extern hi_int32 hi_voip_eventexit(hi_void);



#ifdef __cplusplus
}
#endif


#endif  /* __HI_VOIP_H__ */