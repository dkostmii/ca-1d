#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/math_op.h"
#include "include/params.h"
#include "include/ca_io.h"
#include "include/rnd.h"

int print_state(int* state, Params params) {
	if (state == NULL) {
		return -1;
	}

	if (params.map_alive == NULL) {
		return -1;
	}

	if (params.map_dead == NULL) {
		return -1;
	}

	int map_alive_len = strlen(params.map_alive);
	int map_dead_len = strlen(params.map_dead);

	if (map_alive_len != map_dead_len) {
		return -1;
	}

	const int map_len = 2;
	char map[map_len + 1];
	int id = 0;
	if (map_alive_len > 1) {
		id = range(0, map_alive_len - 1);
	}

	map[0] = params.map_dead[id];
	map[1] = params.map_alive[id];
	map[2] = '\0';

	for (int i = 0; i < params.width; i++) {
		if (i != 0 && map_alive_len > 1 && params.map_frequency == MAP_FREQ_CELL) {
			id = range(0, map_alive_len - 1);

			map[0] = params.map_alive[id];
			map[1] = params.map_dead[id];
		}

		int value = state[i];
		int map_id = mod(value, map_len);
		char mapped = map[map_id];
		printf("%c", mapped);
	}

	printf("\n");

	return 0;
}

int print_states(int** states, Params params) {
	if (states == NULL) {
		return -1;
	}

	if (params.map_alive == NULL) {
		return -1;
	}

	if (params.map_dead == NULL) {
		return -1;
	}

	int map_alive_len = strlen(params.map_alive);
	int map_dead_len = strlen(params.map_dead);

	if (map_alive_len != map_dead_len) {
		return -1;
	}

	if (params.map_frequency == MAP_FREQ_LINE) {
		Params params_line = params;
		params_line.map_alive = NULL;
		params_line.map_dead = NULL;
		for (int i = 0; i < params.height; i++) {
			int id = range(0, map_alive_len - 1);

			if (params_line.map_alive == NULL) {
				params_line.map_alive = (char*) malloc(2 * sizeof(char));
			}

			if (params_line.map_dead == NULL) {
				params_line.map_dead = (char*) malloc(2 * sizeof(char));
			}


			if (params_line.map_alive == NULL) {
				return -1;
			}

			if (params_line.map_dead == NULL) {
				free(params_line.map_alive);
				return -1;
			}

			params_line.map_alive[0] = params.map_alive[id];
			params_line.map_dead[0] = params.map_dead[id];
			params_line.map_alive[1] = '\0';
			params_line.map_dead[1] = '\0';

			int print_state_res = print_state(states[i], params_line);

			if (print_state_res < 0) {
				free(params_line.map_alive);
				free(params_line.map_dead);
				return -1;
			}

		}
	} else {
		Params params_other = params;
		if (params.map_frequency == MAP_FREQ_PROGRAM) {
			params_other.map_alive = (char*) malloc(2 * sizeof(char));

			if (params_other.map_alive == NULL) {
				return -1;
			}

			params_other.map_dead = (char*) malloc(2 * sizeof(char));

			if (params_other.map_dead == NULL) {
				free(params_other.map_alive);
				return -1;
			}

			int id = range(0, map_alive_len - 1);

			params_other.map_alive[0] = params.map_alive[id];
			params_other.map_dead[0] = params.map_dead[id];
			params_other.map_alive[1] = '\0';
			params_other.map_dead[1] = '\0';
		}
		for (int i = 0; i < params.height; i++) {
			int print_state_res = print_state(states[i], params_other);

			if (print_state_res < 0) {
				if (params.map_frequency == MAP_FREQ_PROGRAM) {
					free(params_other.map_alive);
					free(params_other.map_dead);
				}

				return -1;
			}
		}

		if (params.map_frequency == MAP_FREQ_PROGRAM) {
			free(params_other.map_alive);
			free(params_other.map_dead);
		}
	}

	return 0;
}
