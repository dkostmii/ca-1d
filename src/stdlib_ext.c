#include <stdlib.h>
#include <string.h>
#include "include/stdlib_ext.h"

unsigned int strtoui(const char* str, char** endptr, int base) {
	int str_len = strlen(str);

	if (str_len < 1) {
		return 0;
	}

	char* str_copy = malloc((str_len + 1) * sizeof(char));

	if (str_copy == NULL) {
		return 0;
	}

	strcpy(str_copy, str);

	char *p;

	for (p = str_copy; *p != '\0'; p++) {
		if (*p > '9' || *p < '0') {
			return 0;
		}
	}

	free(str_copy);

	unsigned int result = (unsigned int)strtoul(str, endptr, base);
	return result;
}
