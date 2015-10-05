#############################################################
#
# FETCHMAIL
#
#############################################################
 
 
# Dependencies
FETCHMAIL_DEPENDENCIES= ssl  fetchmail_cfg
 
#############################################################
### Version and change logs area
FETCHMAIL_VERSION=
define FETCHMAIL_CHANGELOGS
 
endef
#############################################################
 
# Program name
FETCHMAIL_PROGRAM_NAME=fetchmail
 
# Target name
FETCHMAIL_TARGET=all
 
FETCHMAIL_TYPE=coresdk
 
FETCHMAIL_MAKEFILE_DIR=src
 
FETCHMAIL_INDEX=250
 
FETCHMAIL_HAS_DOC=y
 
FETCHMAIL_LICENSE= "GPLv2"
 
### fetchmail support
$(O)/package/atp/voice/email/service/fetchmail/src/.fetchmail_file_flag :
	chmod -R 777 ./package/atp/voice/email/service/fetchmail/src/configure
	@if [ -f ./package/atp/voice/email/service/fetchmail/src/configure ]; then \
	cd ./package/atp/voice/email/service/fetchmail/src; \
	./configure --host=$(ARCH)-linux CC=$(CC) CFLAGS="-I$(STAGING_INC_DIR)" LDFLAGS=-L$(LIB_ATP_DIR) LIBS="-lgplutil -lsqlite -ldl -lpthread  -lssl -liconv" --disable-ETRN --disable-ODMR --enable-fallback=no --disable-rpath --disable-nls --disable-dependency-tracking --disable-option-checking --without-libintl-prefix --without-hesiod --without-syslog; \
	fi
	@touch ./package/atp/voice/email/service/fetchmail/src/.fetchmail_file_flag; 
	

fetchmail_cfg: $(O)/package/atp/voice/email/service/fetchmail/src/.fetchmail_file_flag 

define FETCHMAIL_INSTALL_TARGET_CMDS
	$(Q)install -m 777 ./package/atp/voice/email/service/fetchmail/src/fetchmail $(TOPDIR)/output/target/bin
endef
 
$(eval $(call ATPTARGETS,package/atp/voice/email/service/fetchmail,fetchmail,BUILD_FETCHEMAIL))