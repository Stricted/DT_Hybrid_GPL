#############################################################
#
# libdaemon
#
#############################################################
 
 
# Dependencies
LIBDAEMON_DEPENDENCIES= atptypes libdaemon_cfg libdaemon_mk
 
#############################################################
### Version and change logs area
LIBDAEMON_VERSION= 1.00
define DAEMON_CHANGELOGS
 
endef
#############################################################

# Program name
DAEMON_PROGRAM_NAME=daemon

# Target name
DAEMON_TARGET = shared

DAEMON_TYPE=opensrclib

DAEMON_MAKEFILE_DIR=src
 
DAEMON_INDEX=307
 
DAEMON_LICENSE="LGPLv2"

# libdaemon directory
SERVICE_LIBDAEMON_DIR = $(TOPDIR)/package/atp/libraries/opensrc/libdaemon
SERVICE_LIBDAEMON_SRC_DIR = $(SERVICE_LIBDAEMON_DIR)/src

libdaemon_cfg:
	$(Q)if [ -f $(SERVICE_LIBDAEMON_SRC_DIR)/configure ]; then \
	cd $(SERVICE_LIBDAEMON_SRC_DIR) && chmod 755 *; \
	CC=$(CC) CPPFLAGS="-I$(STAGING_DIR)/usr/include" LDFLAGS="-L$(LIB_ATP_DIR)" ./configure --host=$(ARCH)-linux ac_cv_func_setpgrp_void="yes"; \
	fi


libdaemon_mk:
	$(Q)if [ -f $(SERVICE_LIBDAEMON_SRC_DIR)/Makefile ];then \
	$(MAKE)	-C $(SERVICE_LIBDAEMON_SRC_DIR); \
	install -m 777 $(SERVICE_LIBDAEMON_SRC_DIR)/libdaemon/.libs/libdaemon.so.0.5.0 $(LIB_ATP_DIR)/; \
	cd $(LIB_ATP_DIR); \
	rm -f libdaemon.so.0 libdaemon.so; \
	ln -s libdaemon.so.0.5.0 libdaemon.so.0; \
	ln -s libdaemon.so.0.5.0 libdaemon.so;\
	elif [ -f $(LIB_ATP_DIR)/libdaemon.so.0.5.0 ];then \
	cd $(LIB_ATP_DIR); \
	rm -f libdaemon.so.0 libdaemon.so; \
	ln -s libdaemon.so.0.5.0 libdaemon.so.0; \
	ln -s libdaemon.so.0.5.0 libdaemon.so;\
	else $(error) $(LIB_ATP_DIR)/libdaemon not exist! exit 1;\
	fi

# Install lib
define LIBDAEMON_INSTALL_TARGET_CMDS
  install -m 777 $(STAGING_DIR)/lib/libdaemon.so.0.5.0 $(TARGET_DIR)/lib; \
	cd $(TARGET_DIR)/lib; rm -f libdaemon.so; \
	ln -s libdaemon.so.0.5.0 libdaemon.so.0; \
	ln -s libdaemon.so.0.5.0 libdaemon.so;
endef

define LIBDAEMON_BUILD_CMDS
	@echo libdaemon.so generate ok..
	@echo libdaemon build done.
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libdaemon,libdaemon,BUILD_LIBDAEMON))
