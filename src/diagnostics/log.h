#ifndef CB_DIAGNOSTICS_LOG_H
#define CB_DIAGNOSTICS_LOG_H

/*
 * Configuration is process-global and not thread-safe
 * Channel names are borrowed and must remain valid until disabled/reset
 * Diagnostic functions never take ownership of FILE * or channel strings
 */

#include <stdio.h>

#include "base/base.h"

typedef enum {
  LL_DEBUG,
  LL_INFO,
  LL_WARNING,
  LL_ERROR,
  LL_DISABLE,
} LogLevel;

/* Configurations */

/**
 * restores destination to stderr,
 * disables ordinary messages
 * every trace channel disabled
 */
void log_reset();
/* assert non nullptr */
void log_set_output(FILE *output);
/* LL_DISABLE to disable logging */
void log_set_level(LogLevel level);

/* Trace Channels */

/*
 * - stores the pointer but does not own or copy the string
 * - channel must remain valid and unchanged until disabled or reset
 * - enabling an already-enabled channel succeeds without duplicating (noop)
 * - if array is full, returns false
 * - channel equality uses strcmp
 * - in practice, recommended to use string literals
 */
bool log_enable_channel(const char *channel);
void log_disable_channel(const char *channel);
void log_disable_all_channels();

/* Log Emissions */

void log_message(LogLevel level, const char *format, ...);
void log_channel(const char *channel, const char *format, ...);

#define CBLOG(level, ...) log_message((level), __VA_ARGS__)
#define CBTRACE(channel, ...) log_channel((channel), __VA_ARGS__)

#endif // CB_DIAGNOSTICS_LOG_H
