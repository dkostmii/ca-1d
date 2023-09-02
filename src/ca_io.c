#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "include/math_op.h"
#include "include/ca_io.h"

int print_state(int* state, int state_len, char* map) {
	if (state == NULL) {
		return -1;
	}

	if (state_len < 0) {
		return -1;
	}

	if (map == NULL) {
		return -1;
	}

	int map_len = strlen(map);

	if (map_len < 2) {
		return -1;
	}

	for (int i = 0; i < state_len; i++) {
		int value = state[i];
		int map_id = mod(value, map_len);
		char mapped = map[map_id];
		printf("%c", mapped);
	}

	printf("\n");

	return 0;
}
