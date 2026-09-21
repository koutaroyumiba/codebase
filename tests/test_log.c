#include <string.h>

#include "diagnostics/log.h"
#include "test.h"

static bool read_capture(FILE *capture, char *buffer, usize capacity) {
  if (capacity == 0) {
    return false;
  }

  if (fflush(capture) != 0) {
    return false;
  }

  rewind(capture);

  usize bytes_read = fread(buffer, 1, capacity - 1, capture);
  if (ferror(capture)) {
    return false;
  }

  buffer[bytes_read] = '\0';
  return true;
}

static bool test_log_silent_by_default() {
  FILE *capture = tmpfile();
  CHECK(capture != nullptr);

  log_reset();
  log_set_output(capture);

  log_message(LL_INFO, "this should be filtered");
  log_channel("binary-search-trace", "this should also be filtered");

  bool flushed = fflush(capture) == 0;
  long bytes_written = ftell(capture);

  log_reset();
  bool closed = fclose(capture) == 0;

  CHECK(flushed);
  CHECK(bytes_written == 0);
  CHECK(closed);

  return true;
}

static bool test_log_boundaries() {
  FILE *capture = tmpfile();
  CHECK(capture != nullptr);

  log_reset();
  log_set_output(capture);
  log_set_level(LL_INFO);

  char output[256];

  log_message(LL_DEBUG, "this should be filtered");
  log_message(LL_INFO, "this should be written");

  bool captured = read_capture(capture, output, sizeof(output));

  log_reset();
  bool closed = fclose(capture) == 0;

  CHECK(captured);
  CHECK(strcmp(output, "[INFO] this should be written\n") == 0);
  CHECK(closed);

  return true;
}

static bool test_log_channels_are_independent() {
  FILE *capture = tmpfile();
  CHECK(capture != nullptr);

  char output[256];
  log_reset();
  log_set_output(capture);

  bool enabled = log_enable_channel("binary-search-trace");

  log_channel("binary-search-trace", "low=%zu high=%zu", (usize)0, (usize)4);
  log_channel("vectorI32-trace", "this should not appear");

  bool captured = read_capture(capture, output, sizeof(output));

  log_reset();
  bool closed = fclose(capture) == 0;

  CHECK(enabled);
  CHECK(captured);
  CHECK(strcmp(output, "[TRACE] [binary-search-trace] low=0 high=4\n") == 0);
  CHECK(closed);

  return true;
}

void register_log_tests(TestStats *stats) {
  record_test(stats, "[log] silent by default", test_log_silent_by_default);
  record_test(stats, "[log] independent channels",
              test_log_channels_are_independent);
  record_test(stats, "[log] debug/info boundary", test_log_boundaries);
}
