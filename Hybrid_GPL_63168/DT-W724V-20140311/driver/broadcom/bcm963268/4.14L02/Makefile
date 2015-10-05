#
# Makefile for the Linux kernel device drivers.
#
# 9 July 2013, Liuye <lyandin.liu@huawei.com>
# Rewritten to use config feature instead of lists.
#

obj-$(CONFIG_BCM_ENET) 		+= ethernet/$(SUB_CHIP)/
obj-$(CONFIG_BCM_PKTFLOW)	+= pktflow/$(SUB_CHIP)/
obj-$(CONFIG_BCM_BPM)		+= bpm/$(SUB_CHIP)/
obj-$(CONFIG_BCM_INGQOS)	+= ingqos/$(SUB_CHIP)/
#obj-$(CONFIG_BCM_P8021AG)	+= p8021ag/$(SUB_CHIP)/
# PnP must come after ACPI since it will eventually need to check if acpi
# was used and do nothing if so
obj-$(CONFIG_BCM_VLAN)		+= vlan/$(SUB_CHIP)/
obj-$(CONFIG_BCM_SERIAL)	+= serial/ 
obj-$(CONFIG_BCM_PCMSHIM)	+= pcmshim/ 
obj-$(CONFIG_BCM_FAP)		+= fap/$(SUB_CHIP)/

obj-$(CONFIG_BCM_WLAN) 		+= wlan/$(SUB_CHIP)/

obj-$(CONFIG_BCM_BCMDSP)	+= ../hyvoice/dspapp/ 
obj-$(CONFIG_BCM_ENDPOINT)	+= ../hyvoice/endpoint/ 

obj-$(CONFIG_BCM_ADSL)		+= adsl/

obj-$(CONFIG_BCM_XTMCFG)	+= xtmcfg/$(SUB_CHIP)/
obj-$(CONFIG_BCM_XTMRT) 	+= xtmrt/$(SUB_CHIP)/

#obj-$(CONFIG_BCM_TRNG)		+= trng/

obj-$(CONFIG_BCM_PWRMNGT) 	+= pwrmngt/$(SUB_CHIP)/
obj-$(CONFIG_BCM_PKTDMA)	+= pktdma/$(SUB_CHIP)/
obj-$(CONFIG_SPI)	        += spi/
obj-$(CONFIG_BCM_TMS)		+= tms/$(SUB_CHIP)/

clean: