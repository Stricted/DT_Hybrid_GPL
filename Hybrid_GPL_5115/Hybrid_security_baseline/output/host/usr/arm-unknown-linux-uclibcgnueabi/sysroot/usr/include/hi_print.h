/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_print.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_11_08

******************************************************************************/
#ifndef __HI_PRINT_H__
#define __HI_PRINT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef CONFIG_HI_KERNEL_MODULE

#define HI_KERNEL_PRINT_MEM( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uchar8) ) \
        {  if ( 0 == (i%16)) printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); printk("%02x ",(*(hi_uchar8*)(ptr_tmp+i))); } printk("\r\n"); \
    }

#define HI_KERNEL_PRINT_MEM_USHORT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); printk("%04x ",(*(hi_ushort16*)(ptr_tmp+i))); } printk("\r\n"); \
    }
#define HI_KERNEL_PRINT_MEM_UINT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); printk("%08x ",(*(hi_uint32*)(ptr_tmp+i))); } printk("\r\n"); \
    }
#define HI_KERNEL_PRINT_MEM_USHORT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); printk("%04x ",H_HTONS(*(hi_ushort16*)(ptr_tmp+i))); } printk("\r\n"); \
    }
#define HI_KERNEL_PRINT_MEM_UINT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); printk("%08x ",H_HTONL(*(hi_uint32*)(ptr_tmp+i))); } printk("\r\n"); \
    }

#else // user space define

/*uchar print*/
#define HI_PRINT_MEM( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i++ ) \
        {  if ( 0 == (i%16)) hi_os_printf("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_os_printf("%02x ",(*(hi_uchar8*)(ptr_tmp+i))); } hi_os_printf("\r\n"); \
    }

/*ushort print*/
#define HI_PRINT_MEM_USHORT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) hi_os_printf("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_os_printf("%04x ",(*(hi_ushort16*)(ptr_tmp+i))); } hi_os_printf("\r\n"); \
    }

/*uint print*/
#define HI_PRINT_MEM_UINT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) hi_os_printf("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_os_printf("%08x ",(*(hi_uint32*)(ptr_tmp+i))); } hi_os_printf("\r\n"); \
    }

/*网络序打印*/
/*ushort print*/
#define HI_PRINT_MEM_USHORT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) hi_os_printf("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_os_printf("%04x ",H_HTONS(*(hi_ushort16*)(ptr_tmp+i))); } hi_os_printf("\r\n"); \
    }


/*uint print*/
#define HI_PRINT_MEM_UINT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) hi_os_printf("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_os_printf("%08x ",H_HTONL(*(hi_uint32*)(ptr_tmp+i))); } hi_os_printf("\r\n"); \
    }

#endif // CONFIG_HI_KERNEL_MODULE

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_PRINT_H__ */
