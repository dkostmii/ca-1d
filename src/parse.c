#include "include/parse.h"
#include "include/ca.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
parse_state_str (char *state_str, int **result, int expected_result_width)
{
  if (state_str == NULL)
    {
      return -1;
    }

  if (result == NULL)
    {
      return -1;
    }

  if (*result != NULL)
    {
      return -1;
    }

  int state_str_len = strlen (state_str);

  if (state_str_len < 1)
    {
      return -1;
    }

  if (isspace (state_str[0]))
    {
      return 1;
    }

  if (expected_result_width < 1)
    {
      return -1;
    }

  *result = (int *)malloc (expected_result_width * sizeof (int));

  if (*result == NULL)
    {
      return -1;
    }

  for (int i = 0; i < expected_result_width; i++)
    {
      if (i > state_str_len - 1)
        {
          *(*result + i) = 0;
        }
      else
        {
          char cell_ch = state_str[i];

          if (cell_ch == CHAR_ALIVE)
            {
              *(*result + i) = 1;
            }
          else if (cell_ch == CHAR_DEAD)
            {
              *(*result + i) = 0;
            }
          else if (isspace (cell_ch) && i == state_str_len - 1)
            {
              *(*result + i) = 0;
              continue;
            }
          else
            {
              fprintf (stderr, "Invalid character at position %d: %c\n", i,
                       cell_ch);
              free (*result);
              return -1;
            }
        }
    }

  return 0;
}
