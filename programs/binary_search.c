#include "algorithms/search.h"
#include "diagnostics/log.h"

int main() {
  /* ===== LOGGING INIT ===== */
  log_reset();
  log_set_output(stdout);
  log_set_level(LL_INFO);
  // enables trace logging for binary-search module
  if (!log_enable_channel(BINARY_SEARCH_CHANNEL)) {
    CBLOG(LL_ERROR, "failed to enable %s trace", BINARY_SEARCH_CHANNEL);
    return 1;
  }

  /* ===== PROGRAM ===== */
  const i32 values[] = {1, 3, 5, 7, 9};

  OptionUsize result = binary_search_i32(values, ArrayLen(values), 7);

  if (result.has_value) {
    CBLOG(LL_INFO, "found at index %zu", result.value);
  } else {
    CBLOG(LL_INFO, "not found");
  }

  return 0;
}
