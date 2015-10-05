#
# Makefile for the netlink driver.
#

obj-y  				:= hw_kerneluserspace.o

hw_kerneluserspace-objs += hw_netlink.o

ifeq ($(CONFIG_ATP_CT_CLEAN),y)
hw_kerneluserspace-objs += hw_ctclean.o
endif