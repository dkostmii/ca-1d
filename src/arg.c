#include "include/arg.h"
#include "include/ca.h"
#include "include/math_op.h"
#include "include/params.h"
#include "include/rnd.h"
#include "include/stdlib_ext.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
parse_range (char *arg_value, int *min, int *max)
{
  if (arg_value == NULL)
    {
      return -1;
    }

  int arg_value_len = strlen (arg_value);

  if (arg_value_len < 1)
    {
      return -1;
    }

  if (max == NULL || min == NULL)
    {
      return -1;
    }

  char *range_delim = strchr (arg_value, '-');

  if (range_delim == NULL)
    {
      return -1;
    }

  int range_delim_id = range_delim - arg_value;

  int min_pos = 0;
  int min_len = range_delim_id;

  int max_pos = range_delim_id + 1;
  int max_len = arg_value_len - max_pos;

  if (min_len < 1 || max_len < 1)
    {
      return -1;
    }

  char *min_str = (char *)malloc ((min_len + 1) * sizeof (char));

  if (min_str == NULL)
    {
      return -1;
    }

  char *max_str = (char *)malloc ((max_len + 1) * sizeof (char));

  if (max_str == NULL)
    {
      free (min_str);
      return -1;
    }

  strncpy (min_str, arg_value + min_pos, min_len);
  strncpy (max_str, arg_value + max_pos, max_len);
  min_str[min_len] = '\0';
  max_str[max_len] = '\0';

  if (strcmp (min_str, "0") == 0)
    {
      *min = 0;
    }

  if (strcmp (max_str, "0") == 0)
    {
      *max = 0;
    }

  int atoi_min_res = atoi (min_str);

  if (atoi_min_res == 0)
    {
      free (min_str);
      free (max_str);
      return -1;
    }

  int atoi_max_res = atoi (max_str);

  if (atoi_max_res == 0)
    {
      free (min_str);
      free (max_str);
      return -1;
    }

  *min = atoi_min_res;
  *max = atoi_max_res;

  return 0;
}

int
parse_int_arg (char *arg_value, int *result, int min_value, int max_value)
{
  if (arg_value == NULL)
    {
      return -1;
    }

  if (result == NULL)
    {
      return -1;
    }

  if (strcmp (arg_value, "r") == 0)
    {
      *result = range (min_value, max_value);
      return 0;
    }

  char *range_delim = strchr (arg_value, '-');

  if (range_delim != NULL)
    {
      int min = 0;
      int max = 0;
      int parse_range_res = parse_range (arg_value, &min, &max);

      if (parse_range_res < 0)
        {
          return -1;
        }

      min = clamp (min, min_value, max_value);
      max = clamp (max, min_value, max_value);

      *result = range (min, max);
      return 0;
    }

  char *choice = NULL;
  int choose_random_result = choose_random (arg_value, &choice);

  if (choose_random_result < 0)
    {
      return -1;
    }

  if (strcmp (choice, "0") == 0)
    {
      free (choice);
      *result = 0;
      return 0;
    }

  int atoi_res = atoi (choice);

  if (atoi_res == 0)
    {
      free (choice);
      return -1;
    }

  if (atoi_res >= min_value && atoi_res <= max_value)
    {
      free (choice);
      *result = atoi_res;
      return 0;
    }

  free (choice);
  return 1;
}

