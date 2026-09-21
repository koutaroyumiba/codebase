#include "test.h"

int main(void) {
  TestStats stats = {.total = 0, .failures = 0};

  register_base_tests(&stats);
  register_log_tests(&stats);
  register_vector_tests(&stats);
  register_search_tests(&stats);

  print_test_stats(&stats);
  return stats.failures == 0 ? 0 : 1;
}
