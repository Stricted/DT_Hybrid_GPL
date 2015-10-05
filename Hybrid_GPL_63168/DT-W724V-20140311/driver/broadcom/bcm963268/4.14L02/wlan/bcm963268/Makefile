#ifneq ($(strip $(BRCMDRIVERS_DIR)),)
OPEN_OBJS =  wl/sys/wl_linux_dslcpe.o wl/sys/wl_linux.o emf/igs/osl_linux.o 
#include $(BRCMDRIVERS_DIR)/broadcom/net/wl/makefile.wlan
#else
#include ../makefile.release
#endif

include $(DRIVER_VENDOR_DIR)/wlan/makefile.wlan


