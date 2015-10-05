cmd_libbb/xfuncs.o := /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,libbb/.xfuncs.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/build/busybox-1.9.1/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=\"v1.9.1\"" -DBB_BT=AUTOCONF_TIMESTAMP -D_FORTIFY_SOURCE=2  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -Wdeclaration-after-statement -Wno-pointer-sign -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include -I/openssl -fno-strict-aliasing    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(xfuncs)"  -D"KBUILD_MODNAME=KBUILD_STR(xfuncs)" -c -o libbb/xfuncs.o libbb/xfuncs.c

deps_libbb/xfuncs.o := \
  libbb/xfuncs.c \
    $(wildcard include/config/debug.h) \
    $(wildcard include/config/verbose/resolution/errors.h) \
    $(wildcard include/config/ture/ipv6.h) \
    $(wildcard include/config/feature/clean/up.h) \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
  include/libbb.h \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/gunzip.h) \
    $(wildcard include/config/bunzip2.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/byteswap.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/byteswap.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/byteswap-common.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/endian.h \
    $(wildcard include/config/.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/c95.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/en2k8.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
    $(wildcard include/config/ix/implicitly.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_config.h \
    $(wildcard include/config/mips/o32/abi//.h) \
    $(wildcard include/config/mips/n32/abi//.h) \
    $(wildcard include/config/mips/n64/abi//.h) \
    $(wildcard include/config/mips/isa/1//.h) \
    $(wildcard include/config/mips/isa/2//.h) \
    $(wildcard include/config/mips/isa/3//.h) \
    $(wildcard include/config/mips/isa/4//.h) \
    $(wildcard include/config/mips/isa/mips32//.h) \
    $(wildcard include/config/mips/isa/mips32r2//.h) \
    $(wildcard include/config/mips/isa/mips64//.h) \
    $(wildcard include/config//.h) \
    $(wildcard include/config/link//.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/endian.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/arpa/inet.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/netinet/in.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stdint.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/stdint.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/wchar.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/wordsize.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/socket.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/uio.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/types.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stddef.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/typesizes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/pthreadtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sched.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/time.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/select.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/select.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sigset.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/time.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/sysmacros.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uio.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/socket.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include-fixed/limits.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include-fixed/syslimits.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/limits.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/posix1_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/local_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/limits.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_local_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/posix2_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/xopen_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/stdio_lim.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sockaddr.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/socket.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/sockios.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/ioctl.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/ioctl.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/in.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stdbool.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/mount.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/ioctl.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/ioctls.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/ioctls.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/ioctl-types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/ttydefaults.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/ctype.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_touplow.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/xlocale.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/locale.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_locale.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/dirent.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/dirent.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/errno.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/errno.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/errno.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/errno.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/errno-base.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/syscall.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sysnum.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/fcntl.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/fcntl.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sgidefs.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/stat.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/stat.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/inttypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/mntent.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/stdio.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/paths.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/rpc/netdb.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/siginfo.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/netdb.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/setjmp.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/setjmp.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/signal.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/signum.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sigaction.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sigcontext.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sigstack.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/ucontext.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/sigthread.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_stdio.h \
    $(wildcard include/config/io/futexes//.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/wchar.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_mutex.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/pthread.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sched.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_clk_tck.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/initspin.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/uClibc_pthread.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stdarg.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/stdlib.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/waitflags.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/waitstatus.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/alloca.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/string.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/poll.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/poll.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/mman.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/mman.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/statfs.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/statfs.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/time.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/wait.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/resource.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/resource.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/termios.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/termios.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/unistd.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/posix_opt.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/environments.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/confname.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/bits/getopt.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/utime.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sys/param.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/param.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/param.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/param.h \
  include/pwd_.h \
    $(wildcard include/config/use/bb/pwd/grp.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/pwd.h \
  include/grp_.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/grp.h \
  include/shadow_.h \
    $(wildcard include/config/use/bb/shadow.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/shadow.h \
  include/xatonum.h \
  libbb/../atpconfig/atpcommon.h \
    $(wildcard include/config/feature/atp/wget/zip.h) \
    $(wildcard include/config/feature/atp/ftp/zip.h) \
    $(wildcard include/config/feature/atp/tftp/zip.h) \
    $(wildcard include/config/feature/atp/mcast/zip.h) \
  include/platform.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atputil.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/if_link.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/int-ll64.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/bitsperlong.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/posix_types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/stddef.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/posix_types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm/sgidefs.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/asm-generic/posix_types.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/netlink.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/socket.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/linux/version.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atptypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/malloc.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atpconfig.h \
    $(wildcard include/config/vss/ssid.h) \
    $(wildcard include/config/e.h) \
    $(wildcard include/config/ti/cfg/with/bootloader/name.h) \
    $(wildcard include/config/antenna.h) \
    $(wildcard include/config/reserved/size.h) \
    $(wildcard include/config/wifisystem/bootup.h) \
    $(wildcard include/config/backup.h) \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/wireless/enable.h) \
    $(wildcard include/config/vss/psk.h) \
    $(wildcard include/config/ti/cfg/without/bootloader/name.h) \
    $(wildcard include/config/file/length/d.h) \
    $(wildcard include/config/ti/cfg/lte/without/bootloader/name.h) \
    $(wildcard include/config/1/size.h) \
    $(wildcard include/config/vss/wep.h) \
    $(wildcard include/config/1/index.h) \
    $(wildcard include/config/config.h) \
    $(wildcard include/config/vss/appin.h) \
    $(wildcard include/config/ti/cfg/lte/with/bootloader/name.h) \
    $(wildcard include/config/backup/vdb.h) \
    $(wildcard include/config/wifibootmark.h) \
    $(wildcard include/config/back/reserved/size.h) \
    $(wildcard include/config/ar.h) \
    $(wildcard include/config/back/size.h) \
    $(wildcard include/config/wireless/on.h) \
    $(wildcard include/config/throughput.h) \
    $(wildcard include/config/file/name/prefix.h) \
    $(wildcard include/config/locale.h) \
    $(wildcard include/config/partition.h) \
    $(wildcard include/config/ti/cfg.h) \
    $(wildcard include/config/expand/1.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atpcbbid.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atpconst.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msgapi.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/shmemapi.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/sysutil.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msgtypeswrapper.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cfmcoremsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/msgcoremsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/atsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/atcmdmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/nascentermsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atptypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/oammsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cwmpappmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cwmpmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/logmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/miccoremsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/micmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/telnetdmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/tr111msgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/upgmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/upnpmsgtypes.h \
    $(wildcard include/config/upnp.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ddnscmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dhcp6cmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dhcp6smsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dhcpsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dnsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/abusecmdmsgtypes.h \
    $(wildcard include/config/types/h//.h) \
    $(wildcard include/config/types/en.h) \
    $(wildcard include/config/rt.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/foncontrlmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/hybridmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ipcheckmsgtypes.h \
    $(wildcard include/config/obj.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/routemsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/voicecoremsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cmsmainmsgtypes.h \
    $(wildcard include/config/set.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/add.h) \
    $(wildcard include/config/del.h) \
    $(wildcard include/config/get/inst.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/webmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/climsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ledservicecmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/nascmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/usbmntcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/tr140cmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ftpservercmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cwmpcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/miccmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/sysinfocmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ipdrcmsmsgtypes.h \
    $(wildcard include/config/change.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dhcpscmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/foncmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/igmpcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ipcheckcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ipintfcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ltecmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/neighdiscmsgtypes.h \
    $(wildcard include/config/file.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/radvdcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/routecmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/wlancmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dsllinecmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/voicecorecmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/voicersscmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/voiceemailcmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/dhcpcmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/ndiscmsmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/pppcmsgtypes.h \
    $(wildcard include/config/failed.h) \
    $(wildcard include/config/st.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cadavermsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/gplmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/cupsprintermsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/sntpmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/kcmsmonitormsgtypes.h \
    $(wildcard include/config/atp/ct/clean.h) \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/msg/bhalmsgtypes.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/atpdebugapi.h \
  /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include/caltime.h \

libbb/xfuncs.o: $(deps_libbb/xfuncs.o)

$(deps_libbb/xfuncs.o):
