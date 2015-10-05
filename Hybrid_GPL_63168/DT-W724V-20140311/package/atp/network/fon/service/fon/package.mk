#############################################################
#
# FON
#
#############################################################
 
 
# Dependencies
FON_DEPENDENCIES = libdaemon libpcap
 
#############################################################
### Version and change logs area
FON_VERSION= V1.00
define FON_CHANGELOGS
V1.00:
	1. First release by l00214441.

 
endef
#############################################################
 
# Program name
FON_PROGRAM_NAME=fon

# Target DIR
FON_MAKEFILE_DIR = src
 
# Target name
FON_TARGET=all
 
FON_TYPE=corebin
 
FON_MAKEFILE_DIR=src
 
FON_INDEX=298
 
FON_LICENSE="PART GPL"

# Build fon
define FON_BUILD_CMDS
    $(Q)if [ -f $$($(2)_SRC_DIR)/Makefile ]; then \
    $(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $$($(2)_SRC_DIR) $$($(2)_TARGET); \
    fi
endef

# Install fon
ifndef GPLREL
define FON_INSTALL_TARGET_CMDS
    mkdir -p $(TARGET_DIR)/etc/init.d; \
    mkdir -p $(TARGET_DIR)/etc/cron.daily; \
    mkdir -p $(TARGET_DIR)/etc/ppp; \
    mkdir -p $(TARGET_DIR)/etc/default; \
    mkdir -p $(TARGET_DIR)/usr/lib/coova-chilli; \
    mkdir -p $(TARGET_DIR)/usr/lib/pppd/2.4.4; \
    mkdir -p $(TARGET_DIR)/usr/share/doc/fonap; \
	ln -s /var/fon/chilli.conf $(TARGET_DIR)/etc/chilli.conf; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/dnsmasq.conf $(TARGET_DIR)/etc; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/fon_device $(TARGET_DIR)/etc; \
	ln -s /var/fon/fon_version $(TARGET_DIR)/etc/fon_version; \
	ln -s /var/fon/fon_revision $(TARGET_DIR)/etc/fon_revision; \
	ln -s /var/fon/xl2tpd.conf $(TARGET_DIR)/etc/xl2tpd.conf; \
	ln -s /var/fon/fon_keyword $(TARGET_DIR)/etc/fon_keyword; \
	ln -s /var/fon/fonsmcd.conf $(TARGET_DIR)/etc/fonsmcd.conf; \
	ln -s /var/fon/fon_mac $(TARGET_DIR)/etc/fon_mac; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/public_sm_rsa_key.pem $(TARGET_DIR)/etc; \
	ln -s /var/fon/ppp-xl2tpd.client $(TARGET_DIR)/etc/ppp/ppp-xl2tpd.client; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/ppp/ip-up $(TARGET_DIR)/etc/ppp; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/ppp/ip-down $(TARGET_DIR)/etc/ppp; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/init.d/* $(TARGET_DIR)/etc/init.d; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/cron.daily/* $(TARGET_DIR)/etc/cron.daily; \
    install -m 777 $$($(2)_SRC_DIR)/deb/etc/default/* $(TARGET_DIR)/etc/default; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/* $(TARGET_DIR)/usr/lib; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/coova-chilli/* $(TARGET_DIR)/usr/lib/coova-chilli; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/pppd/2.4.4/* $(TARGET_DIR)/usr/lib/pppd/2.4.4; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/sbin/* $(TARGET_DIR)/usr/sbin; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/share/doc/fonap/* $(TARGET_DIR)/usr/share/doc/fonap
endef
else
define FON_INSTALL_TARGET_CMDS
    mkdir -p $(TARGET_DIR)/usr/lib/coova-chilli; \
    mkdir -p $(TARGET_DIR)/usr/lib/pppd/2.4.4; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/* $(TARGET_DIR)/usr/lib; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/coova-chilli/* $(TARGET_DIR)/usr/lib/coova-chilli; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/lib/pppd/2.4.4/* $(TARGET_DIR)/usr/lib/pppd/2.4.4; \
    install -m 777 $$($(2)_SRC_DIR)/deb/usr/sbin/* $(TARGET_DIR)/usr/sbin	
endef
endif

$(eval $(call ATPTARGETS,package/atp/network/fon/service/fon,fon,BUILD_FON))
