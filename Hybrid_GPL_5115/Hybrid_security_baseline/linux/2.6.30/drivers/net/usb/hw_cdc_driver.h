/*
 * CDC Ethernet based the networking peripherals of Huawei data card devices
 * This driver is developed based on usbnet.c and cdc_ether.c
 * Copyright (C) 2009 by Franko Fang (Huawei Technologies Co., Ltd.)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will support Huawei data card devices for Linux networking,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*===============================================================
修改记录:
问题单号             修改时间    修改人         修改描述
DTS2013011408161 20130125    z00185914     将hw_cdc_driver中的中断处理移到ndistty中，新增hw_cdc_driver.h文件
DTS2013020600211 20130206    x81004082     使用无线拨号时，速率上不去
================================================================*/
/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
#ifndef _HW_CDC_DRIVER_H
#define _HW_CDC_DRIVER_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/usb/cdc.h>
#include <linux/usbdevice_fs.h>

#include <linux/version.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
#define DRIVER_VERSION "v2.07.00.00"
#define DRIVER_AUTHOR "Franko Fang <huananhu@huawei.com>"
#define DRIVER_DESC "Huawei ether driver for 3G data card ether device"
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define RX_MAX_QUEUE_MEMORY (60 * 1518)
#define    RX_QLEN(dev) ( ((dev)->udev->speed == USB_SPEED_HIGH) ? \
            (RX_MAX_QUEUE_MEMORY / (dev)->rx_urb_size) : 4)
#define    TX_QLEN(dev) (((dev)->udev->speed == USB_SPEED_HIGH) ? \
            (RX_MAX_QUEUE_MEMORY / (dev)->hard_mtu) : 4)

// reawaken network queue this soon after stopping; else watchdog barks
#define TX_TIMEOUT_JIFFIES    (5 * HZ)

// throttle rx/tx briefly after some faults, so khubd might disconnect()
// us (it polls at HZ/4 usually) before we report too many false errors.
#define THROTTLE_JIFFIES    (HZ / 8)

// between wakeups
#define UNLINK_TIMEOUT_MS    3
//////////////////////////////////////////////////////////////////////////////////////////////
#define HW_TLP_MASK_SYNC   0xF800
#define HW_TLP_MASK_LENGTH 0x07FF
#define HW_TLP_BITS_SYNC   0xF800
#pragma pack(push, 1)
struct hw_cdc_tlp
{
    unsigned short pktlength;
    unsigned char payload;
};
#define HW_TLP_HDR_LENGTH sizeof(unsigned short)
#pragma pack(pop)

typedef enum __HW_TLP_BUF_STATE {
    HW_TLP_BUF_STATE_IDLE = 0,
    HW_TLP_BUF_STATE_PARTIAL_FILL,
    HW_TLP_BUF_STATE_PARTIAL_HDR,
    HW_TLP_BUF_STATE_HDR_ONLY,
    HW_TLP_BUF_STATE_ERROR
}HW_TLP_BUF_STATE;

struct hw_cdc_tlp_tmp{
    void *buffer;
    unsigned short pktlength;
    unsigned short bytesneeded;
};
/*max ethernet pkt size 1514*/
#define HW_USB_RECEIVE_BUFFER_SIZE    1600L  
/*for Tin-layer-protocol (TLP)*/
#define HW_USB_MRECEIVE_BUFFER_SIZE   4096L  
/*for TLP*/
#define HW_USB_MRECEIVE_MAX_BUFFER_SIZE (1024 * 16)  

#define HW_JUNGO_BCDDEVICE_VALUE 0x0102
#define BINTERFACESUBCLASS 0x02
///////////////////////////////////////////////////////////////////////////////////////////
#define EVENT_TX_HALT 0
#define EVENT_RX_HALT 1
#define EVENT_RX_MEMORY 2
#define EVENT_STS_SPLIT 3
#define EVENT_LINK_RESET 4


#define NCM_TX_DEFAULT_TIMEOUT_MS 2


