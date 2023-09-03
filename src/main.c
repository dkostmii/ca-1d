#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "include/ca.h"
#include "include/ca_io.h"
#include "include/params.h"
#include "include/arg.h"

int create_params(Params** result) {
	if (result == NULL) {
		return -1;
	}

	if (*result != NULL) {
		return -1;
	}

	*result = (Params*) malloc(sizeof(Params));

	if (result == NULL) {
		return -1;
	}

	(*result)->rule = 90;
	(*result)->height = 10;
	(*result)->width = 11;
	(*result)->map_frequency = MAP_FREQ_PROGRAM;

	(*result)->map_alive = (char*) malloc(2 * sizeof(char));

	if ((*result)->map_alive == NULL) {
		free(result);
		return -1;
	}

	(*result)->map_dead = (char*) malloc(2 * sizeof(char));

	if ((*result)->map_dead == NULL) {
		free((*result)->map_dead);
		free(*result);
		return -1;
	}

	strcpy((*result)->map_alive, "X");
	strcpy((*result)->map_dead, " ");
	(*result)->map_alive[1] = '\0';
	(*result)->map_dead[1] = '\0';

	return 0;
}

int destroy_params(Params* params) {
	if (params == NULL) {
		return -1;
	}

	if (params->map_alive != NULL) {
		free(params->map_alive);
	}

	if (params->map_dead != NULL) {
		free(params->map_dead);
	}

	free(params);
	return 0;
}

int main(int argc, char** argv) {
	Params* params = NULL;
	create_params(&params);

	if (argc > 0) {
		srand(time(NULL));
		int parse_res = parse_args(argc, argv, params);

		if (parse_res < 0) {
			destroy_params(params);
			return -1;
		}

		if (parse_res == 1) {
			destroy_params(params);
			return 0;
		}
	}

	int checkout_params_res = checkout_params(params);

	if (checkout_params < 0) {
		destroy_params(params);
		return -1;
	}

	int** states = (int**) malloc(params->height * sizeof(int*));
	states[0] = (int*) malloc(params->width * sizeof(int));

	if (states[0] == NULL) {
		free(states);
		destroy_params(params);
		return -1;
	}

	int seed_res = seed_state(&states[0], params->width);

	if (seed_res < 0 && states[0] != NULL) {
		free(states[0]);
	}

	if (seed_res < 0) {
		free(states);
		destroy_params(params);
		return -1;
	}

	for (int i = 1; i < params->height; i++) {
		int* next_state_arr = NULL;
		int next_state_err = next_state(states[i - 1], params->width, params->rule, &next_state_arr);

		if (next_state_err < 0 && next_state_arr != NULL) {
			free(next_state_arr);
		}

		if (next_state_err < 0) {
			for (int j = 0; j < i; j++) {
				free(states[j]);
			}
			free(states);
			destroy_params(params);
		}

		states[i] = next_state_arr;
	}

	int print_states_res = print_states(states, *params);

	if (print_states_res < 0) {
		for (int i = 0; i < params->height; i++) {
			free(states[i]);
		}
		free(states);
		destroy_params(params);
		return -1;
	}

	for (int i = 0; i < params->height; i++) {
		free(states[i]);
	}
	free(states);
	destroy_params(params);

	return 0;
}
