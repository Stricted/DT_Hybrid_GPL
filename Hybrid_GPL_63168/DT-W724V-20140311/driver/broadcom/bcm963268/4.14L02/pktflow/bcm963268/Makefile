# File: bcmdrivers/char/fcache/bcm963xx/Makefile
#
# Makefile for the BCM63xx Flow Forwarding Cache module
#
BCMARCH=$(subst ",,$(CONFIG_BCM_CPU_ARCH_NAME))


ifeq ("$(CONFIG_BCM96838)" ,"y")
	CONFIG_PLATFORM=oren
	#to be removed after sasha's RDD move
	EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/rdp
	EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/bcm968500/base
	EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/bcm968500/os
endif


pktflow-objs   := fcache_$(BCMARCH).o fcachedrv.o flwstats.o

ifeq  ($(strip $(CONFIG_BCM_FHW)),y)
pktflow-objs   += fcachehw.o
endif

obj-$(CONFIG_BCM_PKTFLOW) += pktflow.o

#EXTRA_CFLAGS += -I. -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD) -I$(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I. -I$(INC_DRIVER_VENDOR_DIR)
EXTRA_CFLAGS += -Werror -Wfatal-errors

havesrc := $(wildcard $(src)/fcache.c)

ifneq ($(strip $(havesrc)),)
$(src)/fcache_$(BCMARCH).c : $(src)/fcache.c
	ln -fs fcache.c $(src)/fcache_$(BCMARCH).c
endif

ifeq  ($(strip $(BRCM_DRIVER_PKTFLOW_DEBUG)),y)
EXTRA_CFLAGS += -DPKTDBG
endif



# -include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s