int
parse_arg (char *arg, Params *params)
{
  int arg_len = strlen (arg);

  if (arg_len < 3)
    {
      return -1;
    }

  char *equal_ch = strchr (arg, '=');

  if (equal_ch == NULL)
    {
      return -1;
    }

  int equal_ch_id = equal_ch - arg;

  int name_pos = 2;
  int name_len = equal_ch_id - name_pos;

  if (name_len < 1 || name_len > 16)
    {
      return -1;
    }

  char *name = (char *)malloc ((name_len + 1) * sizeof (char));

  if (name == NULL)
    {
      return -1;
    }

  strncpy (name, arg + name_pos, name_len);
  name[name_len] = '\0';

  int value_pos = equal_ch_id + 1;
  int value_len = arg_len - value_pos;

  if (value_len < 1)
    {
      free (name);
      return -1;
    }

  char *value = (char *)malloc ((value_len + 1) * sizeof (char));
  strncpy (value, arg + value_pos, value_len);
  value[value_len] = '\0';

  if (strcmp (name, "rule") == 0)
    {
      int result = 0;
      int parse_int_arg_res = parse_int_arg (value, &result, 0, 255);

      if (parse_int_arg_res < 0)
        {
          free (name);
          free (value);
          return -1;
        }

      if (parse_int_arg_res > 1)
        {
          free (name);
          free (value);
          return 0;
        }

      params->rule = result;
      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "height") == 0)
    {
      int result = 0;
      int parse_int_arg_res = parse_int_arg (value, &result, 1, 1000);

      if (parse_int_arg_res < 0)
        {
          free (name);
          free (value);
          return -1;
        }

      if (parse_int_arg_res > 1)
        {
          free (name);
          free (value);
          return 0;
        }

      params->height = result;
      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "width") == 0)
    {
      int result = 0;
      int parse_int_arg_res = parse_int_arg (value, &result, 1, 1000);

      if (parse_int_arg_res < 0)
        {
          free (name);
          free (value);
          return -1;
        }

      if (parse_int_arg_res > 1)
        {
          free (name);
          free (value);
          return 0;
        }

      params->width = result;
      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "map_frequency") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      if (strcmp (value, "program") == 0)
        {
          params->map_frequency = MAP_FREQ_PROGRAM;
        }
      else if (strcmp (value, "line") == 0)
        {
          params->map_frequency = MAP_FREQ_LINE;
        }
      else if (strcmp (value, "cell") == 0)
        {
          params->map_frequency = MAP_FREQ_CELL;
        }
      else if (strcmp (value, "r") == 0)
        {
          const int values_len = 3;
          int values[] = { MAP_FREQ_PROGRAM, MAP_FREQ_LINE, MAP_FREQ_CELL };

          int values_id = range (0, values_len - 1);
          params->map_frequency = values[values_id];
        }
      else
        {
          free (name);
          free (value);
          return -1;
        }

      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "map_alive") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      if (params->map_alive != NULL)
        {
          free (params->map_alive);
        }

      params->map_alive = (char *)malloc ((value_len + 1) * sizeof (char));
      strcpy (params->map_alive, value);
      params->map_alive[value_len] = '\0';

      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "map_dead") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      if (params->map_dead != NULL)
        {
          free (params->map_dead);
        }

      params->map_dead = (char *)malloc ((value_len + 1) * sizeof (char));
      strcpy (params->map_dead, value);
      params->map_dead[value_len] = '\0';

      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "stdin_char_alive") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      params->stdin_char_alive = value[0];
      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "stdin_char_dead") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      params->stdin_char_dead = value[0];
      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "seed_mode") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      if (strcmp (value, "pulse") == 0)
        {
          params->seed_mode = SEED_MODE_PULSE;
        }
      else if (strcmp (value, "random") == 0)
        {
          params->seed_mode = SEED_MODE_RANDOM;
        }
      else if (strcmp (value, "r") == 0)
        {
          const int values_len = 2;
          int values[] = { SEED_MODE_PULSE, SEED_MODE_RANDOM };

          int values_id = range (0, values_len - 1);
          params->seed_mode = values[values_id];
        }
      else
        {
          free (name);
          free (value);
          return -1;
        }

      free (name);
      free (value);
      return 0;
    }
  else if (strcmp (name, "seed") == 0)
    {
      if (value_len < 1)
        {
          free (name);
          free (value);
          return -1;
        }

      if (strcmp (value, "0") != 0)
        {
          unsigned int parsed_value = strtoui (value, NULL, 10);

          if (parsed_value == 0)
            {
              free (name);
              free (value);
              return -1;
            }

          params->seed = parsed_value;
        }

      free (name);
      free (value);
      return 0;
    }

  free (value);
  free (name);
  return -1;
}

