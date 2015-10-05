/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: hi_drv_hw.h
  功能描述: 驱动头文件
  版本描述: V1.0

  创建日期: 2009年6月1日
  创建作者: 海思驱动开发组    

  修改记录: 
******************************************************************************/
#ifndef __HI_DRV_HW_H__
#define __HI_DRV_HW_H__

#include "base_type_define.h"
#include "hi_type_define.h"

#include "hi_drv_common.h"

#define  HI_HW_MAX_CHANNEL              4

/*接口模式选择，HW/ZSI模式*/
typedef enum
{
    HI_HW_MODE_E = 0,              /*作为HW接口使用*/
    HI_ZSI_MODE_E,                    /*作为ZSI接口使用*/
} HI_INTERFACE_MODE_E;

/*时钟模式*/
typedef enum
{
    HI_HW_CLK_INPUT_E = 0,              /*时钟输入模式,即数据时钟,帧头都由外部输入*/
    HI_HW_CLK_OUTPUT_E,                 /*时钟输出模式,即数据时钟,帧头都内部产生,输出到外部*/
} HI_HW_CLK_MODE_E;

/*帧同步时钟速率*/
typedef enum
{
    HI_HW_FS_8K_E = 0,                     
    HI_HW_FS_16K_E,
} HI_HW_FS_RATE_E;

/*帧同步时钟速率*/
typedef enum
{
    HI_HW_FS_WB_NORMAL_MODE_E = 0,                     
    HI_HW_FS_WB_8K_NODE_E,
} HI_FS_WB_8K_MODE_E;

/*数据时钟速率*/
typedef enum
{
    HI_HW_CLK_8M_E = 0,                    
    HI_HW_CLK_4M_E,                           
    HI_HW_CLK_2M_E,                           
    HI_HW_CLK_1M_E,                           
    HI_HW_CLK_512K_E,                           
} HI_HW_CLK_RATE_E;

/*PCM位宽*/
typedef enum
{
    HI_HW_PCM_8BIT_WIDE_E = 0,                     
    HI_HW_PCM_16BIT_WIDE_E,                           
} HI_HW_PCM_WIDE_E;

/*A U律模式*/
typedef enum
{
    HI_HW_ALAW_E = 0,                      
    HI_HW_ULAW_E,                         
} HI_HW_AULAW_E;

/*中断模式*/
typedef enum
{
    HI_HW_10MS_INT_E = 0,              /*10毫秒中断模式,DSP需要在每次中断中轮循处理完毕4个HW通道*/ 
    HI_HW_2MS5_INT_E,                  /*2.5毫秒中断模式,DSP需要在每次中断中轮循处理完毕1个HW通道*/              
} HI_HW_INT_MDOE_E;


/*通道配置属性*/
typedef struct
{   
    HI_BOOL_E em_enable;               /*通道使能，当禁止时，配置通道输出三态*/
    /*HI_BOOL_E em_g711_en;  */        /*711编码透传模式使能*/
    HI_BOOL_E em_ip_up_loop_en;        /*IP侧上行环回使能*/
    HI_BOOL_E em_tdm_up_loop_en;       /*TDM侧上行环回使能*/
    HI_BOOL_E em_ip_dn_loop_en;        /*IP侧下行环回使能*/
    HI_BOOL_E em_tdm_dn_loop_en;       /*TDM侧下行环回使能*/   
    uchar uc_jbdeq;                    /*2级Jitter Buffer缓存深度 单位为125US / 62.5 US   0 to 320*/
    HI_BOOL_E em_fifo_cnt_en;          /*通道FIFO统计使能*/              
} hi_hw_channel_attr_s;

typedef  struct
{
    uint                ui_index; /*HW/ZSI索引,用于选择是HW0/HW1,或者是ZSI0/ZSI1，目前只对5115H-ASIC有效*/
    HI_INTERFACE_MODE_E em_interface_mode;  /*接口模式选择：HW/ZSI模式，默认为HW接口*/
    HI_BOOL_E em_enable;               /*HW模块工作使能*/
    HI_HW_INT_MDOE_E em_int_mode;      /*中断模式,触发DSP中断*/    
    HI_HW_CLK_MODE_E em_clk_mode;      /*时钟模式*/

    /*帧同步配置FRAM SYNC*/
    HI_HW_FS_RATE_E em_fs_rate;        /*帧同步速率*/    
    HI_LEVEL_E em_fs_level;            /*帧同步信号有效电平*/ 
    HI_EDGE_E em_fs_sample_edge;       /*帧同步信号的采样沿,默认为下降沿*/
    
    HI_BOOL_E em_fas_en;               /*帧失步检测使能*/
    uchar uc_flos_thr;                 /*帧失步门限，即判决由同步到失步的次数*/
    uchar uc_resyc_thr;                /*帧重同步门限，即判决由失步到恢复同步的次数*/

    /*数据时钟配置DATA CLK*/
    HI_HW_CLK_RATE_E em_clk_rate;      /*时钟速率*/    
    HI_EDGE_E em_tx_edge;              /*数据发送信号采样沿上/下,默认为上升沿*/
    HI_EDGE_E em_rx_edge;              /*数据接收信号采样沿上/下,默认为下降沿*/   

    /*数据配置 DATA_TX   DATA_RX*/   
    HI_HW_PCM_WIDE_E em_pcm_wide;      /*PCM位宽*/
    HI_HW_AULAW_E em_aulaw_mode;       /*g711 A U律模式*/ 
    HI_BOOL_E em_codec_rev_en;         /*CODEC输出反转使能*/
    uchar uc_chnnel_tdm_rxalign ;      /*接收时隙偏移*/
    uchar uc_chnnel_tdm_txalign  ;     /*发送时隙偏移*/

    HI_FS_WB_8K_MODE_E em_fs_wb_8k_mode;

    /*通道属性*/
    hi_hw_channel_attr_s  st_channel_attr[HI_HW_MAX_CHANNEL];
} hi_hw_attr_s;



/*状态查询*/
typedef struct
{   
    uint    ui_xxx;                     /*xxx*/
} hi_hw_sta_s;

/*统计，异常记录等*/
typedef struct
{
   uint ui_invalid_parameter_cnt;       /*无效参数计数*/
   uint ui_internal_err;    

   /*硬件统计*/
   uint ui_fifo_full[HI_HW_MAX_CHANNEL];    /*通道FIFO满统计*/   
   uint ui_fifo_emputy[HI_HW_MAX_CHANNEL];  /*通道FIFO空统计*/      

   /*待后续版本考虑释是否实现,驱动内部1秒定时查询统计*/
   uint ui_flos_alm_cnt;                    /*帧失步统计*/
} hi_hw_cnt_s;

/*时间计数*/
typedef struct
{
   uint ui_time_125us;                  /*125微秒计数*/
   uint ui_time_ms;                     /*1毫秒计数*/   
   uint ui_time_10ms;                   /*10毫秒计数*/   
   uint ui_time_s;                      /*1秒计数*/
} hi_hw_time_s;


/*配置属性操作*/
extern uint hi_kernel_hw_attr_get(hi_hw_attr_s *pst_attr);
extern uint hi_kernel_hw_attr_set(hi_hw_attr_s *pst_attr);

/*时间计数操作*/
extern uint hi_kernel_hw_time_get(hi_hw_time_s  *pst_time);

/*状态信息操作*/
extern uint hi_kernel_hw_sta_get(hi_hw_sta_s *pst_sta);

/*统计信息操作*/
extern uint hi_kernel_hw_cnt_get(hi_hw_cnt_s *pst_cnt); 

#endif /* __HI_DRV_XXX_H__ */

