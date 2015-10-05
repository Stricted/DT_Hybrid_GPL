/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_slic.h
  版 本 号   : 初稿
  作    者   : luocheng 00183967
  生成日期   : D2012_05_23

******************************************************************************/
#ifndef __HI_KERNEL_SLIC_H__
#define __HI_KERNEL_SLIC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_SLIC_MAX_CHIP_NUM   (1)                     /* 最大的芯片数 */
#define HI_SLIC_MAX_PORT_PER_CHIP   (2)            /* 每个芯片最大的通道(端口)数 */

/* 系统的通道(端口)数 */
#define HI_SLIC_MAX_PORT_NUM   (HI_SLIC_MAX_CHIP_NUM * HI_SLIC_MAX_PORT_PER_CHIP)  

#define HI_KERNEL_SLIC_RING_RHYTHM_NUM      8
#define HI_KERNEL_SLIC_MAX_REG_LENGTH       16
#define HI_KERNEL_SLIC_RING_LEN             8          /* 振铃段数 */
#define HI_KERNEL_SLIC_IMPEDENCE_PARA_LEN   128        /* 阻抗参数最大配置项数 */
#define HI_KERNEL_SLIC_RING_PARA_LEN        32         /* 振铃参数最大配置项数 */
#define HI_KERNEL_SLIC_HOWLER_PARA_LEN      64         /* 嚎鸣音参数最大配置项数 */


#define HI_SLIC_REPORT_EVENT_MAXBYTE  32

typedef enum
{
    HI_KERNEL_SLIC_CHANNEL_1_E = 0,
    HI_KERNEL_SLIC_CHANNEL_2_E,
}hi_kernel_slic_channel_num_e;

/* 端口系统模式定义 */
typedef enum
{
    HI_KERNEL_SLIC_MODE_IDLE_E = 0,         /* ON hook时的状态 */
    HI_KERNEL_SLIC_MODE_DISCONNECT_E,       /* 模拟线路掉电,呈高阻态，相当于断小网,在来电显示中乐意考虑用来停掉对端口干扰 */
    HI_KERNEL_SLIC_MODE_OHT_E,              /* 使能挂机传输，用于来电显示 */
    HI_KERNEL_SLIC_MODE_OHTR_E,             /* 使能挂机传输，用于来电显示，同时极性反转 */
    HI_KERNEL_SLIC_MODE_RING_E,             /* 开始振铃 */
    HI_KERNEL_SLIC_MODE_LCF_E,              /* 摘机后，芯片应处于该状态;挂机态的来电显示也应处于该状态 */
    HI_KERNEL_SLIC_MODE_RLCF_E,             /* 对应于LCF状态，附加极性反转功能 */
    HI_KERNEL_SLIC_MODE_BUTT
}hi_kernel_slic_mode_e;

/* 极性控制 */
typedef enum 
{
    HI_KERNEL_SLIC_POLARITY_TYPE_NORMAL,  /*正常极性*/
    HI_KERNEL_SLIC_POLARITY_TYPE_REVERSE, /*反极*/ 
    
    HI_KERNEL_SLIC_POLARITY_TYPE_BUTT    
}hi_kernel_slic_polarity_type_e;

/* 收发模式定义 */
typedef enum
{
    HI_KERNEL_SLIC_PORT_SEND_ONLY_E = 0,   /* 只发 */
    HI_KERNEL_SLIC_PORT_RECV_ONLY_E,       /* 只收 */
    HI_KERNEL_SLIC_PORT_SEND_RECV_E,       /* 收发 */
    HI_KERNEL_SLIC_PORT_DISCONNECT_E,      /* 不收不发 */
    
    HI_KERNEL_SLIC_PORT_SR_BUTT
}hi_kernel_slic_snd_recv_e;

typedef enum
{
    HI_KERNEL_SLIC_RING_FREQ_16HZ_E = 0,
    HI_KERNEL_SLIC_RING_FREQ_25HZ_E,
    HI_KERNEL_SLIC_RING_FREQ_50HZ_E,
}hi_kernel_slic_ring_freq_e;

