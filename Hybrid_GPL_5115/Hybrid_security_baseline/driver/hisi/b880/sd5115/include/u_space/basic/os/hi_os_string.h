/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_string.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_03
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_STRING_H__
#define __HI_OS_STRING_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* arg的处理 */
#define HI_VA_START(arglist, args)   va_start( arglist, args)
#define HI_VA_ARG(arglist,type)      va_arg(arglist,type)
#define HI_VA_END(arglist)           va_end(arglist)
#define HI_VA_LIST                   va_list

/* 数字字符判断 digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" */
#define HI_ISDIGIT(x)   (((x) >= '0') && ((x) <= '9'))

extern hi_int32 hi_os_isalnum(hi_int32 i_value);
extern hi_int32 hi_os_isalpha(hi_int32 i_value);
extern hi_int32 hi_os_isdigit(hi_int32 i_value);
extern hi_uint32 hi_os_isdigitstr(hi_char8 *pc_str);
extern hi_int32 hi_os_islower(hi_int32 i_ch);
extern hi_int32 hi_os_isspace(hi_int32 i_ch);
extern hi_uint32 hi_os_isspacestr(hi_char8 *pc_str);
extern hi_int32 hi_os_isupper(hi_int32 i_ch);
extern hi_int32 hi_os_strcasecmp(const hi_char8 *pc_str1, const hi_char8 *pc_str2);
extern hi_char8* hi_os_strcasestr(const hi_char8 *pc_str, const hi_char8 *pc_key);
extern hi_char8* hi_os_strcat( hi_char8* pc_dest, const hi_char8* pc_src);
extern hi_char8* hi_os_strchr(const hi_char8 *pc_str, hi_char8 c_chr);
extern hi_int32 hi_os_strcmp( const hi_char8 *pc_str1, const hi_char8 *pc_str2 );
extern hi_char8* hi_os_strconcat (const hi_char8 *pc_buf, ...);
extern hi_char8*  hi_os_strcpy( hi_char8 *pc_dest, const hi_char8 *pc_src );
extern hi_uint32 hi_os_strcspn(const hi_char8 *pc_str, const hi_char8 *pc_reject);
extern hi_uint32  hi_os_strlen( const hi_char8 *pc_str );
extern hi_int32 hi_os_strncasecmp(const hi_char8 *pc_str1, const hi_char8 *pc_str2,
                                                           hi_uint32 ui_num);
extern hi_char8* hi_os_strncat( hi_char8* pc_dest, const hi_char8* pc_src,hi_uint32 ui_num);
extern hi_int32 hi_os_strncmp(const hi_char8 *pc_str1, const hi_char8 *pc_str2, hi_uint32 ui_num);
extern hi_char8* hi_os_strncpy( hi_char8 *pc_dest, const hi_char8 *pc_src,
                                                                 hi_uint32 ui_num);
extern hi_char8* hi_os_strpbrk( const hi_char8* pc_str,const hi_char8* pc_str_set );
extern  hi_char8* hi_os_strrchr(const hi_char8* pc_str, hi_char8 c_chr);
extern hi_char8* hi_os_strrepeat(const hi_char8* pc_str);
extern hi_char8* hi_os_strsep( hi_char8 **ppc_str,const hi_char8* pc_delim );
extern hi_char8* hi_os_strstr(const hi_char8 *pc_str, const hi_char8 *pc_key);
extern hi_uint32 hi_os_atoi(const hi_char8* pc_str, hi_int32 *pi_value);
extern hi_char8* hi_os_strtok( hi_char8* pc_str,const hi_char8* pc_delim );
extern hi_char8* hi_os_strtok_r( hi_char8* pc_str,const hi_char8* pc_delim, hi_char8** ppc_save_ptr );
extern hi_int32 hi_os_strtol(const hi_char8* pc_str, hi_char8 **ppc_end, hi_int32 i_base);
extern hi_uint32 hi_os_strtoul(const hi_char8* pc_str, hi_char8 **ppc_end, hi_int32 i_base);
extern hi_char8* hi_os_tolower(hi_char8 *pc_str);
extern hi_char8* hi_os_toupper(hi_char8 *pc_str);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_STRING_H__ */
