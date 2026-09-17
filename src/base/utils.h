#ifndef CB_BASE_UTILS_H
#define CB_BASE_UTILS_H

#define ArrayLen(array) (sizeof(array) / sizeof(*(array)))

/* not recommended to have these but...
 *
 * just make sure:
 * - arguments may be evaluated more than once
 * - arguments should not have side effects
 * - arguments should use compatible types
 */
#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(a, x, b) (((x) < (a)) ? (a) : ((b) < (x)) ? (b) : (x))

// kilobyte ^1
// megabyte ^2
// gigabyte ^3

#endif // CB_BASE_UTILS_H
