/*
 <:copyright-BRCM:2007:DUAL/GPL:standard
 
    Copyright (c) 2007 Broadcom Corporation
    All Rights Reserved
 
 Unless you and Broadcom execute a separate written software license
 agreement governing use of this software, this software is licensed
 to you under the terms of the GNU General Public License version 2
 (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
 with the following added to such license:
 
    As a special exception, the copyright holders of this software give
    you permission to link this software with independent modules, and
    to copy and distribute the resulting executable under terms of your
    choice, provided that you also meet, for each linked independent
    module, the terms and conditions of the license of that module.
    An independent module is a module which is not derived from this
    software.  The special exception does not apply to any modifications
    of the software.
 
 Not withstanding the above, under no circumstances may you combine
 this software in any way with any other Broadcom software provided
 under a license other than the GPL, without Broadcom's express prior
 written consent.
 
:>
*/

#ifndef __RDPA_DRV__H_INCLUDED__
#define __RDPA_DRV__H_INCLUDED__

#include "bcmtypes.h"
//#include "rdpa_api.h"
//#include "rdpa_ag_port.h"

#define RDPA_DRV_VERSION              "0.2"
#define RDPA_DRV_VER_STR              "v" RDPA_DRV_VERSION " " __DATE__ " " __TIME__
#define RDPA_DRV_MODNAME              "Broadcom Runner Packet Processor"

#define RDPA_DRV_NAME                 "bcmrdpa"

/* RDPA_DRV Character Device */
#define RDPADRV_MAJOR             251
#define RDPADRV_NAME              RDPA_DRV_NAME
#define RDPADRV_DEVICE_NAME       "/dev/" RDPADRV_NAME

/* RDPA Control Utility Executable */
#define RDPA_CTL_UTILITY_PATH     "/bin/rdpactl"

/* RDPA Proc FS Directory Path */
#define RDPA_PROC_FS_DIR_PATH     RDPA_NAME

#define RDPA_IFNAME_SIZE		  36

typedef enum {
	RDPA_DRV_SUCCESS 			= 0,
	RDPA_DRV_ERROR   			= 1,
	RDPA_DRV_NO_ROOT 			= 2,
	RDPA_DRV_PORT_NOT_ALLOC 	= 3,
	RDPA_DRV_PORT_ID_NOT_VALID 	= 4,
	RDPA_DRV_NO_MORE_TMS 		= 5,
	RDPA_DRV_TM_NOT_ALLOC 		= 6,
	RDPA_DRV_NEW_TM_ALLOC		= 7,
	RDPA_DRV_TM_GET				= 8,
	RDPA_DRV_TM_CFG_GET			= 9,
	RDPA_DRV_TM_CFG_SET			= 10,
	RDPA_DRV_TM_INDEX_GET		= 11,
	RDPA_DRV_PORT_GET			= 12,
	RDPA_DRV_Q_CFG_SET			= 13,
	RDPA_DRV_Q_RATE_SET			= 14,
	RDPA_DRV_GET_ROOT_BY_IF		= 15,
	RDPA_DRV_SUBS_SET			= 16,
	RDPA_DRV_MODE_GET			= 17,
	RDPA_DRV_SH_DESTROY			= 18,
	RDPA_DRV_IC_ERROR       	= 19,
	RDPA_DRV_IC_NOT_FOUND		= 20,
	RDPA_DRV_IC_FLOW_ERROR		= 21,
} rdpaDrvReturn_code_t;;

/*
 *------------------------------------------------------------------------------
 * Common defines for RDPA layers.
 *------------------------------------------------------------------------------
 */
#undef RDPA_DECL
#define RDPA_DECL(x)                 x,  /* for enum declaration in H file */

typedef enum {
	RDPA_DECL(RDPA_IOC_TM)
	RDPA_DECL(RDPA_IOC_IPTV)
	RDPA_DECL(RDPA_IOC_IC)
	RDPA_DECL(RDPA_IOC_WAN_TYPE)
	RDPA_DECL(RDPA_IOC_MAX)
} rdpa_drv_ioctl_t;

typedef enum {
	RDPA_IOCTL_TM_CMD_GET_ROOT_TM		= 0,
	RDPA_IOCTL_TM_GET_BY_QID			= 1,
	RDPA_IOCTL_TM_CMD_ROOT_TM_CONFIG	= 2,
	RDPA_IOCTL_TM_CMD_TM_CONFIG			= 3,
	RDPA_IOCTL_TM_CMD_ROOT_TM_REMOVE	= 4,
	RDPA_IOCTL_TM_CMD_TM_REMOVE			= 5,
	RDPA_IOCTL_TM_CMD_QUEUE_CONFIG		= 6,
	RDPA_IOCTL_TM_CMD_QUEUE_REMOVE		= 7,
	RDPA_IOCTL_TM_CMD_TM_Q_CONFIG		= 8,
	RDPA_IOCTL_TM_CMD_MAX
} rdpa_drv_ioctl_tm_cmd_t;

