#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

typedef struct _Params
{
  int rule;
  int height;
  int width;
  char map_alive;
  char map_dead;
  char stdin_char_alive;
  char stdin_char_dead;
  int seed_mode;
  unsigned int seed;
} Params;

int create_params (Params **result);
int destroy_params (Params *params);
int checkout_params (Params *params);

#endif
