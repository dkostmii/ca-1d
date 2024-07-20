#include "include/ca_io.h"
#include "include/ca.h"
#include "include/math_op.h"
#include "include/params.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
print_state (int *state, Params params)
{
  if (state == NULL)
    {
      return -1;
    }

  const int map_len = 2;
  char map[map_len + 1];
  int id = 0;

  map[0] = CHAR_DEAD;
  map[1] = CHAR_ALIVE;
  map[2] = '\0';

  for (int i = 0; i < params.width; i++)
    {
      int value = state[i];
      int map_id = mod (value, map_len);
      char mapped = map[map_id];
      printf ("%c", mapped);
    }

  printf ("\n");

  return 0;
}
