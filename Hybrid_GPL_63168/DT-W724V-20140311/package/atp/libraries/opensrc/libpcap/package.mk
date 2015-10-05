#############################################################
#
# libpcap
#
#############################################################
 
 
# Dependencies
LIBPCAP_DEPENDENCIES= atptypes libpcap_cfg libpcap_mk
 
#############################################################
### Version and change logs area
LIBPCAP_VERSION= 1.00
define DAEMON_CHANGELOGS
 
endef
#############################################################

# Program name
PCAP_PROGRAM_NAME=pcap 

# Target name
PCAP_TARGET = static

PCAP_TYPE=opensrclib
 
PCAP_MAKEFILE_DIR=src

PCAP_INDEX=310
 
PCAP_LICENSE="BSD"

# libpcap directory
SERVICE_LIBPCAP_DIR = $(TOPDIR)/package/atp/libraries/opensrc/libpcap
SERVICE_LIBPCAP_SRC_DIR = $(SERVICE_LIBPCAP_DIR)/src

libpcap_cfg:
	$(Q)if [ -f $(SERVICE_LIBPCAP_SRC_DIR)/configure ]; then \
	cd $(SERVICE_LIBPCAP_SRC_DIR) && chmod 755 *; \
	CC=$(CC) CFLAGS="-I$(STAGING_DIR)/usr/include" LDFLAGS="-L$(LIB_ATP_DIR)" ./configure --host=$(ARCH)-linux ac_cv_func_setpgrp_void="yes" --with-pcap=linux; \
	fi


libpcap_mk:
	$(Q)if [ -f $(SERVICE_LIBPCAP_SRC_DIR)/Makefile ];then \
	$(MAKE)	-C $(SERVICE_LIBPCAP_SRC_DIR); \
    install -m 777 $(SERVICE_LIBPCAP_SRC_DIR)/libpcap.a $(TARGET_DIR)/lib; \
    install -m 777 $(SERVICE_LIBPCAP_SRC_DIR)/libpcap.a $(STAGING_DIR)/lib; \
    install -m 777 $(SERVICE_LIBPCAP_SRC_DIR)/libpcap.a $(STAGING_DIR)/usr/lib; \
	else $(error) $(LIB_ATP_DIR)/libpcap not exist! exit 1;\
	fi

# Install lib
define LIBPCAP_INSTALL_TARGET_CMDS
  install -m 777 $(STAGING_DIR)/lib/libpcap.a $(TARGET_DIR)/lib; 
endef

define LIBPCAP_BUILD_CMDS
	@echo libpcap.a generate ok..
	@echo libpcap build done.
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libpcap,libpcap,BUILD_LIBPCAP))
