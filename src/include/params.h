#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

typedef struct _Params
{
  int rule;
  int height;
  int width;
} Params;

int create_params (Params **result);
int destroy_params (Params *params);
int checkout_params (Params *params);

#endif
