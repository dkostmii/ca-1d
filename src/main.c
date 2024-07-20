#include "include/arg.h"
#include "include/ca.h"
#include "include/ca_io.h"
#include "include/params.h"
#include "include/parse.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
  Params *params = NULL;
  create_params (&params);

  if (argc > 1)
    {
      int parse_res = parse_args (argc, argv, params);

      if (parse_res < 0)
        {
          destroy_params (params);
          return -1;
        }

      if (parse_res == 1)
        {
          destroy_params (params);
          return 0;
        }
    }

  int checkout_params_res = checkout_params (params);

  if (checkout_params < 0)
    {
      destroy_params (params);
      return -1;
    }

  int *initial_state = NULL;

  int stdin_fileno = fileno (stdin);
  int is_atty_res = isatty (stdin_fileno) == 0;
  struct stat stats;

  int fstat_res = fstat (stdin_fileno, &stats);

  if (fstat_res < 0)
    {
      destroy_params (params);
      return -1;
    }

  int has_input = !S_ISCHR (stats.st_mode);
  int mode_is_valid
      = !has_input || S_ISREG (stats.st_mode) || S_ISFIFO (stats.st_mode);

  if (mode_is_valid == 0)
    {
      destroy_params (params);
      return -1;
    }

  int parser_empty = 0;

  if (is_atty_res && has_input)
    {
      char *input_pipe = (char *)malloc ((params->width + 1) * sizeof (char));

      if (input_pipe == NULL)
        {
          destroy_params (params);
          return -1;
        }

      char *fgets_res = fgets (input_pipe, params->width + 1, stdin);

      if (fgets_res == NULL)
        {
          free (input_pipe);
          destroy_params (params);
          return -1;
        }

      int *stdin_seed_state = NULL;

      int parse_state_str_res
          = parse_state_str (input_pipe, &stdin_seed_state, params->width);

      if (parse_state_str_res < 0)
        {
          free (input_pipe);
          destroy_params (params);
          return -1;
        }

      if (parse_state_str_res == 1)
        {
          parser_empty = 1;
        }

      if (parser_empty == 0)
        {
          if (initial_state != NULL)
            {
              free (initial_state);
            }

          initial_state = stdin_seed_state;
        }

      free (input_pipe);
    }

  if (is_atty_res == 0 || has_input == 0 || parser_empty == 1)
    {
      initial_state = (int *)malloc (params->width * sizeof (int));

      if (initial_state == NULL)
        {
          destroy_params (params);
          return -1;
        }

      int seed_res = seed_state_pulse (&initial_state, params->width);

      if (seed_res < 0 && initial_state != NULL)
        {
          free (initial_state);
        }

      if (seed_res < 0)
        {
          destroy_params (params);
          return -1;
        }
    }

  int print_state_res = print_state (initial_state, *params);
  if (print_state_res < 0)
    {
      free (initial_state);
      destroy_params (params);
      return -1;
    }

  int *prev_state = initial_state;

  for (int i = 1; (i < params->height || params->height == 0); i++)
    {
      int *next_state_arr = NULL;
      int next_state_err = next_state (prev_state, params->width, params->rule,
                                       &next_state_arr);

      if (next_state_err < 0 && next_state_arr != NULL)
        {
          free (next_state_arr);
        }

      if (next_state_err < 0 && prev_state != NULL)
        {
          free (prev_state);
        }

      if (next_state_err < 0)
        {
          free (initial_state);
          destroy_params (params);
          return -1;
        }

      print_state_res = print_state (next_state_arr, *params);
      if (print_state_res < 0)
        {
          free (initial_state);
          destroy_params (params);
          return -1;
        }

      if (prev_state != NULL)
        {
          free (prev_state);
        }

      prev_state = next_state_arr;
    }

  if (prev_state != NULL)
    {
      free (prev_state);
    }

  destroy_params (params);

  return 0;
}
