################### program name 

################### depend objects
#OBJS = gplutil.o gplmsg.o loginctl.o bget.o gplshmem.o procutil.o
OBJS = gplutil.o gplmsg.o bget.o gplshmem.o procutil.o loginctl.o
OBJS += atpmiclisten.o

#ifneq ($(strip $(BUILD_IPV6)),)
OBJS += ifaddrs.o
#endif

#PROGRAM_NAME = gplutil
######################### compile options
CFLAGS = 
CFLAGS += -fno-strict-aliasing


include $(TOPDIR)/package/atp/rule.mak
