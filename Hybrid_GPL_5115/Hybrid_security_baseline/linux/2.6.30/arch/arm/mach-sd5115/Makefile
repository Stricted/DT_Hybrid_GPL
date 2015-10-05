#
# Makefile for the linux kernel.
#


KBUILD_AFLAGS	:=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=softvfp+vfp)
LDFLAGS		+=--no-warn-mismatch

obj-y					:= core.o clock.o hi_drv_wdt.o
obj-y	+= cache-hil2.o
obj-$(CONFIG_PCI)           += pcie.o


