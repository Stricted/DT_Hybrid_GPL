/*
 *  linux/drivers/net/netconsole.c
 *
 *  Copyright (C) 2001  Ingo Molnar <mingo@redhat.com>
 *
 *  This file contains the implementation of an IRQ-safe, crash-safe
 *  kernel console implementation that outputs kernel messages to the
 *  network.
 *
 * Modification history:
 *
 * 2001-09-17    started by Ingo Molnar.
 * 2003-08-11    2.6 port by Matt Mackall
 *               simplified options
 *               generic card hooks
 *               works non-modular
 * 2003-09-07    rewritten with netpoll api
 */

/****************************************************************
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2, or (at your option)
 *      any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ****************************************************************/

#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/string.h>
#include <linux/inet.h>
#include <linux/configfs.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/skbuff.h>

MODULE_AUTHOR("Huawei: s00175839 <bb.sunbo@huawei.com>");
MODULE_DESCRIPTION("Console driver for atp");
MODULE_LICENSE("GPL");

#include "msg/kcmsmonitormsgtypes.h"


static struct sock *atpcon_nl_sock = NULL;
static int us_pid = -1;

#define MAX_PRINT_CHUNK		1024

static DEFINE_SPINLOCK(atpcon_lock);

int atpcon_nl_send_msg(const char *msg, unsigned int msglen)
{
    struct sk_buff *skb;
    struct generic_nl_msg *body_msg;
    struct nlmsghdr *nlh;
    static int seq = 0;
    int size;

    if (-1 == us_pid)
    {
        return -EBADF;
    }

    if (msglen > MAX_PRINT_CHUNK)
    {
        msglen = MAX_PRINT_CHUNK;
    }
    if ((NULL == msg) || (0 == msglen))
    {
        size = NLMSG_SPACE(sizeof(*body_msg));
    }
    else 
    {
        size = NLMSG_SPACE(sizeof(*body_msg) + msglen);
    }

    skb = alloc_skb(size, GFP_ATOMIC);
    if (!skb)
    {
        return -ENOMEM;
    }

    nlh = NLMSG_PUT(skb, KERNEL_PID, seq++, NLMSG_DONE, size - sizeof(*nlh));
    body_msg = (struct generic_nl_msg *)NLMSG_DATA(nlh);

    memset(body_msg, 0, size - sizeof(*nlh)); 

    if ((NULL != msg) && (msglen > 0))
    {
        memcpy(body_msg->data, msg, msglen);
        body_msg->len = msglen;
    }
    else 
    {
        body_msg->len = 0;
    }

    NETLINK_CB(skb).dst_group = SYSWATCH_USERSPACE_GROUP;

    netlink_unicast(atpcon_nl_sock, skb, us_pid, 0);

    return 0;
nlmsg_failure:
    //return netlink_broadcast(syswatch_nl_sock, skb, 0, SYSWATCH_USERSPACE_GROUP, GFP_ATOMIC);
    
    kfree_skb(skb);
    return -1;
}

static void atpcon_nl_rcv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    unsigned long flags;

    if (NULL == skb)
    {
        return;
    }

    spin_lock_irqsave(&atpcon_lock, flags);
    nlh = (struct nlmsghdr*)skb->data;
    us_pid = nlh->nlmsg_pid;
    spin_unlock_irqrestore(&atpcon_lock, flags);
}

static int atpcon_netlink_init(void)
{
    atpcon_nl_sock = netlink_kernel_create(&init_net, NETLINK_ATP_CONSOLE, 0, 
        atpcon_nl_rcv_msg, NULL, THIS_MODULE);
    
    if (!atpcon_nl_sock)
    {
        return -1;
    }

    return 0;
}

static void atpcon_netlink_exit(void)
{
    if (atpcon_nl_sock)
    {
        netlink_kernel_release(atpcon_nl_sock);
        atpcon_nl_sock = NULL;
    }
    
    return;
}

static void write_msg(struct console *con, const char *msg, unsigned int len)
{
	int frag, left;
	unsigned long flags;
	const char *tmp;

	tmp = msg;
    spin_lock_irqsave(&atpcon_lock, flags);
	for (left = len; left;) {
		frag = min(left, MAX_PRINT_CHUNK);
        atpcon_nl_send_msg(tmp, frag);
		tmp += frag;
		left -= frag;
	}    
    spin_unlock_irqrestore(&atpcon_lock, flags);
}

static struct console netconsole = {
	.name	= "atpcon",
	.flags	= CON_ENABLED,
	.write	= write_msg,
};

static int __init init_netconsole(void)
{
	int err = 0;
	struct netconsole_target *nt, *tmp;
	unsigned long flags;
	char *target_config;

    err = atpcon_netlink_init();
    if (0 != err)
    {
        return err;
    }
    
	register_console(&netconsole);

	return err;

}

static void __exit cleanup_netconsole(void)
{
	unregister_console(&netconsole);
    atpcon_netlink_exit();
}

module_init(init_netconsole);
module_exit(cleanup_netconsole);
