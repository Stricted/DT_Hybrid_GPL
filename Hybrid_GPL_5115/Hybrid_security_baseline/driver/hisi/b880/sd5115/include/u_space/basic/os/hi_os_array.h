/******************************************************************************

  Copyright (C), 2008-2018, Huawei Tech. Co., Ltd.

******************************************************************************/
#ifndef __HI_OS_ARRAY_H__
#define __HI_OS_ARRAY_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_OS_ITME_USED      (1)
#define HI_OS_ITME_UNUSED    (0)

/* ����ڵ�Ԫ�� */
typedef struct {
    hi_uint32 ui_flag;   /* �ڵ��־ */
    hi_uint32 ui_value;  /* Ԫ��ֵ */
} hi_os_array_item_s;

/* ��̬�������ݽṹ */
typedef struct {
    hi_uint32 ui_flag;                   /* ״̬��־ */
    hi_uint32 ui_max_count;              /* ������ */
    hi_uint32 ui_current_count;          /* ��ǰ���� */
    hi_os_array_item_s * pst_array;      /* ��̬����ͷ */    
    hi_os_array_item_s * pst_header;     /* ��̬���е�ǰ���õ�ͷ�ڵ� */
    hi_os_array_item_s * pst_tailer;     /* ��̬�����е�ǰ�Ķ�β�ڵ� */
    hi_os_pthread_mutex_s st_locker;     /* ���߳��� */
}hi_os_static_array_s;


extern hi_uint32 hi_os_array_init(hi_os_static_array_s * pst_array, 
                              hi_uint32 i_count);
extern hi_uint32 hi_os_array_dispose(hi_os_static_array_s * pst_array);
extern hi_uint32 hi_os_array_lock(hi_os_static_array_s * pst_array);
extern hi_uint32 hi_os_array_ulock(hi_os_static_array_s * pst_array);
extern hi_uint32 hi_os_array_add_item(hi_os_static_array_s * pst_array,
                              hi_uint32 * puiIndex, hi_uint32 ui_value);
extern hi_uint32 hi_os_array_get_item(hi_os_static_array_s * pst_array,
                              hi_uint32 uiIndex, hi_uint32 * puiOutValue);
extern hi_uint32 hi_os_array_check_item(hi_os_static_array_s * pst_array,
                              hi_uint32 uiIndex);
extern hi_uint32 hi_os_array_release_item(hi_os_static_array_s * pst_array,
                              hi_uint32 uiIndex);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_ARRAY_H__ */
