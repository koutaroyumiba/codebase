#include <assert.h>
#include <stdio.h>

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

OptionUsize binary_search_i32(const i32 *data, usize len, i32 target) {
  assert(data != nullptr || len == 0);

  // we assume the data is sorted
  usize low = 0;
  usize high = len;

  while (low < high) {
    // BAD: usize mid = (low + high) / 2;
    //    (low + high) => can overflow
    usize mid = low + (high - low) / 2;
    if (data[mid] == target) {
      return (OptionUsize){
          .has_value = true,
          .value = mid,
      };
    }

    if (data[mid] < target) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }

  return (OptionUsize){
      .has_value = false,
      .value = 0,
  };
}