typedef struct {
	rdpa_drv_ioctl_tm_cmd_t cmd;
	uint32_t root_tm_id;
	uint32_t tm_id;
	uint32_t port_id;
	uint32_t dir;
	uint32_t q_id;
    uint32_t index;
	uint32_t level; /* Next TM level */
	uint32_t arbiter_mode;
	uint32_t priority;
	uint32_t qsize;
	uint32_t shaping_rate;
	uint32_t weight;
	BOOL     found;
} rdpa_drv_ioctl_tm_t;

typedef enum {
	RDPA_IOCTL_IPTV_CMD_LOOKUP_METHOD_SET,
	RDPA_IOCTL_IPTV_CMD_LOOKUP_METHOD_GET,
	RDPA_IOCTL_IPTV_CMD_ENTRY_ADD,
	RDPA_IOCTL_IPTV_CMD_ENTRY_REMOVE,
	RDPA_IOCTL_IPTV_CMD_ENTRY_FLUSH,
	RDPA_IOCTL_IPTV_CMD_MAX
} rdpa_drv_ioctl_iptv_cmd_t;

typedef enum {
	RDPA_IOCTL_IPTV_METHOD_MAC,
	RDPA_IOCTL_IPTV_METHOD_MAC_VID,
	RDPA_IOCTL_IPTV_METHOD_GROUP_IP,
	RDPA_IOCTL_IPTV_METHOD_GROUP_IP_SOURCE_IP,
	RDPA_IOCTL_IPTV_METHOD_GROUP_IP_SOURCE_IP_VID,
	RDPA_IOCTL_IPTV_METHOD_MAX
 } rdpa_drv_ioctl_iptv_lookup_method_t;


typedef enum {
	RDPA_IOCTL_IPTV_VLAN_UNTAG,
	RDPA_IOCTL_IPTV_VLAN_TRANPARENT,
	RDPA_IOCTL_IPTV_VLAN_TRANSLATION,
	RDPA_IOCTL_IPTV_VLAN_MAX
} rdpa_drv_ioctl_iptv_vlan_action_t;

typedef enum {
	RDPA_IOCTL_IPTV_PORT_LAN0,
	RDPA_IOCTL_IPTV_PORT_LAN1,
	RDPA_IOCTL_IPTV_PORT_LAN2,
	RDPA_IOCTL_IPTV_PORT_LAN3,
	RDPA_IOCTL_IPTV_PORT_LAN4,
	RDPA_IOCTL_IPTV_PORT_LAN5,
	RDPA_IOCTL_IPTV_PORT_LAN6,
	RDPA_IOCTL_IPTV_PORT_LAN7,
	RDPA_IOCTL_IPTV_PORT_MAX
} rdpa_drv_ioctl_iptv_egress_port_t;

typedef enum
{
   RDPACTL_IP_FAMILY_IPV4,
   RDPACTL_IP_FAMILY_IPV6
} rdpactl_ip_family_t;

typedef struct {
    rdpactl_ip_family_t ip_family;
    union {
        uint8_t  mac[6];
        uint32_t ipv4;
        uint8_t  ipv6[16];
    } group; /**< multicast group */

    union {
        uint32_t ipv4;
        uint8_t  ipv6[16];
    } src_ip; /**< multicast ssm ip address */

    uint16_t  vid;
} rdpa_drv_ioctl_iptv_key_t;

typedef struct {
    rdpa_drv_ioctl_iptv_vlan_action_t action;
    uint16_t                          vid;
} rdpa_drv_ioctl_iptv_vlan_entry_t;

typedef struct {
    rdpa_drv_ioctl_iptv_key_t         key;
    rdpa_drv_ioctl_iptv_vlan_entry_t  vlan;
} rdpa_drv_ioctl_iptv_entry_t;

typedef struct {
    rdpa_drv_ioctl_iptv_cmd_t           cmd;
    rdpa_drv_ioctl_iptv_lookup_method_t method;
    rdpa_drv_ioctl_iptv_egress_port_t   egress_port;
    rdpa_drv_ioctl_iptv_entry_t         entry;
    uint16_t                            index;
} rdpa_drv_ioctl_iptv_t;

int rdpa_cmd_tm_ioctl(unsigned long arg);
void rdpa_cmd_tm_init(void);

