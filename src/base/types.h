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

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define USIZE_MAX SIZE_MAX

#define I8_MIN INT8_MIN
#define I8_MAX INT8_MAX
#define I16_MIN INT16_MIN
#define I16_MAX INT16_MAX
#define I32_MIN INT32_MIN
#define I32_MAX INT32_MAX
#define I64_MIN INT64_MIN
#define I64_MAX INT64_MAX
#define ISIZE_MIN PTRDIFF_MIN
#define ISIZE_MAX PTRDIFF_MAX

#endif // CB_BASE_TYPES_H
