/*
 * link_gre.c	gre driver module
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Herbert Xu <herbert@gondor.apana.org.au>
 *
 */

#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <linux/ip.h>
#include <linux/if_tunnel.h>
#include <linux/ip6_tunnel.h>
#include "rt_names.h"
#include "utils.h"
#include "ip_common.h"
#include "tunnel.h"

#define DEFAULT_TNL_HOP_LIMIT	(64)

static void usage(void) __attribute__((noreturn));
static void usage(void)
{
	fprintf(stderr, "Usage: ip link { add | set | change | replace | del } NAME\n");
	fprintf(stderr, "          type { ip6gre } [ remote ADDR ] [ local ADDR ]\n");
	fprintf(stderr, "          [ [i|o]seq ] [ [i|o]key KEY ] [ [i|o]csum ]\n");
	fprintf(stderr, "          [ ttl TTL ] [ dev PHYS_DEV ]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Where: NAME := STRING\n");
	fprintf(stderr, "       ADDR := { IP_ADDRESS | any }\n");
	fprintf(stderr, "       TTL  := { 1..255 | inherit }\n");
	fprintf(stderr, "       KEY  := { DOTTED_QUAD | NUMBER }\n");
	exit(-1);
}

static int ip6gre_parse_opt(struct link_util *lu, int argc, char **argv,
			 struct nlmsghdr *n)
{
	struct {
		struct nlmsghdr n;
		struct ifinfomsg i;
		char buf[1024];
	} req;
	struct ifinfomsg *ifi = (struct ifinfomsg *)(n + 1);
	struct rtattr *tb[IFLA_MAX + 1];
	struct rtattr *linkinfo[IFLA_INFO_MAX+1];
	struct rtattr *greinfo[IFLA_GRE_MAX + 1];
	__u16 iflags = 0;
	__u16 oflags = 0;
	unsigned ikey = 0;
	unsigned okey = 0;
	struct in6_addr laddr;
	struct in6_addr raddr;
	unsigned link = 0;
	__u8 ttl = DEFAULT_TNL_HOP_LIMIT;
	int len;
	__u8 encap_limit = IPV6_DEFAULT_TNL_ENCAP_LIMIT;
    __u32 flags = 0;
    
	if (!(n->nlmsg_flags & NLM_F_CREATE)) {
		memset(&req, 0, sizeof(req));

		req.n.nlmsg_len = NLMSG_LENGTH(sizeof(*ifi));
		req.n.nlmsg_flags = NLM_F_REQUEST;
		req.n.nlmsg_type = RTM_GETLINK;
		req.i.ifi_family = preferred_family;
		req.i.ifi_index = ifi->ifi_index;

		if (rtnl_talk(&rth, &req.n, 0, 0, &req.n, NULL, NULL) < 0) {
get_failed:
			fprintf(stderr,
				"Failed to get existing tunnel info.\n");
			return -1;
		}

		len = req.n.nlmsg_len;
		len -= NLMSG_LENGTH(sizeof(*ifi));
		if (len < 0)
			goto get_failed;

		parse_rtattr(tb, IFLA_MAX, IFLA_RTA(&req.i), len);

		if (!tb[IFLA_LINKINFO])
			goto get_failed;

		parse_rtattr_nested(linkinfo, IFLA_INFO_MAX, tb[IFLA_LINKINFO]);

		if (!linkinfo[IFLA_INFO_DATA])
			goto get_failed;

		parse_rtattr_nested(greinfo, IFLA_GRE_MAX,
				    linkinfo[IFLA_INFO_DATA]);

		if (greinfo[IFLA_GRE_IKEY])
			ikey = *(__u32 *)RTA_DATA(greinfo[IFLA_GRE_IKEY]);

		if (greinfo[IFLA_GRE_OKEY])
			okey = *(__u32 *)RTA_DATA(greinfo[IFLA_GRE_OKEY]);

		if (greinfo[IFLA_GRE_IFLAGS])
			iflags = *(__u16 *)RTA_DATA(greinfo[IFLA_GRE_IFLAGS]);

		if (greinfo[IFLA_GRE_OFLAGS])
			oflags = *(__u16 *)RTA_DATA(greinfo[IFLA_GRE_OFLAGS]);

		if (greinfo[IFLA_GRE_LOCAL])
			memcpy(&laddr, RTA_DATA(greinfo[IFLA_GRE_LOCAL]),
			       sizeof(laddr));

		if (greinfo[IFLA_GRE_REMOTE])
			memcpy(&raddr, RTA_DATA(greinfo[IFLA_GRE_REMOTE]),
			       sizeof(raddr));

		if (greinfo[IFLA_GRE_TTL])
			ttl = *(__u8 *)RTA_DATA(greinfo[IFLA_GRE_TTL]);

		if (greinfo[IFLA_GRE_LINK])
			link = *(__u8 *)RTA_DATA(greinfo[IFLA_GRE_LINK]);
	}