/*
  >2.6.36 some syetem not find ncm.h but find cdc.h 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
#include <linux/usb/ncm.h>
#else
*/
#define USB_CDC_NCM_TYPE        0x1a

/* NCM Functional Descriptor */
/* change usb_cdc_ncm_desc -> usb_cdc_ncm_desc_hw ,prevent cdc.h redefinition 11-05*/
struct usb_cdc_ncm_desc_hw {
    __u8    bLength;
    __u8    bDescriptorType;
    __u8    bDescriptorSubType;
    __le16    bcdNcmVersion;
    __u8    bmNetworkCapabilities;
} __attribute__ ((packed));

#ifdef NCM_NCAP_ETH_FILTER 
#undef NCM_NCAP_ETH_FILTER 
#endif
#ifdef NCM_NCAP_NET_ADDRESS 
#undef NCM_NCAP_NET_ADDRESS 
#endif
#ifdef NCM_NCAP_ENCAP_COMM 
#undef NCM_NCAP_ENCAP_COMM 
#endif
#ifdef NCM_NCAP_MAX_DGRAM 
#undef NCM_NCAP_MAX_DGRAM 
#endif
#ifdef NCM_NCAP_CRC_MODE 
#undef NCM_NCAP_CRC_MODE 
#endif

#define NCM_NCAP_ETH_FILTER    (1 << 0)
#define NCM_NCAP_NET_ADDRESS    (1 << 1)
#define NCM_NCAP_ENCAP_COMM    (1 << 2)
#define NCM_NCAP_MAX_DGRAM    (1 << 3)
#define NCM_NCAP_CRC_MODE    (1 << 4)

#ifdef USB_CDC_GET_NTB_PARAMETERS 
#undef USB_CDC_GET_NTB_PARAMETERS 
#endif
#ifdef USB_CDC_GET_NET_ADDRESS 
#undef USB_CDC_GET_NET_ADDRESS 
#endif
#ifdef USB_CDC_SET_NET_ADDRESS 
#undef USB_CDC_SET_NET_ADDRESS 
#endif
#ifdef USB_CDC_GET_NTB_FORMAT 
#undef USB_CDC_GET_NTB_FORMAT 
#endif
#ifdef USB_CDC_SET_NTB_FORMAT 
#undef USB_CDC_SET_NTB_FORMAT 
#endif
#ifdef USB_CDC_GET_NTB_INPUT_SIZE 
#undef USB_CDC_GET_NTB_INPUT_SIZE 
#endif
#ifdef USB_CDC_SET_NTB_INPUT_SIZE 
#undef USB_CDC_SET_NTB_INPUT_SIZE 
#endif
#ifdef USB_CDC_GET_MAX_DATAGRAM_SIZE 
#undef USB_CDC_GET_MAX_DATAGRAM_SIZE 
#endif
#ifdef USB_CDC_SET_MAX_DATAGRAM_SIZE 
#undef USB_CDC_SET_MAX_DATAGRAM_SIZE 
#endif
#ifdef USB_CDC_GET_CRC_MODE 
#undef USB_CDC_GET_CRC_MODE 
#endif
#ifdef USB_CDC_SET_CRC_MODE 
#undef USB_CDC_SET_CRC_MODE 
#endif

#define USB_CDC_GET_NTB_PARAMETERS        0x80
#define USB_CDC_GET_NET_ADDRESS            0x81
#define USB_CDC_SET_NET_ADDRESS            0x82
#define USB_CDC_GET_NTB_FORMAT            0x83
#define USB_CDC_SET_NTB_FORMAT            0x84
#define USB_CDC_GET_NTB_INPUT_SIZE        0x85
#define USB_CDC_SET_NTB_INPUT_SIZE        0x86
#define USB_CDC_GET_MAX_DATAGRAM_SIZE        0x87
#define USB_CDC_SET_MAX_DATAGRAM_SIZE        0x88
#define USB_CDC_GET_CRC_MODE            0x89
#define USB_CDC_SET_CRC_MODE            0x8a

/*
 * Class Specific structures and constants
 *
 * CDC NCM parameter structure, CDC NCM subclass 6.2.1
 *
 */
