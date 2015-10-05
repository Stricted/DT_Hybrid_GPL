/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾
 
******************************************************************************
  �ļ�����: hi_drv_hw.h
  ��������: ����ͷ�ļ�
  �汾����: V1.0

  ��������: 2009��6��1��
  ��������: ��˼����������    

  �޸ļ�¼: 
******************************************************************************/
#ifndef __HI_DRV_HW_H__
#define __HI_DRV_HW_H__

#include "base_type_define.h"
#include "hi_type_define.h"

#include "hi_drv_common.h"

#define  HI_HW_MAX_CHANNEL              4

/*�ӿ�ģʽѡ��HW/ZSIģʽ*/
typedef enum
{
    HI_HW_MODE_E = 0,              /*��ΪHW�ӿ�ʹ��*/
    HI_ZSI_MODE_E,                    /*��ΪZSI�ӿ�ʹ��*/
} HI_INTERFACE_MODE_E;

/*ʱ��ģʽ*/
typedef enum
{
    HI_HW_CLK_INPUT_E = 0,              /*ʱ������ģʽ,������ʱ��,֡ͷ�����ⲿ����*/
    HI_HW_CLK_OUTPUT_E,                 /*ʱ�����ģʽ,������ʱ��,֡ͷ���ڲ�����,������ⲿ*/
} HI_HW_CLK_MODE_E;

/*֡ͬ��ʱ������*/
typedef enum
{
    HI_HW_FS_8K_E = 0,                     
    HI_HW_FS_16K_E,
} HI_HW_FS_RATE_E;

/*֡ͬ��ʱ������*/
typedef enum
{
    HI_HW_FS_WB_NORMAL_MODE_E = 0,                     
    HI_HW_FS_WB_8K_NODE_E,
} HI_FS_WB_8K_MODE_E;

/*����ʱ������*/
typedef enum
{
    HI_HW_CLK_8M_E = 0,                    
    HI_HW_CLK_4M_E,                           
    HI_HW_CLK_2M_E,                           
    HI_HW_CLK_1M_E,                           
    HI_HW_CLK_512K_E,                           
} HI_HW_CLK_RATE_E;

/*PCMλ��*/
typedef enum
{
    HI_HW_PCM_8BIT_WIDE_E = 0,                     
    HI_HW_PCM_16BIT_WIDE_E,                           
} HI_HW_PCM_WIDE_E;

/*A U��ģʽ*/
typedef enum
{
    HI_HW_ALAW_E = 0,                      
    HI_HW_ULAW_E,                         
} HI_HW_AULAW_E;

/*�ж�ģʽ*/
typedef enum
{
    HI_HW_10MS_INT_E = 0,              /*10�����ж�ģʽ,DSP��Ҫ��ÿ���ж�����ѭ�������4��HWͨ��*/ 
    HI_HW_2MS5_INT_E,                  /*2.5�����ж�ģʽ,DSP��Ҫ��ÿ���ж�����ѭ�������1��HWͨ��*/              
} HI_HW_INT_MDOE_E;


/*ͨ����������*/
typedef struct
{   
    HI_BOOL_E em_enable;               /*ͨ��ʹ�ܣ�����ֹʱ������ͨ�������̬*/
    /*HI_BOOL_E em_g711_en;  */        /*711����͸��ģʽʹ��*/
    HI_BOOL_E em_ip_up_loop_en;        /*IP�����л���ʹ��*/
    HI_BOOL_E em_tdm_up_loop_en;       /*TDM�����л���ʹ��*/
    HI_BOOL_E em_ip_dn_loop_en;        /*IP�����л���ʹ��*/
    HI_BOOL_E em_tdm_dn_loop_en;       /*TDM�����л���ʹ��*/   
    uchar uc_jbdeq;                    /*2��Jitter Buffer������� ��λΪ125US / 62.5 US   0 to 320*/
    HI_BOOL_E em_fifo_cnt_en;          /*ͨ��FIFOͳ��ʹ��*/              
} hi_hw_channel_attr_s;

