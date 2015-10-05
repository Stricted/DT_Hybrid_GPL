/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_os.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_20
  最近修改   :
******************************************************************************/

#ifndef __HI_KERNEL_OS_H__
#define __HI_KERNEL_OS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_GFP_ATOMIC                    GFP_ATOMIC
#define HI_KERNEL_CLONE_KERNEL                  CLONE_KERNEL
#define HI_KERNEL_SIGKILL                       SIGKILL

#define hi_ker_readb(reg) (*((volatile hi_uchar8 *)(reg)))
#define hi_ker_readw(reg) (*((volatile hi_ushort16 *)(reg)))
#define hi_ker_readdw(reg) (*((volatile hi_uint32 *)(reg)))

#define hi_ker_writeb(data,reg)      (*((volatile hi_uchar8 *)(reg)) = (data))        
#define hi_ker_writew(data,reg)      (*((volatile hi_ushort16 *)(reg)) = (data))       
#define hi_ker_writedw(data,reg)     (*((volatile hi_uint32 *)(reg)) = (data))  

/***************          os marco defined   **************************/
#define hi_kernel_va_list           va_list
#define hi_kernel_va_start          va_start
#define hi_kernel_va_end            va_end

/***************          os function  **************************/
#define hi_kernel_malloc(size)      kmalloc(size,HI_KERNEL_GFP_ATOMIC)
#define hi_kernel_dmamalloc(size)   kmalloc(size,GFP_DMA)
#define hi_kernel_free              kfree
#define hi_kernel_memset            memset
#define hi_kernel_memcpy            memcpy
#define hi_kernel_memcmp            memcmp
#define hi_kernel_memmove           memmove

#define hi_kernel_strcpy            strcpy
#define hi_kernel_strncpy           strncpy
#define hi_kernel_strcmp            strcmp
#define hi_kernel_strlen            strlen
#define hi_kernel_strcat            strcat
#define hi_kernel_strncmp           strncmp
#define hi_kernel_sprintf           sprintf
#define hi_kernel_snprintf          snprintf
#define hi_kernel_scnprintf         scnprintf
#define hi_kernel_vsnprintf         vsnprintf
#define hi_kernel_printk            printk

#define hi_kernel_copy_from_user    copy_from_user
#define hi_kernel_copy_to_user      copy_to_user

#define hi_kernel_toupper           toupper
#define hi_kernel_lower             lower

#define hi_kernel_misc_register     misc_register
#define hi_kernel_thread            kernel_thread
#define hi_kernel_kill_proc         kill_pid
#define hi_kernel_daemonize         daemonize

#define hi_kernel_get_random_bytes  get_random_bytes
#define hi_kernel_wmb               wmb

#define hi_kernel_schedule_timeout  schedule_timeout

#define hi_kernel_timer_list        struct timer_list
#define hi_kernel_add_timer         add_timer
#define hi_kernel_init_timer        init_timer
#define hi_kernel_del_timer         del_timer
#define hi_kernel_del_timer_sync    del_timer_sync
#define hi_kernel_timer_pending     timer_pending
#define hi_kernel_mod_timer         mod_timer

#define hi_kernel_time_after_eq     time_after_eq
#define hi_kernel_msecs_to_jiffies  msecs_to_jiffies

#define hi_kernel_jiffies           jiffies
#define hi_kernel_udelay            udelay
#define hi_kernel_mdelay            mdelay

#define hi_kernel_wait_queue_head_t            wait_queue_head_t
#define hi_kernel_init_waitqueue_head          init_waitqueue_head
#define hi_kernel_wait_event_interruptible     wait_event_interruptible
#define hi_kernel_wake_up_interruptible        wake_up_interruptible

