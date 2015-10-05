#! /usr/bin/env python
#coding=utf-8
import sys
import string
import os
# [['BUILD_UPG','# BUILD_UPG is not set'],]
debug_configs = \
[
['BUILD_ATP_RELEASE_IAMGE','# BUILD_ATP_RELEASE_IAMGE is not set\n'],
['BUILD_ATP_DEBUG_IAMGE','BUILD_ATP_DEBUG_IAMGE=y\n'],
['BUILD_DEFAULTCFG_FILE','BUILD_DEFAULTCFG_FILE="$(CUSTOMER)/$(VENDOR)/debug_defaultcfg.xml"\n'],
['BUILD_DEBUG_VERSION_CWMP','BUILD_DEBUG_VERSION_CWMP=y\n'],
['BUILD_CLI_PROTECT','# BUILD_CLI_PROTECT is not set\n'],
['BUILD_SECURITY_REDLINE_CONSOLE','# BUILD_SECURITY_REDLINE_CONSOLE is not set\n'],
['BUILD_MIRROR','BUILD_MIRROR=y\nBUILD_XDSL_MIRROR=y\nBUILD_ETHUP_MIRROR=y\nBUILD_WLAN_MIRROR=y\n'],
['BUILD_XDSL_MIRROR',''],
['BUILD_ETHUP_MIRROR',''],
['BUILD_WLAN_MIRROR',''],
['BUILD_COREDUMP','BUILD_COREDUMP=y\nATP_COREDUMP_FILE_SIZE=1024\nATP_COREDUMP_DIR="/config/coredump"\n'],
['BR2_ENABLE_DEBUG','BR2_ENABLE_DEBUG=y\n# BR2_DEBUG_1 is not set\nBR2_DEBUG_2=y\n# BR2_DEBUG_3 is not set\n'],
['BR2_PACKAGE_GDB_SERVER','BR2_PACKAGE_GDB_SERVER=y\n# BR2_GDB_VERSION_6_7_1 is not set\nBR2_GDB_VERSION_6_8=y\n# BR2_GDB_VERSION_7_0_1 is not set\n# BR2_GDB_VERSION_7_1 is not set\nBR2_GDB_VERSION="6.8"\n']
]

def chgconfigs(filename):
	newconfigs = "";
	with open(filename) as f:
		for line in f:
			bMatch = 0
			for config in debug_configs:
				index= line.find(config[0])
				if -1 != index:
					c = line[index+len(config[0])]
					if ' '== c or '=' == c :
						newconfigs += config[1]
						bMatch = 1
						break
			if 0 == bMatch:
				newconfigs += line				
	file = open(filename, "w")
	file.write(newconfigs)
	file.close
	
# param1 .config file
# param2 0 debug  1 release
# example build debug version: python scripts/build_debug_version.py configs/atp/hg257/.config 
if __name__ == '__main__':
	chgconfigs(sys.argv[1])
