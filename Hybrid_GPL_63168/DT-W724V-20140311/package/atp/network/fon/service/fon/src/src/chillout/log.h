/*
 * Simpler log module
 *
 * Copyright (C) 2007 FON Wireless Limited
 *
 * Created: Oct 2007. Pablo Martín <pablo@fon.com>
 *
 * $Rev: 57 $
 */
#ifndef _LOG_H
#define _LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#ifndef NDEBUG
#define log
#endif

	void log_start(char *name, int log_to_stdout);
	void log_end(void);
	void log_printf(int priority, const char *message, ...);
	void log_message(const char *message, ...);
	void log_warning(const char *message, ...);
	void log_error(const char *message, ...);
	void log_critical(const char *message, ...);

#ifdef __cplusplus
}
#endif
#endif
