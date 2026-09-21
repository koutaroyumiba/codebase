#ifndef CB_COLLECTIONS_VECTOR_H
#define CB_COLLECTIONS_VECTOR_H

#include "base/base.h"

/* vector<i32> implementation
 *
 * i32* data  -> pointer to first element
 * usize len  -> length of the vector
 * usize cap  -> capacity of the vector
 *
 * Invariants:
 * 1. len <= cap
 * 2. cap == 0 iff data == nullptr
 *
 * Notes:
 * - vector owns the data
 * - initialised vector MUST eventually be deinitialised
 * - pointer arguments MUST NOT be nullptr
 * - copying initialized vector by assignment is unsafe
 * - future growth may invalidate pointers into data
 */
typedef struct {
  i32 *data;
  usize len;
  usize cap;
} VectorI32;

void VectorI32_init(VectorI32 *vector);
void VectorI32_deinit(VectorI32 *vector);

/* - vector must not be nullptr
 * - vector must be initialised
 * - on success, cap >= min_capacity
 * - existing elements and len are preserved
 * - never shrinks
 * - returns true (noop) when capacity is already sufficient
 * - returns false on allocation or size-overflow failure
 * - failure leaves the vector unchanged
 * - successful growth may invalidate pointers into data
 */
bool VectorI32_reserve(VectorI32 *vector, usize min_capacity);

/* - vector must not be nullptr
 * - vector must be initialised
 * - on success, value is copied to the end and len increases by one
 * - grows capacity when len == cap
 * - growth may invalidate pointers into data
 * - returns false on allocation or capacity-overflow failure
 * - failure leaves the vector unchanged
 */
bool VectorI32_push(VectorI32 *vector, i32 value);

/* - vector must not be nullptr
 * - vector must be initialised
 * - returns the final element and decreases len when non-empty
 * - returns an option with has_value == false when empty
 * - data and cap are unchanged
 */
OptionI32 VectorI32_pop(VectorI32 *vector);

/* - vector must not be nullptr
 * - vector must be initialised
 * - removes all elements
 * - data and cap are unchanged
 */
void VectorI32_clear(VectorI32 *vector);

#endif // CB_COLLECTIONS_VECTOR_H
