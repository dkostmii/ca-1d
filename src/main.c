#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "include/ca.h"
#include "include/ca_io.h"
#include "include/params.h"
#include "include/arg.h"

int main(int argc, char** argv) {
	Params* params = (Params*) malloc(sizeof(Params));

	params->rule = 90;
	params->height = 10;
	params->width = 11;
	strcpy(params->map, " X");

	if (argc > 0) {
		srand(time(NULL));
		int parse_res = parse_args(argc, argv, params);

		if (parse_res < 0) {
			free(params);
			return -1;
		}

		if (parse_res == 1) {
			free(params);
			return 0;
		}
	}

	int* state = (int*) malloc(params->width * sizeof(int));

	if (state == NULL) {
		free(params);
		return -1;
	}

	for (int i = 0; i < params->width; i++) {
		state[i] = 0;
	}

	state[(params->width - 1) / 2] = 1;

	for (int i = 0; i < params->height; i++) {
		int print_err = print_state(state, params->width, params->map);

		if (print_err < 0) {
			free(state);
			return -1;
		}

		int* next_state_arr = NULL;
		int next_state_err = next_state(state, params->width, params->rule, &next_state_arr);

		if (next_state_err < 0 && next_state_arr != NULL) {
			free(next_state_arr);
		}

		if (next_state_err < 0) {
			free(params);
			free(state);
			return -1;
		}

		free(state);
		state = next_state_arr;
	}

	free(params);
	free(state);

	return 0;
}
