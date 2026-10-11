#include <stdlib.h>

#include "collections/vector.h"
#include "test.h"

static bool test_VectorI32_lifecycle() {
  VectorI32 vector;
  VectorI32_init(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  vector.data = malloc(sizeof(*vector.data) * 2);
  CHECK(vector.data != nullptr);
  vector.len = 2;
  vector.cap = 2;
  VectorI32_deinit(&vector);
  CHECK(vector.data == nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);

  return true;
}

static bool test_VectorI32_reserve_success() {
  VectorI32 vector;
  VectorI32_init(&vector);
  bool success = VectorI32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data != nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap >= 2);
  vector.data[0] = 1;
  vector.data[1] = 2;
  vector.len = 2;
  i32 *previous_data = vector.data;
  usize previous_cap = vector.cap;
  success = VectorI32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data == previous_data);
  CHECK(vector.cap == previous_cap);
  success = VectorI32_reserve(&vector, 8);
  CHECK(success == true);
  CHECK(vector.cap >= 8);
  CHECK(vector.len == 2);
  CHECK(vector.data[0] == 1);
  CHECK(vector.data[1] == 2);

  VectorI32_deinit(&vector);
  return true;
}

static bool test_VectorI32_reserve_overflow() {
  VectorI32 vector;
  VectorI32_init(&vector);
  bool success = VectorI32_reserve(&vector, 2);
  CHECK(success == true);
  CHECK(vector.data != nullptr);
  CHECK(vector.len == 0);
  CHECK(vector.cap >= 2);
  i32 *previous_data = vector.data;
  usize previous_len = vector.len;
  usize previous_cap = vector.cap;
  success = VectorI32_reserve(&vector, SIZE_MAX);
  CHECK(success == false);
  CHECK(vector.data == previous_data);
  CHECK(vector.len == previous_len);
  CHECK(vector.cap == previous_cap);

  VectorI32_deinit(&vector);
  return true;
}

static bool test_VectorI32_push() {
  VectorI32 vector;
  VectorI32_init(&vector);
  bool success = VectorI32_push(&vector, 0);
  CHECK(success == true);
  CHECK(vector.len == 1);
  CHECK(vector.cap >= vector.len);
  CHECK(vector.data[0] == 0);

  usize previous_cap = vector.cap;
  for (usize i = vector.len; i < previous_cap; i++) {
    success = VectorI32_push(&vector, 1);
    CHECK(success == true);
  }
  CHECK(vector.cap == previous_cap);
  CHECK(vector.len == previous_cap);

  success = VectorI32_push(&vector, -1);
  CHECK(success == true);
  CHECK(vector.len == previous_cap + 1);
  CHECK(vector.cap > previous_cap);
  CHECK(vector.data[0] == 0);
  CHECK(vector.data[previous_cap] == -1);
  if (previous_cap > 1) {
    CHECK(vector.data[previous_cap - 1] == 1);
  }

  VectorI32_deinit(&vector);
  return true;
}

static bool test_VectorI32_pop() {
  VectorI32 vector;
  VectorI32_init(&vector);

  OptionI32 popped = VectorI32_pop(&vector);
  CHECK(popped.has_value == false);
  CHECK(vector.len == 0);

  CHECK(VectorI32_push(&vector, 10));
  CHECK(VectorI32_push(&vector, 20));

  i32 *previous_data = vector.data;
  usize previous_cap = vector.cap;

  popped = VectorI32_pop(&vector);
  CHECK(popped.has_value == true);
  CHECK(popped.value == 20);
  CHECK(vector.len == 1);
  CHECK(vector.data == previous_data);
  CHECK(vector.cap == previous_cap);

  popped = VectorI32_pop(&vector);
  CHECK(popped.has_value == true);
  CHECK(popped.value == 10);
  CHECK(vector.len == 0);

  popped = VectorI32_pop(&vector);
  CHECK(popped.has_value == false);

  VectorI32_deinit(&vector);
  return true;
}

static bool test_VectorI32_clear() {
  VectorI32 vector;
  VectorI32_init(&vector);

  VectorI32_clear(&vector);
  CHECK(vector.len == 0);
  CHECK(vector.cap == 0);
  CHECK(vector.data == nullptr);

  CHECK(VectorI32_push(&vector, 10));
  CHECK(VectorI32_push(&vector, 20));

  i32 *previous_data = vector.data;
  usize previous_cap = vector.cap;

  VectorI32_clear(&vector);
  CHECK(vector.len == 0);
  CHECK(vector.data == previous_data);
  CHECK(vector.cap == previous_cap);

  CHECK(VectorI32_push(&vector, 30));
  CHECK(vector.len == 1);
  CHECK(vector.data == previous_data);
  CHECK(vector.data[0] == 30);

  VectorI32_deinit(&vector);
  return true;
}

void register_vector_tests(TestStats *stats) {
  record_test(stats, "[vector<i32>] lifecycle", test_VectorI32_lifecycle);
  record_test(stats, "[vector<i32>] reserve success",
              test_VectorI32_reserve_success);
  record_test(stats, "[vector<i32>] reserve overflow",
              test_VectorI32_reserve_overflow);
  record_test(stats, "[vector<i32>] push", test_VectorI32_push);
  record_test(stats, "[vector<i32>] pop", test_VectorI32_pop);
  record_test(stats, "[vector<i32>] clear", test_VectorI32_clear);
}
