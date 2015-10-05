/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_cmdtype.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_11_08

******************************************************************************/
#ifndef __HI_CMDTYPE_H__
#define __HI_CMDTYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_uint32 hi_ifm_regapi( hi_uchar8 *puc_funcname, HI_FUNCCALLBACK_EXT *pv_callback );
extern hi_uint32 hi_ifm_regso( hi_uchar8 *puc_funcname, HI_FUNCCALLBACK_EXT *pv_callback );
extern hi_uint32 hi_kernel_hlp_regko( hi_uchar8 *puc_funcname, hi_void* pv_callback );
extern hi_uint32 hi_kernel_hlp_unregko( hi_uchar8 *puc_funcname );

/******************************************************************************/
typedef struct {
    hi_void   *pv_callback;//prototype HI_FUNCCALLBACK_EXT
    hi_uchar8  uc_funcname[HI_FUNCNAME_MAXLEN];
}hi_cmdtype_s;

#define HI_CMDTYPE_REGENTRY( func )    { func, #func }

#ifdef CONFIG_HI_KERNEL_MODULE

#define HI_CMDTYPE_INVALIDARG(inmaxlen) \
    if( ( HI_NULL == pv_data ) || ( ui_inlen   != inmaxlen ) || ( HI_NULL == pui_outlen ))\
    {\
        hi_kernel_printk("\r\n[ERROR INPUT NULL PTR %s %d]!!!!!!!!!!!!!!!!!!\r\n",__FILE__,__LINE__);\
        return HI_RET_INVALID_PARA;\
    }

#define HI_CMDTYPE_REGKO(array)  { \
    hi_uint32 ui_cnt,ui_ret; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        ui_ret = hi_kernel_hlp_regko(array[ui_cnt].uc_funcname, array[ui_cnt].pv_callback ); \
        if ( HI_RET_SUCC != ui_ret )\
        {\
            hi_kernel_printk("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,array[ui_cnt].uc_funcname);\
            return ui_ret; \
        }\
    } \
}

#define HI_CMDTYPE_UNREGKO(array){ \
    hi_uint32 ui_cnt; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        hi_kernel_hlp_unregko(array[ui_cnt].uc_funcname); \
    } \
}
    
#else // userspace

#define HI_CMDTYPE_INVALIDARG(inmaxlen) \
    if( ( HI_NULL == pv_data ) || ( ui_inlen   != inmaxlen ) || ( HI_NULL == pui_outlen ))\
    {\
        hi_os_printf("\r\n[ERROR INPUT NULL PTR %s %d]!!!!!!!!!!!!!!!!!!\r\n",__FILE__,__LINE__);\
        return HI_RET_INVALID_PARA;\
    }

#define HI_CMDTYPE_REGSO(array)  { \
    hi_uint32 ui_cnt,ui_ret; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        ui_ret = hi_ifm_regso(array[ui_cnt].uc_funcname, array[ui_cnt].pv_callback ); \
        if ( HI_RET_SUCC != ui_ret )\
        {\
            hi_os_printf("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,array[ui_cnt].uc_funcname);\
            return ui_ret; \
        }\
    } \
}

#define HI_CMDTYPE_REGAPI(array)  { \
    hi_uint32 ui_cnt,ui_ret; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        ui_ret = hi_ifm_regapi(array[ui_cnt].uc_funcname, array[ui_cnt].pv_callback ); \
        if ( HI_RET_SUCC != ui_ret )\
        {\
            hi_os_printf("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,array[ui_cnt].uc_funcname);\
            return ui_ret; \
        }\
    } \
}

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_CMDTYPE_H__ */
