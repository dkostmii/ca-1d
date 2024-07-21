#include "include/arg.h"
#include "include/ca.h"
#include "include/math_op.h"
#include "include/params.h"
#include "include/stdlib_ext.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  if (strncmp (arg_value, "0", strlen ("0")) == 0)
    {
      *result = 0;
      return 0;
    }

  int atoi_res = atoi (arg_value);

  if (atoi_res == 0)
    {
      return -1;
    }

  if (atoi_res >= min_value && atoi_res <= max_value)
    {
      *result = atoi_res;
      return 0;
    }

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

      if (strncmp (value, "inf", strlen ("inf")) != 0)
        {
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

  const int usage_lines_len = 2;
  char *usage_lines[] = {
    "--help",
    "--rule=[0-255] --width=[1-1000] --height=[1-1000]",
  };

  char prefix[] = "\t%s ";
  int prefix_len = strlen (prefix);

  printf ("Usage:\n");
  for (int i = 0; i < usage_lines_len; i++)
    {
      char *usage_line = usage_lines[i];
      int usage_line_len = strlen (usage_line);

      char *line;
      char *suffix = "\n";

      line = (char *)malloc (
          (prefix_len + usage_line_len + strlen (suffix) + 1) * sizeof (char));

      if (line == NULL)
        {
          return -1;
        }

      strcpy (line, prefix);
      strcat (line, usage_line);
      strcat (line, suffix);

      if (write_to_stderr == 1)
        {
          fprintf (stderr, line, command);
        }
      else
        {
          printf (line, command);
        }

      free (line);
    }

  return 0;
}

int
parse_args (int argc, char **argv, Params *params)
{
  for (int i = 1; i < argc; i++)
    {
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
