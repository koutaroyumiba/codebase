#include <assert.h>

#include "algorithms/search.h"
#include "diagnostics/log.h"

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
      CBTRACE("binary-search",
              "target=%d low=%zu high=%zu mid=%zu value=%d decision=found",
              target, low, high, mid, data[mid]);
      CBTRACE("binary-search", "target=%d result=found low=%zu high=%zu",
              target, low, high);
      return (OptionUsize){
          .has_value = true,
          .value = mid,
      };
    }

    if (data[mid] < target) {
      CBTRACE("binary-search",
              "target=%d low=%zu high=%zu mid=%zu value=%d decision=right, "
              "next_low=%zu next_high=%zu",
              target, low, high, mid, data[mid], mid + 1, high);
      low = mid + 1;
    } else {
      CBTRACE("binary-search",
              "target=%d low=%zu high=%zu mid=%zu value=%d decision=left, "
              "next_low=%zu next_high=%zu",
              target, low, high, mid, data[mid], low, mid);
      high = mid;
    }
  }

  CBTRACE("binary-search", "target=%d result=not-found low=%zu high=%zu",
          target, low, high);
  return (OptionUsize){
      .has_value = false,
      .value = 0,
  };
}
