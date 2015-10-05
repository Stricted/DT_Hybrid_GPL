/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_def.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_10_27

******************************************************************************/
#ifndef __HI_OMCI_DEF_H__
#define __HI_OMCI_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum 
{
    HI_SUBMODULE_OMCI_LIB         = ( HI_SRCMODULE_APPS_OMCI | 0x00000100),
    HI_SUBMODULE_OMCI_COM         = ( HI_SRCMODULE_APPS_OMCI | 0x00000200),
    HI_SUBMODULE_OMCI_API         = ( HI_SRCMODULE_APPS_OMCI | 0x00000300),
    HI_SUBMODULE_OMCI_ME_ANI      = ( HI_SRCMODULE_APPS_OMCI | 0x00000500),
    HI_SUBMODULE_OMCI_ME_EQUIP    = ( HI_SRCMODULE_APPS_OMCI | 0x00000600),
    HI_SUBMODULE_OMCI_ME_ETH      = ( HI_SRCMODULE_APPS_OMCI | 0x00000700),
    HI_SUBMODULE_OMCI_ME_GEN      = ( HI_SRCMODULE_APPS_OMCI | 0x00000800),
    HI_SUBMODULE_OMCI_ME_LAYER2   = ( HI_SRCMODULE_APPS_OMCI | 0x00000900),
    HI_SUBMODULE_OMCI_ME_LAYER3   = ( HI_SRCMODULE_APPS_OMCI | 0x00000A00),
    HI_SUBMODULE_OMCI_ME_MISC     = ( HI_SRCMODULE_APPS_OMCI | 0x00000B00),
    HI_SUBMODULE_OMCI_ME_TDM      = ( HI_SRCMODULE_APPS_OMCI | 0x00000C00),
    HI_SUBMODULE_OMCI_ME_VOICE    = ( HI_SRCMODULE_APPS_OMCI | 0x00000D00),
}hi_omci_submod_e;

/*数据库callbacktable表项里面的不同回调函数分类,
  目前分为两类，实体动作函数和告警函数*/
typedef enum {
    HI_OMCI_FUNC_ME_ACTION_E = 1,
    HI_OMCI_FUNC_ALARM_E ,
    HI_OMCI_FUNC_MAX_E,
}hi_omci_sql_function_type_e;

#define hi_omci_systrace( ui_ret, arg1, arg2, arg3, arg4)  \
                        hi_systrace(HI_SUBMODULE_OMCI_COM, ui_ret, arg1, arg2, arg3, arg4)

#define hi_omci_debug( ui_level,fmt,arg...) \
                        hi_debug( HI_SUBMODULE_OMCI_COM, ui_level,fmt, ##arg )

#define hi_omci_print( ui_level, fmt,arg...)  hi_print( HI_SUBMODULE_OMCI_COM, ui_level,fmt, ##arg )

#define hi_omci_printmem(ui_dbglevel,puc_src,ui_len,fmt,arg...) hi_log_memdes(HI_SUBMODULE_OMCI_COM,ui_dbglevel,puc_src,ui_len,fmt, ##arg) 

#define HI_OMCI_RET_CHECK(ui_ret) if(HI_RET_SUCC != ui_ret) \
                                  {\
                                      return HI_OMCI_PRO_ERR_PROCESS_ERR_E; \
                                  }

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_DEF_H__ */
