cmd_/home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act/.install := perl scripts/headers_install.pl /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux-3.4/include/linux/tc_act /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act mips tc_csum.h tc_gact.h tc_ipt.h tc_mirred.h tc_nat.h tc_pedit.h tc_skbedit.h; perl scripts/headers_install.pl /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux-3.4/include/linux/tc_act /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act mips ; perl scripts/headers_install.pl /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux-3.4/include/generated/linux/tc_act /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act mips ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act/$$F; done; touch /home/dslsqa/rel1.0/src/buildroot-2011.11/output/toolchain/linux/include/linux/tc_act/.install