#include "collections/vector.h"

#include <assert.h>
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
