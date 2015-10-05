/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_netlink.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_NETLINK_H__
#define __HI_OS_NETLINK_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Maximum size of response requested or message sent */
#define NETLINK_MAX_MSG_SIZE    (1400)

typedef enum {
    HI_NETLINK_CMD_UNSPEC = 0,
    HI_NETLINK_CMD_HELLO,       /* user->kernel */
    HI_NETLINK_CMD_DUMP,        /* user->kernel */
    HI_NETLINK_CMD_UPMSG,       /* kernel->user */
    HI_NETLINK_CMD_DOWNMSG,     /* user->kernel */
    HI_NETLINK_CMD_BUTT
}hi_netlink_cmd_e;

typedef enum {
    HI_NETLINK_RESCMD_UNSPEC = 0,
    HI_NETLINK_RESCMD_HELLO,   /* HI_NETLINK_RESCMD_HELLO just one u32 data, is dstmodule */
    HI_NETLINK_RESCMD_HLP,
    HI_NETLINK_RESCMD_EXIT,    
    HI_NETLINK_RESCMD_BUTT
} hi_netlink_res_cmd_e;

typedef struct { 
    struct nlmsghdr n;
    struct genlmsghdr g;
    struct nlattr nla;
    hi_char8 uc_buff[NETLINK_MAX_MSG_SIZE];
} hi_netlink_s;

#define NLMSG_ALIGNTO    4
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )

#define GENL_HDRLEN    NLMSG_ALIGN(sizeof(struct genlmsghdr))

/*
 * Generic macros for dealing with netlink sockets. Might be duplicated
 * elsewhere. It is recommended that commercial grade applications use
 * libnl or libnetlink and use the interfaces provided by the library
 */
#define GENLMSG_DATA(glh)       (((char*)NLMSG_DATA(glh) + GENL_HDRLEN))
#define NLA_DATA(na)            (((char*)(na) + NLA_HDRLEN))

extern hi_int32 hi_os_netlink_init( hi_int32 *pi_sock, hi_int32 ui_destmodule );
extern hi_int32 hi_os_netlink_exit( hi_int32 i_socket, hi_int32 ui_destmodule );
extern hi_int32 hi_os_netlink_recv( hi_int32 i_sock, hi_void *pv_buff, hi_int32 i_len, hi_int32 i_flags );

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_NETLINK_H__ */


