#include "include/params.h"
#include "include/ca.h"
#include "include/rnd.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MAP_FREQ_PROGRAM = 0;
const int MAP_FREQ_LINE = 1;
const int MAP_FREQ_CELL = 2;

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
  (*result)->map_frequency = MAP_FREQ_PROGRAM;

  (*result)->map_alive = (char *)malloc (2 * sizeof (char));

  if ((*result)->map_alive == NULL)
    {
      free (result);
      return -1;
    }

  (*result)->map_dead = (char *)malloc (2 * sizeof (char));

  if ((*result)->map_dead == NULL)
    {
      free ((*result)->map_dead);
      free (*result);
      return -1;
    }

  strcpy ((*result)->map_alive, "X");
  strcpy ((*result)->map_dead, " ");
  (*result)->map_alive[1] = '\0';
  (*result)->map_dead[1] = '\0';

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

  if (params->map_alive != NULL)
    {
      free (params->map_alive);
    }

  if (params->map_dead != NULL)
    {
      free (params->map_dead);
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

  if (params->map_alive == NULL)
    {
      return -1;
    }

  if (params->map_dead == NULL)
    {
      return -1;
    }

  int map_alive_len = strlen (params->map_alive);
  int map_dead_len = strlen (params->map_dead);

  if (map_alive_len < 1)
    {
      return -1;
    }

  if (map_dead_len < 1)
    {
      return -1;
    }

  int shortest_len
      = map_alive_len < map_dead_len ? map_alive_len : map_dead_len;
  char *map_alive_trunc = (char *)malloc ((shortest_len + 1) * sizeof (char));

  if (map_alive_trunc == NULL)
    {
      return -1;
    }

  char *map_dead_trunc = (char *)malloc ((shortest_len + 1) * sizeof (char));

  if (map_dead_trunc == NULL)
    {
      free (map_alive_trunc);
      return -1;
    }

  strncpy (map_alive_trunc, params->map_alive, shortest_len);
  strncpy (map_dead_trunc, params->map_dead, shortest_len);
  map_alive_trunc[shortest_len] = '\0';
  map_dead_trunc[shortest_len] = '\0';

  free (params->map_alive);
  free (params->map_dead);

  params->map_alive = map_alive_trunc;
  params->map_dead = map_dead_trunc;

  return 0;
}
