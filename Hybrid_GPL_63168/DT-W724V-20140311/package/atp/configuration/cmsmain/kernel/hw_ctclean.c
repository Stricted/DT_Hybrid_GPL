/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : hw_ctclean.c
  ��    ��  : l00180987
  ��    ��  : a
  ��������  : 2013-6-14
  ��    ��  : ���Ӹ���ɾ���ں�netlink����ģ��
  �����б�  :
                hw_ctclean_add_ct_to_del_list
                hw_ctclean_check_ct
                hw_ctclean_del_ct_by_tuple
                hw_ctclean_do_delete
                hw_ctclean_find_ct
                hw_ctclean_process
                hw_ctclean_recv_msg
                hw_ctclean_send_msg

  ��ʷ��¼      :
   1.��    ��   : 2013-6-14
     ��    ��   : l00180987
     �޸�����   : ��ɳ���

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
  ��������  : ip_match_with_dev
  ��������  : ���ip��ַ�Ƿ�����ر���ip��ַƥ��
  �������  : 
              1.  ip_addr:������ip��ַ
  ���ú���  : kill_all_not_match
  ��������  : 
  �������  : 
  �� �� ֵ  : 0:ip��ַ�������ر��ص�ַ��1:ip�����ر��ص�ַ

  �޸���ʷ      :
   1.��    ��   : 2013-7-27
     ��    ��   : l00180987
     �޸�����   : ��ɳ���

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
  ��������  : kill_all_not_match
  ��������  : ɾ������������local��ַ��ƥ������ӣ�wan upʱ����, �жϺ���
  �������  : 
              1.  *i:���Ӹ���
              2.  *data:��ʹ��
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : 0:���Ӹ��������ر����շ�������nat�ģ�wan upʱ��ɾ��
              1:���Ӹ�����nat�������֮ǰ�����ģ�wan up��Ҫɾ��

  �޸���ʷ      :
   1.��    ��   : 2013-7-27
     ��    ��   : l00180987
     �޸�����   : ��ɳ���

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
    
	/*Reply ��Ŀ��ƥ��ɹ�������࣬�����ж�*/
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
  �������ƣ� hw_ctclean_del_ct
  ���������� ɾ���ض���һ�����Ӹ���
  ���ú����� hw_ctclean_do_delete
  ����������
  ���������
  ���������
  �� �� ֵ��
  ����˵����
******************************************************************************/
static void hw_ctclean_del_ct(struct nf_conn *ct)
{
    if (del_timer(&ct->timeout))
    {
		death_by_timeout((unsigned long)ct);
	}
}

