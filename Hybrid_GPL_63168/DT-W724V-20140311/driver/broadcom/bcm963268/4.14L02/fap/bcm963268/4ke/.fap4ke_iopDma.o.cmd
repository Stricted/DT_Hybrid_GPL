cmd_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o := /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/.fap4ke_iopDma.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude -include /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/objs -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/ffe -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/ethernet/shared -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/pktdma/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/net/core -Werror -Wall -Wfatal-errors -D_MIPSREGS32_ -g  -DMODULE -mlong-calls -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fap4ke_iopDma)"  -D"KBUILD_MODNAME=KBUILD_STR(bcmfap)" -c -o /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.c

source_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o := /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.c

deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o := \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw.h \
    $(wildcard include/config/bcm96362.h) \
    $(wildcard include/config/bcm963268.h) \
    $(wildcard include/config/bcm96828.h) \
    $(wildcard include/config/bcm96818.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw_63268.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_intr.h \
    $(wildcard include/config/bcm96318.h) \
    $(wildcard include/config/bcm96328.h) \
    $(wildcard include/config/bcm96368.h) \
    $(wildcard include/config/bcm96816.h) \
    $(wildcard include/config/bcm96838.h) \
  include/linux/version.h \
  include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/generic/pending/irq.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/errno.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/bcm/kf/unaligned/exception.h) \
    $(wildcard include/config/mips/bcm963xx.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/bitsperlong.h \
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
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/posix_types.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sgidefs.h \
  include/asm-generic/posix_types.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/bcm/kf/cpp/support.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/kernel.h \
    $(wildcard include/config/bcm/kf/printk/int/enabled.h) \
    $(wildcard include/config/bcm/printk/int/enabled.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/atp/common.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/sysinfo.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stdarg.h \
  include/linux/linkage.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/preempt/rt/full.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/bmips.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cpu.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
    $(wildcard include/config/bcm/kf/cpu/data/cpuid.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx/cpu-feature-overrides.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
    $(wildcard include/config/bcm/kf/fixaddr/top.h) \
    $(wildcard include/config/bcm968500.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/preempt/rt/base.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/linux/bug.h \
  include/linux/preempt.h \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/bcm/kf/thread/size/fix.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cachectl.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/prefetch.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/smp.h \
    $(wildcard include/config/bcm/kf/mips/bcm963xx.h) \
    $(wildcard include/config/bcm/hostmips/pwrsave/timers.h) \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/atomic.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/smp-ops.h \
    $(wildcard include/config/smp/up.h) \
    $(wildcard include/config/mips/cmp.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
    $(wildcard include/config/dt/common.h) \
  include/linux/spinlock_types_raw.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/spinlock_types_nort.h \
  include/linux/rwlock_types.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/spinlock.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/memblock/node.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/current.h \
  include/asm-generic/current.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/seqlock.h \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/pfn.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/io.h \
  include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/generic/iomap.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/ioremap.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/preempt/rt.h) \
    $(wildcard include/config/bcm/kf/rcu/constant/bug.h) \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/topology.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/irqreturn.h \
  include/linux/irqnr.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/mips/mt/smtc/irqaff.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
  include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
  include/linux/radix-tree.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mipsmtregs.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/irq.h \
    $(wildcard include/config/irq/cpu/rm7k.h) \
    $(wildcard include/config/irq/cpu/rm9k.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/isadep.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/irq_regs.h \
  include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/sparse/irq.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/hw_irq.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/63268_intr.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmtypes.h \
    $(wildcard include/config/file.h) \
    $(wildcard include/config/psi.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/63268_map_part.h \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/2/bar1/size/mask.h) \
    $(wildcard include/config/2/bar1/disable.h) \
    $(wildcard include/config/bcm/gmac.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmtypes.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_local.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4keLib_types.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4keLib_string.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_mailBox.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_printer.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4keLib_snprintf.h \
  include/linux/bcm_colors.h \
    $(wildcard include/config/bcm/colorize/prints.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_memory.h \
    $(wildcard include/config/bcm/xtmcfg.h) \
    $(wildcard include/config/bcm/ext/switch.h) \
    $(wildcard include/config/bcm/ingqos.h) \
    $(wildcard include/config/bcm/bpm.h) \
    $(wildcard include/config/bcm/fap/layer2.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_init.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_timers.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_task.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_task.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dll.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_irq.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dqm.h \
    $(wildcard include/config/bcm/dsl/ginp/rtx.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_swq.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_packet.h \
    $(wildcard include/config/bcm/fap/ipv6.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_defines.h \
    $(wildcard include/config/bcm/pktdma.h) \
    $(wildcard include/config/bcm/def/nr/rx/dma/channels.h) \
    $(wildcard include/config/bcm/def/nr/tx/dma/channels.h) \
    $(wildcard include/config/bcm/fap.h) \
    $(wildcard include/config/bcm/fap/pwrsave.h) \
    $(wildcard include/config/bcm96338.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_structs.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_map_part.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_defines.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_xtmrt.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_gso.h \
    $(wildcard include/config/bcm/fap/gso.h) \
    $(wildcard include/config/bcm/fap/gso/loopback.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_tm.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_perf.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268/bcmxtmrtbond.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmxtmcfg.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dynmem.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_slob.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dynmem.h \

/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o: $(deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o)

$(deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.o):