	while (argc > 0) {
		if (!matches(*argv, "key")) {
			unsigned uval;

			NEXT_ARG();
			iflags |= GRE_KEY;
			oflags |= GRE_KEY;
			if (strchr(*argv, '.'))
				uval = get_addr32(*argv);
			else {
				if (get_unsigned(&uval, *argv, 0) < 0) {
					fprintf(stderr,
						"Invalid value for \"key\"\n");
					exit(-1);
				}
				uval = htonl(uval);
			}

			ikey = okey = uval;
		} else if (!matches(*argv, "ikey")) {
			unsigned uval;

			NEXT_ARG();
			iflags |= GRE_KEY;
			if (strchr(*argv, '.'))
				uval = get_addr32(*argv);
			else {
				if (get_unsigned(&uval, *argv, 0)<0) {
					fprintf(stderr, "invalid value of \"ikey\"\n");
					exit(-1);
				}
				uval = htonl(uval);
			}
			ikey = uval;
		} else if (!matches(*argv, "okey")) {
			unsigned uval;

			NEXT_ARG();
			oflags |= GRE_KEY;
			if (strchr(*argv, '.'))
				uval = get_addr32(*argv);
			else {
				if (get_unsigned(&uval, *argv, 0)<0) {
					fprintf(stderr, "invalid value of \"okey\"\n");
					exit(-1);
				}
				uval = htonl(uval);
			}
			okey = uval;
		} else if (!matches(*argv, "seq")) {
			iflags |= GRE_SEQ;
			oflags |= GRE_SEQ;
		} else if (!matches(*argv, "iseq")) {
			iflags |= GRE_SEQ;
		} else if (!matches(*argv, "oseq")) {
			oflags |= GRE_SEQ;
		} else if (!matches(*argv, "csum")) {
			iflags |= GRE_CSUM;
			oflags |= GRE_CSUM;
		} else if (!matches(*argv, "icsum")) {
			iflags |= GRE_CSUM;
		} else if (!matches(*argv, "ocsum")) {
			oflags |= GRE_CSUM;
		} else if (!matches(*argv, "remote")) {
			inet_prefix addr;
			NEXT_ARG();
			get_prefix(&addr, *argv, preferred_family);
			if (addr.family == AF_UNSPEC)
				invarg("\"remote\" address family is AF_UNSPEC", *argv);
			memcpy(&raddr, addr.data, addr.bytelen);
		} else if (!matches(*argv, "local")) {
			inet_prefix addr;
			NEXT_ARG();
			get_prefix(&addr, *argv, preferred_family);
			if (addr.family == AF_UNSPEC)
				invarg("\"local\" address family is AF_UNSPEC", *argv);
			memcpy(&laddr, addr.data, addr.bytelen);
		} else if (!matches(*argv, "dev")) {
			NEXT_ARG();
			link = if_nametoindex(*argv);
			if (link == 0)
				exit(-1);
		} else if (!matches(*argv, "ttl") ||
			   !matches(*argv, "hoplimit")) {
			unsigned uval;

			NEXT_ARG();
			if (strcmp(*argv, "inherit") != 0) {
				if (get_unsigned(&uval, *argv, 0))
					invarg("invalid TTL\n", *argv);
				if (uval > 255)
					invarg("TTL must be <= 255\n", *argv);
				ttl = uval;
			}
		} else if (matches(*argv, "encaplimit") == 0) {
			NEXT_ARG();
			if (strcmp(*argv, "none") == 0) {
				flags |= IP6_TNL_F_IGN_ENCAP_LIMIT;
			} else {
				__u8 uval;
				if (get_u8(&uval, *argv, 0) < -1)
					invarg("invalid ELIM", *argv);
				encap_limit = uval;
				flags &= ~IP6_TNL_F_IGN_ENCAP_LIMIT;
			}
		}
        else 
			usage();
		argc--; argv++;
	}

