/*
 * Simpler log module
 *
 * Copyright (C) 2007 FON Wireless Ltd.
 *
 * Created: Oct 2007. Pablo Martín <pablo@fon.com>
 *
 * $Rev: 57 $
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include "log.h"

int global_log_to_stdout = 1;
static void log_function(int priority, const char *message);
static void log_vprintf(int priority, const char *format, va_list ap);

/* log_start. If not initalized, logs are printed to stdout */
void log_start(char *name, int log_to_stdout)
{
	if (log_to_stdout) {
		global_log_to_stdout = 1;
	} else {
		global_log_to_stdout = 0;
		openlog(name, LOG_NDELAY, LOG_DAEMON);
	}
}

void log_end(void)
{
	if (global_log_to_stdout)
		closelog();
}

static void log_vprintf(int priority, const char *format, va_list ap)
{
	int n;
	char buffer[4096];

	n = vsnprintf(buffer, 4095, format, ap);
	if (n > -1 && n < 4096) {
		log_function(priority, buffer);
	} else {
		log_function(LOG_CRIT, "log_printf: message too long to be logged");
	}
}
static void log_function(int priority, const char *message)
{
	/*if (!global_log_to_stdout)
		syslog(priority, "%s", message);
	else*/
		printf("%d: %s\n", priority, message);
}

#define LOGFUNCTION(priority) \
	va_list ap;\
	va_start(ap, message);\
	log_vprintf(priority, message, ap);\
	va_end(ap);

void log_message(const char *message, ...)
{
	LOGFUNCTION(LOG_INFO)
}
void log_warning(const char *message, ...)
{
	LOGFUNCTION(LOG_WARNING)
}
void log_error(const char *message, ...)
{
	LOGFUNCTION(LOG_ERR)
}
void log_critical(const char *message, ...)
{
	LOGFUNCTION(LOG_CRIT)
}
