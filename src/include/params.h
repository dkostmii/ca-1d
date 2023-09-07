#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

extern const int MAP_FREQ_PROGRAM;
extern const int MAP_FREQ_LINE;
extern const int MAP_FREQ_CELL;

typedef struct _Params {
	int rule;
	int height;
	int width;
	char* map_alive;
	char* map_dead;
	int map_frequency;
	char stdin_char_alive;
	char stdin_char_dead;
	int seed_mode;
	unsigned int seed;
} Params;

int create_params(Params** result);
int destroy_params(Params* params);
int checkout_params(Params* params);

#endif
