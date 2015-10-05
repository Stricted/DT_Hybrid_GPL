#############################################################
#
# wlan application for Broadcom
#
#############################################################

#
# Dependencies
#
WLANAPP_DEPENDENCIES = bhalapi bcmcrypto bcmshared wlupnp nas eapd lltd nvram wlcsm acsd wlctl wps

#############################################################
### Version and change logs area
WLANAPP_VERSION = V1.01
define WLANAPP_CHANGELOGS
V1.00:
	1. First release by y00105954.

endef
#############################################################
# Target name
WLANAPP_TARGET = ALL

define WLANAPP_CLEAN_CMDS
	@echo "=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)==";\
	
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/eapd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/eapd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/eapd clean; \
	fi

	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/lltd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/lltd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/lltd clean; \
	fi	
	
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/nvram/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/nvram/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/nvram clean; \
	fi
	
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/nas/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/nas/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/nas clean; \
	fi	

	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wps/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/wps/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wps clean; \
	fi

	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/acsd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/acsd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/acsd clean; \
	fi	
	
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/libupnp/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/wlupnp/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/libupnp clean; \
	fi	

	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/bcmcrypto/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/bcmcrypto/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/bcmcrypto clean; \
	fi	

	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/shared/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/bcmshared/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/shared clean; \
	fi	
	
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wl/exe/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$$(WLANAPP_BDIR)/wlctl/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wl/exe clean; \
	fi	
	
	@echo "==clean $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)==";
endef

ifneq ($(strip $(BUILD_WLAN)),)
eapd: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/eapd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/eapd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/eapd dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@ not exist! exit	1;\
	fi
else
eapd:
endif

##lltd
ifneq ($(strip $(BUILD_WPS)),)
lltd: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/lltd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/lltd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/lltd dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/lld2d ];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/lld2d	not	exist! exit	1;\
	fi
else
lltd:
endif

##nvram
ifneq ($(strip $(BUILD_WLAN)),)
nvram: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/nvram/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/nvram/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/nvram dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@	not	exist! exit	1;\
	fi
else
nvram:
endif

##wlcsm
ifneq ($(strip $(BUILD_WLAN)),)
wlcsm: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wlcsm/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/wlcsm/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wlcsm dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@	not	exist! exit	1;\
	fi
else
wlcsm:
endif

##nas
ifneq ($(strip $(BUILD_WLAN)),)
nas:
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/nas/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/nas/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/nas dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@	not	exist! exit	1;\
	fi
else
nas:
endif

##wps
ifneq ($(strip $(BUILD_WPS)),)
wps:
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wps/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/wps/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wps dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@	not	exist! exit	1;\
	fi
else
wps:
endif

##acsd
ifneq ($(strip $(BUILD_WLAN)),)
acsd:
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/acsd/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/acsd/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/acsd dynamic; \
	elif [ -f $(INSTALL_DIR)/bin/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@	not	exist! exit	1;\
	fi
else
acsd:
endif

#wlupnp
ifneq ($(strip $(BUILD_WLAN)),)
wlupnp: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/libupnp/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/wlupnp/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/libupnp dynamic; \
	elif [ -f $(LIB_ATP_DIR)/$@	];then echo	;\
	else $(error) $(LIB_ATP_DIR)/$@ not exist! exit	1;\
	fi
else
wlupnp:
endif

#bcmcrypto
ifneq ($(strip $(BUILD_WLAN)),)
bcmcrypto: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/bcmcrypto/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/bcmcrypto/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/bcmcrypto dynamic; \
	elif [ -f $(LIB_ATP_DIR)/$@	];then echo	;\
	else $(error) $(LIB_ATP_DIR)/$@ not exist! exit	1;\
	fi
else
bcmcrypto:
endif

#bcmshared
ifneq ($(strip $(BUILD_WLAN)),)
bcmshared: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/shared/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/bcmshared/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/router/shared dynamic; \
	elif [ -f $(LIB_ATP_DIR)/$@	];then echo	;\
	else $(error) $(LIB_ATP_DIR)/$@ not exist! exit	1;\
	fi
else
bcmshared:
endif

#wlctl
ifneq ($(strip $(BUILD_WLAN)),)
wlctl: 
	@if	[ -f $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wl/exe/Makefile ];then \
	$(TARGET_ATP_ENVS) O=$(WLANAPP_BDIR)/wlctl/ $(MAKE1)	-C $(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/wl/exe dynamic; \
	elif [ -f $(LIB_ATP_DIR)/$@	];then echo	;\
	else $(error) $(INSTALL_DIR)/bin/$@ not exist! exit	1;\
	fi
else
wlctl:
endif
	
define WLANAPP_INSTALL_TARGET_CMDS
	@echo "=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)== Install finished"
endef

$(eval $(call ATPTARGETS,$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME),wlanapp,BUILD_WLAN))
