	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"bounds.c"
@ GNU C (crosstool-NG 1.13.2 - hsan-5115) version 4.4.6 (arm-hsan-linux-uclibcgnueabi)
@	compiled by GNU C version 4.6.2, GMP version 4.3.2, MPFR version 3.0.0.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed:  -nostdinc -Iinclude
@ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include
@ -Iinclude
@ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/linux/2.6.30/arch/arm/include
@ -Iarch/arm/mach-sd5115/include
@ -I/home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/output/staging/usr/include
@ -iprefix
@ /opt/toolchain_hisi/arm-hsan-linux-uclibcgnueabi/bin/../lib/gcc/arm-hsan-linux-uclibcgnueabi/4.4.6/
@ -isysroot
@ /home/zengyao/DT_Hybrid_GPL_1.00.052/Hybrid_security_baseline/output/host/usr/arm-unknown-linux-uclibcgnueabi/sysroot
@ -D__KERNEL__ -D__LINUX_ARM_ARCH__=7 -Uarm -DCFG_LINUX_NET_PACKED
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(bounds)
@ -DKBUILD_MODNAME=KBUILD_STR(bounds) -isystem
@ /opt/toolchain_hisi/arm-hsan-linux-uclibcgnueabi/bin/../lib/gcc/arm-hsan-linux-uclibcgnueabi/4.4.6/include
@ -include include/linux/autoconf.h -include include/linux/autoconf.h -MD
@ kernel/.bounds.s.d kernel/bounds.c -D_FORTIFY_SOURCE=2 -mlittle-endian
@ -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork
@ -march=armv7-a -msoft-float -auxbase-strip kernel/bounds.s -O2 -Wall
@ -Wundef -Wstrict-prototypes -Wno-trigraphs
@ -Werror-implicit-function-declaration -Wframe-larger-than=1024
@ -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-aliasing
@ -fno-common -fno-stack-protector -fno-omit-frame-pointer
@ -fno-optimize-sibling-calls -fwrapv -fno-dwarf2-cfi-asm -fverbose-asm
@ options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
@ -fbranch-count-reg -fcaller-saves -fcprop-registers -fcrossjumping
@ -fcse-follow-jumps -fdefer-pop -fdelete-null-pointer-checks
@ -fearly-inlining -feliminate-unused-debug-types -fexpensive-optimizations
@ -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
@ -fguess-branch-probability -fident -fif-conversion -fif-conversion2
@ -findirect-inlining -finline -finline-functions-called-once
@ -finline-small-functions -fipa-cp -fipa-pure-const -fipa-reference
@ -fira-share-save-slots -fira-share-spill-slots -fivopts
@ -fkeep-static-consts -fleading-underscore -fmath-errno -fmerge-constants
@ -fmerge-debug-strings -fmove-loop-invariants -foptimize-register-move
@ -fpeephole -fpeephole2 -freg-struct-return -fregmove -freorder-blocks
@ -freorder-functions -frerun-cse-after-loop -fsched-interblock
@ -fsched-spec -fsched-stalled-insns-dep -fschedule-insns -fschedule-insns2
@ -fsection-anchors -fsigned-zeros -fsplit-ivs-in-unroller
@ -fsplit-wide-types -fstrict-overflow -fthread-jumps -ftoplevel-reorder
@ -ftrapping-math -ftree-builtin-call-dce -ftree-ccp -ftree-ch
@ -ftree-copy-prop -ftree-copyrename -ftree-cselim -ftree-dce
@ -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-im
@ -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
@ -ftree-pre -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-sra
@ -ftree-switch-conversion -ftree-ter -ftree-vect-loop-version -ftree-vrp
@ -funit-at-a-time -fverbose-asm -fwrapv -fzero-initialized-in-bss
@ -mapcs-frame -mlittle-endian -muclibc

@ Compiler executable checksum: 7479b60f1353a98dbd8b4eaf42376eea

	.text
	.align	2
	.global	foo
	.type	foo, %function
foo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp	@,
	stmfd	sp!, {fp, ip, lr, pc}	@,
	sub	fp, ip, #4	@,,
#APP
@ 16 "kernel/bounds.c" 1
	
->NR_PAGEFLAGS #23 __NR_PAGEFLAGS	@
@ 0 "" 2
@ 17 "kernel/bounds.c" 1
	
->MAX_NR_ZONES #3 __MAX_NR_ZONES	@
@ 0 "" 2
	ldmfd	sp, {fp, sp, pc}	@
	.size	foo, .-foo
	.ident	"GCC: (crosstool-NG 1.13.2 - hsan-5115) 4.4.6"
	.section	.note.GNU-stack,"",%progbits