typedef enum
{
    HI_KERNEL_SLIC_RING_AMPLITUDE_74V_E = 0,
    HI_KERNEL_SLIC_RING_AMPLITUDE_65V_E,
    HI_KERNEL_SLIC_RING_AMPLITUDE_50V_E,
}hi_kernel_slic_ring_amplitude_e;

/* 计费脉冲频率枚举 */
typedef enum
{
    HI_KERNEL_SLIC_COUNTPULSE_FREQ_16KC_E = 0,             /* 使用16kc计费脉冲 */
    HI_KERNEL_SLIC_COUNTPULSE_FREQ_12KC_E = 1,             /* 使用12kc计费脉冲 */
}hi_kernel_slic_countpulse_freq_e;

typedef enum
{
    HI_KERNEL_SLIC_REGTYPE_INVALID_E = 0,
    HI_KERNEL_SLIC_REGTYPE_SW_RST_E,
    HI_KERNEL_SLIC_REGTYPE_HW_RST_E,
    HI_KERNEL_SLIC_REGTYPE_NOP_E,
    HI_KERNEL_SLIC_REGTYPE_WR_TX_TS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_TX_TS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_RX_TS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_RX_TS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CLK_SLOT_TX_EDGE_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CLK_SLOT_TX_EDGE_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CE_MODE_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CE_MODE_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SIG_E,
    HI_KERNEL_SLIC_REGTYPE_WR_VP_GAINS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_VP_GAINS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_INOUT_DATA_E,
    HI_KERNEL_SLIC_REGTYPE_RD_INOUT_DATA_E,
    HI_KERNEL_SLIC_REGTYPE_WR_INOUT_DIR_E,
    HI_KERNEL_SLIC_REGTYPE_RD_INOUT_DIR_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SYS_STATE_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SYS_STATE_E,
    HI_KERNEL_SLIC_REGTYPE_WR_OP_FUNC_E,
    HI_KERNEL_SLIC_REGTYPE_RD_OP_FUNC_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SYS_STATE_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SYS_STATE_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_WR_INT_MSK_E,
    HI_KERNEL_SLIC_REGTYPE_RD_INT_MSK_E,
    HI_KERNEL_SLIC_REGTYPE_WR_OP_COND_E,
    HI_KERNEL_SLIC_REGTYPE_RD_OP_COND_E,
    HI_KERNEL_SLIC_REGTYPE_RD_REV_PCN_E,
    HI_KERNEL_SLIC_REGTYPE_WR_GX_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_GX_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_GR_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_GR_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_B_FILT_FIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_B_FILT_FIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_X_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_X_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_R_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_R_FILT_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_B_FILT_IIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_B_FILT_IIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_Z_FILT_FIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_Z_FILT_FIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_Z_FILT_IIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_RD_Z_FILT_IIR_COEFF_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CONV_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CONV_CFG_E,
    HI_KERNEL_SLIC_REGTYPE_WR_LOOP_SPRV_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_LOOP_SPRV_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_DC_FEED_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_DC_FEED_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_DISN_E,
    HI_KERNEL_SLIC_REGTYPE_RD_DISN_E,
    HI_KERNEL_SLIC_REGTYPE_RD_TX_PCM_TEST_E,
    HI_KERNEL_SLIC_REGTYPE_WR_METERING_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_METERING_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SIG_GEN_AB_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SIG_GEN_AB_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SIG_GEN_CD_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SIG_GEN_CD_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SIG_GEN_CTRL_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SIG_GEN_CTRL_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CADENCE_TIMER_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CADENCE_TIMER_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CALLID_DATA_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CALLID_DATA_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SWREG_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SWREG_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_WR_SWREG_CTRL_E,
    HI_KERNEL_SLIC_REGTYPE_RD_SWREG_CTRL_E,
    HI_KERNEL_SLIC_REGTYPE_WR_CALLID_PARMS_E,
    HI_KERNEL_SLIC_REGTYPE_RD_CALLID_PARMS_E,
    
    HI_KERNEL_SLIC_REGTYPE_BUTT_E
}hi_kernel_slic_chip_regtype_e;

