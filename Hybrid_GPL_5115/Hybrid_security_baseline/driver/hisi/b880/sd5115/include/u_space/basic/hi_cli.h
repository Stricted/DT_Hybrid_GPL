/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_cli.h
  版 本 号   : 初稿
  作    者   : luocheng 00183967
  生成日期   : D2012_04_05

******************************************************************************/
#ifndef __HI_CLI_H__
#define __HI_CLI_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_int32  hi_cli_exec( hi_char8 *pc_cmdline );
extern hi_int32  hi_cli_main( hi_uchar8 uc_argc, hi_uchar8 *puc_argv );
extern hi_uint32 hi_cli_init( hi_void );

#define HI_CLI_EXEC(pc_cmdline) {\
                                    ui_ret = hi_cli_exec((hi_char8*)pc_cmdline);\
                                    if ( HI_RET_SUCC != ui_ret )\
                                    {\
                                        hi_os_printf("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[cmdline = %s]",ui_ret,__FILE__,__LINE__,pc_cmdline);\
                                        return ui_ret;\
                                    }\
                                }

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_CLI_H__ */
