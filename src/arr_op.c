#include <stddef.h>
#include "include/arr_op.h"

int reverse_in_place(int *arr, int arr_len) {
  if (arr == NULL) {
    return -1;
  }

	if (arr_len < 2) {
		return -1;
	}

	int half = arr_len / 2;

  for (int i = 0; i < half; i++) {
    int rev_i = arr_len - i - 1;
    int temp = *(arr + i);
    *(arr + i) = *(arr + rev_i);
    *(arr + rev_i) = temp;
  }

	return 0;
}
