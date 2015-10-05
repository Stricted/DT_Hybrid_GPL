#ifndef __LINUX_SPINLOCK_TYPES_H
#define __LINUX_SPINLOCK_TYPES_H

/*
 * include/linux/spinlock_types.h - generic spinlock type definitions
 *                                  and initializers
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 */

#include <linux/spinlock_types_raw.h>

#ifndef CONFIG_PREEMPT_RT_FULL
# include <linux/spinlock_types_nort.h>
# include <linux/rwlock_types.h>
#else
# include <linux/rtmutex.h>
# include <linux/spinlock_types_rt.h>
# include <linux/rwlock_types_rt.h>
#endif
#ifdef CONFIG_DT_COMMON
/* START ADD : X00204200 : ���Hybrid�������⣬��ʱ����ֶΣ��������� */
#define SPIN_LOCK_UNLOCKED	__SPIN_LOCK_UNLOCKED(old_style_spin_init)
#define RW_LOCK_UNLOCKED	__RW_LOCK_UNLOCKED(old_style_rw_init)
/* END ADD : X00204200 : ���Hybrid�������⣬��ʱ����ֶΣ��������� */
#endif
#endif /* __LINUX_SPINLOCK_TYPES_H */
