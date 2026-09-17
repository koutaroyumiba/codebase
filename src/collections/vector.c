#include "collections/vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

void vector_i32_init(VectorI32 *vector) {
  assert(vector != nullptr);
  vector->data = nullptr;
  vector->len = 0;
  vector->cap = 0;
}

void vector_i32_deinit(VectorI32 *vector) {
  assert(vector != nullptr);
  free(vector->data);
  vector->data = nullptr;
  vector->len = 0;
  vector->cap = 0;
}

bool vector_i32_reserve(VectorI32 *vector, usize min_capacity) {
  assert(vector != nullptr);
  if (min_capacity <= vector->cap) {
    return true;
  }

  if (min_capacity > SIZE_MAX / sizeof(*vector->data)) {
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

bool vector_i32_push(VectorI32 *vector, i32 value) {
  assert(vector != nullptr);
  if (vector->len == vector->cap) {
    usize max_capacity = SIZE_MAX / sizeof(*vector->data);
    if (vector->len == max_capacity) {
      return false;
    }

    usize new_capacity;
    if (vector->cap == 0) {
      new_capacity = 8;
    } else if (vector->cap > max_capacity / 2) {
      new_capacity = max_capacity;
    } else {
      new_capacity = vector->cap * 2;
    }

    bool success = vector_i32_reserve(vector, new_capacity);
    if (!success) {
      return false;
    }
  }

  vector->data[vector->len] = value;
  vector->len++;
  return true;
}
