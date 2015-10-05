	.file	1 "bounds.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3

 # -G value = 0, Arch = mips32, ISA = 32
 # GNU C (Buildroot 2011.11) version 4.6.2 (mips-unknown-linux-uclibc)
 #	compiled by GNU C version 4.1.2 20061115 (prerelease) (Debian 4.1.1-21), GMP version 5.0.2, MPFR version 3.0.1-p4, MPC version 0.9
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed:  -nostdinc
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include
 # -I arch/mips/include/generated -I include
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include
 # -I arch/mips/include/generated -I include
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/staging/usr/include
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx
 # -I /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic
 # -isysroot /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot
 # -D __KERNEL__ -D VMLINUX_LOAD_ADDRESS=0x80010000 -D DATAOFFSET=0
 # -D CC_HAVE_ASM_GOTO -D KBUILD_STR(s)=#s
 # -D KBUILD_BASENAME=KBUILD_STR(bounds)
 # -D KBUILD_MODNAME=KBUILD_STR(bounds)
 # -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include
 # -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h
 # -include /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h
 # -MD kernel/.bounds.s.d kernel/bounds.c -G 0 -mno-check-zero-division
 # -mabi=32 -mno-abicalls -msoft-float -march=mips32 -mtune=mips32 -mllsc
 # -mno-synci -mno-shared -auxbase-strip kernel/bounds.s -O2 -Wall -Wundef
 # -Wstrict-prototypes -Wno-trigraphs -Werror=implicit-function-declaration
 # -Wno-format-security -Wframe-larger-than=1024
 # -Wno-unused-but-set-variable -Wdeclaration-after-statement
 # -Wno-pointer-sign -fno-strict-aliasing -fno-common
 # -fno-delete-null-pointer-checks -fno-pic -ffreestanding
 # -fno-stack-protector -fomit-frame-pointer -fno-strict-overflow
 # -fconserve-stack -fverbose-asm
 # options enabled:  -fauto-inc-dec -fbranch-count-reg -fcaller-saves
 # -fcombine-stack-adjustments -fcompare-elim -fcprop-registers
 # -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdelayed-branch
 # -fdevirtualize -fdwarf2-cfi-asm -fearly-inlining
 # -feliminate-unused-debug-types -fexpensive-optimizations
 # -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
 # -fguess-branch-probability -fident -fif-conversion -fif-conversion2
 # -findirect-inlining -finline -finline-functions-called-once
 # -finline-small-functions -fipa-cp -fipa-profile -fipa-pure-const
 # -fipa-reference -fipa-sra -fira-share-save-slots -fira-share-spill-slots
 # -fivopts -fkeep-static-consts -fleading-underscore -fmath-errno
 # -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
 # -fomit-frame-pointer -foptimize-register-move -foptimize-sibling-calls
 # -fpartial-inlining -fpcc-struct-return -fpeephole -fpeephole2
 # -fprefetch-loop-arrays -fregmove -freorder-blocks -freorder-functions
 # -frerun-cse-after-loop -fsched-critical-path-heuristic
 # -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
 # -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
 # -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-insns
 # -fschedule-insns2 -fshow-column -fsigned-zeros -fsplit-ivs-in-unroller
 # -fsplit-wide-types -fstrict-volatile-bitfields -fthread-jumps
 # -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
 # -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-copy-prop
 # -ftree-copyrename -ftree-cselim -ftree-dce -ftree-dominator-opts
 # -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert
 # -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
 # -ftree-parallelize-loops= -ftree-phiprop -ftree-pre -ftree-pta
 # -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slp-vectorize
 # -ftree-sra -ftree-switch-conversion -ftree-ter -ftree-vect-loop-version
 # -ftree-vrp -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss
 # -mdivide-traps -mdouble-float -meb -mexplicit-relocs -mextern-sdata
 # -mfp-exceptions -mfp32 -mfused-madd -mgp32 -mgpopt -mllsc -mlocal-sdata
 # -mlong32 -mno-mdmx -mno-mips16 -mno-mips3d -msoft-float
 # -msplit-addresses -muclibc

 # Compiler executable checksum: 7b2226d9c1c2dc703523e20638e12759

	.text
	.align	2
	.globl	foo
	.set	nomips16
	.ent	foo
	.type	foo, @function
foo:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
 # 17 "kernel/bounds.c" 1
	
->NR_PAGEFLAGS 22 __NR_PAGEFLAGS	 #
 # 0 "" 2
 # 18 "kernel/bounds.c" 1
	
->MAX_NR_ZONES 3 __MAX_NR_ZONES	 #
 # 0 "" 2
 # 19 "kernel/bounds.c" 1
	
->NR_PCG_FLAGS 3 __NR_PCG_FLAGS	 #
 # 0 "" 2
#NO_APP
	j	$31
	.end	foo
	.size	foo, .-foo
	.ident	"GCC: (Buildroot 2011.11) 4.6.2"
