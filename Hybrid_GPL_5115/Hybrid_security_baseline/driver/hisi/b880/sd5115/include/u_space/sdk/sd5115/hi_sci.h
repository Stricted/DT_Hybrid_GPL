/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾
 
******************************************************************************
  �ļ�����: hi_sci.h
  ��������: ��˼sci�����ӿ�
  �汾����: V1.0

  ��������: 2012��10��24��
  ��������: l00220454

  �޸ļ�¼: 
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef __HI_SCI_H__
#define __HI_SCI_H__

#include "base_type_define.h"

/***************************** ioctl����궨�� ********************************/
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

/******************************** ioctl���� **********************************/
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
/**SCI IOCTL SCI����ID��*/
typedef enum
{
    HI_ID_SCI = 0x45
} HI_MOD_ID_E;  

/**SCICLK�����������*/
typedef enum
{
    HI_SCI_CLK_MODE_CMOS,                   /**<CMOS���*/
    HI_SCI_CLK_MODE_OD,                     /**<OD���*/
    HI_SCI_CLK_MODE_BUTT
}HI_SCI_CLK_MODE_E;

/** SCI �˿� */
typedef enum
{
    HI_SCI_PORT0,                           /**< SCI�˿�0*/
    HI_SCI_PORT1,                           /**< SCI�˿�1*/
    HI_SCI_PORT_BUTT     
}HI_SCI_PORT_E;

/** ���ܿ�״̬ */
typedef enum 
{
    HI_SCI_STATUS_UNINIT,                   /**< SCIδ��ʼ�� */
    HI_SCI_STATUS_FIRSTINIT,                /**< SCI��ʼ��������*/
    HI_SCI_STATUS_NOCARD,                   /**< �޿� */
    HI_SCI_STATUS_INACTIVECARD,             /**<��δ��ɼ������Ч�� */
    HI_SCI_STATUS_WAITATR,                  /**<�ȴ�ATR*/
    HI_SCI_STATUS_READATR,                  /**<���ڽ���ATR*/
    HI_SCI_STATUS_READY,                    /**<������ʹ�ã������ */
    HI_SCI_STATUS_RX,                       /**<��æ�����������У� */
    HI_SCI_STATUS_TX                        /**<��æ�����������У� */
} HI_SCI_STATUS_E;

/** SCI Э�� */
typedef enum
{
    HI_SCI_PROTOCOL_T0,                     /**<7816 T0 Э�� */
    HI_SCI_PROTOCOL_T1,                     /**<7816 T1 Э�� */
    HI_SCI_PROTOCOL_T14 ,                   /**<7816 T14 Э�� */
    HI_SCI_PROTOCOL_BUTT
}HI_SCI_PROTOCOL_E;

/**SCI��Ч��ƽ*/
typedef enum
{
    HI_SCI_LEVEL_LOW,                       /**<�͵�ƽ��Ч */
    HI_SCI_LEVEL_HIGH,                      /**<�ߵ�ƽ��Ч */
    HI_SCI_LEVEL_BUTT      
}HI_SCI_LEVEL_E ;

/**SCI��λ���� */
typedef enum
{
    HI_SCI_COLD,                            /**<�临λ*/   
    HI_SCI_WARM                             /**<ů��λ*/ 
}HI_SCI_RESET_E;

/**SCIϵͳ���� */
typedef struct
{
    HI_SCI_PORT_E     e_sciport;            /**<SCI �˿ں� */
    HI_SCI_PROTOCOL_E e_protocol;           /**<ʹ�õ�Э������ */
    uint              ui_clkrate;           /**<ʱ����ת������  */
    uint              ui_bitrate;           /**<������ת������ */
    uint              ui_Fi;                /**<ATR ���ص�Fiֵ */
    uint              ui_Di;                /**<ATR ���ص�Diֵ */
    uint              ui_chguard;           /**<�ַ��䱣��ʱ����(ETU)*/
    uint              ui_chtimeout;         /**<T0 ��T1���ַ���ʱʱ��*/
    uint              ui_blktimeout;        /**<T1�Ŀ鳬ʱʱ��*/
    uint              ui_tx_retry;          /**<�������Դ���*/
}hi_sci_params_s;

/**SCI �������ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    HI_SCI_PROTOCOL_E   e_protocol;         /**<ʹ�õ�Э������ */
    uint                ui_frequency;       /**<Ƶ��ֵ(KHz) */
}hi_sci_conf_s;


/**SCI RESET�ṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    HI_SCI_RESET_E      e_resettype;        /**<SCI��λ����*/         
}hi_sci_reset_s;

/**SCI ATR�ṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    uchar               *puc_atrbuf;        /**<�洢ATR��ָ�� */
    uint                ui_bufsize;         /**<�洢ATR��Buff��С */
    uchar               ui_datalen;         /**<ʵ�ʻ�õ�ATR���ݳ��� */
}hi_sci_atr_s;

/**SCI ״̬�ṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    HI_SCI_STATUS_E     e_scistatus;        /**<���ܿ�״̬ */
}hi_sci_status_s;

/**SCI ���ͽ������ݽṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    uchar               *puc_databuf;       /**<����/���ܵ�����ָ�� */  
    uint                ui_bufsize;         /**<����/���ܵ�Buff��С */ 
    uint                ui_datalen;         /**<ʵ�ʷ���/���ܵ����ݳ��� */
    uint                ui_timeoutms;       /**<��ʱʱ��(ms) */
}hi_sci_data_s;

/**SCI �ߵ͵�ƽ���ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    HI_SCI_LEVEL_E      e_level;            /**SCI��Ч��ƽ*/
}hi_sci_level_s;

/**SCI ʱ��ģʽ���ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E       e_sciport;          /**<SCI �˿ں� */
    HI_SCI_CLK_MODE_E   e_clkmode;          /**SCIʱ�������������*/  
}hi_sci_clk_s;


/**SCI ���������ýṹ�� */
/*BAUD = FREQUENCE*BITRATE/CLKRATE*/
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI �˿ں� */
    uint               ui_clkrate;          /**<ʱ����ת������ */
    uint               ui_bitrate;          /**<������ת������ */  
}hi_sci_baud_s;

/**SCI �ַ��������ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI �˿ں� */
    uint               ui_chguard;          /**�ַ��䱣��ʱ����(ETU) */ 
}hi_sci_chguard_s;

/**SCI PPS���ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI �˿ں� */
    uchar              auc_send[6];         /**<PPS����Buf */
    uchar              auc_recv[6];         /**<PPS����Buf */
    uint               ui_sendlen;          /**<PPS���ͳ���(0��ʾʹ��ATR) */ 
    uint               ui_recvlen;          /**<PPS���ܳ���*/
    uint               ui_timeout;          /**<��ʱʱ��(ms)*/
}hi_sci_pps_s;

/**SCI �����ַ������ʱ���ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI �˿ں� */ 
    HI_SCI_PROTOCOL_E  e_protocol;          /**<ʹ�õ�Э������ */
    uint               ui_chtimeout;        /**<��ʱʱ��*/
}hi_sci_chtimeout_s;

/**SCI ���տ�����ʱ���ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E      e_sciport;           /**<SCI �˿ں� */ 
    uint               ui_blktimeout;       /**<��ʱʱ��*/ 
}hi_sci_blktimeout_s;

/**SCI �������Դ������ýṹ�� */
typedef struct
{
    HI_SCI_PORT_E     e_sciport;           /**<SCI �˿ں� */ 
    uint              ui_tx_retry;         /**<�������Դ��� */ 
}hi_sci_txretry_s; 

#endif

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of __cplusplus */