################### depend objects
OBJS = sntp.o

######################### compile options
CFLAGS = -Wl,-allow-shlib-undefined

######################### lib depend         
EXTRA_LIB += -L$(LIB_ATP_DIR) -lgplutil
	   
include $(TOPDIR)/package/atp/rule.mak
