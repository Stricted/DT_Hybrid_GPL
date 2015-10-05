/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_list.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_09_16

******************************************************************************/
#ifndef __HI_LIST_H__
#define __HI_LIST_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************          list Function  **************************/
typedef struct __hi_list_head{
    struct __hi_list_head *next, *prev;
}hi_list_head;

#define hi_list_init_head(ptr) do{\
      (ptr)->next = (ptr);(ptr)->prev = (ptr);\
}while(0)


#define hi_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)


#define hi_offsetof(TYPE, MEMBER) ((hi_uint32) &((TYPE *)0)->MEMBER)

#define hi_list_entry(ptr, type, member) ({          \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - hi_offsetof(type,member) );})

#define hi_list_getnext( head ) ( ( head != (head)->next) ? ((head)->next) : (HI_NULL) )

#define hi_list_getprev( head ) ( ( head != (head)->prev) ? ((head)->prev) : (HI_NULL) )

static inline int hi_list_is_last(hi_list_head *list,
                                     hi_list_head *head)
{
    return list->next == head;
}
        
static inline void __hi_list_add( hi_list_head *newlist,
                                     hi_list_head *prev,
                                     hi_list_head *next)
{
    next->prev = newlist;
    newlist->next = next;
    newlist->prev = prev;
    prev->next = newlist;
}

static inline void hi_list_add_tail( hi_list_head *newlist,  hi_list_head *head)
{
    __hi_list_add(newlist, head->prev, head);
}

static inline void __hi_list_del( hi_list_head * prev,  hi_list_head * next)
{
    next->prev = prev;
    prev->next = next;
}
static inline void hi_list_del( hi_list_head *entry)
{
    __hi_list_del(entry->prev, entry->next);
    entry->next = HI_NULL;
    entry->prev = HI_NULL;
}

static inline int hi_list_empty(hi_list_head *head)
{
    return head->next == head;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_LIST_H__ */
