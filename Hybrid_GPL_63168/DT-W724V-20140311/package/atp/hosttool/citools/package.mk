#############################################################
#
# citools
#
#############################################################
#
# Dependencies
#
CITOOLS_DEPENDENCIES =

#############################################################
### Version and change logs area
CITOOLS_VERSION = V1.01
define CITOOLS_CHANGELOGS
V1.01:
	1. First release by sunbo.

endef
#############################################################

CITOOLS_FEATURE=buildroot

define CITOOLS_INSTALL_TARGET_CMDS
	
endef

$(eval $(call ATPTARGETS,package/atp/hosttool/citools,citools,BUILD_NONE))

commit-report:
	@LANG=en_US.UTF-8 svn log --xml -v -r {`date +"%Y-%m-%d" -d "1 month ago"`}:{`date +"%Y-%m-%d"`} > $(TOPDIR)/../svnlog.xml
	@LANG=en_US.UTF-8 python $(TOPDIR)/package/atp/hosttool/citools/src/svnlog_parser.py $(TOPDIR)/../svnlog.xml
	@LANG=en_US.UTF-8 java -jar $(TOPDIR)/package/atp/hosttool/citools/src/statsvn.jar $(TOPDIR)/../svnlog.xml . -output-dir $(TOPDIR)/../report

commit-ams:
	#@LANG=en_US.UTF-8 svn log --xml -v -r $(LAST_REV):head > $(TOPDIR)/../svnlog.xml
	@LANG=en_US.UTF-8 python $(TOPDIR)/package/atp/hosttool/citools/src/svnlog_sync.py $(TOPDIR)/../2013_svn_log.xml

upgrade:
ifeq ($(strip $(BUILD_ATP_DEBUG_IAMGE)),y)
	@python $(TOPDIR)/package/atp/hosttool/citools/src/upgrade.py $(TOPDIR)/output/images/$(HGW_IMAGE_DEBUG_WITHOUT_BOOTLOADER_NAME).bin
else
	@python $(TOPDIR)/package/atp/hosttool/citools/src/upgrade.py $(TOPDIR)/output/images/$(HGW_IMAGE_RELEASE_WITHOUT_BOOTLOADER_NAME).bin
endif
