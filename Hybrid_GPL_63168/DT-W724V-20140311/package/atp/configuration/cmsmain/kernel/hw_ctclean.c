/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : hw_ctclean.c
  作    者  : l00180987
  版    本  : a
  创建日期  : 2013-6-14
  描    述  : 连接跟踪删除内核netlink处理模块
  函数列表  :
                hw_ctclean_add_ct_to_del_list
                hw_ctclean_check_ct
                hw_ctclean_del_ct_by_tuple
                hw_ctclean_do_delete
                hw_ctclean_find_ct
                hw_ctclean_process
                hw_ctclean_recv_msg
                hw_ctclean_send_msg

  历史记录      :
   1.日    期   : 2013-6-14
     作    者   : l00180987
     修改内容   : 完成初稿

*********************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <net/sock.h>
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/string.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>


#include "msg/kcmsmonitormsgtypes.h"

#include "atp_interface.h"

extern struct net init_net;

extern void rt_cache_flush(struct net *net, int delay);
extern void death_by_timeout(unsigned long ul_conntrack);

static struct nf_conn *match_ct_list[MAX_RULE_NUM]; 


/******************************************************************************
  函数名称  : ip_match_with_dev
  功能描述  : 检查ip地址是否和网关本地ip地址匹配
  输入参数  : 
              1.  ip_addr:待检查的ip地址
  调用函数  : kill_all_not_match
  被调函数  : 
  输出参数  : 
  返 回 值  : 0:ip地址不是网关本地地址；1:ip是网关本地地址

  修改历史      :
   1.日    期   : 2013-7-27
     作    者   : l00180987
     修改内容   : 完成初稿

******************************************************************************/
static int ip_match_with_dev(__be32 ip_addr)
{
	struct net *net = &init_net;
	struct net_device *dev;
	struct in_device *in_dev;
	struct in_ifaddr *ifa;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		in_dev = __in_dev_get_rtnl(dev);
		if (NULL == in_dev) {
			continue;
		}

		for_ifa(in_dev) {
			if (ip_addr == ifa->ifa_local) {
			    rcu_read_unlock();
				return 1;
			}
		}
		endfor_ifa(in_dev);
	}
	rcu_read_unlock();
	return 0;
}