#define hi_kernel_spinlock_t                   spinlock_t
#define hi_kernel_spin_lock_init               spin_lock_init
#define hi_kernel_spin_lock                    spin_lock 
#define hi_kernel_spin_unlock                  spin_unlock
#define hi_kernel_spin_lock_irq                spin_lock_irq
#define hi_kernel_spin_unlock_irq              spin_unlock_irq
#define hi_kernel_spin_lock_irqsave            spin_lock_irqsave
#define hi_kernel_spin_unlock_irqrestore       spin_unlock_irqrestore
#define hi_kernel_spin_lock_bh                 spin_lock_bh 
#define hi_kernel_spin_unlock_bh               spin_unlock_bh
#define hi_kernel_local_irq_enable             local_irq_enable
#define hi_kernel_local_irq_disable            local_irq_disable
#define hi_kernel_local_bh_enable              local_bh_enable
#define hi_kernel_local_bh_disable             local_bh_disable

#define hi_kernel_sockaddr              struct sockaddr
#define hi_kernel_packet_type_s         struct packet_type 
#define hi_kernel_sk_buff_s             struct sk_buff
#define hi_kernel_dev_alloc_skb         dev_alloc_skb
#define hi_kernel_skb_copy              skb_copy
#define hi_kernel_skb_clone             skb_clone
#define hi_kernel_skb_cloned            skb_cloned
#define hi_kernel_skb_shared            skb_shared
#define hi_kernel_skb_headroom          skb_headroom
#define hi_kernel_skb_push              skb_push
#define hi_kernel_skb_reserve           skb_reserve
#define hi_kernel_skb_put               skb_put
#define hi_kernel_skb_pull              skb_pull
#define hi_kernel_skb_get               skb_get
#define hi_kernel_kfree_skb             kfree_skb
#define hi_kernel_dev_kfree_skb         dev_kfree_skb
#define hi_kernel_eth_type_trans        eth_type_trans

#define hi_kernel_set_bit               set_bit
#define hi_kernel_dev_add_pack          dev_add_pack
#define hi_kernel_dev_remove_pack       dev_remove_pack

#define hi_kernel_net_device_s          struct net_device
#define hi_kernel_net_device_stats_s    struct net_device_stats
#define hi_kernel_net_device_ops        struct net_device_ops
#define hi_kernel_dev_get_by_name       dev_get_by_name
#define hi_kernel_dev_get_by_index      dev_get_by_index 
#define hi_kernel_dev_put               dev_put
#define hi_kernel_dev_hold              dev_hold

#define hi_kernel_ifreq_s               struct ifreq
#define hi_kernel_netdev_priv           netdev_priv
#define hi_kernel_netif_rx              netif_rx
#define hi_kernel_netif_receive_skb     netif_receive_skb
#define hi_kernel_netif_tx_disable      netif_tx_disable
#define hi_kernel_netif_start_queue     netif_start_queue
#define hi_kernel_netif_queue_stopped   netif_queue_stopped
#define hi_kernel_netif_wake_queue      netif_wake_queue
#define hi_kernel_netif_rx_schedule_prep netif_rx_schedule_prep
#define hi_kernel_netif_rx_schedule     __netif_rx_schedule

#define hi_kernel_alloc_etherdev        alloc_etherdev
#define hi_kernel_alloc_netdev          alloc_netdev
#define hi_kernel_ether_setup           ether_setup
#define hi_kernel_register_netdevice    register_netdevice
#define hi_kernel_unregister_netdev     unregister_netdev
#define hi_kernel_free_netdev           free_netdev
#define hi_kernel_rtnl_lock             rtnl_lock
#define hi_kernel_rtnl_unlock           rtnl_unlock
#define hi_kernel_rtnl_trylock          rtnl_trylock
#define hi_kernel_ioremap               ioremap
#define hi_kernel_iounmap               iounmap

#define HI_KERNEL_SA_SHIRQ              IRQF_SHARED
#define HI_KERNEL_IRQ_HANDLED           IRQ_HANDLED
#define HI_KERNEL_IRQ_NONE              IRQ_NONE

#define hi_kernel_request_irq           request_irq
#define hi_kernel_irq_handler_t         irq_handler_t
#define hi_kernel_irqreturn_t           irqreturn_t

#define hi_kernel_tasklet_struct        struct tasklet_struct
#define hi_kernel_tasklet_init          tasklet_init
#define hi_kernel_tasklet_schedule      tasklet_schedule
#define hi_kernel_tasklet_hi_schedule   tasklet_hi_schedule
#define hi_kernel_tasklet_kill          tasklet_kill

