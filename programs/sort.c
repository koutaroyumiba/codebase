#include "algorithms/sort.h"
#include "base/utils.h"
#include "diagnostics/log.h"

// just to make life easier...
#define PrintArrayI32(array)                                                   \
  Statement(printf("[ARR] ["); for (usize i = 0; i < ArrayLen(array); i++) {   \
    if (i != 0)                                                                \
      printf(", ");                                                            \
    printf("%d", array[i]);                                                    \
  } printf("]\n");)

int main() {
  /* === LOGGING INIT === */
  log_reset();
  log_set_output(stdout);
  log_set_level(LL_INFO);
  if (!log_enable_channel(INSERTION_SORT_CHANNEL)) {
    CBLOG(LL_ERROR, "failed to enable %s trace", INSERTION_SORT_CHANNEL);
    return 1;
  }

  /* === PROGRAM === */
  i32 values[] = {3, 5, 7, 1, -4, 6, 7};

  PrintArrayI32(values);
  insertion_sort_i32(values, ArrayLen(values));
  PrintArrayI32(values);

  return 0;
}
