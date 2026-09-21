#include "collections/vector.h"
#include "base/utils.h"

#include <assert.h>
#include <stdlib.h>

void VectorI32_init(VectorI32 *vector) {
  assert(vector != nullptr);
  vector->data = nullptr;
  vector->len = 0;
  vector->cap = 0;
}

void VectorI32_deinit(VectorI32 *vector) {
  assert(vector != nullptr);
  free(vector->data);
  vector->data = nullptr;
  vector->len = 0;
  vector->cap = 0;
}

bool VectorI32_reserve(VectorI32 *vector, usize min_capacity) {
  assert(vector != nullptr);
  if (min_capacity <= vector->cap) {
    return true;
  }

  if (min_capacity > USIZE_MAX / sizeof(*vector->data)) {
    // usize can overflow on multiplication
    return false;
  }

  i32 *new_ptr = realloc(vector->data, min_capacity * sizeof(*vector->data));
  if (new_ptr == nullptr) {
    return false;
  }

  vector->data = new_ptr;
  vector->cap = min_capacity;
  return true;
}

bool VectorI32_push(VectorI32 *vector, i32 value) {
  assert(vector != nullptr);
  if (vector->len == vector->cap) {
    usize max_capacity = USIZE_MAX / sizeof(*vector->data);
    if (vector->len == max_capacity) {
      return false;
    }

    usize new_capacity;
    if (vector->cap > max_capacity / 2) {
      new_capacity = max_capacity;
    } else {
      new_capacity = Max(8, vector->cap * 2);
    }

    bool success = VectorI32_reserve(vector, new_capacity);
    if (!success) {
      return false;
    }
  }

  vector->data[vector->len] = value;
  vector->len++;
  return true;
}

OptionI32 VectorI32_pop(VectorI32 *vector) {
  assert(vector != nullptr);
  if (vector->len == 0) {
    return (OptionI32){
        .has_value = false,
        .value = 0,
    };
  }

  vector->len--;
  i32 popped_element = vector->data[vector->len];

  return (OptionI32){
      .has_value = true,
      .value = popped_element,
  };
}

void VectorI32_clear(VectorI32 *vector) {
  assert(vector != nullptr);
  vector->len = 0;
}
