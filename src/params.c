#include "include/params.h"
#include "include/ca.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int
create_params (Params **result)
{
  if (result == NULL)
    {
      return -1;
    }

  if (*result != NULL)
    {
      return -1;
    }

  *result = (Params *)malloc (sizeof (Params));

  if (result == NULL)
    {
      return -1;
    }

  (*result)->rule = 90;
  (*result)->height = 10;
  (*result)->width = 11;

  (*result)->map_alive = 'X';
  (*result)->map_dead = ' ';
  (*result)->stdin_char_alive = '1';
  (*result)->stdin_char_dead = '0';

  (*result)->seed_mode = SEED_MODE_PULSE;
  (*result)->seed = time (NULL);

  return 0;
}

int
destroy_params (Params *params)
{
  if (params == NULL)
    {
      return -1;
    }

  free (params);
  return 0;
}

int
checkout_params (Params *params)
{
  if (params == NULL)
    {
      return -1;
    }

  return 0;
}