/******************************************************************************
  �������ƣ� hw_ctclean_do_delete
  ���������� ɾ�������ҵ������Ӹ���
  ���ú����� hw_ctclean_process
  ����������
  ���������
  ���������
  �� �� ֵ��
  ����˵����
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
  �������ƣ� hw_ctclean_check_ct
  ���������� �����ж��Ƿ�����Ҫ�����Ӹ���
  ���ú�����find_need_ct
  ����������
  ���������ATP_CT_RULE_INFO* pstRuleInfo �û�̬�������ݣ�
            struct nf_conn *ct ���Ƚϵ����Ӹ���,
  ���������
  �� �� ֵ������Ҫ�����Ӹ��٣�����true�����򷵻�false
  ����˵����
******************************************************************************/
static int hw_ctclean_check_ct(const ATP_CT_RULE_INFO* pstRuleInfo, struct nf_conn *ct)
{
    if ((NULL == pstRuleInfo) || (NULL == ct))
    {
        return false;
    }    

    /*ֻ����lan�෢������Ӹ���*/
    if ((ct->mark & PPP_TRIGER_MARK) != PPP_TRIGER_MARK)
    {
        return false;
    }

    /*Start of Protocol 2014-4-17 16:30 for Filterilst�ļ�ʱ��Ч by t00189036*/
    /*ָ��bypass�������Ӹ��ٲ�����bypass�ϵģ�����*/
    if ((CT_BYPASS == pstRuleInfo->enCtType)
        && (!(ct->mark & ATP_CONNTRACK_BYPASS_MARK)))
    {
        return false;
    }

    /*ָ��bonding�������Ӹ��ٲ�����bonding�ϵģ�����*/
    if ((CT_BONDING == pstRuleInfo->enCtType)
        && (!(ct->mark & ATP_CONNTRACK_TUNNEL_MARK)))
    {
        return false;
    }
    /*End of Protocol 2014-4-17 16:30 for by t00189036*/

    /*�ж�Э������*/
    if ((pstRuleInfo->ucProtocol)
    && (pstRuleInfo->ucProtocol != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum))
    {
        return false;
    }

    /*�ж�Ŀ��IP*/
    if ((pstRuleInfo->ulDstIp)
    && ((pstRuleInfo->ulDstIp & pstRuleInfo->ulDstMask) != (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip & pstRuleInfo->ulDstMask)))
    {
        return false;
    }
    
    /*�ж�Ŀ�Ķ˿�*/
    if ((pstRuleInfo->usDstPort)
    && (pstRuleInfo->usDstPort != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port))
    {
        return false;
    }

    /*�ж�ԴIP*/
    if ((pstRuleInfo->ulSrcIp)
    && ((pstRuleInfo->ulSrcIp & pstRuleInfo->ulSrcMask) != (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip & pstRuleInfo->ulSrcMask)))
    {
        return false;
    }

    /*�ж�Դ�˿�*/
    if ((pstRuleInfo->usSrcPort)
    && (pstRuleInfo->usSrcPort != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port))
    {
        return false;
    }

    /*�ж�ipv6Ŀ�ĵ�ַ*/
    if ((pstRuleInfo->stDstIp6.u6_addr32[0] + pstRuleInfo->stDstIp6.u6_addr32[1] + pstRuleInfo->stDstIp6.u6_addr32[2] + pstRuleInfo->stDstIp6.u6_addr32[3])
    && (0 != ipv6_mask_addr_cmp(&pstRuleInfo->stDstIp6, &pstRuleInfo->stDstMask6, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6)))
    {
        return false;
    }

    /*�ж�ipv6Դ��ַ*/
    if ((pstRuleInfo->stSrcIp6.u6_addr32[0] + pstRuleInfo->stSrcIp6.u6_addr32[1] + pstRuleInfo->stSrcIp6.u6_addr32[2] + pstRuleInfo->stSrcIp6.u6_addr32[3])
    && (0 != ipv6_mask_addr_cmp(&pstRuleInfo->stSrcIp6, &pstRuleInfo->stSrcMask6, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.in6)))
    {
        return false;
    }    

    /*�ж�dscpֵ��dscpΪ0Ҳ�ǺϷ���ֵ*/ 
    if ((pstRuleInfo->ucDscp << DSCP_MARK_OFFSET) != ((pstRuleInfo->ucDscp << DSCP_MARK_OFFSET) & ct->mark))
    {
        return false;
    }
    return true;
}

/******************************************************************************
  �������ƣ� hw_ctclean_add_ct_to_del_list
  ���������� �����ҵ������Ӹ��ټ����ɾ���б�
  ���ú�����find_need_ct
  ����������
  ���������int *ct_num ��ǰ�����Ӹ���������, struct nf_conn *ct ���Ӹ���
  ���������int *ct_num �����ǰ�����Ӹ���������
  �� �� ֵ��
  ����˵����
******************************************************************************/
static void hw_ctclean_add_ct_to_del_list(int *ct_num, struct nf_conn *ct)
{
    int ct_search_index = 0;
    int repeat_flag = 0; /*���ڱ���Ƿ��Ѵ洢��ͬ�����Ӹ��ټ�¼*/

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
            /*�Ѽ�¼�����Ӹ���*/
            repeat_flag = 1;
            break;
        }
        ct_search_index++;
    }

    /*���֮ǰ��û��Ӵ����Ӹ��٣�����ӣ������������*/
    if (0 == repeat_flag) 
    {
        match_ct_list[*ct_num] = ct;
        printk("find conntrack[%d]=%p\n", *ct_num, ct);
        (*ct_num)++;
    }
    
    return;
}


