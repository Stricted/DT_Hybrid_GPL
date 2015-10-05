ATM_OBJS=$(O)lib/text2atm.o $(O)lib/atm2text.o $(O)lib/atmequal.o $(O)lib/sdu2cell.o $(O)lib/text2qos.o $(O)lib/qos2text.o \
  $(O)lib/qosequal.o $(O)lib/sap2text.o $(O)lib/text2sap.o $(O)lib/sapequal.o $(O)lib/misc.o
ifeq ($(LIB_PATH)/libresolv.a,$(wildcard $(LIB_PATH)/libresolv.a))
ATM_OBJS += $(O)lib/ans_l.o
else
ATM_OBJS += $(O)lib/ans.o
endif
ATMD_OBJS=$(O)lib/common.o $(O)lib/diag.o $(O)lib/kptr.o $(O)lib/text2ip.o $(O)lib/timer.o $(O)lib/unix.o
PGMS=#test
GENLIBS=$(O)lib/libatm.a $(O)lib/libatmd.a
SYSHDR=atm.h atmd.h atmsap.h
OPTSYSHDR=stdint.h

do_all:	$(O)lib/libatm.a #$(O)lib/libatmd.a

#include ../../../../../rule.mak
CFLAGS += -I./
#../lib/libatm.a:	libatm.a

$(O)lib/libatm.a:$(ATM_OBJS)
	$(AR) rcs $@ $(ATM_OBJS)

#../lib/libatmd.a:	libatmd.a

$(O)lib/%.o: %.c
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

$(O)libatmd.a:		$(ATMD_OBJS)
			$(AR) rcs $(O)libatmd.a $(ATMD_OBJS)

$(O)ans_l.o:		$(O)ans.o
			$(LD) -r -o $(O)ans_l.o $(O)ans.o -L$(LIB_PATH) -lresolv
clean:
			rm -fr $(O)lib
