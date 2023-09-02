#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "include/arg.h"
#include "include/math_op.h"
#include "include/rnd.h"

int parse_range(char* arg_value, int* min, int* max) {
	if (arg_value == NULL) {
		return -1;
	}

	int arg_value_len = strlen(arg_value);

	if (arg_value_len < 1) {
		return -1;
	}

	if (max == NULL || min == NULL) {
		return -1;
	}

	char* range_delim = strchr(arg_value, '-');

	if (range_delim == NULL) {
		return -1;
	}

	int range_delim_id = range_delim - arg_value;

	int min_pos = 0;
	int min_len = range_delim_id;

	int max_pos = range_delim_id + 1;
	int max_len = arg_value_len - max_pos;

	if (min_len < 1 || max_len < 1) {
		return -1;
	}

	char* min_str = (char*) malloc(min_len * sizeof(char));

	if (min_str == NULL) {
		return -1;
	}

	char* max_str = (char*) malloc(max_len * sizeof(char));

	if (max_str == NULL) {
		free(min_str);
		return -1;
	}

	strncpy(min_str, arg_value+min_pos, min_len);
	strncpy(max_str, arg_value+max_pos, max_len);
	min_str[min_len] = '\0';
	max_str[max_len] = '\0';

	if (strcmp(min_str, "0") == 0) {
		*min = 0;
	}

	if (strcmp(max_str, "0") == 0) {
		*max = 0;
	}

	int atoi_min_res = atoi(min_str);

	if (atoi_min_res == 0) {
		free(min_str);
		free(max_str);
		return -1;
	}

	int atoi_max_res = atoi(max_str);

	if (atoi_max_res == 0) {
		free(min_str);
		free(max_str);
		return -1;
	}

	*min = atoi_min_res;
	*max = atoi_max_res;

	return 0;
}

int parse_int_arg(char* arg_value, int* result, int min_value, int max_value) {
	if (arg_value == NULL) {
		return -1;
	}

	if (result == NULL) {
		return -1;
	}

	if (strcmp(arg_value, "r") == 0) {
		*result = range(min_value, max_value);
		return 0;
	}

	char* range_delim = strchr(arg_value, '-');

	if (range_delim != NULL) {
		int min = 0;
		int max = 0;
		int parse_range_res = parse_range(arg_value, &min, &max);

		if (parse_range_res < 0) {
			return -1;
		}

		min = clamp(min, min_value, max_value);
		max = clamp(max, min_value, max_value);

		*result = range(min, max);
		return 0;
	}

	char* choice = NULL;
	int choose_random_result = choose_random(arg_value, &choice);

	if (choose_random_result < 0) {
		return -1;
	}

	if (strcmp(choice, "0") == 0) {
		free(choice);
		*result = 0;
		return 0;
	}

	int atoi_res = atoi(choice);

	if (atoi_res == 0) {
		free(choice);
		return -1;
	}

	if (atoi_res >= min_value && atoi_res <= max_value) {
		free(choice);
		*result = atoi_res;
		return 0;
	}

	free(choice);
	return 1;
}

int parse_arg(char* arg, Params* params) {
	int arg_len = strlen(arg);

	if (arg_len < 3) {
		return -1;
	}

	char* equal_ch = strchr(arg, '=');

	if (equal_ch == NULL) {
		return -1;
	}

	int equal_ch_id = equal_ch - arg;

	int name_pos = 2;
	int name_len = equal_ch_id - name_pos;

	if (name_len < 1 || name_len > 16) {
		return -1;
	}

	char* name = (char*) malloc(name_len * sizeof(char));

	if (name == NULL) {
		return -1;
	}

	strncpy(name, arg+name_pos, name_len);
	name[name_len] = '\0';

	int value_pos = equal_ch_id + 1;
	int value_len = arg_len - value_pos;

	if (value_len < 1) {
		free(name);
		return -1;
	}

	char* value = (char*) malloc(value_len * sizeof(char));
	strncpy(value, arg+value_pos, value_len);
	value[value_len] = '\0';

	if (strcmp(name, "rule") == 0) {
		int result = 0;
		int parse_int_arg_res = parse_int_arg(value, &result, 0, 255);

		if (parse_int_arg_res < 0) {
			free(name);
			free(value);
			return -1;
		}

		if (parse_int_arg_res > 1) {
			free(name);
			free(value);
			return 0;
		}

		params->rule = result;
		free(name);
		free(value);
		return 0;
	} else if (strcmp(name, "height") == 0) {
		int result = 0;
		int parse_int_arg_res = parse_int_arg(value, &result, 1, 1000);

		if (parse_int_arg_res < 0) {
			free(name);
			free(value);
			return -1;
		}

		if (parse_int_arg_res > 1) {
			free(name);
			free(value);
			return 0;
		}

		params->height = result;
		free(name);
		free(value);
		return 0;
	} else if (strcmp(name, "width") == 0) {
		int result = 0;
		int parse_int_arg_res = parse_int_arg(value, &result, 1, 1000);

		if (parse_int_arg_res < 0) {
			free(name);
			free(value);
			return -1;
		}

		if (parse_int_arg_res > 1) {
			free(name);
			free(value);
			return 0;
		}

		params->width = result;
		free(name);
		free(value);
		return 0;
	} else if (strcmp(name, "map") == 0) {
		if (value_len < 2) {
			free(name);
			free(value);
			return -1;
		}

		if (value_len > 2) {
			int first_id = range(0, value_len - 1);
			int second_id = range(0, value_len - 1);
			while (second_id == first_id) {
				second_id = range(0, value_len - 1);
			}

			params->map[0] = value[first_id];
			params->map[1] = value[second_id];

			free(name);
			free(value);
			return 0;
		}

		strncpy(params->map, value, 2);
		free(name);
		free(value);
		return 0;
	}

	free(value);
	free(name);
	return -1;
}

int parse_args(int argc, char** argv, Params* params) {
	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--help", strlen("--help")) == 0) {
			printf("Usage:\n");
			printf("\t%s --help\n", argv[0]);
			printf("\t%s --rule=[0-255] --width=[1-1000] --height=[1-1000] --map=CC...\n", argv[0]);
			printf("\t%s --rule=[0-255,;|...] --width=[1-1000,;|...] --height=[1-1000,;|...] --map=CC...\n", argv[0]);
			printf("\t%s --rule=r --width=r --height=r --map=CC...\n", argv[0]);
			return 1;
		}

		int parse_arg_res = parse_arg(argv[i], params);

		if (parse_arg_res < 0) {
			fprintf(stderr, "Invalid arg: %s\n", argv[i]);
			fprintf(stderr, "Usage:\n");
			fprintf(stderr, "\t%s --help\n", argv[0]);
			fprintf(stderr, "\t%s --rule=[0-255] --width=[1-1000] --height=[1-1000] --map=CC...\n", argv[0]);
			fprintf(stderr, "\t%s --rule=[0-255,;|...] --width=[1-1000,;|...] --height=[1-1000,;|...] --map=CC...\n", argv[0]);
			fprintf(stderr, "\t%s --rule=r --width=r --height=r --map=CC...\n", argv[0]);
			return -1;
		}
	}

	return 0;
}
