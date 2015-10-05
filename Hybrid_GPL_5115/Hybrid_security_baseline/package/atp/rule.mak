unexport EXTRA_AFLAGS
unexport EXTRA_CFLAGS
unexport EXTRA_LDFLAGS
unexport EXTRA_ARFLAGS
unexport SUBDIRS
unexport SUB_DIRS
unexport O_TARGET
unexport L_TARGET
unexport OBJS

include $(CONFIG_DIR)/.config

REAL_OBJS:=$(patsubst %,$(O)%,$(OBJS))

# Strip quotes and then whitespaces
qstrip=$(strip $(subst ",,$(1)))
#"))

.PHONY: clean

ifeq ($(strip $(BR2_ENABLE_DEBUG)),y)
DBG_FLAGS = -g
CFLAGS += -O0
else
CFLAGS += -s -Os
endif

CFLAGS += -fomit-frame-pointer -Wall -Wl,-allow-shlib-undefined -DCONFIG_32BIT 
CFLAGS += -Wl,-Map=$(O)$(PROGRAM_NAME).map

CFLAGS += $(ATPFLAGS) $(IS_DYNAMIC) $(LDFLAGS)
ifeq ($(strip $(BR2_ARCH)),"i386")
CFLAGS += -DSUPPORT_DESKTOP -DATP_DEBUG_CMS_PC_VERSION -D_GNU_SOURCE -fprofile-arcs -ftest-coverage
CPPFLAGS += -DSUPPORT_DESKTOP -DATP_DEBUG_CMS_PC_VERSION -fprofile-arcs -ftest-coverage
endif

ifeq ($(strip $(BR2_ENDIAN)),"LITTLE")
CFLAGS += -DCONFIG_CPU_LITTLE_ENDIAN
else
CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

CURR_DIR = $(shell pwd)
ifeq ($(strip $(ATP_UNIT_TEST)),yes)
CFLAGS += -DSUPPORT_UNIT_TEST
CPPFLAGS += -DSUPPORT_UNIT_TEST
endif

CFLAGS += $(if $(ATP_DEBUG),-DATP_DEBUG,-DATP_RELEASE)
CPPFLAGS += $(if $(ATP_DEBUG),-DATP_DEBUG,-DATP_RELEASE)
CFLAGS += $(if $(FACTORY_BUILD),-DFEATURE_FACTORY_BUILD,-DFEATURE_RELEASE_BUILD)
CPPFLAGS += $(if $(FACTORY_BUILD),-DFEATURE_FACTORY_BUILD,-DFEATURE_RELEASE_BUILD)
################ maybe we use more variety source code, then add rule below	
$(O)%.o: %.c
	@echo "	Compiling " $<
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

$(O)%.o: %.cpp
	@echo "	Compiling " $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

$(O)%.o: %.s
	$(AS) $(AFLAGS) -o $@ $<

$(O)%.so : $(O)%.o 
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@

all_targets: $(O_TARGET) $(L_TARGET)

ifdef O_TARGET
ifeq ($(O_TARGET), $(REAL_OBJS))
$(O_TARGET):
else
$(O_TARGET): $(REAL_OBJS)
	rm -f $@
	$(LD) $(EXTRA_LDFLAGS) -r -o $(O)$@ $(filter $(REAL_OBJS), $^)
endif
endif

ifdef L_TARGET
$(L_TARGET): $(REAL_OBJS)
	$(RM) lib$@.a
	$(AR) $(EXTRA_ARFLAGS) rcs $(O)lib$@.a $@.o	
endif

subdir-list = $(sort $(patsubst %,_subdir_%,$(SUB_DIRS)))
sub_dirs: $(subdir-list)

ifdef SUB_DIRS
$(subdir-list):
	@echo "mkdir -p $(O)$@"
	@mkdir -p $(O)$(patsubst _subdir_%,%,$@)
	$(MAKE) -C $(patsubst _subdir_%,%,$@)
endif

dynamic: $(O)$(PROGRAM_NAME)

static: $(O)lib$(PROGRAM_NAME).a

$(O)$(PROGRAM_NAME): sub_dirs $(REAL_OBJS)
ifeq ($(strip $(BR2_ARCH)),"i386")
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(DBG_FLAGS) -o $(O)$(PROGRAM_NAME) $(REAL_OBJS) $(LIBS) $(EXTRA_LIB) -lgcov
else
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(DBG_FLAGS) -o $(O)$(PROGRAM_NAME) $(REAL_OBJS) $(LIBS) $(EXTRA_LIB)
endif

$(O)lib$(PROGRAM_NAME).a: $(REAL_OBJS)
	$(AR) rcs $@ $^

shared: sub_dirs $(REAL_OBJS)
	$(CC) $(CFLAGS) -fPIC -shared $(REAL_OBJS) -o $(O)lib$(PROGRAM_NAME).so

clean:
	@rm -f $(O)$(PROGRAM_NAME) $(O)*.o $(O)*.a $(O)*.so $(O)*.gcda $(O)*.gcno
	find $(O) -name "*.o" | xargs rm -f
	find $(O) -name "*.so" | xargs rm -f
	find $(O) -name "*.gcda" | xargs rm -f
	find $(O) -name "*.gcno" | xargs rm -f
	$(foreach dir,$(SUB_DIRS), if [ -d $(dir) ];then cd $(dir);make clean; cd $(CURR_DIR);fi;)
