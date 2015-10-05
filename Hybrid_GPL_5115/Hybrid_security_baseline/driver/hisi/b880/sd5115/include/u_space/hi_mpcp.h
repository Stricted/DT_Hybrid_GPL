/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_mpcp.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_MPCP_H__
#define __HI_MPCP_H__


#ifdef __cplusplus
extern "C" {
#endif

#define HI_MPCP_MAX_LLID_NUM      		8
typedef struct
{
   hi_uchar8   uc_llidindex;
   hi_uchar8   uc_reason;
   hi_ushort16 us_res;
   hi_uint32   ui_timeout;
}hi_mpcp_silence_s;

extern hi_int32 hi_mpcp_start( hi_uchar8 *puc_mac );
extern hi_int32 hi_mpcp_stop( hi_void );
extern hi_int32 hi_mpcp_get_lld0_mac( hi_uchar8 *puc_mac);
extern hi_int32 hi_mpcp_set_lld0_mac( hi_uchar8 *puc_mac );
extern hi_int32 hi_mpcp_get_status( hi_uint32 *pui_status );
extern hi_int32 hi_mpcp_status_callback( HI_FUNCCALLBACK pf_mpcpcallback );
extern hi_int32 hi_mpcp_init_sampleoam( hi_void );
extern hi_int32 hi_mpcp_set_silence_time( hi_uchar8 uc_llidindex, hi_uint32 ui_timeout, hi_uchar8 uc_reaseon );


#ifdef __cplusplus
}
#endif


#endif  /* __HI_MPCP_H__ */
