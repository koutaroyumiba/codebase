#ifndef CB_ALGORITHMS_SORT_H
#define CB_ALGORITHMS_SORT_H

#include "base/base.h"

/* channel for logs */
extern const char INSERTION_SORT_CHANNEL[];

/* - data must not be nullptr when len > 0
 * - output is sorted in ascending order
 * - mutation occurs in-place
 * - stable sorting
 * - the caller retains ownership of *data
 * - does not retain the pointer or allocate memory
 *
 * > Time: best: O(n) | average/worst: O(n^2)
 * > Space: O(1)
 */
void insertion_sort_i32(i32 *data, usize len);

#endif // CB_ALGORITHMS_SORT_H
