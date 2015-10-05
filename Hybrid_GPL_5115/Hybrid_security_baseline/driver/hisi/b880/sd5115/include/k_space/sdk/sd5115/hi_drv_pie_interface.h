#ifndef _HI_PIE_INTERFACE_H_
#define _HI_PIE_INTERFACE_H_

#include "hi_drv_common.h"

#define ETH_TO_KERNEL	0           /* skb to kernel */
#define ETH_TO_DROP		1           /* free skb */
#define ETH_TO_NEXT		2           /* skip this skb */


#define COMMON_FORWARD	            	0x00000000  /*normal forward */
#define SPECIAL_FORWARD_GPON_MODE	  	0x00000001  /*GPON mode appointed forward */
#define SPECIAL_FORWARD_EPON_MODE    	0x00000002  /*EPON mode appointed forward*/

#define ETH_CORE_KERNEL_RX             1           /* direction of kernel receive pkt    */ 
#define ETH_CORE_KERNEL_TX             2           /* direction of kernel send pkt       */ 

/* struct define */
typedef unsigned int (*eth_rx_special_handler)(struct sk_buff *skb);
typedef unsigned int (*eth_tx_special_handler)(struct sk_buff *skb);

struct special_skb
{
    unsigned short gemport;
    unsigned char tcont;
    unsigned char llid;
    unsigned char dport;
    unsigned char priority;
    unsigned char flag;
    unsigned char groupid;
    unsigned char timestamp_add;
#ifdef CONFIG_HISI_SD5115_TYPE_T
	unsigned char spoof;
	unsigned char org_igr;	
#endif
};


struct rcv_skb_info
{
    unsigned char flag;
    unsigned char sport;
    unsigned char reason;
    unsigned char llid;
    unsigned short gemport;
    unsigned char  tagtype;     // 00: Untagged;  01:Priority Tagged;  1X: VLAN Tagged;
#ifdef CONFIG_HISI_SD5115_TYPE_T
    unsigned short pkt_item; 
#endif
    unsigned short reserved; 	
}; 

/* function announce */
extern void eth_rx_special_handler_func_reg(eth_rx_special_handler rx_special_handler);
extern void eth_rx_special_handler_func_dereg(void);
extern void eth_tx_special_handler_func_reg(eth_tx_special_handler tx_special_handler);
extern void eth_tx_special_handler_func_dereg(void);
extern int eth_special_process(struct sk_buff *skb, struct special_skb *sp_skb);
extern int eth_get_packets_info(struct sk_buff *skb,  struct rcv_skb_info *info);
extern int eth_transmit(struct sk_buff *skb, struct net_device *netdev);
extern int eth_senddata_to_special_port(unsigned char *data, unsigned int length, struct special_skb*  pstTransmitSkb);
extern void enable_deprive_rx_vlan(void);
extern void disable_deprive_rx_vlan(void);
extern void enable_deprive_tx_ex_vlan(void);
extern void disable_deprive_tx_ex_vlan(void);
#endif
