/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_log.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_24
  最近修改   :

******************************************************************************/
#ifndef __HI_LOG_H__
#define __HI_LOG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define hi_unmod_systrace(ui_ret, arg1, arg2, arg3, arg4)  \
                        hi_log_systrace( (hi_uint32)HI_SYSBASE_GLB, \
                        (hi_uchar8 *)__FILE__, (hi_uchar8 *)__func__, (hi_uint32)__LINE__, \
                        (hi_uint32)ui_ret, (hi_uint32)arg1, (hi_uint32)arg2, (hi_uint32)arg3, (hi_uint32)arg4)

#define hi_unmod_debug(ui_level,fmt,arg...) \
                        hi_log_debug( (hi_uint32)HI_SYSBASE_GLB, \
                        (hi_uchar8 *)__FILE__, (hi_uchar8 *)__func__,(hi_uint32)__LINE__, \
                        (hi_uint32)ui_level, (hi_uchar8 *)fmt, ##arg )

#define hi_unmod_print(ui_dbglevel,fmt,arg...)  hi_log_print( (hi_uint32)HI_SYSBASE_GLB,(hi_uint32)ui_dbglevel,(hi_uchar8 *)fmt, ##arg )

#define hi_unmod_memdes(ui_dbglevel,puc_src,ui_len,fmt,arg...) do{\
                                           hi_print(HI_SYSBASE_GLB,ui_dbglevel,(hi_uchar8 *)fmt, ##arg);  \
                                           hi_log_printmem(HI_SYSBASE_GLB,ui_dbglevel,(hi_uchar8 *)puc_src,(hi_uint32)ui_len);\
                                           }while(0)

#define hi_systrace(ui_module,ui_ret, arg1, arg2, arg3, arg4)  \
                        hi_log_systrace( (hi_uint32)ui_module, \
                        (hi_uchar8 *)__FILE__, (hi_uchar8 *)__func__, (hi_uint32)__LINE__, \
                        (hi_uint32)ui_ret, (hi_uint32)arg1, (hi_uint32)arg2, (hi_uint32)arg3, (hi_uint32)arg4)

#define hi_debug(ui_module,ui_level,fmt,arg...) \
                        hi_log_debug( (hi_uint32)ui_module, \
                        (hi_char8 *)__FILE__, (hi_uchar8 *)__func__,(hi_uint32)__LINE__, \
                        (hi_uint32)ui_level, (hi_uchar8 *)fmt, ##arg )

#define hi_print(ui_module,ui_dbglevel,fmt,arg...)  hi_log_print( (hi_uint32)ui_module,(hi_uint32)ui_dbglevel,(hi_uchar8 *)fmt, ##arg )

#define hi_log_memdes(ui_module,ui_dbglevel,puc_src,ui_len,fmt,arg...) do{\
                                           hi_print(ui_module,ui_dbglevel,(hi_uchar8 *)fmt, ##arg);  \
                                           hi_log_printmem(ui_module,ui_dbglevel,(hi_uchar8 *)puc_src,(hi_uint32)ui_len);\
                                           }while(0)

#define HI_LOG_GET_DBGGLB(pui_dbglevel)                  hi_log_get_dbgglb(pui_dbglevel);
#define HI_LOG_GET_DBGMODULE(ui_module,pui_dbglevel)     hi_log_get_dbgmodule(ui_module,pui_dbglevel);

/********************************************************/
extern hi_void hi_log_systrace( hi_uint32 ui_module, hi_uchar8 *puc_file, hi_uchar8 *puc_func, hi_uint32 ui_line, 
                            hi_uint32 ui_retcode, hi_uint32 ui_arg1, hi_uint32 ui_arg2,
                            hi_uint32 ui_arg3, hi_uint32 ui_arg4);
extern hi_void hi_log_debug( hi_uint32 ui_module, hi_char8 *puc_file, hi_uchar8 *puc_func,
                                 hi_uint32 ui_line,  hi_uint32 ui_dbglevel, 
                                 hi_uchar8 *uc_fmt, ...);
extern hi_void hi_log_print( hi_uint32 ui_module, hi_uint32 ui_dbglevel,hi_uchar8 *uc_fmt, ...);
/********************************************************/
extern hi_uint32 hi_log_get_dbgglb( hi_uint32 *pui_dbglevel );
extern hi_uint32 hi_log_get_dbgmodule(hi_uint32 ui_module, hi_uint32 *pui_moddbglevel );

extern hi_void hi_log_printmem(hi_uint32 ui_module, hi_uint32 ui_dbglevel, hi_uchar8 *puc_src, hi_uint32 ui_len );
/********************************************************/
extern hi_uint32 hi_log_init( hi_void );
/********************************************************/
extern hi_uint32 hi_logsrv_init( hi_void );
extern hi_uint32 hi_logsrv_exit( hi_void );
extern hi_uint32 hi_logsrv_reginit( hi_void );
extern hi_void hi_logsrv_broadcast( hi_ifm_msg_s *pst_ifmmsg );
/********************************************************/
extern hi_uint32 hi_klog_init( hi_void );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_LOG_H__ */
