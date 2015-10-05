/* Port-triggering target. 
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 */

/* Shared library add-on to iptables to add port-trigger support. */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter/nf_nat.h>
#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"PORT_TRIGGERING v%s options:\n"
" --triggering-type (dnat|in|out)\n"
"				Triggering type\n"
" --triggering-proto proto\n"
"				Triggering protocol\n"
" --triggering-port port[-port]\n"
"				Triggering  port range\n"
" --open-proto proto\n"
"               Open protocol\n"
" --open-port port[-port]\n"
"				Port range to open when trigger happened.\n\n",
IPTABLES_VERSION);
}

static struct option opts[] = {
	{.name = "triggering-type",     .has_arg = true, .val = '1' },
	{.name = "triggering-proto",    .has_arg = true, .val = '2'},
	{.name = "triggering-port",     .has_arg = true, .val = '3'},
    {.name = "open-proto",          .has_arg = true, .val = '4' },
	{.name = "open-port",           .has_arg = true, .val = '5' },
	{ 0 }
};

/* Initialize the target. */
static void
init(struct ipt_entry_target *t)
{
}

/* Parses ports */
static void
parse_ports(const char *arg, u_int16_t *ports)
{
	const char *dash;
	int port;

	port = atoi(arg);
	if (port == 0 || port > 65535)
		xtables_error(PARAMETER_PROBLEM, "Port range `%s' invalid\n", arg);

	dash = strchr(arg, '-');
	if (!dash)
		ports[0] = ports[1] = port;
	else {
		int maxport;

		maxport = atoi(dash + 1);
		if (maxport == 0 || maxport > 65535)
			xtables_error(PARAMETER_PROBLEM,
				   "Port range `%s' invalid\n", dash+1);
		if (maxport < port)
			xtables_error(PARAMETER_PROBLEM,
				   "Port range `%s' invalid\n", arg);
		ports[0] = port;
		ports[1] = maxport;
	}
}

static void
parse_Openports(const char *arg, struct ipt_trigger_ports *ports)
{
    char* pcTmp = NULL;
    char* pcStr1 = NULL;
    char* pcStr2 = NULL;
	int i = 0;

    u_int32_t ulStartPort = 0;
    u_int32_t ulEndPort = 0;

/******************一触发多:内核扩展成用一条规则支持2000-2038,2050-2051,2069,2085,3010-3030格式的模式，
    不需要将外部端口号解析并分段，但必须校验保证格式正确*****************************/
	memset(ports->stOpenPort,0,PT_OPEN_PORT_RANGE_MAX *sizeof(PT_OPEN_APP_PORT_RANGE));

	pcTmp = arg;
	while (NULL != (pcStr1 = strchr(pcTmp, ',')))
	{
		if (i >= PT_OPEN_PORT_RANGE_MAX)
		{
			i = 0;
		}

		*(pcTmp + (pcStr1 - pcTmp)) = '\0';

		if (NULL != (pcStr2 = strchr(pcTmp, '-')))
		{
			*(pcTmp + (pcStr2 - pcTmp)) = '\0';
			pcStr2 += 1;
			ulStartPort = (u_int32_t)atoi(pcTmp);
			ulEndPort = (u_int32_t)atoi(pcStr2);

			if (ulEndPort == 0)
			{
				ulEndPort = ulStartPort;
			}
           // printf("\n ===========startport is %u,endport is %u===========\n",ulStartPort,ulEndPort);
		}
        else
        {
			ulStartPort = (u_int32_t)atoi(pcTmp);
			ulEndPort = ulStartPort;
           // printf("\n =================startport is %u,endport is %u**************\n",ulStartPort,ulEndPort);
            
        }
			ports->stOpenPort[i].ulStartPort = ulStartPort;
			ports->stOpenPort[i].ulEndPort = ulEndPort;

			i ++;
		
		
		pcTmp = pcStr1 + 1;
		
	}

	if (i >= PT_OPEN_PORT_RANGE_MAX)
	{
		i = 0;
	}

//只有一个"-"或最后一个要进行补充
	if (NULL != (pcStr2 = strchr(pcTmp, '-')))
	{
        *(pcTmp + (pcStr2 - pcTmp)) = '\0';
        pcStr2 += 1;
        ulStartPort = (u_int32_t)atoi(pcTmp);
        ulEndPort = (u_int32_t)atoi(pcStr2);
        
		if (ulEndPort == 0)
		{
			ulEndPort = ulStartPort;
		}
	}
	else
	{
        ulStartPort = (u_int32_t)atoi(pcTmp);
        ulEndPort = ulStartPort;
    }
	ports->stOpenPort[i].ulStartPort = ulStartPort;
	ports->stOpenPort[i].ulEndPort = ulEndPort;
}
/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct ipt_entry_target **target)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)(*target)->data;

	switch (c) {
	case '1':
		if (!strcasecmp(optarg, "dnat"))
			info->type = IPT_TRIGGER_DNAT;
		else if (!strcasecmp(optarg, "in"))
			info->type = IPT_TRIGGER_IN;
		else if (!strcasecmp(optarg, "out"))
			info->type = IPT_TRIGGER_OUT;
		else
			xtables_error(PARAMETER_PROBLEM,
				   "triggering type can only be one of `%s', '%s' '%s'", "dnat", "in", "out");
		return 1;

	case '2':
		if (!strcasecmp(optarg, "tcp"))
			info->proto = IPPROTO_TCP;
		else if (!strcasecmp(optarg, "udp"))
			info->proto = IPPROTO_UDP;
		else if (!strcasecmp(optarg, "all"))
			info->proto = 0;
		else
			xtables_error(PARAMETER_PROBLEM,
				   "triggering protocol can only be one of `%s', '%s', '%s'", "tcp", "udp", "all");
		return 1;

	case '3':
		parse_ports(optarg, info->ports.mport);
		return 1;

    case '4':
        if (!strcasecmp(optarg, "tcp"))
            info->rproto = IPPROTO_TCP;
        else if (!strcasecmp(optarg, "udp"))
            info->rproto = IPPROTO_UDP;
        else if (!strcasecmp(optarg, "all"))
            info->rproto = 0;
        else
            xtables_error(PARAMETER_PROBLEM,
                   "open protocol can only be one of '%s', '%s', '%s'", "tcp", "udp", "all");
        return 1;

    case '5':
        parse_Openports(optarg, &(info->ports));
        *flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
        return 1;

	default:
    	return 0;
	}
}

