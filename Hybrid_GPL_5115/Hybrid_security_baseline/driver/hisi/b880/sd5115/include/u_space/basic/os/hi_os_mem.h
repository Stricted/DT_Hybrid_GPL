/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_mem.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_MEM_H__
#define __HI_OS_MEM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern hi_void* hi_os_malloc( const hi_uint32 ui_size );
extern hi_void  hi_os_free( hi_void *pv_buf);
extern hi_void* hi_os_memset( hi_void *pv_toset, hi_uchar8 uc_setchar, hi_uint32 ui_count );
extern hi_void* hi_os_memcpy( hi_void *pv_buf1, const hi_void *pv_buf2, hi_uint32 ui_count);
extern hi_int32 hi_os_memcmp( const hi_void *pv_buf1, const hi_void *pv_buf2, hi_uint32 ui_count);
extern hi_void* hi_os_memmove(hi_void *pv_dest, const hi_void *pv_src, hi_uint32 ui_n);
extern hi_void* hi_os_realloc( hi_void *pv_buf, hi_uint32 ui_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_MEM_H__ */