struct usb_cdc_ncm_ntb_parameter_hw {
    __le16    wLength;
    __le16    bmNtbFormatSupported;
    __le32    dwNtbInMaxSize;
    __le16    wNdpInDivisor;
    __le16    wNdpInPayloadRemainder;
    __le16    wNdpInAlignment;
    __le16    wPadding1;
    __le32    dwNtbOutMaxSize;
    __le16    wNdpOutDivisor;
    __le16    wNdpOutPayloadRemainder;
    __le16    wNdpOutAlignment;
    __le16    wPadding2;
} __attribute__ ((packed));

/*
 * CDC NCM transfer headers, CDC NCM subclass 3.2
 */
#ifdef NCM_NTH16_SIGN 
#undef NCM_NTH16_SIGN 
#endif
#ifdef NCM_NTH32_SIGN 
#undef NCM_NTH32_SIGN 
#endif

#define NCM_NTH16_SIGN        0x484D434E /* NCMH */
#define NCM_NTH32_SIGN        0x686D636E /* ncmh */

/* change usb_cdc_ncm_nth16 -> usb_cdc_ncm_nth16_hw ,prevent cdc.h redefinition */
struct usb_cdc_ncm_nth16_hw {
    __le32    dwSignature;
    __le16    wHeaderLength;
    __le16    wSequence;
    __le16    wBlockLength;
    __le16    wFpIndex;
} __attribute__ ((packed));

/* change usb_cdc_ncm_nth32 -> usb_cdc_ncm_nth_hw ,prevent cdc.h redefinition */
struct usb_cdc_ncm_nth32_hw {
    __le32    dwSignature;
    __le16    wHeaderLength;
    __le16    wSequence;
    __le32    dwBlockLength;
    __le32    dwFpIndex;
} __attribute__ ((packed));

/*
 * CDC NCM datagram pointers, CDC NCM subclass 3.3
 */
#ifdef NCM_NDP16_CRC_SIGN 
#undef NCM_NDP16_CRC_SIGN 
#endif
#ifdef NCM_NDP16_NOCRC_SIGN 
#undef NCM_NDP16_NOCRC_SIGN 
#endif
#ifdef NCM_NDP32_CRC_SIGN 
#undef NCM_NDP32_CRC_SIGN 
#endif
#ifdef NCM_NDP32_NOCRC_SIGN 
#undef NCM_NDP32_NOCRC_SIGN 
#endif

#define NCM_NDP16_CRC_SIGN    0x314D434E /* NCM1 */
#define NCM_NDP16_NOCRC_SIGN    0x304D434E /* NCM0 */
#define NCM_NDP32_CRC_SIGN    0x316D636E /* ncm1 */
#define NCM_NDP32_NOCRC_SIGN    0x306D636E /* ncm0 */

/* change usb_cdc_ncm_ndp16 -> usb_cdc_ncm_ndp16_hw ,prevent cdc.h redefinition */
struct usb_cdc_ncm_ndp16_hw {
    __le32    dwSignature;
    __le16    wLength;
    __le16    wNextFpIndex;
    __u8    data[0];
} __attribute__ ((packed));

/* change usb_cdc_ncm_ndp32 -> usb_cdc_ncm_ndp32_hw ,prevent cdc.h redefinition */
struct usb_cdc_ncm_ndp32_hw {
    __le32    dwSignature;
    __le16    wLength;
    __le16    wReserved6;
    __le32    dwNextFpIndex;
    __le32    dwReserved12;
    __u8    data[0];
} __attribute__ ((packed));

/*
 * Here are options for NCM Datagram Pointer table (NDP) parser.
 * There are 2 different formats: NDP16 and NDP32 in the spec (ch. 3),
 * in NDP16 offsets and sizes fields are 1 16bit word wide,
 * in NDP32 -- 2 16bit words wide. Also signatures are different.
 * To make the parser code the same, put the differences in the structure,
 * and switch pointers to the structures when the format is changed.
 */

