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
} Params;

int checkout_params(Params* params);
#endif