/* Prints out the targinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_target *target,
      int numeric)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)target->data;

	printf("PORT_TRIGGERING ");
	if (info->type == IPT_TRIGGER_DNAT)
		printf("type:dnat ");
	else if (info->type == IPT_TRIGGER_IN)
		printf("type:in ");
	else if (info->type == IPT_TRIGGER_OUT)
		printf("type:out ");

	if (info->proto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->proto == IPPROTO_UDP)
		printf("udp ");

	printf("triggering port:%hu", info->ports.mport[0]);
	if (info->ports.mport[1] > info->ports.mport[0])
		printf("-%hu", info->ports.mport[1]);
	printf(" ");

	if (info->rproto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->rproto == IPPROTO_UDP)
		printf("udp ");
	printf("open port:%hu-%hu,%hu-%hu,%hu-%hu,%hu-%hu,%hu-%hu,%hu-%hu,%hu-%hu,%hu-%hu", 
						info->ports.stOpenPort[0].ulStartPort,info->ports.stOpenPort[0].ulEndPort,
						info->ports.stOpenPort[1].ulStartPort,info->ports.stOpenPort[1].ulEndPort,
						info->ports.stOpenPort[2].ulStartPort,info->ports.stOpenPort[2].ulEndPort,
						info->ports.stOpenPort[3].ulStartPort,info->ports.stOpenPort[3].ulEndPort,
						info->ports.stOpenPort[4].ulStartPort,info->ports.stOpenPort[4].ulEndPort,
						info->ports.stOpenPort[5].ulStartPort,info->ports.stOpenPort[5].ulEndPort,
						info->ports.stOpenPort[6].ulStartPort,info->ports.stOpenPort[6].ulEndPort,
						info->ports.stOpenPort[7].ulStartPort,info->ports.stOpenPort[7].ulEndPort);

	printf(" ");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)target->data;

	printf("--triggering-proto ");
	if (info->proto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->proto == IPPROTO_UDP)
		printf("udp ");
	printf("--triggering-port %hu-%hu ", info->ports.mport[0], info->ports.mport[1]);

	printf("--open-proto ");
	if (info->rproto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->rproto == IPPROTO_UDP)
		printf("udp ");
    
	//printf("--open-port %hu-%hu ", info->ports.rport[0], info->ports.rport[1]);
}

static struct xtables_target trigger = {
	.name		= "PORT_TRIGGERING",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct ipt_trigger_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_trigger_info)),
	.help		= help,
	.init		= init,
	.parse		= parse,
	.print		= print,
	.save		= save,
	.extra_opts	= opts 
};

void _init(void)
{
	xtables_register_target(&trigger);
}
