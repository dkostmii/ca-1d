#include "params.h"
#ifndef ARG_H_INCLUDED
#define ARG_H_INCLUDED
int parse_int_arg (char *arg_value, int *result, int min_value, int max_value);
int parse_arg (char *arg, Params *params);
int print_usage (char *command, int write_to_stderr);
int parse_args (int argc, char **argv, Params *params);
#endif
