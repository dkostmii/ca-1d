#!/bin/bash

~/Projects/ca-1d/build/release/ca-1d \
	--rule=110,22,108,30,218,45,18,24,54,184,90,126,250,150,66,99,182 \
	--width=31-80 \
	--height=16-22 \
	--map_alive=@K\&\%\#\*\?\$\=MW\>86RBE3~xdVN\< \
	--map_dead=e\ i\:\;\"\'\`\/\\\ .\,\|_-qojs \
	--map_frequency=r \
	--seed_mode=r \
	--seed=$(date +%s%3N)
