#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "include/rnd.h"
#include "include/math_op.h"

int choose_random(char* arg_value, char** result) {
	if (arg_value == NULL) {
		return -1;
	}

	int value_len = strlen(arg_value);

	if (value_len < 1) {
		return -1;
	}

	if (result == NULL || *result != NULL) {
		return -1;
	}

	char* value_token = strtok(arg_value, ",;|");

	if (value_token == NULL) {
		return -1;
	}

	int tokens_len = 1;
	char** tokens = (char**) malloc(sizeof(char*));
	int value_token_len = strlen(value_token);
	tokens[0] = (char*) malloc((value_token_len + 1) * sizeof(char));
	strcpy(tokens[0], value_token);
	tokens[0][value_token_len] = '\0';

	while (value_token != NULL) {
		value_token = strtok(NULL, ",;|");
		if (value_token != NULL) {
			tokens_len++;
			char** next_tokens = (char**) realloc(tokens, tokens_len * sizeof(char*));
			if (next_tokens == NULL) {
				for (int i = 0; i < tokens_len - 1; i++) {
					free(tokens[i]);
				}

				free(tokens);
				return -1;
			}
			tokens = next_tokens;

			int last_id = tokens_len - 1;
			int value_token_len = strlen(value_token);
			tokens[last_id] = (char*) malloc((value_token_len + 1) * sizeof(char));
			strcpy(tokens[last_id], value_token);
			tokens[last_id][value_token_len] = '\0';
		}
	}

	int rnd_id = range(0, tokens_len - 1);
	char* rnd_result = tokens[rnd_id];
	int rnd_result_len = strlen(rnd_result);

	*result = (char*) malloc((rnd_result_len + 1) * sizeof(char));
	strcpy(*result, rnd_result);
	(*result)[rnd_result_len] = '\0';

	for (int i = 0; i < tokens_len; i++) {
		free(tokens[i]);
	}

	free(tokens);

	return 0;
}

int range(int min, int max) {
	return cycle(rand(), min, max);
}
