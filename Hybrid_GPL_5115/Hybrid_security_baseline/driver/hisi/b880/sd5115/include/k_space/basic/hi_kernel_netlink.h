/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_netlink.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_20
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_NETLINK_H___
#define __HI_KERNEL_NETLINK_H___

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_NETLINK_GENLIDGENERATE       0
#define HI_KERNEL_NETLINK_GENLNAME             "hi_netlink"
#define HI_KERNEL_NETLINK_GENLVERSION          0x1

typedef enum 
{
    HI_KERNEL_NETLINK_CMD_UNSPEC_E = 0,
    HI_KERNEL_NETLINK_CMD_HELLO_E,		 /* user->kernel */
    HI_KERNEL_NETLINK_CMD_DUMP_E,		 /* user->kernel */
    HI_KERNEL_NETLINK_CMD_UPMSG_E,         /* kernel->user */
    HI_KERNEL_NETLINK_CMD_DOWNMSG_E,       /* user->kernel */
    HI_KERNEL_NETLINK_CMD_BUTT_E,
}hi_kernel_netlink_cmd_e;

typedef enum 
{
    HI_KERNEL_NETLINK_TYPE_UNSPEC_E = 0,
    HI_KERNEL_NETLINK_TYPE_HELLO_E,
    HI_KERNEL_NETLINK_TYPE_HLP_E,
    HI_KERNEL_NETLINK_TYPE_EXIT_E,
    HI_KERNEL_NETLINK_TYPE_BUTT_E,
}hi_kernel_netlink_type_e;

extern hi_uint32 hi_kernel_netlink_init(hi_void);
extern hi_uint32 hi_kernel_netlink_exit( hi_void );
extern hi_uint32 hi_kernel_netlink_senddata(hi_uint32 ui_srcmodule, hi_void *data, hi_uint32 ui_len);
extern hi_uint32 hi_kernel_netlink_dump( hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_NETLINK_H___ */
