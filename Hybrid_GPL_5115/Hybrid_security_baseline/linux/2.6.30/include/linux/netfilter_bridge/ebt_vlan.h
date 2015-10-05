#ifndef __LINUX_BRIDGE_EBT_VLAN_H
#define __LINUX_BRIDGE_EBT_VLAN_H

#define EBT_VLAN_ID	0x01
#define EBT_VLAN_PRIO	0x02
#define EBT_VLAN_ENCAP	0x04
#define EBT_VLAN_MASK (EBT_VLAN_ID | EBT_VLAN_PRIO | EBT_VLAN_ENCAP)
#define EBT_VLAN_MATCH "vlan"

/* start of add by d00107688 to support vlan range */
#define EBT_VLAN_ID_RANGE	0x08
#define EBT_VLAN_PRIO_RANGE	0x10
/* end of add by d00107688 to support vlan range */

struct ebt_vlan_info {
	uint16_t id;		/* VLAN ID {1-4095} */
	uint8_t prio;		/* VLAN User Priority {0-7} */
	__be16 encap;		/* VLAN Encapsulated frame code {0-65535} */

/* start of add by d00107688 to support vlan range */
	uint16_t id_min;
	uint16_t id_max;
	uint8_t  prio_min;
	uint8_t  prio_max;
/* end of add by d00107688 to support vlan range */

	uint8_t bitmask;		/* Args bitmask bit 1=1 - ID arg,
				   bit 2=1 User-Priority arg, bit 3=1 encap*/
	uint8_t invflags;		/* Inverse bitmask  bit 1=1 - inversed ID arg, 
				   bit 2=1 - inversed Pirority arg */
};

#endif
