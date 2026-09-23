#ifndef CB_BASE_TYPES_H
#define CB_BASE_TYPES_H

#include <limits.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef size_t usize;

typedef signed char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef ptrdiff_t isize;

typedef float f32;
typedef double f64;

// some checks to make sure:
static_assert(CHAR_BIT == 8, "codebase requires 8-bit bytes");
static_assert(sizeof(u8) * CHAR_BIT == 8, "u8 must occupy 8 bits");
static_assert(sizeof(u16) * CHAR_BIT == 16, "u16 must occupy 16 bits");
static_assert(sizeof(u32) * CHAR_BIT == 32, "u32 must occupy 32 bits");
static_assert(sizeof(u64) * CHAR_BIT == 64, "u64 must occupy 64 bits");
static_assert(sizeof(i8) * CHAR_BIT == 8, "i8 must occupy 8 bits");
static_assert(sizeof(i16) * CHAR_BIT == 16, "i16 must occupy 16 bits");
static_assert(sizeof(i32) * CHAR_BIT == 32, "i32 must occupy 32 bits");
static_assert(sizeof(i64) * CHAR_BIT == 64, "i64 must occupy 64 bits");
static_assert(sizeof(f32) * CHAR_BIT == 32, "f32 must occupy 32 bits");
static_assert(sizeof(f64) * CHAR_BIT == 64, "f64 must occupy 64 bits");

typedef struct {
  bool has_value;
  i32 value;
} OptionI32;

typedef struct {
  bool has_value;
  usize value;
} OptionUsize;

#endif // CB_BASE_TYPES_H
