/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_pipe.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_PIPE_H__
#define __HI_OS_PIPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern hi_int32 hi_os_mkfifo(const hi_char8* pc_pathname,hi_int32 i_mode);

extern hi_int32 hi_os_pipe(hi_int32 a_i_filedes[2]);

extern hi_int32 hi_os_pclose(HI_FILE_S* pst_stream);

extern HI_FILE_S* hi_os_popen(const hi_char8* pc_command,const hi_char8* pc_openmode);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_PIPE_H__ */



