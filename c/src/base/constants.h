#ifndef CB_BASE_CONSTANTS_H
#define CB_BASE_CONSTANTS_H

#include <limits.h>
#include <stdint.h>

#define U8_MAX UCHAR_MAX
#define U16_MAX USHRT_MAX
#define U32_MAX UINT_MAX
#define U64_MAX ULLONG_MAX
#define USIZE_MAX SIZE_MAX

#define I8_MIN SCHAR_MIN
#define I16_MIN SHRT_MIN
#define I32_MIN INT_MIN
#define I64_MIN LLONG_MIN
#define ISIZE_MIN PTRDIFF_MIN

#define I8_MAX SCHAR_MAX
#define I16_MAX SHRT_MAX
#define I32_MAX INT_MAX
#define I64_MAX LLONG_MAX
#define ISIZE_MAX PTRDIFF_MAX

#endif // CB_BASE_CONSTANTS_H
