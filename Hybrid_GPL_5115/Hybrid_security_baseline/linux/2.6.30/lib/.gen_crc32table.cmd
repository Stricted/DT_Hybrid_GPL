cmd_lib/gen_crc32table := gcc -Wp,-MD,lib/.gen_crc32table.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer     -o lib/gen_crc32table lib/gen_crc32table.c  

deps_lib/gen_crc32table := \
  lib/gen_crc32table.c \
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
  lib/crc32defs.h \
  /usr/include/inttypes.h \
  /usr/include/stdint.h \
  /usr/include/bits/wchar.h \

lib/gen_crc32table: $(deps_lib/gen_crc32table)

$(deps_lib/gen_crc32table):
