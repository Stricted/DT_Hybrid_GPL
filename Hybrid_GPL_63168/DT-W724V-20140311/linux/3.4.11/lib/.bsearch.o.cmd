cmd_lib/bsearch.o := /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,lib/.bsearch.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/staging/usr/include -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(bsearch)"  -D"KBUILD_MODNAME=KBUILD_STR(bsearch)" -c -o lib/bsearch.o lib/bsearch.c

source_lib/bsearch.o := lib/bsearch.c

deps_lib/bsearch.o := \
  include/linux/export.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  include/linux/bsearch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/bcm/kf/unaligned/exception.h) \
    $(wildcard include/config/mips/bcm963xx.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
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
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/posix_types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sgidefs.h \
  include/asm-generic/posix_types.h \

lib/bsearch.o: $(deps_lib/bsearch.o)

$(deps_lib/bsearch.o):
