#############################################################
#
# bftpd
#
#############################################################

#
# Dependencies
#
BFTPD_DEPENDENCIES = atptypes gplutil ssl

#############################################################
### Version and change logs area
BFTPD_VERSION = V1.00
define BFTPD_CHANGELOGS
V1.00:
	1. First release by w00190448.

endef
#############################################################
# Target name
BFTPD_TARGET = all

# Target index
BFTPD_INDEX = 78

# Target type
BFTPD_TYPE = service

# Target license
BFTPD_LICENSE = GPL

# Target name
BFTPD_PROGRAM_NAME = bftpd

define BFTPD_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(O)/package/atp/applications/usbstorage/service/ftpd/$$(BFTPD_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin
	$(Q)install -m 777 $(O)/package/atp/applications/usbstorage/service/ftpd/$$(BFTPD_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin
	$(Q)$(TARGET_STRIP) $(STAGING_DIR)/bin/$$(BFTPD_FULL_PROGRAM_NAME)
	$(Q)$(TARGET_STRIP) $(TARGET_DIR)/bin/$$(BFTPD_FULL_PROGRAM_NAME)
endef

$(eval $(call ATPTARGETS,package/atp/applications/usbstorage/service/ftpd,bftpd,BUILD_FTP_SERVER))
