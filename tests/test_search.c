#include "algorithms/search.h"
#include "test.h"

static bool test_linear_search_i32() {
  i32 values[] = {I32_MIN, -2, 7, -2, I32_MAX};
  usize len = ArrayLen(values);

  OptionUsize result = linear_search_i32(values, len, -2);
  CHECK(result.has_value == true);
  CHECK(result.value == 1);

  result = linear_search_i32(values, len, I32_MAX);
  CHECK(result.has_value == true);
  CHECK(result.value == 4);

  result = linear_search_i32(values, len, 99);
  CHECK(result.has_value == false);

  result = linear_search_i32(nullptr, 0, 99);
  CHECK(result.has_value == false);

  return true;
}

static bool test_binary_search_i32() {
  i32 values[] = {I32_MIN, -2, -2, 0, 7, I32_MAX};
  usize len = ArrayLen(values);

  OptionUsize result = binary_search_i32(values, len, I32_MIN);
  CHECK(result.has_value == true);
  CHECK(result.value == 0);

  result = binary_search_i32(values, len, I32_MAX);
  CHECK(result.has_value == true);
  CHECK(result.value == len - 1);

  result = binary_search_i32(values, len, -2);
  CHECK(result.has_value == true);
  CHECK(result.value < len);
  CHECK(values[result.value] == -2);

  result = binary_search_i32(values, len, 6);
  CHECK(result.has_value == false);

  result = binary_search_i32(nullptr, 0, 6);
  CHECK(result.has_value == false);

  i32 single[] = {42};

  result = binary_search_i32(single, ArrayLen(single), 42);
  CHECK(result.has_value == true);
  CHECK(result.value == 0);

  result = binary_search_i32(single, ArrayLen(single), 10);
  CHECK(result.has_value == false);

  return true;
}

void register_search_tests(TestStats *stats) {
  record_test(stats, "linear_search<i32>", test_linear_search_i32);
  record_test(stats, "binary_search<i32>", test_binary_search_i32);
}
