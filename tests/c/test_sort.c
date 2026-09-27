#include "algorithms/sort.h"
#include "test.h"

#include <stdlib.h>

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

static int compare_i32(const void *left_pointer, const void *right_pointer) {
  i32 left = *(const i32 *)left_pointer;
  i32 right = *(const i32 *)right_pointer;
  return (left > right) - (left < right);
}

static bool test_merge_sort_i32_boundaries() {
  CHECK(merge_sort_i32(nullptr, 0));

  i32 single[] = {42};
  CHECK(merge_sort_i32(single, ArrayLen(single)));
  CHECK(single[0] == 42);

  return true;
}

static bool test_merge_sort_i32_values() {
  i32 mixed[] = {7, -3, 0, 12, -9, 4};
  const i32 mixed_expected[] = {-9, -3, 0, 4, 7, 12};
  CHECK(merge_sort_i32(mixed, ArrayLen(mixed)));
  CHECK(arrays_equal_i32(mixed, mixed_expected, ArrayLen(mixed)));

  i32 duplicates[] = {4, 2, 4, 1, 2, 4};
  const i32 duplicates_expected[] = {1, 2, 2, 4, 4, 4};
  CHECK(merge_sort_i32(duplicates, ArrayLen(duplicates)));
  CHECK(arrays_equal_i32(duplicates, duplicates_expected,
                         ArrayLen(duplicates)));

  i32 equal[] = {6, 6, 6, 6};
  const i32 equal_expected[] = {6, 6, 6, 6};
  CHECK(merge_sort_i32(equal, ArrayLen(equal)));
  CHECK(arrays_equal_i32(equal, equal_expected, ArrayLen(equal)));

  i32 extremes[] = {I32_MAX, 0, I32_MIN, -1, I32_MAX, I32_MIN};
  const i32 extremes_expected[] = {I32_MIN, I32_MIN, -1,
                                    0,       I32_MAX, I32_MAX};
  CHECK(merge_sort_i32(extremes, ArrayLen(extremes)));
  CHECK(arrays_equal_i32(extremes, extremes_expected, ArrayLen(extremes)));

  return true;
}

static bool test_merge_sort_i32_ordered_inputs() {
  i32 sorted[] = {-5, -1, 0, 3, 9};
  const i32 expected[] = {-5, -1, 0, 3, 9};
  CHECK(merge_sort_i32(sorted, ArrayLen(sorted)));
  CHECK(arrays_equal_i32(sorted, expected, ArrayLen(sorted)));

  i32 reversed[] = {9, 3, 0, -1, -5};
  CHECK(merge_sort_i32(reversed, ArrayLen(reversed)));
  CHECK(arrays_equal_i32(reversed, expected, ArrayLen(reversed)));

  return true;
}

static bool test_merge_sort_i32_power_of_two_boundaries() {
  i32 length3[] = {3, 1, 2};
  const i32 expected3[] = {1, 2, 3};
  CHECK(merge_sort_i32(length3, ArrayLen(length3)));
  CHECK(arrays_equal_i32(length3, expected3, ArrayLen(length3)));

  i32 length4[] = {4, 2, 3, 1};
  const i32 expected4[] = {1, 2, 3, 4};
  CHECK(merge_sort_i32(length4, ArrayLen(length4)));
  CHECK(arrays_equal_i32(length4, expected4, ArrayLen(length4)));

  i32 length5[] = {5, 3, 1, 4, 2};
  const i32 expected5[] = {1, 2, 3, 4, 5};
  CHECK(merge_sort_i32(length5, ArrayLen(length5)));
  CHECK(arrays_equal_i32(length5, expected5, ArrayLen(length5)));

  i32 length7[] = {7, 2, 6, 1, 5, 3, 4};
  const i32 expected7[] = {1, 2, 3, 4, 5, 6, 7};
  CHECK(merge_sort_i32(length7, ArrayLen(length7)));
  CHECK(arrays_equal_i32(length7, expected7, ArrayLen(length7)));

  i32 length8[] = {8, 3, 7, 1, 6, 2, 5, 4};
  const i32 expected8[] = {1, 2, 3, 4, 5, 6, 7, 8};
  CHECK(merge_sort_i32(length8, ArrayLen(length8)));
  CHECK(arrays_equal_i32(length8, expected8, ArrayLen(length8)));

  i32 length9[] = {9, 4, 8, 1, 7, 2, 6, 3, 5};
  const i32 expected9[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK(merge_sort_i32(length9, ArrayLen(length9)));
  CHECK(arrays_equal_i32(length9, expected9, ArrayLen(length9)));

  return true;
}

static bool test_merge_sort_i32_larger_input() {
  i32 values[257];
  i32 expected[ArrayLen(values)];

  for (usize i = 0; i < ArrayLen(values); i++) {
    values[i] = (i32)((i * 73 + 19) % 101) - 50;
    expected[i] = values[i];
  }

  qsort(expected, ArrayLen(expected), sizeof(*expected), compare_i32);

  CHECK(merge_sort_i32(values, ArrayLen(values)));
  CHECK(arrays_equal_i32(values, expected, ArrayLen(values)));

  return true;
}

static bool test_merge_sort_i32_size_overflow() {
  i32 value = 42;
  usize overflowing_length = USIZE_MAX / sizeof(value) + 1;

  CHECK(!merge_sort_i32(&value, overflowing_length));
  CHECK(value == 42);

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
  record_test(stats, "[sort] merge_sort<i32> boundaries",
              test_merge_sort_i32_boundaries);
  record_test(stats, "[sort] merge_sort<i32> values",
              test_merge_sort_i32_values);
  record_test(stats, "[sort] merge_sort<i32> ordered inputs",
              test_merge_sort_i32_ordered_inputs);
  record_test(stats, "[sort] merge_sort<i32> power-of-two boundaries",
              test_merge_sort_i32_power_of_two_boundaries);
  record_test(stats, "[sort] merge_sort<i32> larger input",
              test_merge_sort_i32_larger_input);
  record_test(stats, "[sort] merge_sort<i32> size overflow",
              test_merge_sort_i32_size_overflow);
}
