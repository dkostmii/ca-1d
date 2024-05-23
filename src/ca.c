#include "include/ca.h"
#include "include/arr_op.h"
#include "include/math_op.h"
#include <stddef.h>
#include <stdlib.h>

const int NEIGHBOURHOOD_LEN = 3;

const int SEED_MODE_PULSE = 0;
const int SEED_MODE_RANDOM = 1;

int
next_cell_state (int *neighbourhood, int rule)
{
  if (neighbourhood == NULL)
    {
      return -1;
    }

  int neighbourhood_state_count = pow_int (2, NEIGHBOURHOOD_LEN);

  if (neighbourhood_state_count < 0)
    {
      return -1;
    }

  int rule_count = pow_int (2, neighbourhood_state_count);

  if (rule_count < 0)
    {
      return -1;
    }

  rule = mod (abs (rule), rule_count);

  int *rule_digits = NULL;
  int digits_err = digits (rule, 2, &rule_digits, neighbourhood_state_count);

  if (digits_err < 0 && rule_digits != NULL)
    {
      free (rule_digits);
    }

  if (digits_err < 0)
    {
      return -1;
    }

  reverse_in_place (rule_digits, neighbourhood_state_count);

  int rule_pos = from_digits (neighbourhood, NEIGHBOURHOOD_LEN, 2);

  if (rule_pos < 0)
    {
      free (rule_digits);
      return -1;
    }

  int result = rule_digits[rule_pos];
  free (rule_digits);

  return result;
}

int
get_neighbourhood (int *state, int state_len, int pos, int **neighbourhood)
{
  if (state == NULL)
    {
      return -1;
    }

  if (state_len < NEIGHBOURHOOD_LEN)
    {
      return -1;
    }

  if (pos < 0 || pos > state_len - 1)
    {
      return -1;
    }

  if (neighbourhood == NULL)
    {
      return -1;
    }

  if (*neighbourhood != NULL)
    {
      return -1;
    }

  int start = pos - 1;
  int end = pos + 1;
  *neighbourhood = (int *)malloc (NEIGHBOURHOOD_LEN * sizeof (int));

  if (*neighbourhood == NULL)
    {
      return -1;
    }

  for (int i = start; i < end + 1; i++)
    {
      int state_index = cycle (i, 0, state_len - 1);
      int state_value = state[state_index];
      int neighbourhood_index = i - pos + 1;
      *(*neighbourhood + neighbourhood_index) = state_value;
    }

  return 0;
}

int
next_state (int *state, int state_len, int rule, int **next)
{
  if (state == NULL)
    {
      return -1;
    }

  if (state_len < 0)
    {
      return -1;
    }

  if (next == NULL)
    {
      return -1;
    }

  if (*next != NULL)
    {
      return -1;
    }
  *next = (int *)malloc (state_len * sizeof (int));

  if (*next == NULL)
    {
      return -1;
    }

  for (int i = 0; i < state_len; i++)
    {
      int *neighbourhood = NULL;

      int neighbourhood_err
          = get_neighbourhood (state, state_len, i, &neighbourhood);

      if (neighbourhood_err < 0 && neighbourhood != NULL)
        {
          free (neighbourhood);
        }

      if (neighbourhood_err < 0)
        {
          return -1;
        }

      int next_value = next_cell_state (neighbourhood, rule);

      if (next_value < 0)
        {
          free (neighbourhood);
          return -1;
        }

      *(*next + i) = next_value;
      free (neighbourhood);
    }

  return 0;
}

int
seed_state (int **state, int state_len, int seed_mode)
{
  if (state == NULL || *state == NULL)
    {
      return -1;
    }

  if (state_len < 1)
    {
      return -1;
    }

  if (seed_mode == SEED_MODE_PULSE)
    {
      for (int i = 0; i < state_len; i++)
        {
          *(*state + i) = 0;
        }

      int half = (state_len - 1) / 2;

      *(*state + half) = 1;
    }
  else if (seed_mode == SEED_MODE_RANDOM)
    {
      int rand_state = rand ();
      int prev_rand_state = 0;
      for (int i = 0; i < state_len; i++)
        {
          *(*state + i) = rand_state & 1;
          prev_rand_state = rand_state;
          rand_state = rand_state >> 1;

          if (rand_state == prev_rand_state)
            {
              rand_state = rand ();
            }
        }
    }
  else
    {
      return -1;
    }

  return 0;
}
