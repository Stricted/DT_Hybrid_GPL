cmd_scripts/bin2c := gcc -Wp,-MD,scripts/.bin2c.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/linux/3.4.11/tools/include  -o scripts/bin2c scripts/bin2c.c  

source_scripts/bin2c := scripts/bin2c.c

deps_scripts/bin2c := \
  /usr/include/stdio.h \
  /usr/include/features.h \
  /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i586-suse-linux/4.3/include/stddef.h \
  /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h \
  /usr/include/libio.h \
  /usr/include/_G_config.h \
  /usr/include/wchar.h \
  /usr/lib/gcc/i586-suse-linux/4.3/include/stdarg.h \
  /usr/include/bits/stdio_lim.h \
  /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h \

scripts/bin2c: $(deps_scripts/bin2c)

$(deps_scripts/bin2c):
