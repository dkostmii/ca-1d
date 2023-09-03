#ifndef CA_H_INCLUDED
#define CA_H_INCLUDED

int next_cell_state(int* neighbourhood, int rule);
int get_neighbourhood(int* state, int state_len, int pos, int** neighbourhood);
int next_state(int* state, int state_len, int rule, int** next);
int seed_state(int** state, int state_len);

#endif
