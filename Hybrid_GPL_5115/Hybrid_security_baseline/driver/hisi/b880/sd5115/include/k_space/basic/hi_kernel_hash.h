/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_hash.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_21
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_HASH_H__
#define __HI_KERNEL_HASH_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


typedef struct  {
    hi_uint32  ui_state;           /* HI_ENABLE / HI_DISABLE */
    hi_uint32  ui_hashsize;
    hi_uint32  ui_conflictitemsize; 
    hi_uint32  ui_conflictmaxsize; 
    hi_uint32  ui_currnum;
    hi_uint32  ui_conflictnum;
    hi_void   *pv_matchcallback;
    hi_void   *pv_comparecallback;
    hi_void   *pv_freekeycallback;
    hi_void   *pv_freedatacallback;
    hi_void   *pv_hashitems;
    hi_void   *pv_conflictitems;
    hi_list_head  st_idleconflict;
    hi_kernel_spinlock_t st_spinlock;
}hi_kernel_hash_head_s;

typedef struct  {
    hi_list_head  st_listhead;
    hi_uint32  ui_state;           /* HI_ENABLE / HI_DISABLE */
    hi_uint32  ui_cnt;
    hi_void   *pv_keyobject;
    hi_void   *pv_dataobject;
}hi_kernel_hash_item_s;

/*根据关键字key计算HASH索引值,输入关键字返回HASH索引*/
typedef hi_uint32 (* hi_kernel_hash_matchcallback)( hi_void* pv_keyobject );
/*相等返回0:HI_RET_SUCC 否则非0*/
typedef hi_uint32 (* hi_kernel_hash_comparecallback)( hi_void* pv_keyobject1, hi_void* pv_keyobject2 );
typedef hi_void (* hi_kernel_hash_freecallback)( hi_void* pv_object );

extern hi_uint32 hi_kernel_hash_init( hi_uint32 ui_hashsize, hi_uint32 ui_conflictitemsize, hi_uint32 ui_conflictmaxsize,
                               hi_void **pv_hashtable, hi_void *pv_matchcallback, hi_void *pv_comparecallback,
                               hi_void *pv_freekeycallback,hi_void *pv_freedatacallback );
extern hi_uint32 hi_kernel_hash_free( hi_void **pv_hashtable );
extern hi_uint32 hi_kernel_hash_insert( hi_kernel_hash_head_s *pst_hashtable,hi_void *pv_keyobject, hi_void *pv_dataobject );
extern hi_uint32 hi_kernel_hash_remove(  hi_kernel_hash_head_s *pst_hashtable, hi_void *pv_keyobject );
extern hi_uint32 hi_kernel_hash_find( hi_kernel_hash_head_s *pst_hashtable,
                                      hi_void *pv_keyobject, hi_void **pv_dataobject );
extern hi_uint32 hi_kernel_hash_dump( hi_void *pv_hashtable );
extern hi_uint32 hi_kernel_hash_fastfind( hi_kernel_hash_head_s *pst_hashtable,
                                   hi_void *pv_keyobject, hi_void **pv_dataobject );
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_KERNEL_HASH_H__ */