/******************************************************************************
  �������ƣ� hw_ctclean_find_ct
  ���������� ���ڲ��ҷ������������Ӹ���
  ���ú�����
  ����������
  ���������ATP_CT_RULE_INFO* pstCtRule ���ݰ���������Ϣ
  ���������
  �� �� ֵ���ҵ������Ӹ�����Ŀ
  ����˵����
******************************************************************************/
static int hw_ctclean_find_ct(struct net *net, const ATP_CT_RULE_INFO* pstCtRule)
{
    struct nf_conntrack_tuple_hash *h = NULL;
    struct hlist_nulls_node *n = NULL;
    struct nf_conn *ct = NULL;
    unsigned int hash = 0;
    unsigned int num = 0;
    
    int ct_num = 0; /*����ͳ�Ƽ�¼�����Ӹ�������*/
    int main_ct_num = 0; /*���ҵ��������Ӹ�������*/
    int rule_num = 0;

    /*Start of Protocol 2014-4-17 16:30 for Filterilst�ļ�ʱ��Ч by t00189036*/
    if ((NULL == net) || (NULL == pstCtRule) || (CT_NONE == pstCtRule->enCtType))
    {
        return num;
    }
    /*End of Protocol 2014-4-17 16:30 for by t00189036*/

    memset(match_ct_list, 0, sizeof(match_ct_list));

    spin_lock_bh(&nf_conntrack_lock);

    /*���������Ӹ��ټ�¼*/
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

    /*���������Ӹ��ٲ������е�Ԥ�����Ӹ���*/
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
  �������ƣ� hw_ctclean_recv_msg
  ���������� У�鱨�ģ��Ϸ���������Ϣ���ݿ���pstRuleInfo
  ���ú�����
  ����������
  ���������struct sock *sk, u32 *pid, ATP_CT_RULE_INFO *pstRuleInfo
  ���������ATP_CT_RULE_INFO *pstRuleInfo
  �� �� ֵ��0��ʾ��ȷ��-1��ʾ����
  ����˵����
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
  �������ƣ� hw_ctclean_process
  ���������� ������Ϣ
  ���ú�����
  ����������nl_recv_msg�� nl_send_msg�� hw_ctclean_find_ct
  ���������struct sock *sk, int len
  ���������
  �� �� ֵ��
  ����˵����
******************************************************************************/
void hw_ctclean_process(struct sk_buff *skb)
{
    u32 pid;
    ATP_CT_RULE_INFO stCtRuleInfo;
    int num = 0;

    memset(&stCtRuleInfo, 0, sizeof(ATP_CT_RULE_INFO));

    printk("[%s,%d]hw_ctclean_process recv msg!\n", __func__, __LINE__);

    /*���չ�����Ϣ������stCtRuleInfo*/
    if (-1 == hw_ctclean_recv_msg(skb, &pid, &stCtRuleInfo))
    {
        printk("hw_ctclean_process recv msg fail!\n");
        return;
    }

    printk("[%s,%d]hw_ctclean_process recv msg!\n", __func__, __LINE__);
    printk("dst ip %X, mask %X, port is %d\n", stCtRuleInfo.ulDstIp, stCtRuleInfo.ulDstMask, stCtRuleInfo.usDstPort);
    printk("src ip %X, mask %X, port is %d\n", stCtRuleInfo.ulSrcIp, stCtRuleInfo.ulSrcMask, stCtRuleInfo.usSrcPort);
    printk("Protocol %d, Dscp %d\n", stCtRuleInfo.ucProtocol, stCtRuleInfo.ucDscp);

    /*������protocol==255������ǰ��echo 36 > ip_ct_clean�����ӵķ�ʽ*/
    if (NAT_MAX_PROTOCOL == stCtRuleInfo.ucProtocol)
    {
        printk("[%s,%d]hw_ctclean_process recv protocol 255 !\n", __func__, __LINE__);
        nf_ct_iterate_cleanup(&init_net, kill_all_not_match, NULL);
        return;
    }

    /*�ҵ�����ƥ������Ӹ���*/
    if (0 == (num = hw_ctclean_find_ct(&init_net, &stCtRuleInfo)))
    {
        return;
    }

    /*ɾ��ǰ�����·�ɻ���*/
    rt_cache_flush(&init_net, 0);

    /*ɾ�������ҵ������Ӹ���*/
    hw_ctclean_do_delete(num);

    return;     
}
