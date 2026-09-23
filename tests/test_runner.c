#include "test.h"

static bool run_test(const char *name, TestFn test) {
  bool passed = test();
  if (passed) {
    printf("[ PASS   ] %s\n", name);
  } else {
    printf("[   FAIL ] %s\n", name);
  }
  return passed;
}

void record_test(TestStats *stats, const char *name, TestFn test) {
  stats->total++;
  if (!run_test(name, test)) {
    stats->failures++;
  }
}

void print_test_stats(const TestStats *stats) {
  if (stats->failures == 0) {
    printf("All test(s) passed - [%zu/%zu]\n", stats->total - stats->failures,
           stats->total);
  } else {
    printf("%zu test(s) failed - [%zu/%zu]\n", stats->failures,
           stats->total - stats->failures, stats->total);
  }
}
