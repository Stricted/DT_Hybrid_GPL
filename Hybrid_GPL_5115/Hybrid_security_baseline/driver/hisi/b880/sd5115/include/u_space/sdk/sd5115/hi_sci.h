/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: hi_sci.h
  功能描述: 海思sci驱动接口
  版本描述: V1.0

  创建日期: 2012年10月24日
  创建作者: l00220454

  修改记录: 
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef __HI_SCI_H__
#define __HI_SCI_H__

#include "base_type_define.h"

/***************************** ioctl命令宏定义 ********************************/
#ifndef _IOWR
#define _IOWR(x,y,z) (((x)<<8)|y)
#endif
#ifndef _IOW
#define _IOW(x,y,z)  (((x)<<8)|y)
#endif
#ifndef _IOR
#define _IOR(x,y,z)  (((x)<<8)|y)
#endif 
#ifndef _IO
#define _IO(x,y)     (((x)<<8)|y)
#endif

/******************************** ioctl命令 **********************************/
#define HI_CMD_SCI_OPEN               _IOW(HI_ID_SCI,  0x1, hi_sci_conf_s)
#define HI_CMD_SCI_CLOSE              _IOW(HI_ID_SCI,  0x2, HI_SCI_PORT_E)
#define HI_CMD_SCI_RESET              _IOW(HI_ID_SCI,  0x3, hi_sci_reset_s)
#define HI_CMD_SCI_DEACTIVE           _IOW(HI_ID_SCI,  0x4, HI_SCI_PORT_E)
#define HI_CMD_SCI_GET_ATR            _IOWR(HI_ID_SCI, 0x5, hi_sci_atr_s)
#define HI_CMD_SCI_GET_STATUS         _IOWR(HI_ID_SCI, 0x6, hi_sci_status_s)
#define HI_CMD_SCI_CONF_VCC           _IOW(HI_ID_SCI,  0x7, hi_sci_level_s)
#define HI_CMD_SCI_CONF_DETECT        _IOW(HI_ID_SCI,  0x8, hi_sci_level_s)
#define HI_CMD_SCI_CONF_CLK_MODE      _IOW(HI_ID_SCI,  0x9, hi_sci_clk_s)
#define HI_CMD_SCI_SEND_DATA          _IOWR(HI_ID_SCI, 0xa, hi_sci_data_s)
#define HI_CMD_SCI_RECEIVE_DATA       _IOWR(HI_ID_SCI, 0xb, hi_sci_data_s)
#define HI_CMD_SCI_SWITCH_PROTOCOL    _IOW(HI_ID_SCI,  0xc, hi_sci_conf_s)
#define HI_CMD_SCI_SET_BAUD           _IOW(HI_ID_SCI,  0xd, hi_sci_baud_s)
#define HI_CMD_SCI_SET_CHGUARD        _IOW(HI_ID_SCI,  0xe, hi_sci_chguard_s)
#define HI_CMD_SCI_SEND_PPS_DATA      _IOW(HI_ID_SCI,  0xf, hi_sci_pps_s)
#define HI_CMD_SCI_GET_PPS_DATA       _IOWR(HI_ID_SCI, 0x10,hi_sci_pps_s)
#define HI_CMD_SCI_GET_PARAM          _IOWR(HI_ID_SCI, 0x11,hi_sci_params_s)
#define HI_CMD_SCI_SET_CHARTIMEOUT    _IOW(HI_ID_SCI,  0x12,hi_sci_chtimeout_s)
#define HI_CMD_SCI_SET_BLOCKTIMEOUT   _IOW(HI_ID_SCI,  0x13,hi_sci_blktimeout_s)
#define HI_CMD_SCI_SET_TXRETRY        _IOW(HI_ID_SCI,  0x14,hi_sci_txretry_s)
#define HI_CMD_SCI_PRI_REG            _IO(HI_ID_SCI,   0x15)  

/*************************** Structure Definition ****************************/
/**SCI IOCTL SCI函数ID号*/
typedef enum
{
    HI_ID_SCI = 0x45
} HI_MOD_ID_E;  

/**SCICLK引脚输出配置*/
typedef enum
{
    HI_SCI_CLK_MODE_CMOS,                   /**<CMOS输出*/
    HI_SCI_CLK_MODE_OD,                     /**<OD输出*/
    HI_SCI_CLK_MODE_BUTT
}HI_SCI_CLK_MODE_E;

/** SCI 端口 */
typedef enum
{
    HI_SCI_PORT0,                           /**< SCI端口0*/
    HI_SCI_PORT1,                           /**< SCI端口1*/
    HI_SCI_PORT_BUTT     
}HI_SCI_PORT_E;

/** 智能卡状态 */
typedef enum 
{
    HI_SCI_STATUS_UNINIT,                   /**< SCI未初始化 */
    HI_SCI_STATUS_FIRSTINIT,                /**< SCI初始化过程中*/
    HI_SCI_STATUS_NOCARD,                   /**< 无卡 */
    HI_SCI_STATUS_INACTIVECARD,             /**<卡未完成激活（卡无效） */
    HI_SCI_STATUS_WAITATR,                  /**<等待ATR*/
    HI_SCI_STATUS_READATR,                  /**<正在接收ATR*/
    HI_SCI_STATUS_READY,                    /**<卡可以使用（卡激活） */
    HI_SCI_STATUS_RX,                       /**<卡忙（接收数据中） */
    HI_SCI_STATUS_TX                        /**<卡忙（发送数据中） */
} HI_SCI_STATUS_E;