int
print_usage (char *command, int write_to_stderr)
{
  if (command == NULL)
    {
      return -1;
    }

  if (strlen (command) < 1)
    {
      return -1;
    }

  const int usage_lines_len = 4;
  char *usage_lines[] = {
    "--help",
    "--rule=[0-255] --width=[1-1000] --height=[1-1000] --map_alive=C... "
    "--map_dead=C... --map_frequency=program|line|cell --stdin_char_alive=1 "
    "--stdin_char_dead=0 --seed_mode=pulse|random",
    "--rule=[0-255,;|...] --width=[1-1000,;|...] --height=[1-1000,;|...] "
    "--map_alive=C... --map_dead=C... --map_frequency=program|line|cell "
    "--stdin_char_alive=1 --stdin_char_dead=0 --seed_mode=pulse|random",
    "--rule=r --width=r --height=r --map_alive=C... --map_dead=C... "
    "--map_frequency=r --stdin_char_alive=1 --stdin_char_dead=0 --seed_mode=r"
  };

  printf ("Usage:\n");
  for (int i = 0; i < usage_lines_len; i++)
    {
      char prefix[] = "\t%s ";
      int prefix_len = strlen (prefix);
      char *usage_line = usage_lines[i];
      int usage_line_len = strlen (usage_line);
      int append_suffix = i > 0;

      char *line
          = (char *)malloc ((prefix_len + usage_line_len + 1) * sizeof (char));
      strcat (line, prefix);
      strcat (line, usage_line);

      int line_len = strlen (line);

      if (append_suffix == 1)
        {
          const char suffix[] = " --seed=0|[1-%u]\n";
          char *suffix_line = (char *)realloc (
              line, (line_len + strlen (suffix) + 1) * sizeof (char));

          if (suffix_line == NULL)
            {
              free (line);
              return -1;
            }

          strcat (line, suffix);
          if (write_to_stderr == 1)
            {
              fprintf (stderr, line, command, UINT_MAX);
            }
          else
            {
              printf (line, command, UINT_MAX);
            }
        }
      else
        {
          const char suffix[] = "\n";
          char *suffix_line = (char *)realloc (
              line, (line_len + strlen (suffix) + 1) * sizeof (char));

          if (suffix_line == NULL)
            {
              free (line);
              return -1;
            }

          strcat (line, suffix);
          if (write_to_stderr == 1)
            {
              fprintf (stderr, line, command);
            }
          else
            {
              printf (line, command);
            }
        }
      free (line);
    }

  return 0;
}

int
parse_args (int argc, char **argv, Params *params)
{
  int seed_param_id = -1;

  for (int i = 1; i < argc; i++)
    {
      int write_to_stderr = 0;
      if (strncmp (argv[i], "--seed", strlen ("--seed")) == 0)
        {
          seed_param_id = i;
          int parse_arg_res = parse_arg (argv[i], params);

          if (parse_arg_res < 0)
            {
              fprintf (stderr, "Invalid arg: %s\n", argv[i]);
              write_to_stderr = 1;
              int print_usage_res = print_usage (argv[0], write_to_stderr);
              if (print_usage_res < 0)
                {
                  return -1;
                }

              return -1;
            }
        }
    }

  // FIXME: This should be separated from arg logic
  srand (params->seed);

  for (int i = 1; i < argc; i++)
    {
      if (seed_param_id != -1 && i == seed_param_id)
        {
          continue;
        }

      int write_to_stderr = 0;
      if (strncmp (argv[i], "--help", strlen ("--help")) == 0)
        {
          int print_usage_res = print_usage (argv[0], write_to_stderr);

          if (print_usage_res < 0)
            {
              return -1;
            }

          return 1;
        }

      int parse_arg_res = parse_arg (argv[i], params);

      if (parse_arg_res < 0)
        {
          fprintf (stderr, "Invalid arg: %s\n", argv[i]);
          write_to_stderr = 1;
          int print_usage_res = print_usage (argv[0], write_to_stderr);

          if (print_usage_res < 0)
            {
              return -1;
            }

          return -1;
        }
    }

  return 0;
}
