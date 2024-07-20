#ifndef CA_H_INCLUDED
#define CA_H_INCLUDED

#define DEFAULT_RULE 90
#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 11
#define CHAR_ALIVE '1'
#define CHAR_DEAD '0'

int next_cell_state (int *neighbourhood, int rule);
int get_neighbourhood (int *state, int state_len, int pos,
                       int **neighbourhood);
int next_state (int *state, int state_len, int rule, int **next);
int seed_state_pulse (int **state, int state_len);

#endif
