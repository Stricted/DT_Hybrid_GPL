obj-$(CONFIG_BCM_TMS) += nciTMSkmod.o

ifneq ($(strip $(CONFIG_SMP)),)
SMPOPT = SMP
else
SMPOPT = UNI
endif


nciTMSkmod-objs += nciTMSkmod_dep.o

$(obj)/nciTMSkmod_dep.o:
	cp $(obj)/nciTMSkmod.mips32.o_$(SMPOPT)_save $(obj)/nciTMSkmod_dep.o


clean:
	rm -f *.o *.ko *.cmd

