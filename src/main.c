#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "include/ca.h"
#include "include/ca_io.h"
#include "include/params.h"
#include "include/arg.h"
#include "include/parse.h"

int main(int argc, char** argv) {
	Params* params = NULL;
	create_params(&params);

	if (argc > 1) {
		// NOTE: srand(params->seed) call insied parse_args
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

	int stdin_fileno = fileno(stdin);
	int is_atty_res = isatty(stdin_fileno) == 0;
	struct stat stats;

	int fstat_res = fstat(stdin_fileno, &stats);

	if (fstat_res < 0) {
		free(states);
		destroy_params(params);
		return -1;
	}

	int has_input = !S_ISCHR(stats.st_mode);
	int mode_is_valid = !has_input || S_ISREG(stats.st_mode) || S_ISFIFO(stats.st_mode);

	if (mode_is_valid == 0) {
		free(states);
		destroy_params(params);
		return -1;
	}

	int parser_empty = 0;

	if (is_atty_res && has_input) {
		char* input_pipe = (char*) malloc((params->width + 1) * sizeof(char));

		if (input_pipe == NULL) {
			free(states);
			destroy_params(params);
			return -1;
		}

		char* fgets_res = fgets(input_pipe, params->width + 1, stdin);

		if (fgets_res == NULL) {
			free(input_pipe);
			free(states);
			destroy_params(params);
			return -1;
		}

		int* seed_state = NULL;

		int parse_state_str_res = parse_state_str(
			input_pipe,
			&seed_state,
			params->width,
			params->stdin_char_alive,
			params->stdin_char_dead
		);

		if (parse_state_str_res < 0) {
			free(input_pipe);
			free(states);
			destroy_params(params);
			return -1;
		}

		if (parse_state_str_res == 1) {
			parser_empty = 1;
		}

		if (parser_empty == 0) {
			if (states[0] != NULL) {
				free(states[0]);
			}

			states[0] = seed_state;
		}

		free(input_pipe);
	}

	if (is_atty_res == 0 || has_input == 0 || parser_empty == 1) {
		states[0] = (int*) malloc(params->width * sizeof(int));

		if (states[0] == NULL) {
			free(states);
			destroy_params(params);
			return -1;
		}

		int seed_res = seed_state(&states[0], params->width, params->seed_mode);

		if (seed_res < 0 && states[0] != NULL) {
			free(states[0]);
		}

		if (seed_res < 0) {
			free(states);
			destroy_params(params);
			return -1;
		}
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
