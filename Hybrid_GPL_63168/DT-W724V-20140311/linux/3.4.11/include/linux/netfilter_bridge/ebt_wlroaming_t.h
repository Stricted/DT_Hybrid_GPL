#ifndef __LINUX_BRIDGE_EBT_WLROAMING_T_H
#define __LINUX_BRIDGE_EBT_WLROAMING_T_H

struct ebt_wlroaming_t_info
{
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define EBT_WLROAMING_TARGET "wlroaming"

#endif
