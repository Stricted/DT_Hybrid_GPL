/*
 * USB Network driver infrastructure
 * Copyright (C) 2000-2005 by David Brownell
 * Copyright (C) 2003-2005 David Hollis <dhollis@davehollis.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * This is a generic "USB networking" framework that works with several
 * kinds of full and high speed networking devices:  host-to-host cables,
 * smart usb peripherals, and actual Ethernet adapters.
 *
 * These devices usually differ in terms of control protocols (if they
 * even have one!) and sometimes they define new framing to wrap or batch
 * Ethernet packets.  Otherwise, they talk to USB pretty much the same,
 * so interface (un)binding, endpoint I/O queues, fault handling, and other
 * issues can usefully be addressed by this framework.
 */
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z00159386    2012/09/19    DTS2012091310003    移动上行使用ipv6拨号时，不能处理大于自己MTU的包
*******************************************************************************/

// #define	DEBUG			// error path messages, extra info
// #define	VERBOSE			// more; success messages

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#include "qmitty.h"
#include "usbd_rmnet.h"
/* DTS2012031902908 g68080 2012-05-28 mod end> */
#include <linux/usb/usbnet.h>
/* <DTS2012110301136 l00205975 20121103 begin */
struct net_device		*g_usbnet=NULL;
typedef unsigned int (* rt_usb_dev_recv_handler)(struct sk_buff *,struct net_device *);
rt_usb_dev_recv_handler g_rt_usb_dev_recv_handler = NULL;
static int g_usb_disconnected = 1; /* DTS2013101507084 j81004134 2013/10/19 */
/* DTS2012110301136 l00205975 20121103 end> */
#define DRIVER_VERSION		"22-Aug-2005"

/* <DTS2012072102909 w00211169 2012-7-31 begin */
#define IP4_HDR     (0x45)
#define IP6_HDR     (0x60)
#define SIOCSUSBNET_PEER_HWADDR  (0x89F1)
/* DTS2012072102909 w00211169 2012-7-31 end> */
/* <DTS2012082701764 w00211169 2012-9-3 begin */
#define DNS_LAN_MARK 0x200000
/* DTS2012082701764 w00211169 2012-9-3 end> */
/*-------------------------------------------------------------------------*/

/*
 * Nineteen USB 1.1 max size bulk transactions per frame (ms), max.
 * Several dozen bytes of IPv4 data can fit in two such transactions.
 * One maximum size Ethernet packet takes twenty four of them.
 * For high speed, each frame comfortably fits almost 36 max size
 * Ethernet packets (so queues should be bigger).
 *
 * REVISIT qlens should be members of 'struct usbnet'; the goal is to
 * let the USB host controller be busy for 5msec or more before an irq
 * is required, under load.  Jumbograms change the equation.
 */
#define RX_MAX_QUEUE_MEMORY (60 * 1518)
#define	RX_QLEN(dev) (((dev)->udev->speed == USB_SPEED_HIGH) ? \
			(RX_MAX_QUEUE_MEMORY/(dev)->rx_urb_size) : 4)
#define	TX_QLEN(dev) (((dev)->udev->speed == USB_SPEED_HIGH) ? \
			(RX_MAX_QUEUE_MEMORY/(dev)->hard_mtu) : 4)

// reawaken network queue this soon after stopping; else watchdog barks
#define TX_TIMEOUT_JIFFIES	(5*HZ)

// throttle rx/tx briefly after some faults, so khubd might disconnect()
// us (it polls at HZ/4 usually) before we report too many false errors.
#define THROTTLE_JIFFIES	(HZ/8)

// between wakeups
#define UNLINK_TIMEOUT_MS	3

/*-------------------------------------------------------------------------*/

// randomly generated ethernet address
static u8	node_id [ETH_ALEN];
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
static const char driver_name [] = "rmnet";
#else
static const char driver_name [] = "usbnet";
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
/* use ethtool to change the level for any given device */
static int msg_level = -1;
module_param (msg_level, int, 0);
MODULE_PARM_DESC (msg_level, "Override default message level");

/*-------------------------------------------------------------------------*/

