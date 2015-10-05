cmd_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o := /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/.fap_interrupt.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude -include /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/include -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/objs -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/ffe -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/ethernet/shared -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/pktdma/bcm963268 -I/home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/net/core -Werror -Wall -Wfatal-errors -D_MIPSREGS32_ -g  -DMODULE -mlong-calls -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fap_interrupt)"  -D"KBUILD_MODNAME=KBUILD_STR(bcmfap)" -c -o /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.c

source_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o := /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.c

deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o := \
  include/linux/module.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
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
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/stat.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/bcm/kf/printk/int/enabled.h) \
    $(wildcard include/config/bcm/printk/int/enabled.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/atp/common.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
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
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
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
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/bcm/kf/cpp/support.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/linux/bug.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cachectl.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/prefetch.h \
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
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/atomic.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/math64.h \
  include/linux/kmod.h \
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
  include/linux/errno.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
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
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/smp.h \
    $(wildcard include/config/bcm/kf/mips/bcm963xx.h) \
    $(wildcard include/config/bcm/hostmips/pwrsave/timers.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/smp-ops.h \
    $(wildcard include/config/smp/up.h) \
    $(wildcard include/config/mips/cmp.h) \
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
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/timex.h \
  include/linux/sysctl.h \
    $(wildcard include/config/atp/hybrid/greaccel.h) \
    $(wildcard include/config/atp/port/scan.h) \
    $(wildcard include/config/atp/igmp.h) \
    $(wildcard include/config/dt/igmp/qqic.h) \
    $(wildcard include/config/atp/hybrid/reorder.h) \
    $(wildcard include/config/dt/qos.h) \
    $(wildcard include/config/atp/conntrack/clean.h) \
    $(wildcard include/config/sysctl.h) \
  include/linux/rbtree.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/static_key.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
    $(wildcard include/config/preempt/base.h) \
  include/linux/export.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
    $(wildcard include/config/cpu/xlr.h) \
    $(wildcard include/config/cpu/xlp.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/uaccess.h \
  include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/prio_tree.h \
  include/linux/pid.h \
  include/linux/capability.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/shrinker.h \
  include/linux/migrate_mode.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/fcntl.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_intr.h \
    $(wildcard include/config/bcm96318.h) \
    $(wildcard include/config/bcm96828.h) \
    $(wildcard include/config/bcm963268.h) \
    $(wildcard include/config/bcm96328.h) \
    $(wildcard include/config/bcm96368.h) \
    $(wildcard include/config/bcm96816.h) \
    $(wildcard include/config/bcm96818.h) \
    $(wildcard include/config/bcm96362.h) \
    $(wildcard include/config/bcm96838.h) \
  include/linux/version.h \
  include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/generic/pending/irq.h) \
  include/linux/irqreturn.h \
  include/linux/irqnr.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/mips/mt/smtc/irqaff.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
  include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
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
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/fap.h \
    $(wildcard include/config/rx.h) \
    $(wildcard include/config/tx.h) \
    $(wildcard include/config/bcm/arl.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/pktHdr.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_OS_Deps.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/r4kcache.h \
    $(wildcard include/config/mips/mt.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/asm.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cacheops.h \
  include/linux/proc_fs.h \
    $(wildcard include/config/proc/devicetree.h) \
    $(wildcard include/config/proc/kcore.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  include/linux/slab_def.h \
  include/linux/kmalloc_sizes.h \
  include/linux/magic.h \
  include/linux/delay.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/delay.h \
  include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/hardirq.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/irq/time/accounting.h) \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/hardirq.h \
  include/asm-generic/hardirq.h \
  include/linux/irq_cpustat.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/missed/timer/offsets/hist.h) \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/io.h \
    $(wildcard include/config/mmu.h) \
  include/linux/mm.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/bcm/kf/usb/storage.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mmu.h \
  include/linux/range.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/pgtable.h \
    $(wildcard include/config/cpu/supports/uncached/accelerated.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/pgtable-32.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/fixmap.h \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
  include/linux/poll.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/poll.h \
  include/asm-generic/poll.h \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/bcm/kf/schedaudit.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/wakeup/latency/hist.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/cfs/bandwidth.h) \
    $(wildcard include/config/cgroup/sched.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  include/asm-generic/kmap_types.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/ipcbuf.h \
  include/asm-generic/ipcbuf.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sembuf.h \
  include/linux/signal.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  include/asm-generic/signal-defs.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sigcontext.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/key.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/cdev.h \
  include/linux/mii.h \
  include/linux/ethtool.h \
  include/linux/compat.h \
    $(wildcard include/config/arch/want/old/compat/ipc.h) \
  include/linux/if_ether.h \
    $(wildcard include/config/bcm/kf/p8021ag.h) \
    $(wildcard include/config/bcm/kf/vlan.h) \
    $(wildcard include/config/atp/hybrid.h) \
  include/linux/skbuff.h \
    $(wildcard include/config/bcm/kf/blog.h) \
    $(wildcard include/config/blog.h) \
    $(wildcard include/config/bcm/kf/nbuff.h) \
    $(wildcard include/config/bcm/vlan.h) \
    $(wildcard include/config/skbshinfo/has/dirtyp.h) \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/bcm/kf/runner.h) \
    $(wildcard include/config/bcm/rdpa.h) \
    $(wildcard include/config/bcm/runner.h) \
    $(wildcard include/config/nf/defrag/ipv4.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/bcm/kf/wl.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/blog/feature.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  include/linux/kmemcheck.h \
  include/linux/net.h \
  include/linux/socket.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/socket.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/sockios.h \
  include/linux/sockios.h \
    $(wildcard include/config/atp/mirror.h) \
    $(wildcard include/config/bcm/in/kernel.h) \
    $(wildcard include/config/bcm/kf/misc/ioctls.h) \
    $(wildcard include/config/bcm/kf/netfilter.h) \
    $(wildcard include/config/bcm/kf/igmp.h) \
    $(wildcard include/config/bcm/kf/mld.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/atpconfig.h \
    $(wildcard include/config/vss/ssid.h) \
    $(wildcard include/config/antenna.h) \
    $(wildcard include/config/reserved/size.h) \
    $(wildcard include/config/wifisystem/bootup.h) \
    $(wildcard include/config/backup.h) \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/wireless/enable.h) \
    $(wildcard include/config/vss/psk.h) \
    $(wildcard include/config/file/length/d.h) \
    $(wildcard include/config/1/size.h) \
    $(wildcard include/config/vss/wep.h) \
    $(wildcard include/config/1/index.h) \
    $(wildcard include/config/vss/appin.h) \
    $(wildcard include/config/backup/vdb.h) \
    $(wildcard include/config/wifibootmark.h) \
    $(wildcard include/config/back/reserved/size.h) \
    $(wildcard include/config/back/size.h) \
    $(wildcard include/config/wireless/on.h) \
    $(wildcard include/config/throughput.h) \
    $(wildcard include/config/file/name/prefix.h) \
    $(wildcard include/config/partition.h) \
    $(wildcard include/config/expand/1.h) \
  include/linux/textsearch.h \
  include/net/checksum.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/checksum.h \
    $(wildcard include/config/bcm/kf/csum/unaligned.h) \
  include/linux/in6.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/device.h \
  include/linux/pm_wakeup.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  include/linux/blog.h \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/bcm/kf/fap.h) \
    $(wildcard include/config/bcm/kf/extstats.h) \
    $(wildcard include/config/blog/mcast.h) \
    $(wildcard include/config/blog/ipv6.h) \
    $(wildcard include/config/blog/gre.h) \
    $(wildcard include/config/.h) \
  include/linux/blog_net.h \
  include/linux/nbuff_types.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/dma-direction.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/dma-mapping.h \
    $(wildcard include/config/sgi/ip27.h) \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic/dma-coherence.h \
  include/asm-generic/dma-mapping-common.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  include/linux/netdev_features.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/atp_interface.h \
  include/linux/if.h \
    $(wildcard include/config/bcm/kf/enet/switch.h) \
    $(wildcard include/config/bcm/gpon.h) \
    $(wildcard include/config/bcm/kf/ip.h) \
    $(wildcard include/config/bcm/kf/wandev.h) \
    $(wildcard include/config/bcm/kf/ppp.h) \
  include/linux/hdlc/ioctl.h \
  include/linux/if_pppox.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/bcm/kf/module/owner.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/bcm/kf/netdev/path.h) \
    $(wildcard include/config/bcm/max/gem/ports.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/netprio/cgroup.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
    $(wildcard include/config/bcm/kf/fap/gso/loopback.h) \
    $(wildcard include/config/bcm/fap/gso/loopback.h) \
    $(wildcard include/config/bcm/kf/skb/defines.h) \
  include/linux/if_packet.h \
  include/linux/if_link.h \
  include/linux/netlink.h \
  include/linux/pm_qos.h \
  include/linux/miscdevice.h \
  include/linux/major.h \
  include/linux/dynamic_queue_limits.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/net.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  include/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/dccp.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  include/linux/netfilter.h \
    $(wildcard include/config/nf/nat/needed.h) \
  include/linux/in.h \
    $(wildcard include/config/sipalg/recoreddstip.h) \
    $(wildcard include/config/atp/brcm.h) \
    $(wildcard include/config/mips/brcm.h) \
  include/net/flow.h \
  include/net/netns/conntrack.h \
  include/linux/list_nulls.h \
  include/net/netns/xfrm.h \
  include/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/seq_file.h \
  include/net/dsa.h \
  include/net/netprio_cgroup.h \
  include/linux/cgroup.h \
  include/linux/cgroupstats.h \
  include/linux/taskstats.h \
  include/linux/prio_heap.h \
  include/linux/idr.h \
  include/linux/ppp_channel.h \
  include/linux/if_pppol2tp.h \
  include/net/sock.h \
    $(wildcard include/config/cgroup/mem/res/ctlr/kmem.h) \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
  include/linux/uaccess.h \
  include/linux/memcontrol.h \
    $(wildcard include/config/cgroup/mem/res/ctlr/swap.h) \
  include/linux/res_counter.h \
  include/linux/filter.h \
    $(wildcard include/config/bpf/jit.h) \
  include/linux/rculist_nulls.h \
  include/net/dst.h \
    $(wildcard include/config/ip/route/classid.h) \
  include/linux/rtnetlink.h \
  include/linux/if_addr.h \
  include/linux/neighbour.h \
  include/net/neighbour.h \
  include/net/rtnetlink.h \
  include/net/netlink.h \
  include/linux/if_vlan.h \
  include/linux/etherdevice.h \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/unaligned.h \
  include/linux/unaligned/be_struct.h \
  include/linux/unaligned/packed_struct.h \
  include/linux/unaligned/le_byteshift.h \
  include/linux/unaligned/generic.h \
  include/linux/ip.h \
  include/linux/tcp.h \
    $(wildcard include/config/tcp/md5sig.h) \
  include/net/inet_connection_sock.h \
  include/net/inet_sock.h \
  include/linux/jhash.h \
  include/net/request_sock.h \
  include/net/netns/hash.h \
  include/net/inet_timewait_sock.h \
  include/net/tcp_states.h \
  include/net/timewait_sock.h \
  include/linux/udp.h \
  include/linux/ppp_defs.h \
  include/linux/crc-ccitt.h \
  include/net/ip.h \
    $(wildcard include/config/inet.h) \
  include/linux/ipv6.h \
    $(wildcard include/config/ipv6/privacy.h) \
    $(wildcard include/config/ipv6/router/pref.h) \
    $(wildcard include/config/ipv6/route/info.h) \
    $(wildcard include/config/ipv6/optimistic/dad.h) \
    $(wildcard include/config/ipv6/mip6.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
  include/linux/icmpv6.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_OS_Deps.h \
  include/linux/bcm_log.h \
    $(wildcard include/config/bcm/kf/log.h) \
    $(wildcard include/config/brcm/colorize/prints.h) \
    $(wildcard include/config/bcm/log.h) \
  include/linux/bcm_log_mod.h \
    $(wildcard include/config/bcm/gmac.h) \
  include/linux/bcm_colors.h \
    $(wildcard include/config/bcm/colorize/prints.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw_63268.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmtypes.h \
    $(wildcard include/config/file.h) \
    $(wildcard include/config/psi.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/63268_map_part.h \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/2/bar1/size/mask.h) \
    $(wildcard include/config/2/bar1/disable.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmtypes.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_local.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dqmHost.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma.h \
    $(wildcard include/config/bcm/xtmcfg.h) \
    $(wildcard include/config/bcm/fap.h) \
    $(wildcard include/config/bcm/pktdma/rx/splitting.h) \
    $(wildcard include/config/bcm/pktdma/tx/splitting.h) \
    $(wildcard include/config/bcm/def/nr/rx/dma/channels.h) \
    $(wildcard include/config/bcm/def/nr/tx/dma/channels.h) \
    $(wildcard include/config/bcm/ingqos.h) \
    $(wildcard include/config/bcm/bpm.h) \
    $(wildcard include/config/bcm/ports/on/int/ext/sw.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_map_part.h \
  include/linux/nbuff.h \
    $(wildcard include/config/fkb/fn/inline.h) \
    $(wildcard include/config/fkb/stats.h) \
    $(wildcard include/config/fkb/color.h) \
    $(wildcard include/config/fkb/debug.h) \
    $(wildcard include/config/fkb/audit.h) \
    $(wildcard include/config/fkb/stack.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm_pkt_lengths.h \
    $(wildcard include/config/bcm/jumbo/frame.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_bds.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/ethernet/shared/bcmenet.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/boardparms.h \
    $(wildcard include/config/mdio.h) \
    $(wildcard include/config/mdio/pseudo/phy.h) \
    $(wildcard include/config/spi/ssb/0.h) \
    $(wildcard include/config/spi/ssb/1.h) \
    $(wildcard include/config/spi/ssb/2.h) \
    $(wildcard include/config/spi/ssb/3.h) \
    $(wildcard include/config/mmap.h) \
    $(wildcard include/config/gpio/mdio.h) \
    $(wildcard include/config/hs/spi/ssb/0.h) \
    $(wildcard include/config/hs/spi/ssb/1.h) \
    $(wildcard include/config/hs/spi/ssb/2.h) \
    $(wildcard include/config/hs/spi/ssb/3.h) \
    $(wildcard include/config/hs/spi/ssb/4.h) \
    $(wildcard include/config/hs/spi/ssb/5.h) \
    $(wildcard include/config/hs/spi/ssb/6.h) \
    $(wildcard include/config/hs/spi/ssb/7.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmnet.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/skb_defines.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_defines.h \
    $(wildcard include/config/bcm/pktdma.h) \
    $(wildcard include/config/bcm/fap/pwrsave.h) \
    $(wildcard include/config/bcm96338.h) \
    $(wildcard include/config/bcm/dsl/ginp/rtx.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcm/bcmswapitypes.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/ethernet/shared/bcmenet_common.h \
    $(wildcard include/config/bcm/pktcmf.h) \
    $(wildcard include/config/bcm/endpoint.h) \
    $(wildcard include/config/bcm/gpon/stack.h) \
    $(wildcard include/config/bcm/epon/stack.h) \
    $(wildcard include/config/bcmgpon.h) \
    $(wildcard include/config/bcm/ext/switch.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/ethernet/shared/bcmenet_dma.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_structs.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/robosw_reg.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmxtmcfg.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmxtmrt.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268/bcmxtmrtimpl.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmxtmrt.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268/bcmxtmrtbond.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmxtmcfg.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_structs.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_hw.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_memory.h \
    $(wildcard include/config/bcm/fap/layer2.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_init.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_timers.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_task.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_task.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dll.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_irq.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dqm.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_swq.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_packet.h \
    $(wildcard include/config/bcm/fap/ipv6.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_iopDma.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_mailBox.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_printer.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4keLib_snprintf.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDma_defines.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_xtmrt.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_gso.h \
    $(wildcard include/config/bcm/fap/gso.h) \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_tm.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_perf.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/xtmrt/bcm963268/bcmxtmrtbond.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dynmem.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_slob.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_dynmem.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/output/staging/usr/include/bcmPktDmaHooks.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_timers.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_packet.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/4ke/fap4ke_msg.h \
  /home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_tm.h \

/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o: $(deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o)

$(deps_/home/liyupeng/zengyao/6.13/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/fap/bcm963268/fap_interrupt.o):
