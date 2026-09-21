#include "algorithms/search.h"
#include "diagnostics/log.h"

int main() {
  const i32 values[] = {1, 3, 5, 7, 9};
  log_reset();
  log_set_output(stdout);
  log_set_level(LL_INFO);

  if (!log_enable_channel("binary-search")) {
    CBLOG(LL_ERROR, "failed to enable binary-search trace");
    return 1;
  }

  OptionUsize result = binary_search_i32(values, ArrayLen(values), 7);

  if (result.has_value) {
    CBLOG(LL_INFO, "found at index %zu", result.value);
  } else {
    CBLOG(LL_INFO, "not found");
  }

  return 0;
}