#define HI_KERNEL_DMA_BIDIRECTIONAL     DMA_BIDIRECTIONAL
#define HI_KERNEL_DMA_FROM_DEVICE       DMA_FROM_DEVICE
#define HI_KERNEL_DMA_TO_DEVICE         DMA_TO_DEVICE
#define hi_kernel_dma_mapping_error     dma_mapping_error
#define hi_kernel_bus_to_virt           __bus_to_virt
#define hi_kernel_virt_to_bus           __virt_to_bus
#define hi_kernel_dma_set_mask          dma_set_mask
#define hi_kernel_dma_set_coherent_mask dma_set_coherent_mask

#define hi_kernel_ndelay                ndelay

#define hi_kernel_task_struct           struct task_struct
#define hi_kernel_kthread_create        kthread_create
#define hi_kernel_kthread_stop          kthread_stop
#define hi_kernel_kthread_run           kthread_run
#define hi_kernel_kthread_should_stop   kthread_should_stop
#define hi_kernel_schedule              schedule
#define hi_kernel_wake_up_process       wake_up_process
#define hi_kernel_set_current_state     set_current_state

#define hi_kernel_likely                likely
#define hi_kernel_unlikely              unlikely

#define hi_kernel_wait_queue_head_t     wait_queue_head_t
#define hi_kernel_init_waitqueue_head   init_waitqueue_head
#define hi_kernel_wait_event_interruptible wait_event_interruptible
#define hi_kernel_wake_up_interruptible wake_up_interruptible

#define hi_kernel_atomic_t              atomic_t
#define hi_kernel_atomic_set            atomic_set
#define hi_kernel_atomic_read           atomic_read
#define hi_kernel_atomic_inc            atomic_inc
#define hi_kernel_atomic_dec            atomic_dec
#define hi_kernel_atomic_dec_and_test   atomic_dec_and_test

#define hi_kernel_napi_struct           struct napi_struct
#define hi_kernel_netif_napi_add        netif_napi_add
#define hi_kernel_napi_enable           napi_enable
#define hi_kernel_napi_disable          napi_disable
#define hi_kernel_napi_schedule_prep    napi_schedule_prep
#define hi_kernel_napi_schedule         __napi_schedule
#define hi_kernel_napi_complete         napi_complete

#define hi_kernel_ethhdr                struct ethhdr
#define hi_kernel_pppoe_hdr             struct pppoe_hdr
#define hi_kernel_skb_mac_header        skb_mac_header
#define hi_kernel_iphdr                 struct iphdr
#define hi_kernel_skb_network_header    skb_network_header
#define hi_kernel_compare_ether_addr    compare_ether_addr
#define hi_kernel_skb_set_network_header  skb_set_network_header
#define hi_kernel_skb_reset_network_header  skb_reset_network_header


#define hi_kernel_skb_dst                  skb_dst
#define hi_kernel_dst_entry                struct dst_entry
#define hi_kernel_nf_conn                  struct nf_conn
#define hi_kernel_nf_conn_help             struct nf_conn_help
#define hi_kernel_nfct_help                nfct_help
#define hi_kernel_nf_ct_is_dying           nf_ct_is_dying
#define hi_kernel_nf_conntrack_tuple_hash  struct nf_conntrack_tuple_hash
#define hi_kernel_nf_ct_is_confirmed       nf_ct_is_confirmed
#define hi_kernel_nf_ct_get                nf_ct_get
#define hi_kernel_nf_ct_ipv6_skip_exthdr   nf_ct_ipv6_skip_exthdr



#define hi_kernel_ipv6hdr                  struct ipv6hdr
#define hi_kernel_ipv6_hdr                 ipv6_hdr
#define hi_kernel_ipv6_addr_type           ipv6_addr_type
#define hi_kernel_in6_addr                 struct	in6_addr

#define hi_kernel_tcphdr                   struct tcphdr
#define hi_kernel_udphdr                   struct udphdr


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_OS_H__ */
