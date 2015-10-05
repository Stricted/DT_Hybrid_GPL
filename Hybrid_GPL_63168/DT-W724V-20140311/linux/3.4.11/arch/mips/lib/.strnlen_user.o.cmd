cmd_arch/mips/lib/strnlen_user.o := /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,arch/mips/lib/.strnlen_user.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -D__ASSEMBLY__  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic        -c -o arch/mips/lib/strnlen_user.o arch/mips/lib/strnlen_user.S

source_arch/mips/lib/strnlen_user.o := arch/mips/lib/strnlen_user.S

deps_arch/mips/lib/strnlen_user.o := \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sgidefs.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/regdef.h \

arch/mips/lib/strnlen_user.o: $(deps_arch/mips/lib/strnlen_user.o)

$(deps_arch/mips/lib/strnlen_user.o):
