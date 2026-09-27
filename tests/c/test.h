#ifndef CB_TEST_H
#define CB_TEST_H

#include <stdio.h>

#include "base/base.h"

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "\t>> [ %s:%d ] check failed: %s\n", __FILE__, __LINE__, \
              #condition);                                                     \
      return false;                                                            \
    }                                                                          \
  } while (false)

typedef bool (*TestFn)(void);

typedef struct {
  usize total;
  usize failures;
} TestStats;

void record_test(TestStats *stats, const char *name, TestFn test);
void print_test_stats(const TestStats *stats);

void register_base_tests(TestStats *stats);
void register_log_tests(TestStats *stats);
void register_vector_tests(TestStats *state);
void register_search_tests(TestStats *stats);
void register_sort_tests(TestStats *stats);

#endif // CB_TEST_H
