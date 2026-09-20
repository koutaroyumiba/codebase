#include "test.h"

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

void register_base_tests(TestStats *stats) {
  record_test(stats, "[base] type sizes", test_base_type_sizes);
}
