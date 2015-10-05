/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_slic.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : D2012_06_01

******************************************************************************/

#ifndef __HI_SLIC_H__
#define __HI_SLIC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct
{
    hi_uint32 ui_port_idx;
    hi_uint32 ui_event_type;
    hi_uchar8 auc_event[HI_SLIC_REPORT_EVENT_MAXBYTE];
}hi_slic_reportevent_s;

typedef enum
{
    HI_SLIC_CTRL_SPI_E,   /* SPI控制 */
    HI_SLIC_CTRL_ZSI_E    /* ZSI控制 */
}hi_slic_ctrl_mode_e;

typedef enum
{
    HI_SLIC_EVENT_OFFHOOK=0,            /* off hook*/
    HI_SLIC_EVENT_ONHOOK,               /* on hook */
    HI_SLIC_EVENT_FLASH,                /* flash */
    HI_SLIC_EVENT_RING_CAD,             /* ring cadence */
    HI_SLIC_EVENT_BOTTOM,
    HI_SLIC_EVENT_NONE=0xFFFFFFFF,
}hi_slic_report_event_e;

extern hi_uint32 hi_slic_init( hi_uint32 ui_chip_num, hi_uint32 ui_port_per_chip, hi_slic_ctrl_mode_e en_ctrl_mode );
extern hi_uint32 hi_slic_connect_timeslot (hi_uint32 ui_port_idx, hi_uint32 ui_timeslot);
extern hi_uint32 hi_slic_disconnect_timeslot(hi_uint32 ui_port_idx);

extern hi_uint32 hi_slic_send_ring(hi_uint32 ui_port_idx, hi_void *p_ring_profile );
extern hi_uint32 hi_slic_stop_ring(hi_uint32 ui_port_idx);

extern hi_uint32 hi_slic_set_flashtime (hi_uint32 ui_port_idx, hi_uint32 ui_flash_up_limit, hi_uint32 ui_flash_down_limit);
extern hi_uint32 hi_slic_set_gain (hi_uint32 ui_port_idx, hi_int32 i_tx_gain, hi_int32 i_rx_gain);

extern hi_uint32 hi_slic_send_howl(hi_uint32 ui_port_idx, hi_void *p_howl_profile );
extern hi_uint32 hi_slic_stop_howl(hi_uint32 ui_port_idx);

extern hi_uint32 hi_slic_set_impedence (hi_uint32 ui_chip_idx, hi_void *p_impedence_profile);

extern hi_uint32 hi_slic_eventinit( hi_void );
extern hi_uint32 hi_slic_eventrecv( hi_void *pv_data, hi_int32 i_len );
extern hi_uint32 hi_slic_eventexit( hi_void );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_SLIC_H__ */

