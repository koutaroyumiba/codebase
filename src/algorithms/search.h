#ifndef CB_ALGORITHMS_SEARCH_H
#define CB_ALGORITHMS_SEARCH_H

#include "base/base.h"

/* channel for logs */
extern const char BINARY_SEARCH_CHANNEL[];

/* - data must not be nullptr when len > 0
 * - returns the index of the first matching element
 * - returns an option with has_value == false when no match exists
 * - does not modify data
 *
 * > Time: O(n)
 * > Space: O(1)
 */
OptionUsize linear_search_i32(const i32 *data, usize len, i32 target);

/* - data must not be nullptr when len > 0
 * - data must be sorted in ascending order
 * - returns the index of a matching element
 * - duplicate values may return any matching index
 * - returns an option with has_value == false when no match exists
 * - does not modify data
 *
 * > Time: O(log n)
 * > Space: O(1)
 */
OptionUsize binary_search_i32(const i32 *data, usize len, i32 target);

#endif // CB_ALGORITHMS_SEARCH_H
