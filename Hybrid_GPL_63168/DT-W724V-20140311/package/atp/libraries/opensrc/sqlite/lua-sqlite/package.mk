#############################################################
#
# lua-sqlite
#
#############################################################

#
# Dependencies
#
LUA_SQLITE_DEPENDENCIES = atptypes lua-sqlite-extract lua-sqlite-cfg lua sqlite

#############################################################
### Version and change logs area
LUA_SQLITE_VERSION = V1.01
define LUA_SQLITE_CHANGELOGS
V1.01:
	1. First release by Handy.
endef
#############################################################

LUA_SQLITE_TAR_SRC = $(TOPDIR)/dl/lua-sqlite3-0.4.1.tar.bz2
LUA_SQLITE_AFTER_EXTR_SRC = $(O)/build/lua-sqlite3-0.4.1
LUA_SQLITE_TAR_EXTRACT_OPT = tar xjf

# Makefile directory
LUA_SQLITE_MAKEFILE_DIR =src

# License
LUA_SQLITE_LICENSE = MIT

# PROGRAME_NAME
LUA_SQLITE_PROGRAM_NAME =luasqlite3

# Target name
LUA_SQLITE_TARGET = all

$(O)/package/atp/libraries/opensrc/sqlite/lua-sqlite/.stamp_configured:
	cd $(LUA_SQLITE_SRC_DIR);\
	$(TARGET_ATP_ENVS) ./configure --host $(BR2_TOOLCHAIN_EXTERNAL_CUSTOM_PREFIX)
	touch $(O)/package/atp/libraries/opensrc/sqlite/lua-sqlite/.stamp_configured

lua-sqlite-cfg:$(O)/package/atp/libraries/opensrc/sqlite/lua-sqlite/.stamp_configured

define LUA_SQLITE_INSTALL_TARGET_CMDS
	install -m 777 $$($(2)_BDIR)/libluasqlite3.so $(STAGING_DIR)/lib; \
	install -m 777 $$($(2)_BDIR)/libluasqlite3.so $(TARGET_DIR)/lib; \
	$(TARGET_STRIP) $(STAGING_DIR)/lib/libluasqlite3.so; \
	$(TARGET_STRIP) $(TARGET_DIR)/lib/libluasqlite3.so;\
	install -m 644 $$($(2)_BDIR)/libluasqlite3-loader.lua $(TARGET_DIR)/etc/lua;\
	install -m 644 $$(LUA_SQLITE_SRC_DIR)/sqlite3.lua $$(LUA_SQLITE_SRC_DIR)/luasql-sqlite3.lua $(TARGET_DIR)/etc/lua
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/sqlite/lua-sqlite,lua-sqlite,BUILD_LUA_SQLITE))