/** SCI 协议 */
typedef enum
{
    HI_SCI_PROTOCOL_T0,                     /**<7816 T0 协议 */
    HI_SCI_PROTOCOL_T1,                     /**<7816 T1 协议 */
    HI_SCI_PROTOCOL_T14 ,                   /**<7816 T14 协议 */
    HI_SCI_PROTOCOL_BUTT
}HI_SCI_PROTOCOL_E;

/**SCI有效电平*/
typedef enum
{
    HI_SCI_LEVEL_LOW,                       /**<低电平有效 */
    HI_SCI_LEVEL_HIGH,                      /**<高电平有效 */
    HI_SCI_LEVEL_BUTT      
}HI_SCI_LEVEL_E ;

/**SCI复位类型 */
typedef enum
{
    HI_SCI_COLD,                            /**<冷复位*/   
    HI_SCI_WARM                             /**<暖复位*/ 
}HI_SCI_RESET_E;

/**SCI系统参数 */
typedef struct
{
    HI_SCI_PORT_E     e_sciport;            /**<SCI 端口号 */
    HI_SCI_PROTOCOL_E e_protocol;           /**<使用的协议类型 */
    uint              ui_clkrate;           /**<时钟率转换因子  */
    uint              ui_bitrate;           /**<比特率转换因子 */
    uint              ui_Fi;                /**<ATR 返回的Fi值 */
    uint              ui_Di;                /**<ATR 返回的Di值 */
    uint              ui_chguard;           /**<字符间保护时间间隔(ETU)*/
    uint              ui_chtimeout;         /**<T0 或T1的字符超时时间*/
    uint              ui_blktimeout;        /**<T1的块超时时间*/
    uint              ui_tx_retry;          /**<发送重试次数*/
}hi_sci_params_s;

/**SCI 基本配置结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    HI_SCI_PROTOCOL_E   e_protocol;         /**<使用的协议类型 */
    uint                ui_frequency;       /**<频率值(KHz) */
}hi_sci_conf_s;


/**SCI RESET结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    HI_SCI_RESET_E      e_resettype;        /**<SCI复位类型*/         
}hi_sci_reset_s;

/**SCI ATR结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    uchar               *puc_atrbuf;        /**<存储ATR的指针 */
    uint                ui_bufsize;         /**<存储ATR的Buff大小 */
    uchar               ui_datalen;         /**<实际获得的ATR数据长度 */
}hi_sci_atr_s;

/**SCI 状态结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    HI_SCI_STATUS_E     e_scistatus;        /**<智能卡状态 */
}hi_sci_status_s;

/**SCI 发送接收数据结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    uchar               *puc_databuf;       /**<发送/接受的数据指针 */  
    uint                ui_bufsize;         /**<发送/接受的Buff大小 */ 
    uint                ui_datalen;         /**<实际发送/接受的数据长度 */
    uint                ui_timeoutms;       /**<超时时间(ms) */
}hi_sci_data_s;

/**SCI 高低电平配置结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    HI_SCI_LEVEL_E      e_level;            /**SCI有效电平*/
}hi_sci_level_s;

/**SCI 时钟模式配置结构体 */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI 端口号 */
    HI_SCI_CLK_MODE_E   e_clkmode;          /**SCI时钟引脚输出配置*/  
}hi_sci_clk_s;


/**SCI 波特率配置结构体 */
/*BAUD = FREQUENCE*BITRATE/CLKRATE*/
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI 端口号 */
    uint               ui_clkrate;          /**<时钟率转换因子 */
    uint               ui_bitrate;          /**<比特率转换因子 */  
}hi_sci_baud_s;

/**SCI 字符保护配置结构体 */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI 端口号 */
    uint               ui_chguard;          /**字符间保护时间间隔(ETU) */ 
}hi_sci_chguard_s;

/**SCI PPS配置结构体 */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI 端口号 */
    uchar              auc_send[6];         /**<PPS发送Buf */
    uchar              auc_recv[6];         /**<PPS接受Buf */
    uint               ui_sendlen;          /**<PPS发送长度(0表示使用ATR) */ 
    uint               ui_recvlen;          /**<PPS接受长度*/
    uint               ui_timeout;          /**<超时时间(ms)*/
}hi_sci_pps_s;

/**SCI 接收字符间隔超时配置结构体 */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI 端口号 */ 
    HI_SCI_PROTOCOL_E  e_protocol;          /**<使用的协议类型 */
    uint               ui_chtimeout;        /**<超时时间*/
}hi_sci_chtimeout_s;

/**SCI 接收块间隔超时配置结构体 */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI 端口号 */ 
    uint               ui_blktimeout;       /**<超时时间*/ 
}hi_sci_blktimeout_s;

/**SCI 发送重试次数配置结构体 */
typedef struct
{
    HI_SCI_PORT_E     e_sciport;           /**<SCI 端口号 */ 
    uint              ui_tx_retry;         /**<发送重试次数 */ 
}hi_sci_txretry_s; 

#endif

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of __cplusplus */