/* change usb_cdc_ncm_ndp32 -> usb_cdc_ncm_ndp32_hw ,prevent redefinition */
struct ndp_parser_opts_hw {
    u32        nth_sign;
    u32        ndp_sign;
    unsigned    nth_size;
    unsigned    ndp_size;
    unsigned    ndplen_align;
    /* sizes in u16 units */
    unsigned    dgram_item_len; /* index or length */
    unsigned    block_length;
    unsigned    fp_index;
    unsigned    reserved1;
    unsigned    reserved2;
    unsigned    next_fp_index;
};

#ifdef NCM_CONTROL_TIMEOUT 
#undef NCM_CONTROL_TIMEOUT 
#endif

#define NCM_CONTROL_TIMEOUT        (5 * 1000)
/*#endif*/

/* 'u' must be of unsigned type */
#define IS_POWER2(u) (((u) > 0) && !((u) & ((u) - 1)))

/* 'p' must designate a variable of type * __le16 (in all get/put_ncm_leXX) */
#define get_ncm_le16(p)                \
    ({ __le16 val = get_unaligned_le16(p); p += 1; val; })

#define get_ncm_le32(p)                \
    ({ __le32 val = get_unaligned_le32(p); p += 2; val; })

#define put_ncm_le16(val, p)                \
    ({ put_unaligned_le16((val), p); p += 1; })

#define put_ncm_le32(val, p)                \
    ({ put_unaligned_le32((val), p); p += 2; })

#define NCM_NDP_MIN_ALIGNMENT        4

#ifdef NCM_NTB_MIN_IN_SIZE
#undef NCM_NTB_MIN_IN_SIZE
#endif
#define NCM_NTB_MIN_IN_SIZE        2048

#ifdef NCM_NTB_MIN_OUT_SIZE
#undef NCM_NTB_MIN_OUT_SIZE
#endif

#define NCM_NDP16_ENTRY_LEN        4

/* NTB16 must include: NTB16 header, NDP16 header, datagram pointer entry,
 * terminating (NULL) datagram entry
 */
#define NCM_NTB_MIN_OUT_SIZE        (sizeof(struct usb_cdc_ncm_nth16_hw) \
    + sizeof(struct usb_cdc_ncm_ndp16_hw) + 2 * NCM_NDP16_ENTRY_LEN)

#ifndef max
#define max(_a, _b)     (((_a) > (_b)) ? (_a) : (_b))
#endif

#ifndef min
#define min(_a, _b)     (((_a) < (_b)) ? (_a) : (_b))
#endif
/* <DTS2013020600211   x81004082 2013/2/6 begin*/
#if 1
#define NCM_NTB_HARD_MAX_IN_SIZE ((u32)(max(16,(int)ncm_read_size_in1k) * 1024))
#else
#define NCM_NTB_HARD_MAX_IN_SIZE ((u32)(max(2,(int)ncm_read_size_in1k) * 1024))
#endif
/* DTS2013020600211   x81004082 2013/2/6 end> */

#define RX_QLEN_NCM ncm_read_buf_count
#define TX_QLEN_NCM 4

/* These are actually defined in usbnet.c and we need to redefine these here in
 * order to calculate the size of the SKB pool
 */


struct ndp_entry {
    struct list_head list;
    unsigned idx;
    unsigned len;
};

struct ntb {
    /* Maximum possible length of this NTB */
    unsigned max_len;
    /* The current offset of the NDP */
    unsigned ndp_off;
    /* The current length of the NDP */
    unsigned ndp_len;
    /* End of the datagrams section */
    unsigned dgrams_end;
    /* Entries list (datagram index/lenght pairs) */
    struct list_head entries;
    /* Number of datagrams in this NTB */
    unsigned ndgrams;
    /* The SKB with the actual NTB data */
    struct sk_buff *skb;
};

#define NTB_LEN(n) ((n)->ndp_off + (n)->ndp_len)
#define NTB_IS_EMPTY(n) ((n)->ndgrams == 0)

