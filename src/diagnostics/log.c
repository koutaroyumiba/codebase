#include "diagnostics/log.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

constexpr int LOG_MAX_ENABLED_CHANNELS = 16;

static const char *enabled_channels[LOG_MAX_ENABLED_CHANNELS];
static usize enabled_channel_count;

/* configured_output == nullptr means 'use stderr' */
static FILE *configured_output;
static LogLevel minimum_level = LL_DISABLE;

/* =================
 * PRIVATE FUNCTIONS
 * ================= */

static FILE *current_output() {
  if (configured_output == nullptr) {
    return stderr;
  }

  return configured_output;
}

static OptionUsize find_channel_index(const char *channel) {
  for (usize i = 0; i < enabled_channel_count; i++) {
    if (strcmp(enabled_channels[i], channel) == 0) {
      return (OptionUsize){
          .has_value = true,
          .value = i,
      };
    }
  }

  return (OptionUsize){
      .has_value = false,
      .value = 0,
  };
}

static const char *level_name(LogLevel level) {
  switch (level) {
  case LL_DEBUG:
    return " DEBUG ";
  case LL_INFO:
    return " INFO  ";
  case LL_WARNING:
    return "WARNING";
  case LL_ERROR:
    return " ERROR ";
  case LL_DISABLE:
    break;
  }

  return "UNKNOWN";
}

/* =================
 * PUBLIC FUNCTIONS
 * ================= */
void log_reset() {
  configured_output = nullptr;
  minimum_level = LL_DISABLE;
  enabled_channel_count = 0;
}

void log_set_output(FILE *output) {
  assert(output != nullptr);
  configured_output = output;
}

void log_set_level(LogLevel level) {
  assert(level >= LL_DEBUG);
  assert(level <= LL_DISABLE);

  minimum_level = level;
}

bool log_enable_channel(const char *channel) {
  assert(channel != nullptr);
  assert(channel[0] != '\0');

  OptionUsize result = find_channel_index(channel);
  if (result.has_value) {
    return true;
  }

  if (enabled_channel_count == LOG_MAX_ENABLED_CHANNELS) {
    return false;
  }

  enabled_channels[enabled_channel_count] = channel;
  enabled_channel_count++;
  return true;
}

void log_disable_channel(const char *channel) {
  assert(channel != nullptr);
  assert(channel[0] != '\0');

  OptionUsize result = find_channel_index(channel);

  if (!result.has_value) {
    return;
  }

  for (usize i = result.value; i + 1 < enabled_channel_count; i++) {
    enabled_channels[i] = enabled_channels[i + 1];
  }

  enabled_channel_count--;
}

void log_disable_all_channels() { enabled_channel_count = 0; }

void log_message(LogLevel level, const char *format, ...) {
  assert(level >= LL_DEBUG);
  assert(level <= LL_ERROR);
  assert(format != nullptr);

  if (minimum_level == LL_DISABLE || level < minimum_level) {
    return;
  }

  FILE *output = current_output();

  fprintf(output, "[%s] ", level_name(level));
  va_list arguments;
  va_start(arguments, format);
  vfprintf(output, format, arguments);
  va_end(arguments);

  fputc('\n', output);
}

void log_channel(const char *channel, const char *format, ...) {
  assert(channel != nullptr);
  assert(channel[0] != '\0');
  assert(format != nullptr);

  OptionUsize result = find_channel_index(channel);
  if (!result.has_value) {
    return;
  }

  FILE *output = current_output();

  fprintf(output, "[%s] ", channel);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(output, format, arguments);
  va_end(arguments);

  fputc('\n', output);
}
