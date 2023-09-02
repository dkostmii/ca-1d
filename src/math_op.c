#include <stddef.h>
#include <stdlib.h>
#include "include/arr_op.h"
#include "include/math_op.h"

int abs(int n) {
	if (n < 0) {
		return -n;
	}

	return n;
}

int pow_int(int a, int n) {
	if (a == 0 && n == 0) {
		return -1;
	}

	int result = 1;

	if (n < 1) {
		return result;
	}

	for (int i = 0; i < n; i++) {
		result *= a;
	}

	return result;
}

int mod(int n, int a) {
	return ((n % a) + a) % a;
}

int cycle(int value, int min, int max) {
	if (max < min) {
		int temp = min;
		min = max;
		max = temp;
	}

	return mod(value, max - min + 1) + min;
}

int clamp(int value, int min, int max) {
	if (max < min) {
		int temp = min;
		min = max;
		max = temp;
	}

	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	}

	return value;
}

int n_digits(int value, int radix) {
	if (radix == 0 || radix == 1) {
		return -1;
	}

	value = abs(value);

	int result = 1;
	value /= radix;

	while (value > 0) {
		value /= radix;
		result++;
	}

	return result;
}

int digits(int value, int radix, int** dest, int expected_length) {
	int result_len = n_digits(value, radix);

	if (result_len < 1) {
		return -1;
	}

	if (dest == NULL) {
		return -1;
	}

	if (*dest != NULL) {
		return -1;
	}

	value = abs(value);

	if (expected_length > result_len) {
		result_len = expected_length;
	}

	*dest = (int*) malloc(result_len * sizeof(int));

	if (*dest == NULL) {
		return -1;
	}

	for (int i = 0; i < result_len; i++) {
		*(*dest+i) = mod(value, radix);
		value /= radix;
	}

	int rev_err = reverse_in_place(dest, result_len);
	if (rev_err < 0) {
		return -1;
	}

	return 0;
}

int from_digits(int* source, int source_len, int radix) {
	if (source == NULL) {
		return -1;
	}

	if (source_len < 0) {
		return -1;
	}

	if (radix == 0 || radix == 1) {
		return -1;
	}

	int rev_err = reverse_in_place(&source, source_len);

	if (rev_err < 0) {
		return -1;
	}

	int result = 0;

	for (int i = 0; i < source_len; i++) {
		int digit = mod(source[i], radix);

		int pow_res = pow_int(radix, i);
		if (pow_res < 0) {
			return -1;
		}

		result += digit * pow_res;
	}

	return result;
}