/******************************************************************************
  函数名称  : kill_all_not_match
  功能描述  : 删除所有与网关local地址不匹配的连接，wan up时调用, 判断函数
  输入参数  : 
              1.  *i:连接跟踪
              2.  *data:不使用
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 0:连接跟踪是网关本地收发或做过nat的，wan up时不删除
              1:连接跟踪是nat规则加入之前建立的，wan up需要删除

  修改历史      :
   1.日    期   : 2013-7-27
     作    者   : l00180987
     修改内容   : 完成初稿

******************************************************************************/
static int kill_all_not_match(struct nf_conn *i, void *data)
{
	if (NULL == i)
	{
		return 0;
	}

    /* start DTS2014032407709 IPv6 ct not clean by f00110348 */
    if (AF_INET6 == nf_ct_l3num(i))
	{
		return 0;
	}    
    /* end DTS2014032407709 IPv6 ct not clean by f00110348 */
    
	/*Reply 的目的匹配成功情形最多，优先判断*/
	if (ip_match_with_dev(i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
	{
		return 0;
	}

	if (ip_match_with_dev(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip))
	{
		return 0;
	}

	if (ip_match_with_dev(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip))
	{
		return 0;
	}

	if (ip_match_with_dev(i->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip))
	{
		return 0;
	}

	pr_debug("ct [%p] ORIGINAL src ip ["NIPQUAD_FMT"] dst ip ["NIPQUAD_FMT"] src port [%d] dst port [%d] \
		REPLY src ip ["NIPQUAD_FMT"] dst ip ["NIPQUAD_FMT"] src port [%d] dst port [%d]  will be deleted\n",
	i,
	NIPQUAD(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip),
	NIPQUAD(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip),
	i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all,
	i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all,
	NIPQUAD(i->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip),
	NIPQUAD(i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip),
	i->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all,
	i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all);

	return 1;
}


/******************************************************************************
  函数名称： hw_ctclean_del_ct
  功能描述： 删除特定的一条连接跟踪
  调用函数： hw_ctclean_do_delete
  被调函数：
  输入参数：
  输出参数：
  返 回 值：
  其它说明：
******************************************************************************/
static void hw_ctclean_del_ct(struct nf_conn *ct)
{
    if (del_timer(&ct->timeout))
    {
		death_by_timeout((unsigned long)ct);
	}
}

/******************************************************************************
  函数名称： hw_ctclean_do_delete
  功能描述： 删除所有找到的连接跟踪
  调用函数： hw_ctclean_process
  被调函数：
  输入参数：
  输出参数：
  返 回 值：
  其它说明：
******************************************************************************/
static void hw_ctclean_do_delete(int num)
{
    int index = 0;

    while (index < num && match_ct_list[index] != NULL)
    {       
        hw_ctclean_del_ct(match_ct_list[index]);
        match_ct_list[index] = NULL;
        index++;
    }

    return;
}

static ipv6_mask_addr_cmp(const struct in6_addr_ex *a1, const struct in6_addr_ex *m,
		     const struct in6_addr *a2)
{
	return !!(((a1->u6_addr32[0] ^ a2->s6_addr32[0]) & m->u6_addr32[0]) |
		  ((a1->u6_addr32[1] ^ a2->s6_addr32[1]) & m->u6_addr32[1]) |
		  ((a1->u6_addr32[2] ^ a2->s6_addr32[2]) & m->u6_addr32[2]) |
		  ((a1->u6_addr32[3] ^ a2->s6_addr32[3]) & m->u6_addr32[3]));
}



/******************************************************************************
  函数名称： hw_ctclean_check_ct
  功能描述： 用于判断是否是需要的连接跟踪
  调用函数：find_need_ct
  被调函数：
  输入参数：ATP_CT_RULE_INFO* pstRuleInfo 用户态传入数据，
            struct nf_conn *ct 待比较的连接跟踪,
  输出参数：
  返 回 值：是需要的连接跟踪，返回true，否则返回false
  其它说明：
******************************************************************************/
static int hw_ctclean_check_ct(const ATP_CT_RULE_INFO* pstRuleInfo, struct nf_conn *ct)
{
    if ((NULL == pstRuleInfo) || (NULL == ct))
    {
        return false;
    }    

    /*只处理lan侧发起的连接跟踪*/
    if ((ct->mark & PPP_TRIGER_MARK) != PPP_TRIGER_MARK)
    {
        return false;
    }

    /*Start of Protocol 2014-4-17 16:30 for Filterilst的及时生效 by t00189036*/
    /*指定bypass，但连接跟踪不是在bypass上的，不清*/
    if ((CT_BYPASS == pstRuleInfo->enCtType)
        && (!(ct->mark & ATP_CONNTRACK_BYPASS_MARK)))
    {
        return false;
    }

    /*指定bonding，但连接跟踪不是在bonding上的，不清*/
    if ((CT_BONDING == pstRuleInfo->enCtType)
        && (!(ct->mark & ATP_CONNTRACK_TUNNEL_MARK)))
    {
        return false;
    }
    /*End of Protocol 2014-4-17 16:30 for by t00189036*/

    /*判断协议类型*/
    if ((pstRuleInfo->ucProtocol)
    && (pstRuleInfo->ucProtocol != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum))
    {
        return false;
    }

    /*判断目的IP*/
    if ((pstRuleInfo->ulDstIp)
    && ((pstRuleInfo->ulDstIp & pstRuleInfo->ulDstMask) != (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip & pstRuleInfo->ulDstMask)))
    {
        return false;
    }
    
    /*判断目的端口*/
    if ((pstRuleInfo->usDstPort)
    && (pstRuleInfo->usDstPort != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port))
    {
        return false;
    }

    /*判断源IP*/
    if ((pstRuleInfo->ulSrcIp)
    && ((pstRuleInfo->ulSrcIp & pstRuleInfo->ulSrcMask) != (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip & pstRuleInfo->ulSrcMask)))
    {
        return false;
    }

    /*判断源端口*/
    if ((pstRuleInfo->usSrcPort)
    && (pstRuleInfo->usSrcPort != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port))
    {
        return false;
    }

    /*判断ipv6目的地址*/
    if ((pstRuleInfo->stDstIp6.u6_addr32[0] + pstRuleInfo->stDstIp6.u6_addr32[1] + pstRuleInfo->stDstIp6.u6_addr32[2] + pstRuleInfo->stDstIp6.u6_addr32[3])
    && (0 != ipv6_mask_addr_cmp(&pstRuleInfo->stDstIp6, &pstRuleInfo->stDstMask6, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6)))
    {
        return false;
    }

    /*判断ipv6源地址*/
    if ((pstRuleInfo->stSrcIp6.u6_addr32[0] + pstRuleInfo->stSrcIp6.u6_addr32[1] + pstRuleInfo->stSrcIp6.u6_addr32[2] + pstRuleInfo->stSrcIp6.u6_addr32[3])
    && (0 != ipv6_mask_addr_cmp(&pstRuleInfo->stSrcIp6, &pstRuleInfo->stSrcMask6, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.in6)))
    {
        return false;
    }    

    /*判断dscp值，dscp为0也是合法的值*/ 
    if ((pstRuleInfo->ucDscp << DSCP_MARK_OFFSET) != ((pstRuleInfo->ucDscp << DSCP_MARK_OFFSET) & ct->mark))
    {
        return false;
    }
    return true;
}

/******************************************************************************
  函数名称： hw_ctclean_add_ct_to_del_list
  功能描述： 将查找到的连接跟踪加入待删除列表。
  调用函数：find_need_ct
  被调函数：
  输入参数：int *ct_num 当前的连接跟踪总条数, struct nf_conn *ct 连接跟踪
  输出参数：int *ct_num 加入后当前的连接跟踪总条数
  返 回 值：
  其它说明：
******************************************************************************/
static void hw_ctclean_add_ct_to_del_list(int *ct_num, struct nf_conn *ct)
{
    int ct_search_index = 0;
    int repeat_flag = 0; /*用于标记是否已存储相同的连接跟踪记录*/

    if (NULL == ct_num)
    {
        return;
    }

    if (NULL == ct)
    {
        return;
    }

    if (MAX_RULE_NUM <= *ct_num)
    {
        printk("ct num hit max [%d]\n", MAX_RULE_NUM);
        return;
    }

    while (ct_search_index < MAX_RULE_NUM && NULL != match_ct_list[ct_search_index])
    {
        if (match_ct_list[ct_search_index] == ct)
        {
            /*已记录此连接跟踪*/
            repeat_flag = 1;
            break;
        }
        ct_search_index++;
    }

    /*如果之前并没添加此连接跟踪，则添加，否则跳过添加*/
    if (0 == repeat_flag) 
    {
        match_ct_list[*ct_num] = ct;
        printk("find conntrack[%d]=%p\n", *ct_num, ct);
        (*ct_num)++;
    }
    
    return;
}


/******************************************************************************
  函数名称： hw_ctclean_find_ct
  功能描述： 用于查找符合条件的连接跟踪
  调用函数：
  被调函数：
  输入参数：ATP_CT_RULE_INFO* pstCtRule 数据包含规则信息
  输出参数：
  返 回 值：找到的连接跟踪数目
  其它说明：
******************************************************************************/
static int hw_ctclean_find_ct(struct net *net, const ATP_CT_RULE_INFO* pstCtRule)
{
    struct nf_conntrack_tuple_hash *h = NULL;
    struct hlist_nulls_node *n = NULL;
    struct nf_conn *ct = NULL;
    unsigned int hash = 0;
    unsigned int num = 0;
    
    int ct_num = 0; /*用于统计记录的连接跟踪条数*/
    int main_ct_num = 0; /*查找到的主连接跟踪条数*/
    int rule_num = 0;

    /*Start of Protocol 2014-4-17 16:30 for Filterilst的及时生效 by t00189036*/
    if ((NULL == net) || (NULL == pstCtRule) || (CT_NONE == pstCtRule->enCtType))
    {
        return num;
    }
    /*End of Protocol 2014-4-17 16:30 for by t00189036*/

    memset(match_ct_list, 0, sizeof(match_ct_list));

    spin_lock_bh(&nf_conntrack_lock);

    /*查找主连接跟踪记录*/
    for (hash = 0; hash < nf_conntrack_htable_size; hash++)
    {
        hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) 
        {
            ct = nf_ct_tuplehash_to_ctrack(h);

            if (hw_ctclean_check_ct(pstCtRule, ct))
            {
                hw_ctclean_add_ct_to_del_list(&ct_num, ct);
                num++;
            }
        }
    }

    main_ct_num = ct_num;

    /*根据主连接跟踪查找所有的预期连接跟踪*/
    for (hash = 0; hash < nf_conntrack_htable_size; hash++)
    {
        hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) 
        {
            ct = nf_ct_tuplehash_to_ctrack(h);
            rule_num = 0;
            
            while (ct_num < MAX_RULE_NUM && rule_num < main_ct_num)
            {
                if (NULL != ct->master && ct->master == match_ct_list[rule_num])
                {
                    hw_ctclean_add_ct_to_del_list(&ct_num, ct);
                    num++;
                }
                rule_num++;
            }
        }
    }
    spin_unlock_bh(&nf_conntrack_lock);
    return num;
}


