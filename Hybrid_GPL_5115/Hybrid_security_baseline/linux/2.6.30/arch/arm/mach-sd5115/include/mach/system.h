#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <linux/io.h>
#include <mach/platform.h>

void arch_idle(void);
void arch_reset(char mode,const char *cmd);

#endif
