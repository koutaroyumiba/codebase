#include <assert.h>

#include "algorithms/search.h"

OptionUsize linear_search_i32(const i32 *data, usize len, i32 target) {
  assert(data != nullptr || len == 0);

  for (usize i = 0; i < len; i++) {
    if (data[i] == target) {
      return (OptionUsize){
          .has_value = true,
          .value = i,
      };
    }
  }

  return (OptionUsize){
      .has_value = false,
      .value = 0,
  };
}
