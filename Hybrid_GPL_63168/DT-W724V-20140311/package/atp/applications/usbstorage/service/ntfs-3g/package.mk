#############################################################
#
#SAMBA
#
#############################################################

#
# Dependencies
#
NTFS_DEPENDENCIES =atptypes ntfs-extract ntfs-3g

#############################################################
### Version and change logs area
NTFS_VERSION = V1.01
define NTFS_CHANGELOGS
V1.00:
	1. First release by l00193021.

endef
#############################################################

NTFS_TAR_SRC = $(TOPDIR)/dl/ntfs-3g-2012.1.15.tar.gz
NTFS_AFTER_EXTR_SRC = $(O)/build/ntfs-3g-2012.1.15
NTFS_TAR_EXTRACT_OPT = tar zxvf

SERVICE_NTFS_SRC_DIR = $(NTFS_AFTER_EXTR_SRC)/src

# Target name
NTFS_TARGET = all

# Target index
NTFS_INDEX = 124

INC_DIR=$(O)/staging/usr/include

ntfs-3g:
	@if [ -f $(SERVICE_NTFS_SRC_DIR)/Makefile ];then \
	cd $(SERVICE_NTFS_SRC_DIR); \
	$(MAKE)	-C $(SERVICE_NTFS_SRC_DIR) CC=$(CROSS_COMPILE)gcc;\
	else \
	cd 	$(SERVICE_NTFS_SRC_DIR); \
	chmod 777 configure;\
	./configure --host=mips-linux  --target=mips-linux  --enable-shared  CC=$(CC) CFLAGS="-I$(INC_DIR)";\
	$(MAKE)	-C $(SERVICE_NTFS_SRC_DIR) CC=$(CROSS_COMPILE)gcc ;\
	fi

define NTFS_INSTALL_TARGET_CMDS
	$(Q)install -m 755 $(SERVICE_NTFS_SRC_DIR)/libntfs-3g/.libs/libntfs-3g.so.83.0.0 $(O)/target/lib/libntfs-3g.so.83
	$(Q)install -m 755 $(SERVICE_NTFS_SRC_DIR)/src/.libs/ntfs-3g $(O)/target/bin
endef

define NTFS_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_NTFS_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_NTFS_SRC_DIR) clean; \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/applications/usbstorage/service/ntfs-3g,ntfs,BUILD_NTFS_3G))
