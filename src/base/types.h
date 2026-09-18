#ifndef CB_BASE_TYPES_H
#define CB_BASE_TYPES_H

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ptrdiff_t isize;

typedef float f32;
typedef double f64;

static_assert(CHAR_BIT == 8, "codebase requires 8-bit bytes");
static_assert(sizeof(float) * CHAR_BIT == 32, "f32 requires a 32-bit float");
static_assert(sizeof(double) * CHAR_BIT == 64, "f64 requires a 64-bit float");

typedef struct {
  bool has_value;
  i32 value;
} OptionI32;

#endif // CB_BASE_TYPES_H
