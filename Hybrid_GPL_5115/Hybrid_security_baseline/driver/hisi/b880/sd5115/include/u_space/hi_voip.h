/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_voip.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_VOIP_H__
#define __HI_VOIP_H__


#ifdef __cplusplus
extern "C" {
#endif


/*******************************
       枚举&宏定义
*******************************/


/*SD511x芯片最大通道数*/
#define HI_VOIP_MAX_CHANNEL_PER_CHIP   4  

/*SD511x芯片最大支持会议数*/
#define HI_VOIP_MAX_CONF_PER_CHIP      2 

#define HI_VOIP_MAX_G711_CHAN_NUM      HI_VOIP_MAX_CHANNEL_PER_CHIP

/* 音检测类型的定义 */
#define	HI_VOIP_TONE_DTMF_0	   	0   /* DTMF号码0 */
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
#define HI_VOIP_TONE_ANSBAR     33   /* 2100Hz带相位翻转 */                 
#define HI_VOIP_TONE_ANI        36
#define HI_VOIP_TONE_1800HZ     46
#define HI_VOIP_TONE_V21H       47
#define HI_VOIP_TONE_CM_FUN0    48   /* V21L信号：callfunction的内容是：000*/
#define HI_VOIP_TONE_CM_FUN1    49   /* V21L信号：callfunction的内容是：001*/
#define HI_VOIP_TONE_CM_FUN2    50   /* V21L信号：callfunction的内容是：002*/
#define HI_VOIP_TONE_CM_FUN3    51   /* V21L信号：callfunction的内容是：003*/
#define HI_VOIP_TONE_CM_FUN4    52   /* V21L信号：callfunction的内容是：004*/
#define HI_VOIP_TONE_CM_FUN5    53   /* V21L信号：callfunction的内容是：005*/
#define HI_VOIP_TONE_CM_FUN6    54   /* V21L信号：callfunction的内容是：006*/
#define HI_VOIP_TONE_CM_FUN7    55   /* V21L信号：callfunction的内容是：007*/
#define HI_VOIP_TONE_INVALID    63   /* 所有信号音都不上报*/

#define DTMF_NUMBER_SEQUENCE_MAX_LEN       32

#define HI_VOIP_TONEFREQMAX   4   /*最大的单音数*/
#define HI_VOIP_RHYTHMMAX     8   /*最大的节拍音数*/

#define HI_VOIP_CID_FSK_MSG_PAYLOAD_LEN 220
#define HI_VOIP_CT_SIMPLETONE_MAX       8   /*每个 Composed Tone 包含最多的Simple Tone的数目*/
#define HI_VOIP_MAX_PARAM_NUM           36
#define HI_VOIP_DOWNBUF_DATA_SIZE       1598    /* 报文数据区大小 */

#define LENGTH_OF_DATE_BUF              5
#define LENGTH_OF_TIME_BUF              5
#define LENGTH_OF_CALLING_NUMBER        32
#define LENGTH_OF_CALLER_NAME           64

/* 2833包的传输模式 */
typedef enum
{
    HI_VOIP_TRANS_NONE_E = 0,    /* none : 擦除*/
    HI_VOIP_TRANS_INBAND_E,      /* 带内传输:透传 */
    HI_VOIP_TRANS_OUTBAND_E      /* 带外传输:2833 */    
} hi_voip_2833transmode_e;

/* 上下行方向枚举 */
typedef enum
{
    HI_VOIP_DIRECTION_NONE_E,        /* none */
    HI_VOIP_DIRECTION_FROM_TDM_E,     /* 本端，TDM侧 */
    HI_VOIP_DIRECTION_FROM_IP_E,      /* 远端，IP侧 */
    HI_VOIP_DIRECTION_BOTH_E         /* IP/TDM双向 */
} hi_voip_direction_e;

typedef enum
{
    HI_VOIP_RTCP_STOP, 
    HI_VOIP_RTCP_START  			
} hi_voip_rtcpctl_e;

/* 2833 play mode */
typedef enum
{
    HI_VOIP_MUTE2833_E = 0,   /* 静音 */
	HI_VOIP_PLAY2833_E = 1    /* 播放 */
} hi_voip_2833playmode_e;

/* 编解码枚举 */
typedef enum
{
    HI_VOIP_CODEC_G711A_E = 0,      /* G.711 A率 */    
    HI_VOIP_CODEC_G711MU_E = 1,     /* G.711 Mu率 */   
    HI_VOIP_CODEC_G729AB_E = 2,     /* G.729 */     
    HI_VOIP_CODEC_G72353K_E = 3,    /* G.723 5.3K 预留,不支持*/    
    HI_VOIP_CODEC_G72363K_E = 4,    /* G.723 6.3K 预留,不支持*/    
    HI_VOIP_CODEC_ILBC_E = 5,       /* iLBC 预留,不支持*/      
    HI_VOIP_CODEC_VBDCLEAR_E = 6,   /* VBD Clear 预留,不支持*/    
    HI_VOIP_CODEC_G722_E = 7,       /* G.722 */     
    HI_VOIP_CODEC_T38_E = 0x5a,     /* T38时，Codec填一个魔鬼数字 */   
    HI_VOIP_CODEC_NUM_E = 8         /* 总编码数，非编码类型*/
} hi_voip_codec_e;

/* 会议成员方向枚举 */
typedef enum
{
    HI_VOIP_CONF_DIRECTION_TDM_E = 0,  /* TDM, it means the user of local media gateway */
    HI_VOIP_CONF_DIRECTION_IP_E        /* IP, it means the user of remote media gateway */   
}hi_voip_conf_direction_e;

/*会议室成员状态修改枚举*/
typedef enum{
	HI_VOIP_CONF_USER_SIMPLEHOLD_E = 0,  
	HI_VOIP_CONF_USER_ANNHOLD_E = 1,
	HI_VOIP_CONF_USER_UNHOLD_E = 2,
	HI_VOIP_CONF_USER_MUTE_E = 3,
	HI_VOIP_CONF_USER_UNMUTE_E = 4
} hi_voip_conf_userctrl_e;

typedef enum
{
    HI_VOIP_ECM_DISABLE_E = 0,/*非ECM模式*/
    HI_VOIP_ECM_ENABLE_E       /*ECM模式 */  
}hi_voip_ecmmode_e;

/*t38 所支持的最高速率类型 */
typedef enum
{
    HI_VOIP_FAX_RATE_NO_LIMIT_E   = 0,    /* 保留*/
    HI_VOIP_FAX_RATE_V27_2400_E   = 1,    /*V27 2,400bps */
    HI_VOIP_FAX_RATE_V27_4800_E   = 2,    /*V27 4,800bps */
    HI_VOIP_FAX_RATE_V29_7200_E   = 3,    /* V29 7,200bps */
    HI_VOIP_FAX_RATE_V29_9600_E   = 4,    /* V29 9,600bps */
    HI_VOIP_FAX_RATE_V17_7200_E   = 5,    /* V17 7,200bps */
    HI_VOIP_FAX_RATE_V17_9600_E   = 6,    /* V17 9,600bps */
    HI_VOIP_FAX_RATE_V17_12000_E  = 7,    /* V17 12,000bps */
    HI_VOIP_FAX_RATE_V17_14400_E  = 8     /* V17 14,400bps */
}hi_voip_t38_bitrate_e;

/*t38 纠错模式 */
typedef enum
{
    HI_VOIP_T38EC_REDUNDANCY_E   = 0,  /* 冗余包模式*/
    HI_VOIP_T38EC_FEC_E          = 1   /*前向纠错模式，目前不支持 */
}hi_voip_t38_ec_e;

/*t38 传输协议 */
typedef enum
{
    HI_VOIP_T38EC_TCPTL_E   = 0,    /*TCPTL模式，目前不支持*/
    HI_VOIP_T38EC_UDPTL_E   = 1     /*UDPTL模式*/
}hi_voip_t38_transpro_e;

/*VBD 模式 */
typedef enum
{
    HI_VOIP_VBD_PASSMODE_E   = 0,  /*透传模式*/
    HI_VOIP_VBD_CLEARMODE_E  = 1   /*CLEAR模式*/
}hi_voip_vbd_mode_e;

typedef enum
{
    HI_VOIP_VBD_ALAW_E   = 0,    /*A率*/
    HI_VOIP_VBD_ULAW_E   = 1     /*U率*/
}hi_voip_vbd_g711law_e;

typedef enum
{
    HI_VOIP_EVENT_TONE_DETECTE_E = 0,    /* 检测到信号音 */
    HI_VOIP_EVENT_VBD_END_E,             /* VBD结束 */
    HI_VOIP_EVENT_REMOTE_FAX_START_E,    /* 远端传真开始 */
    HI_VOIP_EVENT_2833_E,                /* announcement播放结束 */
    HI_VOIP_EVENT_FSK_END_E,             /* FSK产生结束 */
    HI_VOIP_EVENT_DTMF_END_E,            /* DTMF产生结束 */
    HI_VOIP_EVENT_TG_END_E,              /* 音产生结束*/
    HI_VOIP_EVENT_T38_END_E,             /* T38 传真结束*/
    HI_VOIP_EVENT_NO_HEARTBEAT_E,        /* DSP 心跳停止*/
    HI_VOIP_EVENT_COREDUMP_E,            /* COREDUMP上报*/
    HI_VOIP_EVENT_PKT_E,                 /*RTP发送*/
    
    HI_VOIP_EVENT_RTCP_STAT_E = 0X101,   /*RTCP查询的上报结果*/
    HI_VOIP_EVENT_JB_STAT_E,             /*JB查询的上报结果*/
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
    HI_VOIP_POTS_FREQ_8KHZ = 0,      /*POTS 8K采样模式*/
    HI_VOIP_POTS_FREQ_16KHZ          /*POTS 16K采样模式 */  
}hi_voip_pots_sampling_freq_e;
/* DTS2013071102608  z00159386  20130725  end> */

/*******************************
       结构体定义
*******************************/

/* 事件上报结构 */
typedef struct
{
    hi_ushort16    us_event_type;    /*事件类型*/
    hi_ushort16    us_chan_id;       /*通道号*/
    hi_ushort16    us_direction;     /*方向*/
    hi_ushort16    us_reserved;      /*保留*/
} hi_voip_report_event_s;

/* 报文描述信息结构 */
typedef struct
{
    hi_ushort16    us_pkt_type;     /*报文类型*/
    hi_ushort16    us_pkt_len;      /*报文长度*/
}hi_voip_pkt_des_s;

/* 音检测事件上报信息 */
typedef struct {
    hi_ushort16  us_tone_type;     /*信号音类型*/
    hi_short16   s_level;          /*能量*/
    hi_ushort16  auc_reserved[4];  /*保留*/
}hi_voip_tonedetect_info_s;

typedef struct
{
    hi_uchar8     uc_event;            /*  事件  */
    hi_uchar8     b_volume6 : 6;       /*  音量  */
    hi_uchar8     b_resv : 1;          /*  保留，置0  */
    hi_uchar8     b_end : 1;           /*  结束标志  */
    hi_ushort16   us_duration;      
}hi_voip_event_2833_s;

typedef struct
{
    hi_ushort16  us_calltime;          /*呼叫时长 */
    hi_ushort16  us_tx_pagenum;        /*发送的页数 */
    hi_ushort16  us_rx_pagenum;        /*接收的页数*/
    hi_ushort16  us_faxrate;           /*最终的传真速度*/
    hi_uint32    ui_tx_ifppktnum;      /*发送的IFP包数*/
    hi_uint32    ui_tx_ifpbytenum;     /*发送的IFP包 的字节数*/
    hi_uint32    ui_rx_ifppktnum;      /*接收的IFP包数*/
    hi_uint32    ui_rx_ifpbytenum;     /*接收的IFP包 的字节数*/
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
    hi_uint32   ui_discarded_packets; /*由于jitter buffer溢出导致的丢包数*/
} hi_voip_report_rtcpstat_s;

typedef struct
{
    hi_ushort16  us_jbcur_size;           /* JB初始值 */ 
    hi_ushort16  us_jbcur_dep;            /* JB当前深度 */
    hi_ushort16  us_min_dep;              /* 最小JB深度，主机可配，单位ms */
    hi_ushort16  us_max_dep;              /* 最大JB深度，主机可配，单位ms */
    hi_ushort16  us_jitteradj_min;        /* JB深度曾经出现过的最小值 */   
    hi_ushort16  us_jitteradj_max;        /* JB深度曾经出现过的最大值 */

    hi_ushort16  us_jitterearly_num;      /* JB早到个数 */   
    hi_ushort16  us_jitterlate_num;       /* JB迟到个数 */
} hi_voip_report_jbstat_s;

/*IP/UDP头部配置结构*/
typedef struct
{
    hi_char8  uc_reserved[3];        /* 保留*/                 
    hi_char8  uc_ipfilterenable;     /* 源IP、UDP端口号过滤使能 */  
    hi_uint32 ui_localip;            /* 本端IP*/
    hi_uint32 ui_remoteip;           /* 远端IP*/
    hi_ushort16 us_localrtpport;     /* 本端RTP端口号 */
    hi_ushort16 us_remotertpport;    /* 远端RTP端口号 */
    hi_ushort16 us_localrtcpport;    /* 本端RTCP端口号 */
    hi_ushort16 us_remotertcpport;   /* 远端RTCP端口号 */
    hi_ushort16 us_localudptlport;   /* 本端UDPTL端口号 */
    hi_ushort16 us_remoteudptlport;  /* 远端UDPTL端口号 */
} hi_voip_ipudphdr_s;

/*用户RTCP配置结构体*/
typedef struct 
{
    hi_ushort16 us_rtcpinterval;     /*RTCP发送周期，单位为秒*/
    hi_uchar8   uc_rtcpevtenable;    /*事件上报使能*/
} hi_voip_rtcp_s;

/*业务接口EC配置结构体*/
typedef struct
{
    hi_ushort16 us_ecenable;        /*EC使能*/
    hi_ushort16 us_erl;             /*ERL*/
    hi_ushort16 us_clear;           /*清空寄存器标志*/
    hi_ushort16 us_hpenable;        /*高通滤波器使能*/
    hi_ushort16 us_nlpenable;       /*NLP使能*/
    hi_ushort16 us_agcenable;       /*AGC使能*/
    hi_ushort16 us_cngenable;       /*CNG使能*/
    hi_ushort16 us_hreset;          /*H系数复位*/
    hi_ushort16 us_hfreeze;         /*H系数冻结*/
} hi_voip_ec_s;

/*业务接口AGC配置结构体*/
typedef struct{
    hi_short16 s_agc_targetlevel_tdm;      /*上行VQE长时间能量的目标值*/
    hi_short16 s_agc_slewgainup_tdm;       /* TDM侧的AGC增益的微调速度最大值*/
    hi_short16 s_agc_slewgaindown_tdm;     /* TDM侧的AGC增益的微调速度最小值*/
    hi_short16 s_agc_targetlevel_ip;       /*下行VQE长时间能量的目标值*/
    hi_short16 s_agc_slewgainup_ip;        /* IP侧的AGC增益的微调速度最大值*/
    hi_short16 s_agc_slewgaindown_ip;      /* IP侧的AGC增益的微调速度最小值*/
} hi_voip_agc_s;

/*播放音特征结构*/
typedef struct 
{
    hi_short16 ToneFreq;  /*播放音频率*/
    hi_short16 ToneAmp;   /*播放音幅度*/                 
} hi_voip_tone_profile_s;

/*节奏型定义结构*/
typedef struct 
{
    hi_short16 TPIndex;       /*音特征索引*/
    hi_short16 OnTime;        /*音持续时长*/
    hi_short16 OffTime;       /*音静默时长*/
    hi_short16 Reserved;      /*保留*/                
} hi_voip_beat_type_s;

typedef struct
{
    hi_ushort16 us_mixflag;     /*混音标志 0: 不混音，1: 表示混音*/
    hi_ushort16 us_reserved;    /*保留*/ 
    hi_ushort16 us_rhythmnum;   /*节拍的有效个数 1~8*/                                
    hi_ushort16 us_loop;        /*播放的循环次数。实际播放次数为loop+1，
                                  0xffffffff表示无限循环播放*/
    hi_voip_tone_profile_s ast_toneinfo[HI_VOIP_TONEFREQMAX];   /*音信息结构体*/
    hi_voip_beat_type_s    ast_rhythminfo[HI_VOIP_RHYTHMMAX];   /*节拍属性*/                
} hi_voip_playtoneinfo_s;

typedef struct
{
    hi_char8   auc_name[32];        /*音的名字*/
    hi_voip_playtoneinfo_s   st_playtoneinfo;
}hi_voip_simpletonecfg_s;

typedef struct
{
    hi_char8   a_uc_name[32];              /*音名称 */
    hi_uint32  ui_loop;                   /* 播放的循环次数 */
    hi_uint32  ui_alternatvoicepath;      /*循环间可传输语音的事件，单位10ms */                 
    hi_uint32  ui_cnt;                    /*本音包含的simple tone数 */  
    hi_uint32  a_ui_tones[HI_VOIP_CT_SIMPLETONE_MAX];               /*本音包含的simple tone 类型，VOIP驱动将顺序播放这些音*/
} hi_voip_composedtoneinfo_s;

typedef struct 
{
    hi_uint32  ui_maxbitrate;         /*传真最大速率*/
    hi_uint32  ui_udpec;              /*UDP 错误纠正模式，目前只支持冗余包方式*/
    hi_uint32  ui_ratemng;            /*数据速率管理模式*/
    hi_uint32  ui_transpro;           /*t38 包使用的传输协议，目前只支持UDPTL*/
    hi_uint32  ui_udpmaxbuffersize;   /*保留，目前不支持, 配置最大buffersize*/
    hi_uint32  ui_t38datagramsize;    /*保留，目前不支持, 配置最大分组长度*/
    hi_uint32  ui_t38version;         /*保留，不支持配置，目前使用版本2*/
    hi_uint32  ui_bitopt;             /*保留，目前不支持, 转换图像编码格式配置*/
}hi_voip_t38_params_s;

typedef struct 
{
    hi_ushort16  us_vbdmode;      /*调制类型*/
    hi_ushort16  us_aulaw;        /*发送电平*/
    hi_ushort16  us_tdmlevel;     /*消息长度*/
    hi_ushort16  us_iplevel;      /*通道占用长度*/
    hi_ushort16  us_pritimer;     /*双向静音检测的主计时器*/
    hi_ushort16  us_sectimer;     /*双向静音检测的次计数器*/   
 } hi_voip_vbd_params_s;

typedef struct
{
    hi_uint32  ui_hlevel;         /*高频电平值   */
    hi_uint32  ui_llevel;         /*低频电平值  */
    hi_uint32  ui_play_time;      /*OnTime时长，单位ms */
    hi_uint32  ui_pause_time;     /*OffTime 时长，单位ms */
    hi_uint32  ui_dtmfnum;        /*需要播放的DTMF个数*/
    hi_char8   ac_dtmf[DTMF_NUMBER_SEQUENCE_MAX_LEN];     /* 0～9, *, #, A~D  */
} hi_voip_dtmf_params_s;

/*FSK配置结构*/
typedef struct 
{
    hi_ushort16 us_mod_type;       /*调制类型*/
    hi_ushort16 us_tx_level;       /*发送电平*/
    hi_ushort16 us_msg_length;     /*消息长度*/
    hi_ushort16 us_seize_length;   /*通道占用长度*/
    hi_ushort16 us_mark_length;    /*标志长度*/
    hi_ushort16 us_crc_stopbitnum; /*FSK校验位的停止位个数（1的个数）*/
    hi_char8  ac_msg_payload[HI_VOIP_CID_FSK_MSG_PAYLOAD_LEN];  /*消息净荷+2byte的类型和长度+2byte保留字节 */
} hi_voip_fsk_params_s;

typedef struct 
{
    hi_uint32  ui_srcip;    /*源IP*/
    hi_uint32  ui_desip;    /*目的IP*/
    hi_uint32  ui_srcport;  /*源端口号*/
    hi_uint32  ui_desport;  /*目的端口号*/
}hi_voip_ipport_s; 

typedef struct{
    hi_ushort16 us_omnipotence;  /*万能参数*/
    hi_ushort16 us_chanid;       /*通道号*/ 
    hi_ushort16 us_paranum;      /*参数个数*/
    hi_ushort16 aus_ParaSeq[HI_VOIP_MAX_PARAM_NUM]; /*参数序列*/   
}hi_voip_omnipotent_s;

typedef struct 
{
    hi_uchar8 aucDump[1024];  /*dump内容*/
} hi_voip_coredump_s; 

/* 事件上报结构 */
typedef struct
{
    hi_uint32  ui_pkt_type;    /*包类型*/
    hi_uchar8  auc_pkt_body[1598];   /*包数据*/
} hi_voip_report_pkt_s;

/****************************
     函数声明
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