typedef  struct
{
    uint                ui_index; /*HW/ZSI����,����ѡ����HW0/HW1,������ZSI0/ZSI1��Ŀǰֻ��5115H-ASIC��Ч*/
    HI_INTERFACE_MODE_E em_interface_mode;  /*�ӿ�ģʽѡ��HW/ZSIģʽ��Ĭ��ΪHW�ӿ�*/
    HI_BOOL_E em_enable;               /*HWģ�鹤��ʹ��*/
    HI_HW_INT_MDOE_E em_int_mode;      /*�ж�ģʽ,����DSP�ж�*/    
    HI_HW_CLK_MODE_E em_clk_mode;      /*ʱ��ģʽ*/

    /*֡ͬ������FRAM SYNC*/
    HI_HW_FS_RATE_E em_fs_rate;        /*֡ͬ������*/    
    HI_LEVEL_E em_fs_level;            /*֡ͬ���ź���Ч��ƽ*/ 
    HI_EDGE_E em_fs_sample_edge;       /*֡ͬ���źŵĲ�����,Ĭ��Ϊ�½���*/
    
    HI_BOOL_E em_fas_en;               /*֡ʧ�����ʹ��*/
    uchar uc_flos_thr;                 /*֡ʧ�����ޣ����о���ͬ����ʧ���Ĵ���*/
    uchar uc_resyc_thr;                /*֡��ͬ�����ޣ����о���ʧ�����ָ�ͬ���Ĵ���*/

    /*����ʱ������DATA CLK*/
    HI_HW_CLK_RATE_E em_clk_rate;      /*ʱ������*/    
    HI_EDGE_E em_tx_edge;              /*���ݷ����źŲ�������/��,Ĭ��Ϊ������*/
    HI_EDGE_E em_rx_edge;              /*���ݽ����źŲ�������/��,Ĭ��Ϊ�½���*/   

    /*�������� DATA_TX   DATA_RX*/   
    HI_HW_PCM_WIDE_E em_pcm_wide;      /*PCMλ��*/
    HI_HW_AULAW_E em_aulaw_mode;       /*g711 A U��ģʽ*/ 
    HI_BOOL_E em_codec_rev_en;         /*CODEC�����תʹ��*/
    uchar uc_chnnel_tdm_rxalign ;      /*����ʱ϶ƫ��*/
    uchar uc_chnnel_tdm_txalign  ;     /*����ʱ϶ƫ��*/

    HI_FS_WB_8K_MODE_E em_fs_wb_8k_mode;

    /*ͨ������*/
    hi_hw_channel_attr_s  st_channel_attr[HI_HW_MAX_CHANNEL];
} hi_hw_attr_s;



/*״̬��ѯ*/
typedef struct
{   
    uint    ui_xxx;                     /*xxx*/
} hi_hw_sta_s;

/*ͳ�ƣ��쳣��¼��*/
typedef struct
{
   uint ui_invalid_parameter_cnt;       /*��Ч��������*/
   uint ui_internal_err;    

   /*Ӳ��ͳ��*/
   uint ui_fifo_full[HI_HW_MAX_CHANNEL];    /*ͨ��FIFO��ͳ��*/   
   uint ui_fifo_emputy[HI_HW_MAX_CHANNEL];  /*ͨ��FIFO��ͳ��*/      

   /*�������汾�������Ƿ�ʵ��,�����ڲ�1�붨ʱ��ѯͳ��*/
   uint ui_flos_alm_cnt;                    /*֡ʧ��ͳ��*/
} hi_hw_cnt_s;

/*ʱ�����*/
typedef struct
{
   uint ui_time_125us;                  /*125΢�����*/
   uint ui_time_ms;                     /*1�������*/   
   uint ui_time_10ms;                   /*10�������*/   
   uint ui_time_s;                      /*1�����*/
} hi_hw_time_s;


/*�������Բ���*/
extern uint hi_kernel_hw_attr_get(hi_hw_attr_s *pst_attr);
extern uint hi_kernel_hw_attr_set(hi_hw_attr_s *pst_attr);

/*ʱ���������*/
extern uint hi_kernel_hw_time_get(hi_hw_time_s  *pst_time);

/*״̬��Ϣ����*/
extern uint hi_kernel_hw_sta_get(hi_hw_sta_s *pst_sta);

/*ͳ����Ϣ����*/
extern uint hi_kernel_hw_cnt_get(hi_hw_cnt_s *pst_cnt); 

#endif /* __HI_DRV_XXX_H__ */

