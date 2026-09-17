#include <stdio.h>

#include "base/types.h"

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__,         \
              #condition);                                                     \
      return false;                                                            \
    }                                                                          \
  } while (false)

typedef bool (*test_fn)(void);

typedef struct {
  usize total;
  usize failures;
} TestStats;

static bool test_base_type_sizes(void) {
  CHECK(sizeof(u8) == 1);
  CHECK(sizeof(u16) == 2);
  CHECK(sizeof(u32) == 4);
  CHECK(sizeof(u64) == 8);

  CHECK(sizeof(i8) == 1);
  CHECK(sizeof(i16) == 2);
  CHECK(sizeof(i32) == 4);
  CHECK(sizeof(i64) == 8);

  CHECK(sizeof(f32) == 4);
  CHECK(sizeof(f64) == 8);

  return true;
}

static bool run_test(const char *name, test_fn test) {
  bool passed = test();

  printf("[%s] %s\n", passed ? "PASS" : "FAIL", name);
  return passed;
}

static void record_test(TestStats *stats, const char *name, test_fn test) {
  stats->total++;
  if (!run_test(name, test)) {
    stats->failures++;
  }
}

static void print_stats(TestStats *stats) {
  printf("%zu test(s) failed - [%zu/%zu]\n", stats->failures,
         stats->total - stats->failures, stats->total);
}

int main(void) {
  TestStats stats = {.total = 0, .failures = 0};
  record_test(&stats, "base type sizes", test_base_type_sizes);

  print_stats(&stats);
  return stats.failures == 0 ? 0 : 1;
}
