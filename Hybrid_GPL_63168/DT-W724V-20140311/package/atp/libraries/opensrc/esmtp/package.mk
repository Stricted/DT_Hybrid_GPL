#############################################################
#
# esmtp: a MTA tool for sending email
#
#############################################################

#
# Dependencies
#
ESMTP_DEPENDENCIES = atptypes openssl prepare_openssl libesmtp_cfg build_libesmtp end_openssl install_libesmtp esmtp_cfg build_esmtp 

ESMTP_VERSION=1.00
define ESMTP_CHANGELOGS
V1.00:
	1. First port from DT W724v code base.
endef

#Program name
ESMTP_PROGRAM_NAME=esmtp

# Target name
ESMTP_TARGET=esmtp

ESMTP_TYPE=opensrcbin

ESMTP_INDEX=333

ESMTP_HAS_DOC=y

ESMTP_LICENSE="LGPL"

# libesmtp directory
SERVICE_LIBESMTP_DIR = $(TOPDIR)/package/atp/libraries/opensrc/esmtp/libesmtp
SERVICE_LIBESMTP_SRC_DIR = $(SERVICE_LIBESMTP_DIR)/src
SERVICE_LIBESMTP_CRAMMD5_LIB_DIR = $(SERVICE_LIBESMTP_SRC_DIR)/crammd5/.libs
SERVICE_LIBESMTP_LOGIN_LIB_DIR = $(SERVICE_LIBESMTP_SRC_DIR)/login/.libs
SERVICE_LIBESMTP_NTLM_LIB_DIR = $(SERVICE_LIBESMTP_SRC_DIR)/ntlm/.libs
SERVICE_LIBESMTP_PLAIN_LIB_DIR = $(SERVICE_LIBESMTP_SRC_DIR)/plain/.libs

SERVICE_OPENSSL_DIR = $(TOPDIR)/output/build/openssl-1.0.1
SERVICE_OPENSSL_SRC_DIR = $(SERVICE_OPENSSL_DIR)/src

SERVICE_ESMTP_DIR = $(TOPDIR)/package/atp/libraries/opensrc/esmtp
SERVICE_ESMTP_SRC_DIR = $(SERVICE_ESMTP_DIR)/src

$(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_libesmtp_configured:
	if [ -f $(SERVICE_LIBESMTP_SRC_DIR)/configure ]; then \
		cd $(SERVICE_LIBESMTP_SRC_DIR); \
		chmod 777 ./configure; \
		./configure  --enable-more-warnings=yes --enable-ntlm --enable-etrn --enable-pthreads=no --host=$(ARCH)-linux  --enable-shared --enable-debug --with-openssl=$(STAGING_DIR) --with-auth-plugin-dir=/var/local/esmtp-plugins CC=$(CC); \
		else $(error) $(SERVICE_LIBESMTP_SRC_DIR)/configure not exist! exit 1;\
		fi
	-@touch $(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_libesmtp_configured

libesmtp_cfg: $(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_libesmtp_configured 

esmtp_cfg: $(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_configured

$(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_configured:
	if [ -f $(SERVICE_ESMTP_SRC_DIR)/configure ]; then \
		cd $(SERVICE_ESMTP_SRC_DIR); \
		chmod 777 ./configure; \
		./configure --with-libesmtp=$(STAGING_DIR) --host=$(ARCH)-linux CC=$(CC); \
		else $(error) $(SERVICE_ESMTP_SRC_DIR)/configure not exist! exit 1; \
		fi
	-@touch $(TOPDIR)/output/package/atp/libraries/opensrc/esmtp/.stamp_configured
	
#说明:由于库上默认为polar_ssl，而polar_ssl和openssl的头文件不同，因此当编译esmtp前需要将openssl的头文件替换
#staging/usr/include/openssl下面的头文件，编译完毕后再替换回来
prepare_openssl:
	$(Q)if [ -d $(STAGING_DIR)/usr/include/polar_openssl ]; then \
	    rm -rf $(STAGING_DIR)/usr/include/openssl; \
		mkdir $(STAGING_DIR)/usr/include/openssl; \
		install -m 777 $(SERVICE_OPENSSL_SRC_DIR)/include/openssl/*.h $(STAGING_DIR)/usr/include/openssl; \
		echo "openssl directory already exist!"; \
	else \
		mv $(STAGING_DIR)/usr/include/openssl $(STAGING_DIR)/usr/include/polar_openssl; \
		mkdir $(STAGING_DIR)/usr/include/openssl; \
		install -m 777 $(SERVICE_OPENSSL_SRC_DIR)/include/openssl/*.h $(STAGING_DIR)/usr/include/openssl; \
		echo "install openssl directory!"; \
	fi
	
ifndef GPLREL
end_openssl:
	rm -rf $(STAGING_DIR)/usr/include/openssl; \
	mv $(STAGING_DIR)/usr/include/polar_openssl $(STAGING_DIR)/usr/include/openssl
else
end_openssl: 
	$(Q)if [ -d $(STAGING_DIR)/usr/include/polar_openssl ]; then \
		rm -rf $(STAGING_DIR)/usr/include/openssl; \
		mv $(STAGING_DIR)/usr/include/polar_openssl $(STAGING_DIR)/usr/include/openssl; \
	else \
		rm -rf $(STAGING_DIR)/usr/include/openssl; \
	fi 
endif	

build_libesmtp:
	$(Q)if [ -f $(SERVICE_LIBESMTP_SRC_DIR)/Makefile ]; then \
		$(TARGET_ATP_ENVS) $(MAKE1) -C $(SERVICE_LIBESMTP_SRC_DIR) all; \
	fi

build_esmtp:
	$(Q)if [ -f $(SERVICE_ESMTP_SRC_DIR)/Makefile ]; then \
		$(TARGET_ATP_ENVS) $(MAKE1) -C $(SERVICE_ESMTP_SRC_DIR) all; \
	fi
	
install_libesmtp:
	install -m 777 $(SERVICE_LIBESMTP_SRC_DIR)/libesmtp.h $(STAGING_DIR)/usr/include; \
	install -m 777 $(SERVICE_LIBESMTP_SRC_DIR)/auth-client.h $(STAGING_DIR)/usr/include; \
	install -m 777 $(SERVICE_LIBESMTP_SRC_DIR)/.libs/libesmtp.a $(STAGING_DIR)/lib; \
	install -m 777 $(SERVICE_LIBESMTP_SRC_DIR)/.libs/libesmtp.a $(TARGET_DIR)/lib; \
	install -m 777 $(SERVICE_LIBESMTP_CRAMMD5_LIB_DIR)/sasl-cram-md5.so $(TARGET_DIR)/lib; \
	install -m 777 $(SERVICE_LIBESMTP_LOGIN_LIB_DIR)/sasl-login.so $(TARGET_DIR)/lib; \
	install -m 777 $(SERVICE_LIBESMTP_NTLM_LIB_DIR)/sasl-ntlm.so $(TARGET_DIR)/lib; \
	install -m 777 $(SERVICE_LIBESMTP_PLAIN_LIB_DIR)/sasl-plain.so $(TARGET_DIR)/lib; \

	
define ESMTP_BUILD_CMDS
	@echo libesmtp build ok..
	@echo libesmtp build done.
endef

define ESMTP_INSTALL_TARGET_CMDS
    install -m 777 $(SERVICE_ESMTP_SRC_DIR)/esmtp $(STAGING_DIR)/bin; \
	install -m 777 $(SERVICE_ESMTP_SRC_DIR)/esmtp $(TARGET_DIR)/bin
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/esmtp, esmtp, BUILD_ESMTP))
