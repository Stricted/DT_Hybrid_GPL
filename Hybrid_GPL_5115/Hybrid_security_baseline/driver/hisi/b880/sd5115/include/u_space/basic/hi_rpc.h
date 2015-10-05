/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_rpc.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_07
  最近修改   :

******************************************************************************/
#ifndef __HI_RPC_H__
#define __HI_RPC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_RPC_MESSAGE_LEN              2048

/************************************************************************
消息键值不能够超过65535即必须为正数,
预留0x2000~0x2100知名服务,顺序分配0x2100~0x3000
*************************************************************************/
#define HI_RPC_MSGKEY_SYSTEM_BASE       0x2000

#define HI_RPC_MSGKEY_SYSTEM_KNOW       0x2100
#define HI_RPC_MSGKEY_SYSTEM_MAX        8192

typedef enum {
    HI_RPC_MSGKEY_SYSTEM_LOG_SERVER_E           = ( HI_RPC_MSGKEY_SYSTEM_BASE | 0x001 ),
    HI_RPC_MSGKEY_SYSTEM_IFM_SERVER_E           = ( HI_RPC_MSGKEY_SYSTEM_BASE | 0x002 ),
    HI_RPC_MSGKEY_SYSTEM_IFM_CLIENT_E           = ( HI_RPC_MSGKEY_SYSTEM_BASE | 0x003 ),
}hi_rpc_msgkey_system_e;

/*************************************************************************/
typedef enum {
    HI_RPC_MSGTYPE_ASYNMSG_E = 1,
    HI_RPC_MSGTYPE_SYNMSG_E,
    HI_RPC_MSGTYPE_BUTT_E,
}hi_rpc_msgtype_e;

typedef struct {
    hi_uint32   ui_msgtype;          /*本地消息类型同步或者异步*/
    hi_uint32   ui_sndmsgkey;         /*远端消息KEY*/
    hi_uint32   ui_rcvmsgkey;         /*本地消息KEY*/
    hi_uint32   ui_ret;             /*远端函数返回值*/
    hi_uint32   ui_len;             /*发送数据长度或远端返回数据长度*/
    hi_uchar8   uc_data[HI_RPC_MESSAGE_LEN];
}hi_rpc_data_s;

/*************************************************************************/
extern hi_uint32 hi_rpc_msgkey( hi_void );
extern hi_uint32 hi_rpc_unmsgkey( hi_uint32 ui_msgkey );
extern hi_uint32 hi_rpc_regcallback( HI_FUNCCALLBACK *pv_asyncallback, HI_FUNCCALLBACK_EXT *pv_syncallback );
extern hi_uint32 hi_rpc_init( hi_uint32 ui_msgkey );
extern hi_uint32 hi_rpc_exit( hi_void );
extern hi_uint32 hi_rpc_sendto( hi_uint32 ui_sndmsgkey, hi_void *pv_data,hi_uint32 ui_inlen );
extern hi_uint32 hi_rpc_sendrpc( hi_uint32 ui_sndmsgkey, hi_uint32  ui_rcvmsgkey,
                                 hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );
extern hi_uint32 hi_rpc_recv( hi_uint32 ui_rcvmsgkey, hi_void *pv_data, hi_uint32 *pui_inlen );
extern hi_uint32 hi_rpc_getrpcpid( hi_void );
extern hi_uint32 hi_rpc_getmsgkey( hi_void );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_RPC_H__ */
