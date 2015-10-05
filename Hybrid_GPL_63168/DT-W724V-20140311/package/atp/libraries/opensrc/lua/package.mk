#############################################################
#
# lua
#
#############################################################

#############################################################
### Version and change logs area
LUA_VERSION = V1.01
define LUA_CHANGELOGS
V1.01:
	1. First release by sunbo.
endef
#############################################################

LUA_DEPENDENCIES = 

LUA_MAKE_FLAGS=\
	PLAT="linux"

# Makefile directory
LUA_MAKEFILE_DIR = src

# License
LUA_LICENSE=MIT

# Target name
LUA_TARGET=all

# Install libz.so
define LUA_INSTALL_TARGET_CMDS
	$(TARGET_ATP_ENVS) O=$$(LUA_BDIR)/ $(MAKE1) $(LUA_MAKE_FLAGS) -C $$(LUA_DIR)/src install
endef

define LUA_BUILD_CMDS
	$(TARGET_ATP_ENVS) O=$$(LUA_BDIR)/ $(MAKE1) $(LUA_MAKE_FLAGS) -C $$(LUA_DIR)/src all	
endef

define HOST_LUA_BUILD_CMDS
	$(HOST_ATP_ENVS) O=$$(HOST_LUA_BDIR)/ $(MAKE1) $(LUA_MAKE_FLAGS) -C $$(HOST_LUA_DIR)/src all	
endef

define HOST_LUA_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $$(HOST_LUA_BDIR)/lua $(HOST_DIR)/usr/bin;\
	install -m 777 $$(HOST_LUA_BDIR)/luac $(HOST_DIR)/usr/bin
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/lua,lua,BUILD_LUA))
$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/lua,lua,BUILD_LUA,host))
