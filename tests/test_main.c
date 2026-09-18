#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "base/types.h"
#include "collections/vector.h"

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

static bool test_vector_i32() {
  VectorI32 vector;
  vector_i32_init(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  vector.data = malloc(sizeof(*vector.data) * 2);
  CHECK(vector.data != nullptr);
  vector.len = 2;
  vector.cap = 2;
  vector_i32_deinit(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  return true;
}

static bool test_vector_i32_reserve_success() {
  VectorI32 vector;
  vector_i32_init(&vector);
  bool success = vector_i32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data != nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap >= 2);
  vector.data[0] = 1;
  vector.data[1] = 2;
  vector.len = 2;
  i32 *previous_data = vector.data;
  usize previous_cap = vector.cap;
  success = vector_i32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data == previous_data);
  CHECK(vector.cap == previous_cap);
  success = vector_i32_reserve(&vector, 8);
  CHECK(success == true);
  CHECK(vector.cap >= 8);
  CHECK(vector.len == 2);
  CHECK(vector.data[0] == 1);
  CHECK(vector.data[1] == 2);
  vector_i32_deinit(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  return true;
}

static bool test_vector_i32_reserve_overflow() {
  VectorI32 vector;
  vector_i32_init(&vector);
  bool success = vector_i32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data != nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap >= 2);
  i32 *previous_data = vector.data;
  usize previous_len = vector.len;
  usize previous_cap = vector.cap;
  success = vector_i32_reserve(&vector, SIZE_MAX);
  CHECK(success == false);
  CHECK(vector.data == previous_data);
  CHECK(vector.len == previous_len);
  CHECK(vector.cap == previous_cap);
  vector_i32_deinit(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  return true;
}

static bool test_vector_i32_push() {
  VectorI32 vector;
  vector_i32_init(&vector);
  bool success = vector_i32_push(&vector, 0);
  CHECK(success == true);
  CHECK(vector.len == 1);
  CHECK(vector.cap >= vector.len);
  CHECK(vector.data[0] == 0);

  usize previous_cap = vector.cap;
  for (usize i = vector.len; i < previous_cap; i++) {
    success = vector_i32_push(&vector, 1);
    CHECK(success == true);
  }
  CHECK(vector.cap == previous_cap);
  CHECK(vector.len == previous_cap);

  success = vector_i32_push(&vector, -1);
  CHECK(success == true);
  CHECK(vector.len == previous_cap + 1);
  CHECK(vector.cap > previous_cap);
  CHECK(vector.data[0] == 0);
  CHECK(vector.data[previous_cap] == -1);
  if (previous_cap > 1) {
    CHECK(vector.data[previous_cap - 1] == 1);
  }

  vector_i32_deinit(&vector);
  return true;
}

static bool test_vector_i32_pop() {
  VectorI32 vector;
  vector_i32_init(&vector);

  OptionI32 popped = vector_i32_pop(&vector);
  CHECK(popped.has_value == false);
  CHECK(vector.len == 0);

  CHECK(vector_i32_push(&vector, 10));
  CHECK(vector_i32_push(&vector, 20));

  i32 *previous_data = vector.data;
  usize previous_cap = vector.cap;

  popped = vector_i32_pop(&vector);
  CHECK(popped.has_value == true);
  CHECK(popped.value == 20);
  CHECK(vector.len == 1);
  CHECK(vector.data == previous_data);
  CHECK(vector.cap == previous_cap);

  popped = vector_i32_pop(&vector);
  CHECK(popped.has_value == true);
  CHECK(popped.value == 10);
  CHECK(vector.len == 0);

  popped = vector_i32_pop(&vector);
  CHECK(popped.has_value == false);

  vector_i32_deinit(&vector);
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
  record_test(&stats, "vector<i32> lifecycle", test_vector_i32);
  record_test(&stats, "vector<i32> reserve success",
              test_vector_i32_reserve_success);
  record_test(&stats, "vector<i32> reserve overflow",
              test_vector_i32_reserve_overflow);
  record_test(&stats, "vector<i32> push", test_vector_i32_push);
  record_test(&stats, "vector<i32> pop", test_vector_i32_pop);

  print_stats(&stats);
  return stats.failures == 0 ? 0 : 1;
}
