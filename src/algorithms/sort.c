#include <assert.h>

#include "algorithms/sort.h"
#include "diagnostics/log.h"

const char INSERTION_SORT_CHANNEL[] = "insertion-sort";
/**
Iteratively insert each element of an unsorted list into its correct position in
a sorted portion of the list

- Start with the second element as the first element is assumed to be sorted
- Compare second element with the first
- If the second is smaller, than swap them
- Move to the third element and compare it with the first two, and put it in its
correct position
- Repeat until the entire array is sorted.
*/

void insertion_sort_i32(i32 *data, usize len) {
  assert(data != nullptr || len == 0);

  for (usize i = 1; i < len; i++) {
    CBTRACE(INSERTION_SORT_CHANNEL, "sorted:[0..%zu] unsorted:[%zu..%zu]",
            i - 1, i, len - 1);
    for (usize j = i; j > 0; j--) {
      if (data[j - 1] > data[j]) {
        CBTRACE(INSERTION_SORT_CHANNEL, "swap [%zu], [%zu] - (%d %d)", j - 1, j,
                data[j - 1], data[j]);
        i32 temp = data[j - 1];
        data[j - 1] = data[j];
        data[j] = temp;
      } else {
        CBTRACE(INSERTION_SORT_CHANNEL, "[%zu] <= [%zu] - no swap", j - 1, j);
        break;
      }
    }
  }

  return;
}
