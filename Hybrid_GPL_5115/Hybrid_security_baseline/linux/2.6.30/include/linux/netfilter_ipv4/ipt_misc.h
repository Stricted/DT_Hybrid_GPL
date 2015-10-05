#ifndef _IPT_MISC_H_
#define  _IPT_MISC_H_

#ifdef __KERNEL__
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>
#include <linux/jhash.h>
#include <linux/random.h>

#define TCPPTSCAN_PTHASH_WDWIDTH 32  //WORD WIDTH
#define TCPPTSCAN_PTHASH_SIZE1 1024  //first level
#define TCPPTSCAN_PTHASH_SIZE2 (TCPPTSCAN_PTHASH_SIZE1/TCPPTSCAN_PTHASH_WDWIDTH) //second level
#define TCPPTSCAN_LISTHASH_SIZE (1 << 8)
#define TCPPTSCAN_GC_INTERVAL  (60*HZ) // 1 minute
#define TCPPTSCAN_REFRESH_INTERVAL  HZ/1000 // 1 ms
#define TCPPTSCAN_SLIENT_MAX   (5*60*HZ) // 5 minutes

#define RTP_DIFF_HASH_BITS 8
#define RTP_DIFF_HASH_SIZE (1<<RTP_DIFF_HASH_BITS)
#define RTP_DIFF_GC_TIMEOUT HZ // 1s
#define RTP_DIFF_FACTOR_MIN 3

struct iptmisc_tcpptscan_node
{
    struct list_head list;
    spinlock_t lock;
    unsigned int srcip;
    unsigned int dstip;
    unsigned int silent;
    unsigned int silent_start;
    unsigned long dst_ports[TCPPTSCAN_PTHASH_SIZE2];
    unsigned long recorded_ports;
    struct timer_list timer_refresh;
    struct timer_list timer_gc;
};

//quadratic difference
struct rtphdr
{
    unsigned char ver:2; //version
    unsigned char p:1;   //fill
    unsigned char x:1;   //extention
    unsigned char cc:4;  //counter of CSRC
    unsigned char m:1;   //silence
    unsigned char pt:7;  //payload
    unsigned short seq;  //sequence number
    unsigned int tstamp; //time stamp
    unsigned int ssrc;   //Synchronization Source
};
struct rtp_hash_dst {
	union {
		struct {
			unsigned int src;
			unsigned int  dst;
		} ip;
		/*
		struct {
			__u32 src[4];
			__u32 dst[4];
		} ip6;
		*/
	} addr;
	unsigned short  src_port;
	unsigned short dst_port;
};
struct rtp_diff_pos
{
    unsigned char ver; //simple difference
    unsigned char pt;  //simple difference
    unsigned short seq; //quadratic difference
    unsigned int tstamp; //quadratic difference
    unsigned int ssrc;  //simple difference
};
struct rtp_diff_val
{
    int ver_diff;
    int pt_diff;
    int seq_diff;
    int tstamp_diff;
    int ssrc_diff;
    int seq_diff2;
    int tstamp_diff2;
};
struct rtp_diff_node
{
   struct rtp_diff_pos   pos;
   struct rtp_diff_val   value;
   struct rtp_diff_node *prev;
   struct rtp_diff_node *next;
};
struct rtp_diff_head
{
    struct list_head list;
    struct timer_list timer_gc;
    spinlock_t    lock;
    unsigned long count; //number of nodes
    unsigned long refresh; //jiffies
    struct rtp_diff_val   value; 
    struct rtp_hash_dst   dst;    
    struct rtp_diff_node *head;
    struct rtp_diff_node *curr;
};

struct rtp_diff_block
{
    struct list_head head[RTP_DIFF_HASH_SIZE]; //rtp_diff_head
    unsigned int rnd;
};
#endif//__KERNEL__

#define MISC_TYPE  0x01
#define MISC_PORTS 0x02
#define MISC_PTEXPIRE 0x04
#define MISC_DNSURL   0x08
#define MISC_RTPFCTR  0x10
#define MISC_TCPLEN   0X20
#define MISC_UDPLEN   0X40
#define MISC_DNS_URL_MAX 128
struct ipt_misc_invert
{
//	unsigned char flag_ipland:1;      // ip land
//	unsigned char flag_tcpptscan:1;  //tcp port scan
//	unsigned char flag_dnsurl:1;     //match url against string in the dns request
//	unsigned char flag_rtp:1;
//	unsigned char flag_rtcp:1;
	unsigned char flag_tcplen:1;  //tcp payload length match
	unsigned char flag_udplen:1;  //udp playload length match
};

struct ipt_misc_info {
	unsigned char flag_ipland:1;      // ip land
	unsigned char flag_tcpptscan:1;  //tcp port scan
	unsigned char flag_dnsurl:1;     //match url against string in the dns request
	unsigned char flag_rtp:1;
	unsigned char flag_rtcp:1;
	unsigned char flag_tcplen:1;  //tcp payload length match
	unsigned char flag_udplen:1;  //udp playload length match
	unsigned char flag_udpbomb:1;

	struct ipt_misc_invert invert;
	
    unsigned int  ports_ptscan;  //for tcp port scan
    unsigned int  expire_ptscan; //for tcp port scan, ms
    char dns_url[MISC_DNS_URL_MAX]; //for dns url blocking
    unsigned int  rtp_diff_factor;  //RTP stream distinguish 
    unsigned int  payload; //for tcplen and udplen
};


#endif// _IPT_MISC_H_
