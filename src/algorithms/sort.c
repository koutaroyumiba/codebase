#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms/sort.h"
#include "diagnostics/log.h"

const char INSERTION_SORT_CHANNEL[] = "insertion-sort";
const char MERGE_SORT_CHANNEL[] = "merge-sort";

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

/* helper function for merge sort */
static void sort_and_merge_lists(i32 *original, i32 *temp_array,
                                 usize left_index, usize left_length,
                                 usize right_index, usize right_length) {
  if (left_length == 0 || right_length == 0) {
    return;
  }

  usize half_left = left_length / 2;
  usize half_right = right_length / 2;
  CBTRACE(MERGE_SORT_CHANNEL, "sorting: [%zu..%zu]", left_index,
          left_index + left_length - 1);
  sort_and_merge_lists(original, temp_array, left_index, half_left,
                       left_index + half_left, left_length - half_left);
  CBTRACE(MERGE_SORT_CHANNEL, "sorting: [%zu..%zu]", right_index,
          right_index + right_length - 1);
  sort_and_merge_lists(original, temp_array, right_index, half_right,
                       right_index + half_right, right_length - half_right);

  CBTRACE(MERGE_SORT_CHANNEL, "merging: [%zu..%zu] and [%zu..%zu]", left_index,
          left_index + left_length - 1, right_index,
          right_index + right_length - 1);

  usize l = left_index;
  usize r = right_index;
  usize curr = left_index;
  while (l < left_index + left_length && r < right_index + right_length) {
    if (original[l] > original[r]) {
      temp_array[curr] = original[r];
      r++;
    } else {
      temp_array[curr] = original[l];
      l++;
    }

    curr++;
  }

  while (l < left_index + left_length) {
    temp_array[curr] = original[l];
    curr++;
    l++;
  }

  while (r < right_index + right_length) {
    temp_array[curr] = original[r];
    curr++;
    r++;
  }

  memcpy(original + left_index, temp_array + left_index,
         sizeof(i32) * (left_length + right_length));

  return;
}

bool merge_sort_i32(i32 *data, usize len) {
  assert(data != nullptr || len == 0);

  if (len < 2) {
    return true;
  }

  if (len > (USIZE_MAX / sizeof(i32))) {
    CBLOG(LL_ERROR, "len too long for usize_max");
    return false;
  }

  i32 *temp_array = malloc(sizeof(i32) * len);
  if (temp_array == nullptr) {
    CBLOG(LL_ERROR, "failed to allocate temp memory for merge sort");
    return false;
  }

  usize half_len = len / 2;
  sort_and_merge_lists(data, temp_array, 0, half_len, half_len, len - half_len);

  free(temp_array);
  return true;
}
