#ifndef CB_ALGORITHMS_SORT_H
#define CB_ALGORITHMS_SORT_H

#include "base/base.h"

/* channel for logs */
extern const char INSERTION_SORT_CHANNEL[];

/* - sorted-prefix loop invariant
 * - in-place mutation
 * - stable sorting
 * - unsigned indexes
 *
 * > Time: best: O(n) | average/worst: O(n^2)
 * > Space: O(1)
 */
void insertion_sort_i32(i32 *data, usize len);

#endif // CB_ALGORITHMS_SORT_H
