#!/bin/sh
#
# Copyright (C) 2007 FON Wireless Ltd. All rights Reserved
#
# $Rev: 57 $
# 
# Usage: rule.sh set <local_address> <device> <table> <wan_address>
#        rule.sh delete <local_address> <device> <table ><wan_address>
#
if [ $# -ne 5 ]; then
	echo "Error: invalid number of parameters ($#)"
	echo "Usage: rule.sh <set|delete> <addr> <device> <table> <wanaddr>"
	exit 1
fi
mode=$1
addr=$2
device=$3
table=$4
wanaddr=$5
pppfile="/var/fon/$device"
echo $mode $addr $device $table $wanadd
delrule() { ip rule del from $1 table $2 && delrule $1 $2; }
if [ "$mode" = "set" ]; then 
	ip route add default dev $device table $table
	if [ $? -ne 0 ]; then
		exit 1
	fi
	delrule $addr $table
	ip rule add from $addr table $table
	delrule $wanaddr $table
	ip rule add from $wanaddr table $table
	if [ $? -ne 0 ]; then
		exit 2
	fi
	
	echo -n "$addr" > $pppfile
	
	iptables -t nat -I POSTROUTING 1 -o $device -j SNAT --to-source=$wanaddr
	if [ $? -ne 0 ]; then
		exit 3
	fi
	exit 0
elif [ "$mode" = "unset" ]; then
	delrule $addr $table
	delrule $wanaddr $table
	ip route del default dev $device table $table
	iptables -t nat -D POSTROUTING -o $device -j SNAT --to-source=$wanaddr
	exit $?
else
	echo "Error: invalid mode $mode"
	echo "Usage: rule.sh <set|delete> <addr> <device> <table> <wanaddr>"
	exit 1
fi