/******************************************************************************
  函数名称： hw_ctclean_recv_msg
  功能描述： 校验报文，合法将接收消息内容拷入pstRuleInfo
  调用函数：
  被调函数：
  输入参数：struct sock *sk, u32 *pid, ATP_CT_RULE_INFO *pstRuleInfo
  输出参数：ATP_CT_RULE_INFO *pstRuleInfo
  返 回 值：0表示正确，-1表示错误
  其它说明：
******************************************************************************/
static int hw_ctclean_recv_msg(struct sk_buff *skb, u32 *pid, ATP_CT_RULE_INFO *pstRuleInfo)
{
    struct nlmsghdr *nlhdr = NULL;

    if (NULL == skb)
    {
        return -1;
    }

    if (NULL == pid)
    {
        return -1;
    }

    if (NULL == pstRuleInfo)
    {
        return -1;
    }

    nlhdr = (struct nlmsghdr *)skb->data;

    if (skb->len < NLMSG_SPACE(0))
    {
        kfree_skb(skb);
        return -1;
    }

    if (skb->len < nlhdr->nlmsg_len)
    {
        kfree_skb(skb);
        return -1;
    }

    if (nlhdr->nlmsg_len < NLMSG_LENGTH(sizeof(ATP_CT_RULE_INFO)))
    {
        kfree_skb(skb);
        return -1;
    }
    
    memcpy(pstRuleInfo, NLMSG_DATA(nlhdr), sizeof(ATP_CT_RULE_INFO));
    
    *pid = nlhdr->nlmsg_pid;

    return 0;
}

