cmd_kernel/sys_ni.o := /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,kernel/.sys_ni.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/staging/usr/include -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sys_ni)"  -D"KBUILD_MODNAME=KBUILD_STR(sys_ni)" -c -o kernel/sys_ni.o kernel/sys_ni.c

source_kernel/sys_ni.o := kernel/sys_ni.c

deps_kernel/sys_ni.o := \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/bcm/kf/bounce.h) \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/brcm/bounce.h) \
  include/linux/compiler-gcc4.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/linkage.h \
  include/linux/errno.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/unistd.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/mips32/o32.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sgidefs.h \

kernel/sys_ni.o: $(deps_kernel/sys_ni.o)

$(deps_kernel/sys_ni.o):
