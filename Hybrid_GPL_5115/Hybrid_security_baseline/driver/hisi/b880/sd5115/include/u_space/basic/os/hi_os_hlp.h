/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_hlp.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_24
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_HLP_H__
#define __HI_OS_HLP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define  HI_HLP_DEV_HLP_PATH        "/dev/hi_hlp"

extern hi_uint32 hi_os_hlp_sendioctl( hi_uchar8 *puc_funcname, hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );
extern hi_uint32 hi_os_hlp_sendsimple( hi_uchar8 *puc_funcname, hi_void *pv_data, hi_uint32 ui_inlen );
extern hi_uint32 hi_os_sendioctl( hi_char8 *pc_devname, hi_uint32 ui_cmd, hi_void *pv_data );


#define HI_HLP_EXEC( puc_funcname,pv_data,ui_inlen,pui_outlen ) {\
                                    hi_uint32 ui_ret;\
                                    ui_ret = hi_os_hlp_sendioctl( (hi_uchar8*)puc_funcname,(hi_void*)pv_data,(hi_uint32)ui_inlen,(hi_uint32*)pui_outlen );\
                                    if ( HI_RET_SUCC != ui_ret )\
                                    {\
                                        hi_os_printf("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,puc_funcname);\
                                        return ui_ret;\
                                    }\
                                }

#define HI_HLP_EXEC_SIMPLE( puc_funcname,pv_data,ui_inlen ) {\
                                    hi_uint32 ui_ret;\
                                    ui_ret = hi_os_hlp_sendsimple( (hi_uchar8*)puc_funcname,(hi_void*)pv_data,(hi_uint32)ui_inlen );\
                                    if ( HI_RET_SUCC != ui_ret )\
                                    {\
                                        hi_os_printf("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,puc_funcname);\
                                        return ui_ret;\
                                    }\
                                }
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_HLP_H__ */
