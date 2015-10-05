/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_component.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_COMPONENT_H__
#define __HI_COMPONENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define HI_READ_PHY_MEM_MAX_LEN    1000

extern hi_int32 hi_os_netlink_init( hi_int32 *pi_sock, hi_int32 ui_destmodule );
extern hi_int32 hi_os_netlink_exit( hi_int32 i_socket, hi_int32 ui_destmodule );
extern hi_int32 hi_os_netlink_recv( hi_int32 i_sock, hi_void *pv_buff, hi_int32 i_len, hi_int32 i_flags );
extern hi_int32 hi_board_read_phymem( hi_uint32 ui_addr, hi_uint32 ui_len, hi_uchar8 *puc_buf );
extern hi_int32 hi_board_write_phymem( hi_uint32 ui_addr, hi_uint32 ui_value );


#ifdef __cplusplus
}
#endif


#endif  /* __HI_COMPONENT_H__ */