#ifndef MATH_OP_H_INCLUDED
#define MATH_OP_H_INCLUDED

int abs (int n);
int pow_int (int a, int n);
int mod (int n, int a);
int cycle (int value, int min, int max);
int clamp (int value, int min, int max);
int n_digits (int value, int radix);
int digits (int value, int radix, int **dest, int expected_length);
int from_digits (int *source, int source_len, int radix);

#endif
