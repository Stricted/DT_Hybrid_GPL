#############################################################
#
# cadaver: a webdav client for network attached storage
#
#############################################################

#
# Dependencies
#
CADAVER_DEPENDENCIES=atptypes openssl openssl_for_nas  cadaver_cfg

CADAVER_VERSION=1.00
define CADAVER_CHANGELOGS
V1.00:
	1. First port from DT W724v code base.
endef

#Program name
CADAVER_PROGRAM_NAME=cadaver

# Target name
CADAVER_TARGET=cadaver

CADAVER_TYPE=opensrcbin

CADAVER_INDEX=299

CADAVER_HAS_DOC=y

CADAVER_LICENSE="GPLv2"

openssl_for_nas: install_openssl_headers_for_nas

# cadaver directory
SERVICE_CADAVER_DIR = $(TOPDIR)/package/atp/libraries/opensrc/cadaver
SERVICE_CADAVER_SRC_DIR = $(SERVICE_CADAVER_DIR)/src
SERVICE_NEON_0_29_6_DIR = package/atp/libraries/opensrc/cadaver/src/lib/neon-0.29.6

SERVICE_OPENSSL_SRC_DIR = $(TOPDIR)/output/build/openssl-1.0.1/src

package/atp/libraries/opensrc/cadaver/.stamp_configured:
	if [ -f $(SERVICE_CADAVER_SRC_DIR)/configure ]; then \
		cd $(SERVICE_CADAVER_SRC_DIR); \
		chmod 777 ./configure; \
		./configure --host=$(ARCH)-linux CC=$(CC) \
		--disable-nls  \
		--enable-warnings  \
		--enable-threadsafe-ssl=posix \
		--enable-threads=posix \
		--prefix=$(STAGING_DIR)/bin \
		--includedir=$(STAGING_DIR)/usr/include \
		--libexecdir=$(BR2_TOOLCHAIN_EXTERNAL_PATH)/libexec \
		--libdir=$(STAGING_DIR)/lib:$(STAGING_DIR)/usr/lib \
		--with-ssl=openssl \
		--with-libs=$(STAGING_DIR) \
		--with-atpusrinc=$(STAGING_DIR)/usr/include \
		--with-atpusrlib=$(STAGING_DIR)/usr/lib \
		--with-atpusrbin=$(STAGING_DIR)/usr/bin \
		--with-atpcrypto=crypto_openssl \
		--with-atpssl=ssl_openssl \
		--with-openssldirfornas=openssl \
        --with-included-expat \
		#--with-atpinc=$(STAGING_DIR)/usr/include \
		#--with-atpdrvinc=$(STAGING_DIR)/usr/include \
		#--with-atplib=$(STAGING_DIR)/lib \
		#--with-atplibname=gplutil \
		#--with-crypto=crypto_openssl \
		#--with-cryptolibdir=$(STAGING_DIR)/lib; \
		else $(error) $(SERVICE_CADAVER_SRC_DIR)/configure not exist! exit 1;\
		fi
	-@touch package/atp/libraries/opensrc/cadaver/.stamp_configured

package/atp/libraries/opensrc/cadaver/src/lib/neon-0.29.6/.stamp_configured:
	if [ -f $(SERVICE_NEON_0_29_6_DIR)/configure ]; then \
		cd $(SERVICE_NEON_0_29_6_DIR); \
		chmod 777 ./configure; \
		./configure --host=$(ARCH)-linux CC=$(CC) \
		--disable-nls  \
		--enable-warnings  \
		--enable-threadsafe-ssl=posix \
		--enable-threads=posix \
		--prefix=$(STAGING_DIR)/bin \
		--includedir=$(STAGING_DIR)/usr/include \
		--libexecdir=$(BR2_TOOLCHAIN_EXTERNAL_PATH)/libexec \
		--libdir=$(STAGING_DIR)/lib:$(STAGING_DIR)/usr/lib \
		--with-ssl=openssl \
		--with-libs=$(STAGING_DIR) \
		--with-expat \
		--enable-shared \
		--with-atpusrinc=$(STAGING_DIR)/usr/include \
		--with-atpusrlib=$(STAGING_DIR)/usr/lib \
		--with-atpusrbin=$(STAGING_DIR)/usr/bin \
		--with-atpcrypto=crypto_openssl \
		--with-atpssl=ssl_openssl \
		#--with-openssldirfornas=openssl \
		#--with-atpinc=$(STAGING_DIR)/usr/include \
		#--with-atpdrvinc=$(STAGING_DIR)/usr/include \
		#--with-atplib=$(STAGING_DIR)/lib \
		#--with-atplibname=gplutil \
		#--with-crypto=crypto_openssl \
		#--with-cryptolibdir=$(STAGING_DIR)/lib; \
		else $(error) $(SERVICE_NEON_0_29_6_DIR)/configure not exist! exit 1;\
		fi

	-@touch package/atp/libraries/opensrc/cadaver/src/lib/neon-0.29.6/.stamp_configured