/* handles CDC Ethernet and many other network "bulk data" interfaces */
int usbnet_get_endpoints(struct usbnet *dev, struct usb_interface *intf)
{
	int				tmp;
	struct usb_host_interface	*alt = NULL;
	struct usb_host_endpoint	*in = NULL, *out = NULL;
	struct usb_host_endpoint	*status = NULL;
    struct usb_host_endpoint    *interruption = NULL;

	for (tmp = 0; tmp < intf->num_altsetting; tmp++) {
		unsigned	ep;

		in = out = status = NULL;
		alt = intf->altsetting + tmp;

		/* take the first altsetting with in-bulk + out-bulk;
		 * remember any status endpoint, just in case;
		 * ignore other endpoints and altsetttings.
		 */
		for (ep = 0; ep < alt->desc.bNumEndpoints; ep++) {
			struct usb_host_endpoint	*e;
			int				intr = 0;

			e = alt->endpoint + ep;
			switch (e->desc.bmAttributes) {
			case USB_ENDPOINT_XFER_INT:
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
				if (!usb_endpoint_dir_in(&e->desc))
					continue;
				intr = 1;
#else
                // record interrupt ep
                if (usb_endpoint_dir_in(&e->desc))
                {
                    interruption = e;
                }

                continue;
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
				/* FALLTHROUGH */
			case USB_ENDPOINT_XFER_BULK:
				break;
			default:
				continue;
			}
			if (usb_endpoint_dir_in(&e->desc)) {
				if (!intr && !in)
					in = e;
				else if (intr && !status)
					status = e;
			} else {
				if (!out)
					out = e;
			}
		}
		if (in && out)
			break;
	}
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
    if (!alt || !in || !out || !interruption)  
#else
	if (!alt || !in || !out)
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
		return -EINVAL;

	if (alt->desc.bAlternateSetting != 0
			|| !(dev->driver_info->flags & FLAG_NO_SETINT)) {
		tmp = usb_set_interface (dev->udev, alt->desc.bInterfaceNumber,
				alt->desc.bAlternateSetting);
		if (tmp < 0)
			return tmp;
	}

	dev->in = usb_rcvbulkpipe (dev->udev,
			in->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->out = usb_sndbulkpipe (dev->udev,
			out->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->status = status;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
       set_rmnet_intr_ep(&(interruption->desc));
       set_rmnet_device(dev->udev);

    printk("in->desc.bEndpointAddress=%x, dev->in=%ul\n", in->desc.bEndpointAddress, dev->in);
    printk("out->desc.bEndpointAddress=%x, dev->out=%ul\n", out->desc.bEndpointAddress, dev->out);
    printk("intr desc:\n"
           "bLength\t=\t0x%0x\n"
           "bDescriptorType\t=\t0x%0x\n"
           "bEndpointAddress\t=\t0x%0x\n"
           "bmAttributes\t=\t0x%0x\n"
           "wMaxPacketSize\t=\t0x%0x\n"
           "bInterval\t=\t0x%0x\n",
           interruption->desc.bLength,
           interruption->desc.bDescriptorType,
           interruption->desc.bEndpointAddress,
           interruption->desc.bmAttributes,
           interruption->desc.wMaxPacketSize,
           interruption->desc.bInterval
    );

    printk("dev->status=%x\n", (NULL == dev->status) ? 0 : dev->status);
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_get_endpoints);

static void intr_complete (struct urb *urb);

static int init_status (struct usbnet *dev, struct usb_interface *intf)
{
	char		*buf = NULL;
	unsigned	pipe = 0;
	unsigned	maxp;
	unsigned	period;

	if (!dev->driver_info->status)
		return 0;

	pipe = usb_rcvintpipe (dev->udev,
			dev->status->desc.bEndpointAddress
				& USB_ENDPOINT_NUMBER_MASK);
	maxp = usb_maxpacket (dev->udev, pipe, 0);

	/* avoid 1 msec chatter:  min 8 msec poll rate */
	period = max ((int) dev->status->desc.bInterval,
		(dev->udev->speed == USB_SPEED_HIGH) ? 7 : 3);

	buf = kmalloc (maxp, GFP_KERNEL);
	if (buf) {
		dev->interrupt = usb_alloc_urb (0, GFP_KERNEL);
		if (!dev->interrupt) {
			kfree (buf);
			return -ENOMEM;
		} else {
			usb_fill_int_urb(dev->interrupt, dev->udev, pipe,
				buf, maxp, intr_complete, dev, period);
			dev_dbg(&intf->dev,
				"status ep%din, %d bytes period %d\n",
				usb_pipeendpoint(pipe), maxp, period);
		}
	}
	return 0;
}
/* <DTS2012110301136 l00205975 20121103 begin */
/* register hook function in rx process*/
int usb_dev_recv_reg(rt_usb_dev_recv_handler rt_usb_dev_recv)
{
    g_rt_usb_dev_recv_handler = rt_usb_dev_recv;
}
EXPORT_SYMBOL(usb_dev_recv_reg);

/* unregister hook function in rx process*/
void usb_dev_recv_dereg(void)
{
    g_rt_usb_dev_recv_handler = NULL;
}
EXPORT_SYMBOL(usb_dev_recv_dereg);
/* DTS2012110301136 l00205975 20121103 end> */
/* Passes this packet up the stack, updating its accounting.
 * Some link protocols batch packets, so their rx_fixup paths
 * can return clones as well as just modify the original skb.
 */
void usbnet_skb_return (struct usbnet *dev, struct sk_buff *skb)
{
	int	status;

    skb->dev = dev->net;

    /* <DTS2012072102909 w00211169 2012-7-31 begin */
    /* <DTS2012082803596 w00211169 2012-8-29 begin */
    struct ethhdr eth;
    /* <DTS2014020800156 z00182709 2014-2-8 begin */
    /* 回退问题单DTS2013090203313，由于无线过来的数据包可能会不带mac信息，还需要进行判断一下 */
    if ((IP4_HDR == skb->data[0]) || (IP6_HDR == skb->data[0]))
    {
        //unsigned char	peer_addr[ETH_ALEN] = {0x02,0x50,0xf3,0x00,0x00,0x00};
        if (IP4_HDR == skb->data[0])
            eth.h_proto = htons(ETH_P_IP);
        else
            eth.h_proto = htons(ETH_P_IPV6);
        
        skb_push(skb, sizeof(struct ethhdr));
        memcpy(eth.h_dest,   skb->dev->dev_addr, ETH_ALEN);
        memcpy(eth.h_source, dev->peer_addr, ETH_ALEN);

        memcpy(skb->data, (unsigned char *)&eth, sizeof(struct ethhdr));
    }   
    /* DTS2014020800156 z00182709 2014-2-8 end> */
    /* DTS2012082803596 w00211169 2012-8-29 end> */
    /* DTS2012072102909 w00211169 2012-7-31 end> */

    /* <DTS2012100600085 w00211169 2012-10-9 begin */
	/* A temporary expedient to fix the bug from AMSS */
    struct ethhdr *_eth = NULL;
    if (IP6_HDR == (0xF0 & skb->data[sizeof(struct ethhdr)]))
    {
        _eth = (struct ethhdr *)skb->data;
        if (htons(ETH_P_IPV6) != _eth->h_proto)
        {
            //printk("[wrong proto : 0x%04x]\n", _eth->h_proto);
            
            eth.h_proto = htons(ETH_P_IPV6);
            memcpy(eth.h_dest,   skb->dev->dev_addr, ETH_ALEN);
            memcpy(eth.h_source, dev->peer_addr, ETH_ALEN);

            memcpy(skb->data, (unsigned char *)&eth, sizeof(struct ethhdr));
        }
    }     
    /* DTS2012100600085 w00211169 2012-10-9 end> */
    /* <DTS2012110301136 l00205975 20121103 begin */
	  if ( NULL != g_rt_usb_dev_recv_handler )
    {
        status  = g_rt_usb_dev_recv_handler(skb, skb->dev);  // if action don`t equal ETH_TO_DROP, it will call eth_type_trans()
    }
	  else
    {
		    skb->protocol = eth_type_trans (skb, dev->net);
		    status = netif_rx (skb);
    }
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;
    /* DTS2012110301136 l00205975 20121103 end> */  
}
EXPORT_SYMBOL_GPL(usbnet_skb_return);

/*-------------------------------------------------------------------------
 *
 * Network Device Driver (peer link to "Host Device", from USB host)
 *
 *-------------------------------------------------------------------------*/

int usbnet_change_mtu (struct net_device *net, int new_mtu)
{
	struct usbnet	*dev = netdev_priv(net);
	int		ll_mtu = new_mtu + net->hard_header_len;
	int		old_hard_mtu = dev->hard_mtu;
	int		old_rx_urb_size = dev->rx_urb_size;

	if (new_mtu <= 0)
		return -EINVAL;
	// no second zero-length packet read wanted after mtu-sized packets
	if ((ll_mtu % dev->maxpacket) == 0)
		return -EDOM;
	net->mtu = new_mtu;

	dev->hard_mtu = net->mtu + net->hard_header_len;
/* <DTS2012091310003  z00159386  20120919  begin */
#if 0
    /* 在修改MTU时不设置usbnet的接收缓冲区大小，避免收大包导致缓冲区溢出 */
	if (dev->rx_urb_size == old_hard_mtu) {
		dev->rx_urb_size = dev->hard_mtu;
		if (dev->rx_urb_size > old_rx_urb_size)
			usbnet_unlink_rx_urbs(dev);
	}
#else
    if (dev->hard_mtu > old_hard_mtu)
		usbnet_unlink_rx_urbs(dev);
#endif
/* DTS2012091310003  z00159386  20120919  end> */
	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_change_mtu);
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
static struct net_device_stats *usbnet_get_stats (struct net_device *net)
{
    struct usbnet   *dev = netdev_priv(net);

    return &dev->stats;
}
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */

/*-------------------------------------------------------------------------*/

/* some LK 2.4 HCDs oopsed if we freed or resubmitted urbs from
 * completion callbacks.  2.5 should have fixed those bugs...
 */

static void defer_bh(struct usbnet *dev, struct sk_buff *skb, struct sk_buff_head *list)
{
	unsigned long		flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_unlink(skb, list);
	spin_unlock(&list->lock);
	spin_lock(&dev->done.lock);
	__skb_queue_tail(&dev->done, skb);
	if (dev->done.qlen == 1)
		tasklet_schedule(&dev->bh);
	spin_unlock_irqrestore(&dev->done.lock, flags);
}

/* some work can't be done in tasklets, so we use keventd
 *
 * NOTE:  annoying asymmetry:  if it's active, schedule_work() fails,
 * but tasklet_schedule() doesn't.  hope the failure is rare.
 */
void usbnet_defer_kevent (struct usbnet *dev, int work)
{
	set_bit (work, &dev->flags);
	if (!schedule_work (&dev->kevent))
		deverr (dev, "kevent %d may have been dropped", work);
	else
		devdbg (dev, "kevent %d scheduled", work);
}
EXPORT_SYMBOL_GPL(usbnet_defer_kevent);

/*-------------------------------------------------------------------------*/

static void rx_complete (struct urb *urb);

static void rx_submit (struct usbnet *dev, struct urb *urb, gfp_t flags)
{
	struct sk_buff		*skb;
	struct skb_data		*entry;
	int			retval = 0;
	unsigned long		lockflags;
	size_t			size = dev->rx_urb_size;

/* <DTS2012091310003  z00159386  20120919  begin */
    /* 申请内存时将ethhdr内存也需要申请 */
	if ((skb = alloc_skb (size + NET_IP_ALIGN + sizeof(struct ethhdr), flags)) == NULL) {
/* DTS2012091310003  z00159386  20120919  end> */
		if (netif_msg_rx_err (dev))
			devdbg (dev, "no rx skb");
		usbnet_defer_kevent (dev, EVENT_RX_MEMORY);
		usb_free_urb (urb);
		return;
	}
	skb_reserve (skb, NET_IP_ALIGN);

    /* <DTS2012072102909 w00211169 2012-7-31 begin */
    skb_reserve (skb, sizeof(struct ethhdr));
    /* DTS2012072102909 w00211169 2012-7-31 end> */

	entry = (struct skb_data *) skb->cb;
	entry->urb = urb;
	entry->dev = dev;
	entry->state = rx_start;
	entry->length = 0;

	usb_fill_bulk_urb (urb, dev->udev, dev->in,
		skb->data, size, rx_complete, skb);

	spin_lock_irqsave (&dev->rxq.lock, lockflags);

	if (netif_running (dev->net)
			&& netif_device_present (dev->net)
			&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
		switch (retval = usb_submit_urb (urb, GFP_ATOMIC)) {
		case -EPIPE:
			usbnet_defer_kevent (dev, EVENT_RX_HALT);
			break;
		case -ENOMEM:
			usbnet_defer_kevent (dev, EVENT_RX_MEMORY);
			break;
		case -ENODEV:
			if (netif_msg_ifdown (dev))
				devdbg (dev, "device gone");
			netif_device_detach (dev->net);
			break;
		default:
			if (netif_msg_rx_err (dev))
				devdbg (dev, "rx submit, %d", retval);
			tasklet_schedule (&dev->bh);
			break;
		case 0:
			__skb_queue_tail (&dev->rxq, skb);
		}
	} else {
		if (netif_msg_ifdown (dev))
			devdbg (dev, "rx: stopped");
		retval = -ENOLINK;
	}
	spin_unlock_irqrestore (&dev->rxq.lock, lockflags);
	if (retval) {
		dev_kfree_skb_any (skb);
		usb_free_urb (urb);
	}
}


/*-------------------------------------------------------------------------*/

static inline void rx_process (struct usbnet *dev, struct sk_buff *skb)
{
	if (dev->driver_info->rx_fixup
			&& !dev->driver_info->rx_fixup (dev, skb))
		goto error;
	// else network stack removes extra byte if we forced a short packet

	if (skb->len)
		usbnet_skb_return (dev, skb);
	else {
		if (netif_msg_rx_err (dev))
			devdbg (dev, "drop");
error:
		dev->stats.rx_errors++;
		skb_queue_tail (&dev->done, skb);
	}
}

/*-------------------------------------------------------------------------*/

static void rx_complete (struct urb *urb)
{
	struct sk_buff		*skb = (struct sk_buff *) urb->context;
	struct skb_data		*entry = (struct skb_data *) skb->cb;
	struct usbnet		*dev = entry->dev;
	int			urb_status = urb->status;

	skb_put (skb, urb->actual_length);
	entry->state = rx_done;
	entry->urb = NULL;

	switch (urb_status) {
	/* success */
	case 0:
		if (skb->len < dev->net->hard_header_len) {
			entry->state = rx_cleanup;
			dev->stats.rx_errors++;
			dev->stats.rx_length_errors++;
			if (netif_msg_rx_err (dev))
				devdbg (dev, "rx length %d", skb->len);
		}
		break;

	/* stalls need manual reset. this is rare ... except that
	 * when going through USB 2.0 TTs, unplug appears this way.
	 * we avoid the highspeed version of the ETIMEOUT/EILSEQ
	 * storm, recovering as needed.
	 */
	case -EPIPE:
		dev->stats.rx_errors++;
		usbnet_defer_kevent (dev, EVENT_RX_HALT);
		// FALLTHROUGH

	/* software-driven interface shutdown */
	case -ECONNRESET:		/* async unlink */
	case -ESHUTDOWN:		/* hardware gone */
		if (netif_msg_ifdown (dev))
			devdbg (dev, "rx shutdown, code %d", urb_status);
		goto block;

	/* we get controller i/o faults during khubd disconnect() delays.
	 * throttle down resubmits, to avoid log floods; just temporarily,
	 * so we still recover when the fault isn't a khubd delay.
	 */
	case -EPROTO:
	case -ETIME:
	case -EILSEQ:
		dev->stats.rx_errors++;
		if (!timer_pending (&dev->delay)) {
			mod_timer (&dev->delay, jiffies + THROTTLE_JIFFIES);
			if (netif_msg_link (dev))
				devdbg (dev, "rx throttle %d", urb_status);
		}
block:
		entry->state = rx_cleanup;
		entry->urb = urb;
		urb = NULL;
		break;

	/* data overrun ... flush fifo? */
	case -EOVERFLOW:
		dev->stats.rx_over_errors++;
		// FALLTHROUGH

	default:
		entry->state = rx_cleanup;
		dev->stats.rx_errors++;
		if (netif_msg_rx_err (dev))
			devdbg (dev, "rx status %d", urb_status);
		break;
	}

	defer_bh(dev, skb, &dev->rxq);

	if (urb) {
		if (netif_running (dev->net)
				&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
			rx_submit (dev, urb, GFP_ATOMIC);
			return;
		}
		usb_free_urb (urb);
	}
	if (netif_msg_rx_err (dev))
		devdbg (dev, "no read resubmitted");
}

static void intr_complete (struct urb *urb)
{
	struct usbnet	*dev = urb->context;
	int		status = urb->status;

	switch (status) {
	/* success */
	case 0:
		dev->driver_info->status(dev, urb);
		break;

	/* software-driven interface shutdown */
	case -ENOENT:		/* urb killed */
	case -ESHUTDOWN:	/* hardware gone */
		if (netif_msg_ifdown (dev))
			devdbg (dev, "intr shutdown, code %d", status);
		return;

	/* NOTE:  not throttling like RX/TX, since this endpoint
	 * already polls infrequently
	 */
	default:
		devdbg (dev, "intr status %d", status);
		break;
	}

	if (!netif_running (dev->net))
		return;

	memset(urb->transfer_buffer, 0, urb->transfer_buffer_length);
	status = usb_submit_urb (urb, GFP_ATOMIC);
	if (status != 0 && netif_msg_timer (dev))
		deverr(dev, "intr resubmit --> %d", status);
}

/*-------------------------------------------------------------------------*/

// unlink pending rx/tx; completion handlers do all other cleanup

static int unlink_urbs (struct usbnet *dev, struct sk_buff_head *q)
{
	unsigned long		flags;
	struct sk_buff		*skb, *skbnext;
	int			count = 0;

	spin_lock_irqsave (&q->lock, flags);
	skb_queue_walk_safe(q, skb, skbnext) {
		struct skb_data		*entry;
		struct urb		*urb;
		int			retval;

		entry = (struct skb_data *) skb->cb;
		urb = entry->urb;

		// during some PM-driven resume scenarios,
		// these (async) unlinks complete immediately
		retval = usb_unlink_urb (urb);
		if (retval != -EINPROGRESS && retval != 0)
			devdbg (dev, "unlink urb err, %d", retval);
		else
			count++;
	}
	spin_unlock_irqrestore (&q->lock, flags);
	return count;
}

// Flush all pending rx urbs
// minidrivers may need to do this when the MTU changes

void usbnet_unlink_rx_urbs(struct usbnet *dev)
{
	if (netif_running(dev->net)) {
		(void) unlink_urbs (dev, &dev->rxq);
		tasklet_schedule(&dev->bh);
	}
}
EXPORT_SYMBOL_GPL(usbnet_unlink_rx_urbs);

/*-------------------------------------------------------------------------*/

// precondition: never called in_interrupt

int usbnet_stop (struct net_device *net)
{
	struct usbnet		*dev = netdev_priv(net);
	int			temp;
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK (unlink_wakeup);
	DECLARE_WAITQUEUE (wait, current);

	netif_stop_queue (net);

	if (netif_msg_ifdown (dev))
		devinfo (dev, "stop stats: rx/tx %ld/%ld, errs %ld/%ld",
			dev->stats.rx_packets, dev->stats.tx_packets,
			dev->stats.rx_errors, dev->stats.tx_errors
			);

	// ensure there are no more active urbs
	add_wait_queue (&unlink_wakeup, &wait);
	dev->wait = &unlink_wakeup;
	temp = unlink_urbs (dev, &dev->txq) + unlink_urbs (dev, &dev->rxq);

	// maybe wait for deletions to finish.
	while (!skb_queue_empty(&dev->rxq)
			&& !skb_queue_empty(&dev->txq)
			&& !skb_queue_empty(&dev->done)) {
		msleep(UNLINK_TIMEOUT_MS);
		if (netif_msg_ifdown (dev))
			devdbg (dev, "waited for %d urb completions", temp);
	}
	dev->wait = NULL;
	remove_wait_queue (&unlink_wakeup, &wait);

	usb_kill_urb(dev->interrupt);

	/* deferred work (task, timer, softirq) must also stop.
	 * can't flush_scheduled_work() until we drop rtnl (later),
	 * else workers could deadlock; so make workers a NOP.
	 */
	dev->flags = 0;
	del_timer_sync (&dev->delay);
	tasklet_kill (&dev->bh);
	usb_autopm_put_interface(dev->intf);

	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_stop);

/*-------------------------------------------------------------------------*/

// posts reads, and enables write queuing

// precondition: never called in_interrupt

int usbnet_open (struct net_device *net)
{
	struct usbnet		*dev = netdev_priv(net);
	int			retval;
	struct driver_info	*info = dev->driver_info;

	if ((retval = usb_autopm_get_interface(dev->intf)) < 0) {
		if (netif_msg_ifup (dev))
			devinfo (dev,
				"resumption fail (%d) usbnet usb-%s-%s, %s",
				retval,
				dev->udev->bus->bus_name, dev->udev->devpath,
			info->description);
		goto done_nopm;
	}

	// put into "known safe" state
	if (info->reset && (retval = info->reset (dev)) < 0) {
		if (netif_msg_ifup (dev))
			devinfo (dev,
				"open reset fail (%d) usbnet usb-%s-%s, %s",
				retval,
				dev->udev->bus->bus_name, dev->udev->devpath,
			info->description);
		goto done;
	}

	// insist peer be connected
	if (info->check_connect && (retval = info->check_connect (dev)) < 0) {
		if (netif_msg_ifup (dev))
			devdbg (dev, "can't open; %d", retval);
		goto done;
	}

	/* start any status interrupt transfer */
	if (dev->interrupt) {
		retval = usb_submit_urb (dev->interrupt, GFP_KERNEL);
		if (retval < 0) {
			if (netif_msg_ifup (dev))
				deverr (dev, "intr submit %d", retval);
			goto done;
		}
	}

	netif_start_queue (net);
	if (netif_msg_ifup (dev)) {
		char	*framing;

		if (dev->driver_info->flags & FLAG_FRAMING_NC)
			framing = "NetChip";
		else if (dev->driver_info->flags & FLAG_FRAMING_GL)
			framing = "GeneSys";
		else if (dev->driver_info->flags & FLAG_FRAMING_Z)
			framing = "Zaurus";
		else if (dev->driver_info->flags & FLAG_FRAMING_RN)
			framing = "RNDIS";
		else if (dev->driver_info->flags & FLAG_FRAMING_AX)
			framing = "ASIX";
		else
			framing = "simple";

		devinfo (dev, "open: enable queueing "
				"(rx %d, tx %d) mtu %d %s framing",
			(int)RX_QLEN (dev), (int)TX_QLEN (dev), dev->net->mtu,
			framing);
	}

	// delay posting reads until we're fully open
	tasklet_schedule (&dev->bh);
	return retval;
done:
	usb_autopm_put_interface(dev->intf);
done_nopm:
	return retval;
}
EXPORT_SYMBOL_GPL(usbnet_open);

/*-------------------------------------------------------------------------*/

/* ethtool methods; minidrivers may need to add some more, but
 * they'll probably want to use this base set.
 */
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
int usbnet_get_settings (struct net_device *net, struct ethtool_cmd *cmd)
{
	struct usbnet *dev = netdev_priv(net);

	if (!dev->mii.mdio_read)
		return -EOPNOTSUPP;

	return mii_ethtool_gset(&dev->mii, cmd);
}
EXPORT_SYMBOL_GPL(usbnet_get_settings);

int usbnet_set_settings (struct net_device *net, struct ethtool_cmd *cmd)
{
	struct usbnet *dev = netdev_priv(net);
	int retval;

	if (!dev->mii.mdio_write)
		return -EOPNOTSUPP;

	retval = mii_ethtool_sset(&dev->mii, cmd);

	/* link speed/duplex might have changed */
	if (dev->driver_info->link_reset)
		dev->driver_info->link_reset(dev);

	return retval;

}
EXPORT_SYMBOL_GPL(usbnet_set_settings);

u32 usbnet_get_link (struct net_device *net)
{
	struct usbnet *dev = netdev_priv(net);

	/* If a check_connect is defined, return its result */
	if (dev->driver_info->check_connect)
		return dev->driver_info->check_connect (dev) == 0;

	/* if the device has mii operations, use those */
	if (dev->mii.mdio_read)
		return mii_link_ok(&dev->mii);

	/* Otherwise, dtrt for drivers calling netif_carrier_{on,off} */
	return ethtool_op_get_link(net);
}
EXPORT_SYMBOL_GPL(usbnet_get_link);

int usbnet_nway_reset(struct net_device *net)
{
	struct usbnet *dev = netdev_priv(net);

	if (!dev->mii.mdio_write)
		return -EOPNOTSUPP;

	return mii_nway_restart(&dev->mii);
}
EXPORT_SYMBOL_GPL(usbnet_nway_reset);
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */

void usbnet_get_drvinfo (struct net_device *net, struct ethtool_drvinfo *info)
{
	struct usbnet *dev = netdev_priv(net);
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
    strncpy (info->driver, driver_name, sizeof info->driver);
#else
	strncpy (info->driver, dev->driver_name, sizeof info->driver);
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	strncpy (info->version, DRIVER_VERSION, sizeof info->version);
	strncpy (info->fw_version, dev->driver_info->description,
		sizeof info->fw_version);
	usb_make_path (dev->udev, info->bus_info, sizeof info->bus_info);
}
EXPORT_SYMBOL_GPL(usbnet_get_drvinfo);

u32 usbnet_get_msglevel (struct net_device *net)
{
	struct usbnet *dev = netdev_priv(net);

	return dev->msg_enable;
}
EXPORT_SYMBOL_GPL(usbnet_get_msglevel);

void usbnet_set_msglevel (struct net_device *net, u32 level)
{
	struct usbnet *dev = netdev_priv(net);

	dev->msg_enable = level;
}
EXPORT_SYMBOL_GPL(usbnet_set_msglevel);

/* drivers may override default ethtool_ops in their bind() routine */
static struct ethtool_ops usbnet_ethtool_ops = {
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
	.get_settings		= usbnet_get_settings,
	.set_settings		= usbnet_set_settings,
	.get_link		= usbnet_get_link,
	.nway_reset		= usbnet_nway_reset,
#endif	
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	.get_drvinfo		= usbnet_get_drvinfo,
	.get_msglevel		= usbnet_get_msglevel,
	.set_msglevel		= usbnet_set_msglevel,
};

/*-------------------------------------------------------------------------*/

/* work that cannot be done in interrupt context uses keventd.
 *
 * NOTE:  with 2.5 we could do more of this using completion callbacks,
 * especially now that control transfers can be queued.
 */
static void
kevent (struct work_struct *work)
{
	struct usbnet		*dev =
		container_of(work, struct usbnet, kevent);
	int			status;

	/* usb_clear_halt() needs a thread context */
	if (test_bit (EVENT_TX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->txq);
		status = usb_clear_halt (dev->udev, dev->out);
		if (status < 0
				&& status != -EPIPE
				&& status != -ESHUTDOWN) {
			if (netif_msg_tx_err (dev))
				deverr (dev, "can't clear tx halt, status %d",
					status);
		} else {
			clear_bit (EVENT_TX_HALT, &dev->flags);
			if (status != -ESHUTDOWN)
				netif_wake_queue (dev->net);
		}
	}
	if (test_bit (EVENT_RX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->rxq);
		status = usb_clear_halt (dev->udev, dev->in);
		if (status < 0
				&& status != -EPIPE
				&& status != -ESHUTDOWN) {
			if (netif_msg_rx_err (dev))
				deverr (dev, "can't clear rx halt, status %d",
					status);
		} else {
			clear_bit (EVENT_RX_HALT, &dev->flags);
			tasklet_schedule (&dev->bh);
		}
	}

	/* tasklet could resubmit itself forever if memory is tight */
	if (test_bit (EVENT_RX_MEMORY, &dev->flags)) {
		struct urb	*urb = NULL;

		if (netif_running (dev->net))
			urb = usb_alloc_urb (0, GFP_KERNEL);
		else
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
		if (urb != NULL) {
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
			rx_submit (dev, urb, GFP_KERNEL);
			tasklet_schedule (&dev->bh);
		}
	}

	if (test_bit (EVENT_LINK_RESET, &dev->flags)) {
		struct driver_info	*info = dev->driver_info;
		int			retval = 0;

		clear_bit (EVENT_LINK_RESET, &dev->flags);
		if(info->link_reset && (retval = info->link_reset(dev)) < 0) {
			devinfo(dev, "link reset failed (%d) usbnet usb-%s-%s, %s",
				retval,
				dev->udev->bus->bus_name, dev->udev->devpath,
				info->description);
		}
	}

	if (dev->flags)
		devdbg (dev, "kevent done, flags = 0x%lx",
			dev->flags);
}

/*-------------------------------------------------------------------------*/

static void tx_complete (struct urb *urb)
{
	struct sk_buff		*skb = (struct sk_buff *) urb->context;
	struct skb_data		*entry = (struct skb_data *) skb->cb;
	struct usbnet		*dev = entry->dev;

	if (urb->status == 0) {
		dev->stats.tx_packets++;

        /* <DTS2012121007643  w00211169 2012-12-11 begin */
        struct net_device *in_dev = NULL; 
        in_dev = dev_get_by_index(dev_net(dev), skb->iif);
        if (NULL != in_dev)
        {
            if (((in_dev->name[0] == 'e') || (in_dev->name[0] == 'w')))
            {  
                dev->stats.tx_bytes += entry->length;
            }  

            dev_put(in_dev); 
        } else
        /* DTS2012121007643  w00211169 2012-12-11 end> */         
        /* <DTS2012071908795 w00211169 2012-7-26 begin */
        if ((NULL != skb->lanindev)
            && ((skb->lanindev->name[0] == 'e') || (skb->lanindev->name[0] == 'w')))
        {
            dev->stats.tx_bytes += entry->length;
        }            
        /* DTS2012071908795 w00211169 2012-7-26 end> */
        /* <DTS2012082701764 w00211169 2012-9-3 begin */
        else if (skb->mark & DNS_LAN_MARK)
        {
            dev->stats.tx_bytes += entry->length;
        }
        else
        {
            ;/* do nothing */
        }
        /* DTS2012082701764 w00211169 2012-9-3 end> */
	} else {
		dev->stats.tx_errors++;

		switch (urb->status) {
		case -EPIPE:
			usbnet_defer_kevent (dev, EVENT_TX_HALT);
			break;

		/* software-driven interface shutdown */
		case -ECONNRESET:		// async unlink
		case -ESHUTDOWN:		// hardware gone
			break;

		// like rx, tx gets controller i/o faults during khubd delays
		// and so it uses the same throttling mechanism.
		case -EPROTO:
		case -ETIME:
		case -EILSEQ:
			if (!timer_pending (&dev->delay)) {
				mod_timer (&dev->delay,
					jiffies + THROTTLE_JIFFIES);
				if (netif_msg_link (dev))
					devdbg (dev, "tx throttle %d",
							urb->status);
			}
			netif_stop_queue (dev->net);
			break;
		default:
			if (netif_msg_tx_err (dev))
				devdbg (dev, "tx err %d", entry->urb->status);
			break;
		}
	}

	urb->dev = NULL;
	entry->state = tx_done;
	defer_bh(dev, skb, &dev->txq);
}

/*-------------------------------------------------------------------------*/

void usbnet_tx_timeout (struct net_device *net)
{
	struct usbnet		*dev = netdev_priv(net);

	unlink_urbs (dev, &dev->txq);
	tasklet_schedule (&dev->bh);

	// FIXME: device recovery -- reset?
}
EXPORT_SYMBOL_GPL(usbnet_tx_timeout);

/*-------------------------------------------------------------------------*/

int usbnet_start_xmit (struct sk_buff *skb, struct net_device *net)
{
	struct usbnet		*dev = netdev_priv(net);
	int			length;
	int			retval = NET_XMIT_SUCCESS;
	struct urb		*urb = NULL;
	struct skb_data		*entry;
	struct driver_info	*info = dev->driver_info;
	unsigned long		flags;

	// some devices want funky USB-level framing, for
	// win32 driver (usually) and/or hardware quirks
	if (info->tx_fixup) {
		skb = info->tx_fixup (dev, skb, GFP_ATOMIC);
		if (!skb) {
			if (netif_msg_tx_err (dev))
				devdbg (dev, "can't tx_fixup skb");
			goto drop;
		}
	}
	length = skb->len;

 /* <DTS2013101507084 j81004134 2013/10/19 begin */
    /* usb已断开，则丢弃发送的数据，避免数据发送到不存在的设备上引发内核panic */
	if (0 < g_usb_disconnected)
	{
        printk("[%s]:g_usb_disconnected=%d\n",__FUNCTION__ ,g_usb_disconnected);
	    goto drop;
	}
 /* DTS2013101507084 j81004134 2013/10/19 end> */

	if (!(urb = usb_alloc_urb (0, GFP_ATOMIC))) {
		if (netif_msg_tx_err (dev))
			devdbg (dev, "no urb");
		goto drop;
	}

	entry = (struct skb_data *) skb->cb;
	entry->urb = urb;
	entry->dev = dev;
	entry->state = tx_start;
	entry->length = length;

	usb_fill_bulk_urb (urb, dev->udev, dev->out,
			skb->data, skb->len, tx_complete, skb);

	/* don't assume the hardware handles USB_ZERO_PACKET
	 * NOTE:  strictly conforming cdc-ether devices should expect
	 * the ZLP here, but ignore the one-byte packet.
	 */
	if ((length % dev->maxpacket) == 0) {
		urb->transfer_buffer_length++;
		if (skb_tailroom(skb)) {
			skb->data[skb->len] = 0;
			__skb_put(skb, 1);
		}
	}

	spin_lock_irqsave (&dev->txq.lock, flags);

	switch ((retval = usb_submit_urb (urb, GFP_ATOMIC))) {
	case -EPIPE:
		netif_stop_queue (net);
		usbnet_defer_kevent (dev, EVENT_TX_HALT);
		break;
	default:
		if (netif_msg_tx_err (dev))
			devdbg (dev, "tx: submit urb err %d", retval);
		break;
	case 0:
		net->trans_start = jiffies;
		__skb_queue_tail (&dev->txq, skb);
		if (dev->txq.qlen >= TX_QLEN (dev))
			netif_stop_queue (net);
	}
	spin_unlock_irqrestore (&dev->txq.lock, flags);

	if (retval) {
		if (netif_msg_tx_err (dev))
			devdbg (dev, "drop, code %d", retval);
drop:
		retval = NET_XMIT_SUCCESS;
		dev->stats.tx_dropped++;
		if (skb)
			dev_kfree_skb_any (skb);
		usb_free_urb (urb);
	} else if (netif_msg_tx_queued (dev)) {
		devdbg (dev, "> tx, len %d, type 0x%x",
			length, skb->protocol);
	}
	return retval;
}
EXPORT_SYMBOL_GPL(usbnet_start_xmit);

/*-------------------------------------------------------------------------*/

// tasklet (work deferred from completions, in_irq) or timer

static void usbnet_bh (unsigned long param)
{
	struct usbnet		*dev = (struct usbnet *) param;
	struct sk_buff		*skb;
	struct skb_data		*entry;

	while ((skb = skb_dequeue (&dev->done))) {
		entry = (struct skb_data *) skb->cb;
		switch (entry->state) {
		case rx_done:
			entry->state = rx_cleanup;
			rx_process (dev, skb);
			continue;
		case tx_done:
		case rx_cleanup:
			usb_free_urb (entry->urb);
			dev_kfree_skb (skb);
			continue;
		default:
			devdbg (dev, "bogus skb state %d", entry->state);
		}
	}

	// waiting for all pending urbs to complete?
	if (dev->wait) {
		if ((dev->txq.qlen + dev->rxq.qlen + dev->done.qlen) == 0) {
			wake_up (dev->wait);
		}

	// or are we maybe short a few urbs?
	} else if (netif_running (dev->net)
			&& netif_device_present (dev->net)
			&& !timer_pending (&dev->delay)
			&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
		int	temp = dev->rxq.qlen;
		int	qlen = RX_QLEN (dev);

		if (temp < qlen) {
			struct urb	*urb;
			int		i;

			// don't refill the queue all at once
			for (i = 0; i < 10 && dev->rxq.qlen < qlen; i++) {
				urb = usb_alloc_urb (0, GFP_ATOMIC);
				if (urb != NULL)
					rx_submit (dev, urb, GFP_ATOMIC);
			}
			if (temp != dev->rxq.qlen && netif_msg_link (dev))
				devdbg (dev, "rxqlen %d --> %d",
						temp, dev->rxq.qlen);
			if (dev->rxq.qlen < qlen)
				tasklet_schedule (&dev->bh);
		}
		if (dev->txq.qlen < TX_QLEN (dev))
			netif_wake_queue (dev->net);
	}
}

/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
static int usbnet_ethtool_ioctl(struct net_device *net, void *useraddr)
{
    struct usbnet		*dev = netdev_priv(net);
    u32 cmd;

    if (get_user(cmd, (u32 *)useraddr))
    {
        return -EFAULT;
    }

    switch (cmd)
    {
    case ETHTOOL_GDRVINFO:
    {
        struct ethtool_drvinfo info;
        memset(&info, 0, sizeof info);
        info.cmd = ETHTOOL_GDRVINFO;
        strncpy(info.driver, driver_name, sizeof info.driver);
        strncpy(info.version, DRIVER_VERSION, sizeof info.version);
        strncpy(info.fw_version, dev->driver_info->description, sizeof
                info.fw_version);
        usb_make_path(dev->udev, info.bus_info, sizeof info.bus_info);
        if (copy_to_user(useraddr, &info, sizeof (info)))
        {
            return -EFAULT;
        }

        return 0;
    }
    case ETHTOOL_GLINK:
        if (dev->driver_info->check_connect)
        {
            struct ethtool_value edata = {ETHTOOL_GLINK};

            edata.data = dev->driver_info->check_connect(dev) == 0;
            if (copy_to_user(useraddr, &edata, sizeof(edata)))
            {
                return -EFAULT;
            }

            return 0;
        }

    case ETHTOOL_GMSGLVL:
    {
        struct ethtool_value edata = {ETHTOOL_GMSGLVL};
        edata.data = dev->msg_enable;
        if (copy_to_user(useraddr, &edata, sizeof (edata)))
        {
            return -EFAULT;
        }

        return 0;
    }
    case ETHTOOL_SMSGLVL:
    {
        struct ethtool_value edata;
        if (copy_from_user(&edata, useraddr, sizeof (edata)))
        {
            return -EFAULT;
        }

        dev->msg_enable = edata.data;
        return 0;
    }
    }

    return -EOPNOTSUPP;
}

/* <DTS2010121702766.QMI: gkf34687 2010-12-28 MODIFY BEGIN*/
int usbnet_ioctl(struct net_device *net, struct ifreq *rq, int cmd)
{
    struct usbnet		*dev = netdev_priv(net);
    struct net_device_stats *pstats;

    switch (cmd)
    {
    case SIOCGUSBNETSTATS:
        pstats = usbnet_get_stats(dev->net);
        memset(pstats,0,sizeof(struct net_device_stats));
        return 0;
    /* <DTS2012072102909 w00211169 2012-7-26 begin */
    case SIOCSUSBNET_PEER_HWADDR:
        /*
    	printk("usbnet_ioctl: %02x:%02x:%02x:%02x:%02x:%02x\n",
    			rq->ifr_hwaddr.sa_data[0],
    			rq->ifr_hwaddr.sa_data[1],
    			rq->ifr_hwaddr.sa_data[2],
    			rq->ifr_hwaddr.sa_data[3],
    			rq->ifr_hwaddr.sa_data[4],
    			rq->ifr_hwaddr.sa_data[5]);
	    */
        memcpy(dev->peer_addr, rq->ifr_hwaddr.sa_data, ETH_ALEN); 
        return 0;    
    /* DTS2012072102909 w00211169 2012-7-26 end> */        
    default:
        return -EOPNOTSUPP;
    }
}
/* DTS2010121702766.QMI: gkf34687 2010-12-28 MODIFY END>*/
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
/*-------------------------------------------------------------------------
 *
 * USB Device Driver support
 *
 *-------------------------------------------------------------------------*/

// precondition: never called in_interrupt

void usbnet_disconnect (struct usb_interface *intf)
{
	struct usbnet		*dev;
	struct usb_device	*xdev;
	struct net_device	*net;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
       exit_tty();
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	dev = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);
	if (!dev)
		return;

	xdev = interface_to_usbdev (intf);

	if (netif_msg_probe (dev))
		devinfo (dev, "unregister '%s' usb-%s-%s, %s",
			intf->dev.driver->name,
			xdev->bus->bus_name, xdev->devpath,
			dev->driver_info->description);


	//net = dev->net;
	if (dev->driver_info->unbind)
		dev->driver_info->unbind (dev, intf);

    /* <DTS2013101507084 j81004134 2013/10/19 begin */
    /* usb断开后，需要卸载设备 */
    netif_device_detach(g_usbnet);

	usb_put_dev (xdev);
    
    /* 标记usb断开标志 */
	g_usb_disconnected = 1;
	printk("[%s]:g_usb_disconnected=%d\n", __FUNCTION__ ,g_usb_disconnected);
    /* DTS2013101507084 j81004134 2013/10/19 end> */
}
EXPORT_SYMBOL_GPL(usbnet_disconnect);

static const struct net_device_ops usbnet_netdev_ops = {
	.ndo_open		= usbnet_open,
	.ndo_stop		= usbnet_stop,
	.ndo_start_xmit		= usbnet_start_xmit,
	.ndo_tx_timeout		= usbnet_tx_timeout,
	.ndo_change_mtu		= usbnet_change_mtu,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	/* <For adapter  w00211169 2012-6-25 begin */
	.ndo_get_stats      = usbnet_get_stats,
	/* For adapter  w00211169 2012-6-25 end> */
     /* <DTS2012072102909 w00211169 2012-7-31 begin */
    .ndo_do_ioctl       = usbnet_ioctl,   
    /* DTS2012072102909 w00211169 2012-7-31 end> */       
};

/*-------------------------------------------------------------------------*/

// precondition: never called in_interrupt

int
usbnet_probe (struct usb_interface *udev, const struct usb_device_id *prod)
{
	struct usbnet			*dev;
  /* <DTS2012110301136 l00205975 20121103 delete 1 line> */
	struct usb_host_interface	*interface;
	struct driver_info		*info;
	struct usb_device		*xdev;
	int				status;
	const char			*name;
	printk("[%s][%s][%d]pid=%d\r\n",__FILE__,__FUNCTION__,__LINE__,current->pid);
	name = udev->dev.driver->name;
	info = (struct driver_info *) prod->driver_info;
	if (!info) {
		dev_dbg (&udev->dev, "blacklisted by %s\n", name);
		return -ENODEV;
	}
	xdev = interface_to_usbdev (udev);
	interface = udev->cur_altsetting;

	usb_get_dev (xdev);
    interface = &udev->altsetting;
	status = -ENOMEM;

/* <DTS2012110301136 l00205975 20121103 begin */


	dev = netdev_priv(g_usbnet);
/* DTS2012110301136 l00205975 20121103 end> */  
	dev->udev = xdev;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
	dev->intf = udev;
#endif
	dev->driver_info = info;
#ifndef CONFIG_USB_QMITTY_MODULE
	dev->driver_name = name;
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	dev->msg_enable = netif_msg_init (msg_level, NETIF_MSG_DRV
				| NETIF_MSG_PROBE | NETIF_MSG_LINK);
	skb_queue_head_init (&dev->rxq);
	skb_queue_head_init (&dev->txq);
	skb_queue_head_init (&dev->done);
	dev->bh.func = usbnet_bh;
	dev->bh.data = (unsigned long) dev;
	INIT_WORK (&dev->kevent, kevent);
	dev->delay.function = usbnet_bh;
	dev->delay.data = (unsigned long) dev;
	init_timer (&dev->delay);
	mutex_init (&dev->phy_mutex);
  /* <DTS2012110301136 l00205975 20121103 begin */
	dev->net = g_usbnet;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
	strcpy (g_usbnet->name, "usb%d");
	memcpy (g_usbnet->dev_addr, node_id, sizeof node_id);
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
  /* DTS2012110301136 l00205975 20121103 end> */
	/* rx and tx sides can use different message sizes;
	 * bind() should set rx_urb_size in that case.
	 */
  /* <DTS2012110301136 l00205975 20121103 begin */
	dev->hard_mtu = g_usbnet->mtu + g_usbnet->hard_header_len;
#if 0
// dma_supported() is deeply broken on almost all architectures
	// possible with some EHCI controllers
	if (dma_supported (&udev->dev, DMA_BIT_MASK(64)))
		g_usbnet->features |= NETIF_F_HIGHDMA;
#endif

	g_usbnet->netdev_ops = &usbnet_netdev_ops;
#ifdef CONFIG_COMPAT_NET_DEV_OPS
	g_usbnet->hard_start_xmit = usbnet_start_xmit;
	g_usbnet->open = usbnet_open;
	g_usbnet->stop = usbnet_stop;
	g_usbnet->tx_timeout = usbnet_tx_timeout;
#endif
	g_usbnet->watchdog_timeo = TX_TIMEOUT_JIFFIES;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
    g_usbnet->do_ioctl = usbnet_ioctl;  /* <DTS2010121702766.QMI: gkf34687 2010-12-28 MODIFY>*/
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	g_usbnet->ethtool_ops = &usbnet_ethtool_ops;
  /* DTS2012110301136 l00205975 20121103 end> */
	// allow device-specific bind/init procedures
	// NOTE net->name still not usable ...
	if (info->bind) {
		status = info->bind (dev, udev);
		if (status < 0)
			goto out1;
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
		// heuristic:  "usb%d" for links we know are two-host,
		// else "eth%d" when there's reasonable doubt.  userspace
		// can rename the link if it knows better.
    /* <DTS2012110301136 l00205975 20121103 begin */
		if ((dev->driver_info->flags & FLAG_ETHER) != 0
				&& (g_usbnet->dev_addr [0] & 0x02) == 0)
			strcpy (g_usbnet->name, "eth%d");
		/* WLAN devices should always be named "wlan%d" */
		if ((dev->driver_info->flags & FLAG_WLAN) != 0)
			strcpy(g_usbnet->name, "wlan%d");
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
		/* maybe the remote can't receive an Ethernet MTU */
		if (g_usbnet->mtu > (dev->hard_mtu - g_usbnet->hard_header_len))
			g_usbnet->mtu = dev->hard_mtu - g_usbnet->hard_header_len;
	} else if (!info->in || !info->out)
		status = usbnet_get_endpoints (dev, udev);
	else {
		dev->in = usb_rcvbulkpipe (xdev, info->in);
		dev->out = usb_sndbulkpipe (xdev, info->out);
		if (!(info->flags & FLAG_NO_SETINT))
			status = usb_set_interface (xdev,
				interface->desc.bInterfaceNumber,
				interface->desc.bAlternateSetting);
		else
			status = 0;

	}
    /* DTS2012110301136 l00205975 20121103 end> */
	if (status >= 0 && dev->status)
		status = init_status (dev, udev);
	if (status < 0)
		goto out3;
		
    /* <DTS2012091310003  z00159386  20120920  begin */
	dev->rx_urb_size = 2048;
    /* DTS2012091310003  z00159386  20120920  end> */
	
	dev->maxpacket = usb_maxpacket (dev->udev, dev->out, 1);
  /* <DTS2012110301136 l00205975 20121103 begin */
	SET_NETDEV_DEV(g_usbnet, &udev->dev);

	if (netif_msg_probe (dev))
		devinfo (dev, "register '%s' at usb-%s-%s, %s, %pM",
			udev->dev.driver->name,
			xdev->bus->bus_name, xdev->devpath,
			dev->driver_info->description,
			g_usbnet->dev_addr);

	// ok, it's ready to go.
	usb_set_intfdata (udev, dev);
	printk("[%s][%s][%d]\r\n",__FILE__,__FUNCTION__,__LINE__);
	// start as if the link is up
	netif_device_attach (g_usbnet);
	g_usb_disconnected = 0; /* DTS2013101507084 j81004134 2013/10/19 */
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifdef CONFIG_USB_QMITTY_MODULE
       trigger_tty();
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
	return 0;

out3:
	if (info->unbind)
		info->unbind (dev, udev);
out1:
	printk("[%s][%s][%d]free_netdev(net)\r\n",__FILE__,__FUNCTION__,__LINE__);
	free_netdev(g_usbnet);
out:
	usb_put_dev(xdev);
  /* DTS2012110301136 l00205975 20121103 end> */
	return status;
}
EXPORT_SYMBOL_GPL(usbnet_probe);

/*-------------------------------------------------------------------------*/

/*
 * suspend the whole driver as soon as the first interface is suspended
 * resume only when the last interface is resumed
 */

int usbnet_suspend (struct usb_interface *intf, pm_message_t message)
{
	struct usbnet		*dev = usb_get_intfdata(intf);

	if (!dev->suspend_count++) {
		/*
		 * accelerate emptying of the rx and queues, to avoid
		 * having everything error out.
		 */
		netif_device_detach (dev->net);
		(void) unlink_urbs (dev, &dev->rxq);
		(void) unlink_urbs (dev, &dev->txq);
		/*
		 * reattach so runtime management can use and
		 * wake the device
		 */
		netif_device_attach (dev->net);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_suspend);

int usbnet_resume (struct usb_interface *intf)
{
	struct usbnet		*dev = usb_get_intfdata(intf);

	if (!--dev->suspend_count)
		tasklet_schedule (&dev->bh);

	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_resume);


/*-------------------------------------------------------------------------*/

static int __init usbnet_init(void)
{
  /* <DTS2012110301136 l00205975 20121103 begin */
	struct usbnet			*dev;
	int 			status;
  /* DTS2012110301136 l00205975 20121103 end> */
	/* compiler should optimize this out */
	BUILD_BUG_ON (sizeof (((struct sk_buff *)0)->cb)
			< sizeof (struct skb_data));

	random_ether_addr(node_id);
	

/* <DTS2012110301136 l00205975 20121103 begin */
		// set up our own records
#ifdef CONFIG_USB_QMITTY_MODULE
		g_usbnet = alloc_netdev(sizeof(*dev), "rmnet%d", ether_setup);
#else
		g_usbnet = alloc_etherdev(sizeof(*dev));
#endif

		if (!g_usbnet) {
			dbg ("can't kmalloc dev");
			return -ENOMEM;
		}		
	status=register_netdev (g_usbnet);
		if (status){
			dbg ("can't register dev");
			return status;
		}
/* DTS2012110301136 l00205975 20121103 end> */
	return 0;
}
module_init(usbnet_init);

static void __exit usbnet_exit(void)
{

usb_dev_recv_dereg();

unregister_netdev (g_usbnet);
/* we don't hold rtnl here ... */
flush_scheduled_work ();
free_netdev(g_usbnet);


}
module_exit(usbnet_exit);

MODULE_AUTHOR("David Brownell");
MODULE_DESCRIPTION("USB network driver framework");
MODULE_LICENSE("GPL");