/******************************************************************************
  函数名称： hw_ctclean_process
  功能描述： 发送消息
  调用函数：
  被调函数：nl_recv_msg， nl_send_msg， hw_ctclean_find_ct
  输入参数：struct sock *sk, int len
  输出参数：
  返 回 值：
  其它说明：
******************************************************************************/
void hw_ctclean_process(struct sk_buff *skb)
{
    u32 pid;
    ATP_CT_RULE_INFO stCtRuleInfo;
    int num = 0;

    memset(&stCtRuleInfo, 0, sizeof(ATP_CT_RULE_INFO));

    printk("[%s,%d]hw_ctclean_process recv msg!\n", __func__, __LINE__);

    /*接收规则信息，存入stCtRuleInfo*/
    if (-1 == hw_ctclean_recv_msg(skb, &pid, &stCtRuleInfo))
    {
        printk("hw_ctclean_process recv msg fail!\n");
        return;
    }

    printk("[%s,%d]hw_ctclean_process recv msg!\n", __func__, __LINE__);
    printk("dst ip %X, mask %X, port is %d\n", stCtRuleInfo.ulDstIp, stCtRuleInfo.ulDstMask, stCtRuleInfo.usDstPort);
    printk("src ip %X, mask %X, port is %d\n", stCtRuleInfo.ulSrcIp, stCtRuleInfo.ulSrcMask, stCtRuleInfo.usSrcPort);
    printk("Protocol %d, Dscp %d\n", stCtRuleInfo.ucProtocol, stCtRuleInfo.ucDscp);

    /*用特殊protocol==255代替以前用echo 36 > ip_ct_clean清连接的方式*/
    if (NAT_MAX_PROTOCOL == stCtRuleInfo.ucProtocol)
    {
        printk("[%s,%d]hw_ctclean_process recv protocol 255 !\n", __func__, __LINE__);
        nf_ct_iterate_cleanup(&init_net, kill_all_not_match, NULL);
        return;
    }

    /*找到所有匹配的连接跟踪*/
    if (0 == (num = hw_ctclean_find_ct(&init_net, &stCtRuleInfo)))
    {
        return;
    }

    /*删除前先清空路由缓存*/
    rt_cache_flush(&init_net, 0);

    /*删除所有找到的连接跟踪*/
    hw_ctclean_do_delete(num);

    return;     
}