/** Classification rule mask fields bitmask */
typedef enum
{
    RDPACTL_IC_MASK_SRC_IP          = 0x00000001, /**< Source IP address */
    RDPACTL_IC_MASK_DST_IP          = 0x00000002, /**< Destination IP address */
    RDPACTL_IC_MASK_SRC_PORT        = 0x00000004, /**< Source port */
    RDPACTL_IC_MASK_DST_PORT        = 0x00000008, /**< Destination port */
    RDPACTL_IC_MASK_OUTER_VID       = 0x00000010, /**< Outer VID */
    RDPACTL_IC_MASK_INNER_VID       = 0x00000020, /**< Inner VID */
    RDPACTL_IC_MASK_DST_MAC         = 0x00000040, /**< Destination MAC address */
    RDPACTL_IC_MASK_SRC_MAC         = 0x00000080, /**< Source MAC address */
    RDPACTL_IC_MASK_ETHER_TYPE      = 0x00000100, /**< Ether Type */
    RDPACTL_IC_MASK_IP_PROTOCOL     = 0x00000200, /**< Protocol */
    RDPACTL_IC_MASK_DSCP            = 0x00000400, /**< IP Differentiated Services Code Point */
    RDPACTL_IC_MASK_SSID            = 0x00000800, /**< Ingress SSID */
    RDPACTL_IC_MASK_INGRESS_PORT    = 0x00001000,
    RDPACTL_IC_MASK_OUTER_PBIT      = 0x00002000, /**< Outer PBIT */
    RDPACTL_IC_MASK_INNER_PBIT      = 0x00004000, /**< Inner PBIT */
    RDPACTL_IC_MASK_NUM_OF_VLANS    = 0x00008000, /**< Number of VLAN in packet */
} rdpactl_ic_fields;

typedef enum
{
    RDPACTL_IF_LAN0,           /**< LAN0 port */
    RDPACTL_IF_LAN1,           /**< LAN1 port */
    RDPACTL_IF_LAN2,           /**< LAN2 port */
    RDPACTL_IF_LAN3,           /**< LAN3 port */
    
    RDPACTL_IF_MAX,
} rdpactl_ingress_port;

typedef enum
{
    RDPACTL_IC_TYPE_ACL = 1, /**< Classification type ACL */
    RDPACTL_IC_TYPE_FLOW = 2,   /**< Classification type Flow */
    RDPACTL_IC_TYPE_QOS = 3,    /**< Classification type QoS */
} rdpactl_ic_type;

#define RDPACTL_QUEUEID_BITS_NUMBER 16
#define RDPACTL_WANFLOW_MASK ((~0) << RDPACTL_QUEUEID_BITS_NUMBER)

typedef struct
{
    rdpactl_ic_type type;
    //Classification Mask & rule priority
    uint32_t prty;  /**< Defined the priority of classifier. value between 0 - 256, 0 is highest priority */
    uint32_t field_mask; /**< Fields used for classification. A combination of rdpactl_ic_fields */

    //Classification key
    rdpactl_ip_family_t ip_family;
    union {
        uint32_t ipv4;
        uint8_t ipv6[16];
    } src_ip; /**< source ipv4/ipv6 ip */
    union {
        uint32_t ipv4;
        uint8_t ipv6[16];
    } dst_ip; /**< dest ipv4/ipv6 ip */
    uint16_t src_port;  /**< source port */
    uint16_t dst_port;  /**< destination port */
    uint8_t   protocol; /**< IP protocols. For example, UDP(17) */
    uint16_t  outer_vid; /**< Outer VID */
    uint16_t  inner_vid;   /**< Inner VID */
    uint8_t dst_mac[6];  /**<DA  MAC address */
    uint8_t src_mac[6];  /**<SA MAC address */
    uint16_t  etype;  /**< Ethernet type */
    uint8_t   dscp; /**< dscp val */
    uint8_t ingress_port_id; /**<DS- GEM or LLID index US - ingress port index */
    uint8_t   outer_pbits;  /**< Outer pbit */
    uint8_t   inner_pbits;  /**< Inner PBIT */
    uint8_t   number_of_vlans; /**< number of vlans */

 //Classification result
    uint32_t queue_id; /**< Egress queue id and wan flow */
    int8_t dscp_remark; /*-1: no remark */
    int8_t opbit_remark; /*-1: no remark */
    int8_t ipbit_remark; /*-1: no remark */
    uint32_t shaping_rate;  //to be done
    uint32_t shaping_burst_size; //to be done
} rdpactl_classification_rule_t ;

typedef enum {
    RDPA_IOCTL_IC_CMD_ADD_CLASSIFICATION_RULE	= 0,
    RDPA_IOCTL_IC_CMD_DEL_CLASSIFICATION_RULE	= 1,
    RDPA_IOCTL_IC_CMD_ADD	= 2,
    RDPA_IOCTL_IC_CMD_DEL	= 3,
} rdpa_drv_ioctl_ic_cmd_t;

typedef struct {
	rdpa_drv_ioctl_ic_cmd_t cmd;
	rdpactl_classification_rule_t rule;
} rdpa_drv_ioctl_ic_t;

#endif /* __RDPA_DRV__H_INCLUDED__ */