	addattr32(n, 1024, IFLA_GRE_IKEY, ikey);
	addattr32(n, 1024, IFLA_GRE_OKEY, okey);
	addattr_l(n, 1024, IFLA_GRE_IFLAGS, &iflags, 2);
	addattr_l(n, 1024, IFLA_GRE_OFLAGS, &oflags, 2);
	addattr_l(n, 1024, IFLA_GRE_LOCAL, &laddr, sizeof(laddr));
	addattr_l(n, 1024, IFLA_GRE_REMOTE, &raddr, sizeof(raddr));
	if (link)
		addattr32(n, 1024, IFLA_GRE_LINK, link);
	addattr_l(n, 1024, IFLA_GRE_TTL, &ttl, 1);
	addattr32(n, 1024, IFLA_GRE_FLAGS, flags);
	addattr8(n, 1024, IFLA_GRE_ENCAP_LIMIT, encap_limit);
	return 0;
}

static void ip6gre_print_opt(struct link_util *lu, FILE *f, struct rtattr *tb[])
{
	char s1[1024];
	char s2[64];
	unsigned iflags = 0;
	unsigned oflags = 0;

	if (!tb)
		return;

	if (tb[IFLA_GRE_REMOTE]) {
		fprintf(f, "remote %s ",
			rt_addr_n2a(AF_INET6,
				    RTA_PAYLOAD(tb[IFLA_GRE_REMOTE]),
				    RTA_DATA(tb[IFLA_GRE_REMOTE]),
				    s1, sizeof(s1)));
	}

	if (tb[IFLA_GRE_LOCAL]) {
		fprintf(f, "local %s ",
			rt_addr_n2a(AF_INET6,
				    RTA_PAYLOAD(tb[IFLA_GRE_LOCAL]),
				    RTA_DATA(tb[IFLA_GRE_LOCAL]),
				    s1, sizeof(s1)));
	}

	if (tb[IFLA_GRE_LINK] && *(__u32 *)RTA_DATA(tb[IFLA_GRE_LINK])) {
		unsigned link = *(__u32 *)RTA_DATA(tb[IFLA_GRE_LINK]);
		char *n = tnl_ioctl_get_ifname(link);

		if (n)
			fprintf(f, "dev %s ", n);
		else
			fprintf(f, "dev %u ", link);
	}

	if (tb[IFLA_GRE_TTL] && *(__u8 *)RTA_DATA(tb[IFLA_GRE_TTL]))
		fprintf(f, "ttl %d ", *(__u8 *)RTA_DATA(tb[IFLA_GRE_TTL]));
	else
		fprintf(f, "ttl inherit ");

	if (tb[IFLA_GRE_IFLAGS])
		iflags = *(__u16 *)RTA_DATA(tb[IFLA_GRE_IFLAGS]);

	if (tb[IFLA_GRE_OFLAGS])
		oflags = *(__u16 *)RTA_DATA(tb[IFLA_GRE_OFLAGS]);

	if (iflags & GRE_KEY && tb[IFLA_GRE_IKEY] &&
	    *(__u32 *)RTA_DATA(tb[IFLA_GRE_IKEY])) {
		inet_ntop(AF_INET, RTA_DATA(tb[IFLA_GRE_IKEY]), s2, sizeof(s2));
		fprintf(f, "ikey %s ", s2);
	}

	if (oflags & GRE_KEY && tb[IFLA_GRE_OKEY] &&
	    *(__u32 *)RTA_DATA(tb[IFLA_GRE_OKEY])) {
		inet_ntop(AF_INET, RTA_DATA(tb[IFLA_GRE_OKEY]), s2, sizeof(s2));
		fprintf(f, "ikey %s ", s2);
	}

	if (iflags & GRE_SEQ)
		fputs("iseq ", f);
	if (oflags & GRE_SEQ)
		fputs("oseq ", f);
	if (iflags & GRE_CSUM)
		fputs("icsum ", f);
	if (oflags & GRE_CSUM)
		fputs("ocsum ", f);
}

struct link_util ip6gre_link_util = {
	.id = "ip6gre",
	.maxattr = IFLA_GRE_MAX,
	.parse_opt = ip6gre_parse_opt,
	.print_opt = ip6gre_print_opt,
};

