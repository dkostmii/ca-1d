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

  (*result)->rule = DEFAULT_RULE;
  (*result)->height = DEFAULT_HEIGHT;
  (*result)->width = DEFAULT_WIDTH;

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
