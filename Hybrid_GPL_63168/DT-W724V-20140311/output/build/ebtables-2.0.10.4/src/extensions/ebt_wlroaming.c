/*
 *  ebt_wlroaming
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/ebtables_u.h"
#include "../include/linux/netfilter_bridge/ebt_wlroaming_t.h"

//static int mark_supplied;
#define WLROAMING_TARGET '1'

static struct option opts[] =
{
	{ "wlroaming-target" 	, required_argument, 0, WLROAMING_TARGET },
	{ 0 }
};

static void print_help()
{
	printf(
	"wlroaming target options:\n"
	" --wlroaming-target target : ACCEPT, DROP, RETURN or CONTINUE\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_wlroaming_t_info *markinfo =
	   (struct ebt_wlroaming_t_info *)target->data;

	markinfo->target = EBT_ACCEPT;
}

#define OPT_WLROAMING_TARGET	0x01

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_wlroaming_t_info *markinfo =
	   (struct ebt_wlroaming_t_info *)(*target)->data;

	switch (c) {
	case WLROAMING_TARGET:
		ebt_check_option2(flags, OPT_WLROAMING_TARGET);
		if (FILL_TARGET(optarg, markinfo->target))
			ebt_print_error2("Illegal --wlroaming-target target");
		break;		
		
	 default:
		return 0;
	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target, const char *name,
   unsigned int hookmask, unsigned int time)
{
	struct ebt_wlroaming_t_info *markinfo =
	   (struct ebt_wlroaming_t_info *)target->data;
			
	if (BASE_CHAIN && markinfo->target == EBT_RETURN)
		ebt_print_error2("--wlroaming-target RETURN not allowed on base chain");
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	
	struct ebt_wlroaming_t_info *markinfo =
	   (struct ebt_wlroaming_t_info *)target->data;
	printf(" --wlroaming-target %s", TARGET_NAME(markinfo->target));
}

static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_wlroaming_t_info *markinfo1 =
	   (struct ebt_wlroaming_t_info *)t1->data;
	struct ebt_wlroaming_t_info *markinfo2 =
	   (struct ebt_wlroaming_t_info *)t2->data;

	return markinfo1->target == markinfo2->target; 
}

static struct ebt_u_target mark_target =
{
	.name		= EBT_WLROAMING_TARGET,
	.size		= sizeof(struct ebt_wlroaming_t_info),
	.help		= print_help,
	.init		= init,
	.parse		= parse,
	.final_check	= final_check,
	.print		= print,
	.compare	= compare,
	.extra_ops	= opts,
};

//static void _init(void) __attribute__ ((constructor));
void _init(void)
{
	ebt_register_target(&mark_target);
}
