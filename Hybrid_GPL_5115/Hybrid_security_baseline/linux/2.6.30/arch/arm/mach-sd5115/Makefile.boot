
ifeq ($(CONFIG_HISI_SD5115_AF_FPGA),y)
   zreladdr-y	:= 0x80008000
params_phys-y	:= 0x80000100
initrd_phys-y	:= 0x80a00000
else
   zreladdr-y	:= 0x80e08000
params_phys-y	:= 0x80e00100
initrd_phys-y	:= 0x81800000
endif