cadaver_cfg: package/atp/libraries/opensrc/cadaver/.stamp_configured  #package/atp/libraries/opensrc/cadaver/src/lib/neon-0.29.6/.stamp_configured

cadaver_mk:
	$(Q)if [ -f $(SERVICE_CADAVER_SRC_DIR)/Makefile ]; then \
		$(TARGET_ATP_ENVS) $(MAKE1) -C $(SERVICE_CADAVER_SRC_DIR); \
		fi

#当前(STAGING_DIR)/usr/include/openssl目录下的*.h文件实际为开源软件polar_ssl库中的头文件，
#但该模块实际需要openssL的头文件，为了不将当前已有的polar_ssl头文件给覆盖，因此编译前需要将
#usr/include/openssl（polar_ssl）中的头文件保存起来，故将其暂时放到$(STAGING_DIR)/usr/include/polar_openssl
#文件夹中，然后将开源软件openssL的头文件导入到(STAGING_DIR)/usr/include/openssl目录下，让esmtp可以正常编译通过
#等该模块编译完毕后，再需要将polar_ssl的头文件重新放到(STAGING_DIR)/usr/include/openssl目录下，
#该操作在end_openssl中完成

install_openssl_headers_for_nas: 
	if [ -e package/atp/libraries/opensrc/cadaver/.stamp_opensslconfigured ]; then \
		echo "headers have installed already"; \
	else \
		if [ -d $(STAGING_DIR)/usr/include/polar_openssl ]; then \
			rm -fr $(STAGING_DIR)/usr/include/openssl; \
			mkdir -p $(STAGING_DIR)/usr/include/openssl; \
			install -m 777 $(SERVICE_OPENSSL_SRC_DIR)/include/openssl/*.h $(STAGING_DIR)/usr/include/openssl; \
			touch package/atp/libraries/opensrc/cadaver/.stamp_opensslconfigured; \
		else \
			mv  $(STAGING_DIR)/usr/include/openssl  $(STAGING_DIR)/usr/include/polar_openssl ; \
			mkdir -p $(STAGING_DIR)/usr/include/openssl; \
			install -m 777 $(SERVICE_OPENSSL_SRC_DIR)/include/openssl/*.h $(STAGING_DIR)/usr/include/openssl; \
			touch package/atp/libraries/opensrc/cadaver/.stamp_opensslconfigured; \
		fi \
	fi


# Install bin
define CADAVER_INSTALL_TARGET_CMDS
	install $(SERVICE_CADAVER_SRC_DIR)/cadaver $(STAGING_DIR)/bin; \
	install $(SERVICE_CADAVER_SRC_DIR)/cadaver $(TARGET_DIR)/bin; \
	rm -rf $(STAGING_DIR)/usr/include/openssl ; \
	if [ -e $(STAGING_DIR)/usr/include/polar_openssl ]; then \
		rm -rf $(STAGING_DIR)/usr/include/openssl; \
		mv $(STAGING_DIR)/usr/include/polar_openssl $(STAGING_DIR)/usr/include/openssl; \
	fi 
endef

#define CADAVER_BUILD_CMDS
#	@echo cadaver build ok..
#	@echo cadaver build done.
#endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/cadaver, cadaver, BUILD_CADAVER))
