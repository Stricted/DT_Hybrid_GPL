cmd_net/netfilter/nf_conntrack_h323_asn1.o := /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/output/host/usr/bin/arm-hsan-linux-uclibcgnueabi-gcc -Wp,-MD,net/netfilter/.nf_conntrack_h323_asn1.o.d  -nostdinc -isystem /opt/toolchain_hisi/arm-hsan-linux-uclibcgnueabi/bin/../lib/gcc/arm-hsan-linux-uclibcgnueabi/4.4.6/include -Iinclude  -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -Iinclude -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include -include include/linux/autoconf.h -mlittle-endian -Iarch/arm/mach-sd5115/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/output/staging/usr/include -O2 -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fwrapv -fno-dwarf2-cfi-asm -DCFG_LINUX_NET_PACKED -I/   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(nf_conntrack_h323_asn1)"  -D"KBUILD_MODNAME=KBUILD_STR(nf_conntrack_h323)"  -c -o net/netfilter/nf_conntrack_h323_asn1.o net/netfilter/nf_conntrack_h323_asn1.c

deps_net/netfilter/nf_conntrack_h323_asn1.o := \
  net/netfilter/nf_conntrack_h323_asn1.c \
  include/linux/kernel.h \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/mips/brcm.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /opt/toolchain_hisi/arm-hsan-linux-uclibcgnueabi/bin/../lib/gcc/arm-hsan-linux-uclibcgnueabi/4.4.6/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/brcm/bounce.h) \
  include/linux/compiler-gcc4.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  include/linux/posix_types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/irqflags.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/hwcap.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/netfilter/nf_conntrack_h323_asn1.h \
  include/linux/netfilter/nf_conntrack_h323_types.h \
  net/netfilter/nf_conntrack_h323_types.c \

net/netfilter/nf_conntrack_h323_asn1.o: $(deps_net/netfilter/nf_conntrack_h323_asn1.o)

$(deps_net/netfilter/nf_conntrack_h323_asn1.o):
