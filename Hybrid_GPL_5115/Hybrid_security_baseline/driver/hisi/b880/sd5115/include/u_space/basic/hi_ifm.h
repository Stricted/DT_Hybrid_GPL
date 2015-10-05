/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_ifm.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_16
  最近修改   :

******************************************************************************/
#ifndef __HI_IFM_H__
#define __HI_IFM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* 系统支持最大接口数 */
#define HI_IFM_HASHSIZE              0xFFFF
#define HI_IFM_ITEMSIZE              16
#define HI_IFM_CONFLICTSIZE          1024

typedef enum {
    HI_IFM_MSGTYPE_REG_API_CALLBACK_E = 2012,
    HI_IFM_MSGTYPE_REG_SO_CALLBACK_E,
    HI_IFM_MSGTYPE_REG_KO_CALLBACK_E,
    HI_IFM_MSGTYPE_REG_BROADCAST_CALLBACK_E,
    HI_IFM_MSGTYPE_UNREG_BROADCAST_CALLBACK_E,
    HI_IFM_MSGTYPE_CLI_CMD_E,
    HI_IFM_MSGTYPE_BUTT_E,
}hi_ifm_msgtype_e;

typedef struct {
    hi_uint32       ui_msgtype;
    hi_uint32       ui_msgkey;
    hi_uint32       ui_pid;
    HI_FUNCCALLBACK_EXT  *pv_callback;
    hi_uint32       ui_len;
    hi_uchar8       uc_funcname[HI_FUNCNAME_MAXLEN];
    hi_uchar8       uc_text[4];
}hi_ifm_msg_s;

typedef struct {
    hi_list_head  st_listhead;
    hi_ifm_msg_s  st_ifm_msg;
}hi_ifm_list_s;

/*******************************************************************************************/
extern hi_uint32 hi_ifm_init( hi_void );
extern hi_uint32 hi_ifm_exit( hi_void );
extern hi_uint32 hi_ifm_regbroadcast( HI_FUNCCALLBACK *pv_callback );
extern hi_uint32 hi_ifm_getrcvmsgkey( hi_void );
extern hi_void hi_ifm_init_srv( hi_void );
/*******************************************************************************************/
extern hi_uint32 hi_ifmsrv_init( hi_void );
extern hi_uint32 hi_ifmsrv_sendbroadcast( hi_void *pv_data, hi_uint32 ui_len );
extern hi_uint32 hi_ifmsrv_sendto( hi_ifm_msg_s *pst_ifmmsg,hi_void *pv_data, hi_uint32 ui_len );
/*******************************************************************************************/
extern hi_uint32 hi_ifmcli_exec( hi_uchar8 *puc_funcname, hi_void *pv_data, 
                                 hi_uint32 ui_inlen, hi_uint32 *pui_outlen );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_IFM_H__ */
