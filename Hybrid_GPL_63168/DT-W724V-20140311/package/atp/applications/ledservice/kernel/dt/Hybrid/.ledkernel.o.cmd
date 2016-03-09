cmd_/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o := /usr1/xuhui/DT-W724V-20140311/output/host/usr/bin/mips-unknown-linux-uclibc-gcc -Wp,-MD,/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/.ledkernel.o.d  -nostdinc -isystem /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /usr1/xuhui/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D__KERNEL__ -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/arch/mips/include -Iarch/mips/include/generated -Iinclude -include /usr1/xuhui/DT-W724V-20140311/linux/3.4.11/include/linux/kconfig.h -D"VMLINUX_LOAD_ADDRESS=0x80010000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -I/usr1/xuhui/DT-W724V-20140311/output/staging/usr/include -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32 -Wa,-mips32 -Wa,--trap -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-bcm963xx -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/../../bcmdrivers/opensource/include/bcm963xx -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/../../shared/opensource/include/bcm963xx -I/usr1/xuhui/DT-W724V-20140311/linux/3.4.11/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I/usr1/xuhui/DT-W724V-20140311/driver/broadcom/bcm963268/4.14L02/include    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ledkernel)"  -D"KBUILD_MODNAME=KBUILD_STR(ledkernel)" -c -o /usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o /usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.c

source_/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o := /usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.c

deps_/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o := \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bhal.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atpconfig.h \
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
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atpconfig.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msgapi.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atptypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/stdio.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/features.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/uClibc_config.h \
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
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sys/cdefs.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stddef.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/types.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/wordsize.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/typesizes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/pthreadtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/sched.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/uClibc_stdio.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/wchar.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/wchar.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/uClibc_mutex.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/pthread.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sched.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/time.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/time.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/uClibc_clk_tck.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/signal.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/sigset.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/initspin.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/sigthread.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/uClibc_pthread.h \
  /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/lib/gcc/mips-unknown-linux-uclibc/4.6.2/include/stdarg.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/stdio_lim.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/stdlib.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sys/types.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/endian.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/endian.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/byteswap.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/byteswap.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/byteswap-common.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sys/select.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bits/select.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sys/sysmacros.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/alloca.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/string.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/malloc.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atpcbbid.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atpconst.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/shmemapi.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/sysutil.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msgtypeswrapper.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cfmcoremsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/msgcoremsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/atsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/atcmdmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/nascentermsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atptypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/oammsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cwmpappmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cwmpmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/logmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/miccoremsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/micmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/telnetdmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/tr111msgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/upgmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/upnpmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ddnscmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dhcp6cmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dhcp6smsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dhcpsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dnsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/abusecmdmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/foncontrlmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/hybridmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ipcheckmsgtypes.h \
    $(wildcard include/config/obj.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/routemsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/voicecoremsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cmsmainmsgtypes.h \
    $(wildcard include/config/set.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/add.h) \
    $(wildcard include/config/del.h) \
    $(wildcard include/config/get/inst.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/webmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/climsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ledservicecmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/nascmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/usbmntcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/tr140cmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ftpservercmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cwmpcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/miccmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/sysinfocmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ipdrcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dhcpscmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/foncmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/igmpcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ipcheckcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ipintfcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ltecmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/neighdiscmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/radvdcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/routecmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/wlancmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dsllinecmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/voicecorecmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/voicersscmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/voiceemailcmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/dhcpcmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/ndiscmsmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/pppcmsgtypes.h \
    $(wildcard include/config/failed.h) \
    $(wildcard include/config/st.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cadavermsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/gplmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/cupsprintermsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/sntpmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/kcmsmonitormsgtypes.h \
    $(wildcard include/config/atp/ct/clean.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/msg/bhalmsgtypes.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/atpdebugapi.h \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/board.h \
    $(wildcard include/config/bcm96368.h) \
    $(wildcard include/config/bcm96816.h) \
    $(wildcard include/config/bcm96818.h) \
    $(wildcard include/config/bcm/cpld1.h) \
    $(wildcard include/config/bcm/avs/pwrsave.h) \
    $(wildcard include/config/bcm/ddr/self/refresh/pwrsave.h) \
    $(wildcard include/config/bcm/pwrmngt/ddr/sr/api.h) \
    $(wildcard include/config/bcm96838.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/bcm_hwdefs.h \
    $(wildcard include/config/brcm/ikos.h) \
    $(wildcard include/config/bcm963381.h) \
    $(wildcard include/config/bcm96328.h) \
    $(wildcard include/config/bcm96362.h) \
    $(wildcard include/config/bcm963268.h) \
    $(wildcard include/config/bcm96828.h) \
    $(wildcard include/config/bcm96318.h) \
    $(wildcard include/config/bcm968500.h) \
  /usr1/xuhui/DT-W724V-20140311/output/staging/usr/include/boardparms.h \
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

/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o: $(deps_/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o)

$(deps_/usr1/xuhui/DT-W724V-20140311/package/atp/applications/ledservice/kernel/dt/Hybrid/ledkernel.o):
