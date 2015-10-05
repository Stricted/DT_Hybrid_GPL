# File: bcmdrivers/char/xtmcfg/bcm963xx/Makefile
#
# Makefile for the BCM63xx bcmxtmcfg driver.
#

obj-$(CONFIG_BCM_XTMCFG) += bcmxtmcfg.o
bcmxtmcfg-objs := xtmcfgdrv.o xtmcfgmain.o xtmprocessor.o xtminterface.o xtmconnection.o xtmoamhandler.o xtmoslinux.o xtmasmhandler.o

EXTRA_CFLAGS += -DCONFIG_$(BRCM_CHIP) -fno-exceptions -fno-rtti -I. -I$(INC_DRIVER_VENDOR_DIR) -I$(DRIVER_VENDOR_DIR)/adsl
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/xtmrt/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268/4ke 
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/ethernet/shared 
EXTRA_CFLAGS += -DDPHY_LOOPBACK
EXTRA_CFLAGS += -Werror -Wfatal-errors
ifeq "$(BRCM_CHIP)" "63268"
EXTRA_CFLAGS += -DCONFIG_VDSL_SUPPORTED
endif

# no-rtti is not applicable to c files
CFLAGS_REMOVE_xtmcfgdrv.o := -fno-rtti
CFLAGS_REMOVE_xtmoslinux.o := -fno-rtti

# Some compiler flags which are defined at the top level Makefile are not
# applicable to C++ files.  Filter them out to avoid warnings.  This is a
# brute force approach, but is safer than modifying the top level Makefiles.
STRIP_FROM_CPP := -Wstrict-prototypes -Werror-implicit-function-declaration \
                  -Wdeclaration-after-statement -Wno-pointer-sign
CFLAGS_REMOVE_xtmcfgmain.o := $(STRIP_FROM_CPP)
CFLAGS_REMOVE_xtmprocessor.o := $(STRIP_FROM_CPP)
CFLAGS_REMOVE_xtminterface.o := $(STRIP_FROM_CPP)
CFLAGS_REMOVE_xtmconnection.o := $(STRIP_FROM_CPP)
CFLAGS_REMOVE_xtmoamhandler.o := $(STRIP_FROM_CPP)

# $(O)%.o: %.c
#	@echo "	Compiling " $<
#	@mkdir -p $(dir $@)
#	$(Q)$(CC) $(CFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

#$(O)%.o: %cc
#	@echo "	Compiling " $<
#	@mkdir -p $(dir $@)
#	@$(CPP) $(CPPFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

clean:
	rm -f core *.o *.a *.s

