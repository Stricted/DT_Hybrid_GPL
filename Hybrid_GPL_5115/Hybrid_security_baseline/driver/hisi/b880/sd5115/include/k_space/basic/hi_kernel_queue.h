/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_queue.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_QUEUE_H__
#define __HI_KERNEL_QUEUE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    hi_list_head  st_listhead;        // message queue header
    hi_kernel_wait_queue_head_t st_waitqueuehead;  // block the thread which reading empty message queue
    hi_kernel_spinlock_t        st_spinlock;

    hi_uint32 ui_cntcurr;           // the message conut in queue current 
    hi_uint32 ui_cntmax;            // the maximum message conut of the queue
    hi_uint32 ui_cntdrop;           // the message drop count when the queue is full
} hi_kernel_queue_head_s;

typedef struct {
    hi_list_head st_listhead;
    hi_uint32 ui_len;
    hi_uchar8 uc_text[4];
}hi_kernel_queue_items_s;

extern hi_uint32 hi_kernel_queue_init( hi_kernel_queue_head_s **ppst_queuehead, hi_uint32 ui_maxlen );
extern hi_uint32 hi_kernel_queue_free( hi_kernel_queue_head_s **ppst_queuehead );
extern hi_uint32 hi_kernel_queue_send( hi_kernel_queue_head_s *pst_queuehead, hi_void *pv_data, hi_uint32 ui_len );
extern hi_uint32 hi_kernel_queue_recv( hi_kernel_queue_head_s *pst_queuehead, hi_void *pv_data, hi_uint32 ui_maxlen );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_QUEUE_H__ */
