#include "algorithms/sort.h"
#include "test.h"

static bool arrays_equal_i32(const i32 *actual, const i32 *expected,
                             usize len) {
  for (usize index = 0; index < len; index++) {
    if (actual[index] != expected[index]) {
      return false;
    }
  }

  return true;
}

static bool test_insertion_sort_i32_boundaries() {
  insertion_sort_i32(nullptr, 0);

  i32 single[] = {42};
  const i32 expected[] = {42};
  insertion_sort_i32(single, ArrayLen(single));
  CHECK(arrays_equal_i32(single, expected, ArrayLen(single)));

  return true;
}

static bool test_insertion_sort_i32_mixed_values() {
  i32 values[] = {7, -3, 0, 12, -9, 4};
  const i32 expected[] = {-9, -3, 0, 4, 7, 12};
  insertion_sort_i32(values, ArrayLen(values));
  CHECK(arrays_equal_i32(values, expected, ArrayLen(values)));

  return true;
}

static bool test_insertion_sort_i32_duplicates() {
  i32 values[] = {4, 2, 4, 1, 2, 4};
  const i32 expected[] = {1, 2, 2, 4, 4, 4};
  insertion_sort_i32(values, ArrayLen(values));
  CHECK(arrays_equal_i32(values, expected, ArrayLen(values)));

  i32 equal[] = {6, 6, 6, 6};
  const i32 equal_expected[] = {6, 6, 6, 6};
  insertion_sort_i32(equal, ArrayLen(equal));
  CHECK(arrays_equal_i32(equal, equal_expected, ArrayLen(equal)));

  return true;
}

static bool test_insertion_sort_i32_ordered_inputs() {
  i32 sorted[] = {-5, -1, 0, 3, 9};
  const i32 expected[] = {-5, -1, 0, 3, 9};
  insertion_sort_i32(sorted, ArrayLen(sorted));
  CHECK(arrays_equal_i32(sorted, expected, ArrayLen(sorted)));

  i32 reversed[] = {9, 3, 0, -1, -5};
  insertion_sort_i32(reversed, ArrayLen(reversed));
  CHECK(arrays_equal_i32(reversed, expected, ArrayLen(reversed)));

  return true;
}

static bool test_insertion_sort_i32_extremes() {
  i32 values[] = {I32_MAX, 0, I32_MIN, -1, I32_MAX, I32_MIN};
  const i32 expected[] = {I32_MIN, I32_MIN, -1, 0, I32_MAX, I32_MAX};
  insertion_sort_i32(values, ArrayLen(values));
  CHECK(arrays_equal_i32(values, expected, ArrayLen(values)));

  return true;
}

void register_sort_tests(TestStats *stats) {
  record_test(stats, "[sort] insertion_sort<i32> boundaries",
              test_insertion_sort_i32_boundaries);
  record_test(stats, "[sort] insertion_sort<i32> mixed values",
              test_insertion_sort_i32_mixed_values);
  record_test(stats, "[sort] insertion_sort<i32> duplicates",
              test_insertion_sort_i32_duplicates);
  record_test(stats, "[sort] insertion_sort<i32> ordered inputs",
              test_insertion_sort_i32_ordered_inputs);
  record_test(stats, "[sort] insertion_sort<i32> extremes",
              test_insertion_sort_i32_extremes);
}