typedef struct
{
    hi_uchar8 uc_hook;   /* hook状态 */
    hi_uchar8 uc_gnk;    /* Ground-key */
    hi_uchar8 uc_tmp;    /* 温度 */
    hi_uchar8 uc_resv;   /* 保留 */
}hi_kernel_chn_status_s;

/* 初始化参数结构 */
typedef struct
{
    hi_uint32 ui_chip_num;      /* 芯片数*/
    hi_uint32 ui_port_per_chip; /* 每芯片支持的端口数*/
    hi_uint32 ui_ctrl_mode;     /* 控制模式:SPI/ZSI */
}hi_kernel_slic_init_set_s;


/*-----------------------------用户态/内核态消息结构体----------------------------*/
typedef struct
{
    hi_uint32 ui_chip_idx;
    hi_uint32 ui_port_idx;
}hi_kernel_slic_chip_port_s;

/* 连时隙参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_uint32 ui_timeslot;    /* HW时隙号*/
}hi_kernel_slic_active_set_s;

typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_uchar8 auc_howler_para[HI_KERNEL_SLIC_HOWLER_PARA_LEN];
}hi_kernel_slic_howl_set_s;

/* 播放振铃参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_uchar8 auc_ring_para[HI_KERNEL_SLIC_RING_PARA_LEN];
}hi_kernel_slic_ring_set_s;

typedef struct
{
    hi_uint32   ui_ring_loop;             /* 振铃段数,若为0则表示当前为空振铃 */
    hi_ushort16 aus_ring_table[HI_KERNEL_SLIC_RING_RHYTHM_NUM];  /* 振铃断续比 */
}hi_kernel_slic_cadence_s;

/* 播放振铃参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_kernel_slic_cadence_s   st_cadence;
}hi_kernel_slic_ring_send_s;


/* 拍叉上下限设置参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_uint32 ui_flash_up_limit;  /*拍叉上限,单位:ms */
    hi_uint32 ui_flash_down_limit;  /*拍叉下限, 单位:ms */
}hi_kernel_slic_flashhook_time_set_s;

/* 阻抗设置参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_uchar8 auc_impedence[HI_KERNEL_SLIC_IMPEDENCE_PARA_LEN];
}hi_kernel_slic_impedence_set_s;


/* 增益设置参数结构 */
typedef struct
{
    hi_kernel_slic_chip_port_s st_chipport;
    hi_int32 i_tx_gain;   /* 发送增益 [-60,60] 单位0.1dB */
    hi_int32 i_rx_gain;   /* 发送增益 [-120,0] 单位0.1dB */
}hi_kernel_slic_gain_set_s;

/* SPI读写参数结构体 */
typedef struct
{
    hi_uint32 ui_chip_idx;
    hi_uint32 ui_chan_idx;
    hi_uint32 ui_cmd;
    hi_uint32 ui_len;
    hi_uchar8 auc_data[32];
}hi_kernel_slic_readwrite_spi_s;

typedef struct
{
    hi_uint32 ui_chip_idx;
    hi_uint32 ui_chan_idx;
    hi_uint32 ui_cmd;
    hi_uint32 ui_value;
    hi_uint32 ui_mask;
}hi_kernel_slic_readwritemask_spi_s;


#if 1
#define hi_kernel_slic_systrace(ui_ret, arg1, arg2, arg3, arg4) hi_kernel_systrace(HI_SRCMODULE_KDRIVER_CODEC,ui_ret, arg1, arg2, arg3, arg4)
#define hi_kernel_slic_memdes(ui_dbglevel,puc_src,ui_len)  hi_kernel_log_mem(HI_SRCMODULE_KDRIVER_CODEC,ui_dbglevel,puc_src,ui_len)
#define hi_kernel_slic_log(ui_dbglevel,fmt,arg...)   hi_kernel_log_print(HI_SRCMODULE_KDRIVER_CODEC,ui_dbglevel,(hi_uchar8 *)fmt, ##arg)
#else
#define hi_kernel_slic_systrace(ui_ret, arg1, arg2, arg3, arg4)
#define hi_kernel_slic_printmemdes(ui_dbglevel,puc_src,ui_len,fmt,arg...)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_SLIC_H__ */
