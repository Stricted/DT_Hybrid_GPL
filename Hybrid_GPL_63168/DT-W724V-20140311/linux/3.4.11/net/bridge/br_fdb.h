#ifndef _BR_FDB_H
#define _BR_FDB_H

#if defined(CONFIG_BCM_KF_NETFILTER) && !defined(CONFIG_BCM_SKIPKF_NETFILTER)
extern int br_fdb_adddel_static(struct net_bridge *br, 
                                struct net_bridge_port *source,
                                const unsigned char *addr, int bInsert);
#endif

#endif /* _BR_FDB_H */