struct ncm_ctx {
    struct usb_cdc_ncm_desc_hw *ncm_desc;
    //struct usbnet *unet;
    struct hw_cdc_net *ndev;
    struct usb_interface *control;
    struct usb_interface *data;

#define NTB_FORMAT_SUPPORTED_16BIT 0x0001
#define NTB_FORMAT_SUPPORTED_32BIT 0x0002
    u16 formats;
    u32 rx_max_ntb;
    u32 tx_max_ntb;
    u16 tx_divisor;
    u16 tx_remainder;
    u16 tx_align;

#define NCM_BIT_MODE_16        0
#define NCM_BIT_MODE_32        1
    u8 bit_mode;
#define NCM_CRC_MODE_NO        0
#define NCM_CRC_MODE_YES    1
    u8 crc_mode;

    struct ndp_parser_opts_hw popts;

    struct ntb curr_ntb;
    spinlock_t tx_lock;
    struct sk_buff **skb_pool;
    unsigned skb_pool_size;
    struct timer_list tx_timer;
    /* The maximum amount of jiffies that a datagram can be held (in the
     * current-NTB) before it must be sent on the bus
     */
    unsigned long tx_timeout_jiffies;
#ifdef CONFIG_CDC_ENCAP_COMMAND
    struct cdc_encap *cdc_encap_ctx;
#endif
};


struct hw_cdc_net{
    /* housekeeping */
    struct usb_device    *udev;
    struct usb_interface    *intf;
    const char        *driver_name;
    const char         *driver_desc;
    void            *driver_priv;
    wait_queue_head_t    *wait;
    struct mutex        phy_mutex;
    unsigned char        suspend_count;

    /* i/o info: pipes etc */
    unsigned        in, out;
    struct usb_host_endpoint *status;
    unsigned        maxpacket;
    struct timer_list    delay;

    /* protocol/interface state */
    struct net_device    *net;
    struct net_device_stats    stats;
    int            msg_enable;
    unsigned long        data [5];
    u32            xid;
    u32            hard_mtu;    /* count any extra framing */
    size_t            rx_urb_size;    /* size for rx urbs */
    struct mii_if_info    mii;

    /* various kinds of pending driver work */
    struct sk_buff_head    rxq;
    struct sk_buff_head    txq; 
    struct sk_buff_head    done;
    struct urb        *interrupt;
    struct tasklet_struct    bh;

    struct work_struct    kevent;
    struct delayed_work status_work;//fangxiaozhi added for work
    int            qmi_sync;
    unsigned long        flags;

    /*The state and buffer for the data of TLP*/
    HW_TLP_BUF_STATE hw_tlp_buffer_state;
    struct hw_cdc_tlp_tmp hw_tlp_tmp_buf;
    /*indicate the download tlp feature is activated or not*/
    int hw_tlp_download_is_actived;

    /*Add for ncm */
    int is_ncm;
    struct ncm_ctx *ncm_ctx;
    unsigned char	peer_addr[ETH_ALEN];/* <DTS2013092206920 xiazhongyue 20130922 modify  > */
};

/* Drivers that reuse some of the standard USB CDC infrastructure
 * (notably, using multiple interfaces according to the CDC
 * union descriptor) get some helper code.
 */
struct hw_dev_state {
    struct usb_cdc_header_desc    *header;
    struct usb_cdc_union_desc    *u;
    struct usb_cdc_ether_desc    *ether;
    struct usb_interface        *control;
    struct usb_interface        *data;
};


/* we record the state for each of our queued skbs */
enum skb_state {
    illegal = 0,
    tx_start, tx_done,
    rx_start, rx_done, rx_cleanup
};

struct skb_data {    /* skb->cb is one of these */
    struct urb        *urb;
    struct hw_cdc_net        *dev;
    enum skb_state        state;
    size_t            length;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern int set_rmnet_device(struct usb_device * rmnet_device);
extern int set_rmnet_intr_ep(struct usb_endpoint_descriptor * ep_int);
extern int trigger_tty(void);
extern int exit_tty(void);
extern int set_hwcdc_device(struct hw_cdc_net *dev);

#endif

/* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */

/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */
