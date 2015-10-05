#############################################################
#
# Build the squashfs root filesystem image
#
#############################################################

ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4),y)
ROOTFS_SQUASHFS_DEPENDENCIES = host-squashfs

ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_LZO),y)
ROOTFS_SQUASHFS_ARGS += -comp lzo
else
ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_LZMA),y)
ROOTFS_SQUASHFS_ARGS += -comp lzma
else
ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_XZ),y)
ROOTFS_SQUASHFS_ARGS += -comp xz
else
ROOTFS_SQUASHFS_ARGS += -comp gzip
endif
endif
endif
endif

ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_0),y)
ROOTFS_SQUASHFS_DEPENDENCIES = host-squashfs
ROOTFS_SQUASHFS_ARGS=-be
endif

ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS2),y)
ROOTFS_SQUASHFS_DEPENDENCIES = host-squashfs

ifeq ($(BR2_ENDIAN),"BIG")
ROOTFS_SQUASHFS_ARGS=-be
else
ROOTFS_SQUASHFS_ARGS=-le
endif

else
ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS3),y)
ROOTFS_SQUASHFS_DEPENDENCIES = host-squashfs3

ifeq ($(BR2_ENDIAN),"BIG")
ROOTFS_SQUASHFS_ARGS=-be
else
ROOTFS_SQUASHFS_ARGS=-le
endif
endif

endif

define ROOTFS_SQUASHFS_CMD
	$(HOST_DIR)/usr/bin/mksquashfs $(TARGET_DIR) $$@ -noappend -processors 8 -b $(ATP_SQUASHFS_BLOCK_SIZE)\
		$(ROOTFS_SQUASHFS_ARGS) && \
	chmod 0644 $$@
endef

$(eval $(call ROOTFS_TARGET,squashfs))
