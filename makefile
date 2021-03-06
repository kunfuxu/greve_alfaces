#makefile 

all: comboios
comboios.o: comboios.c comboios.h cores.h
	gcc -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/bin/sdl2-config -lm -lSDL2 -lSDL2_image -lSDL2_gfx -c comboios.c
ler.c: comboios.h cores.h 
	gcc -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/bin/sdl2-config -lm -lSDL2 -lSDL2_image -lSDL2_gfx -c ler.c
main.o: main.c comboios.h cores.h
	gcc -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/bin/sdl2-config -lm -lSDL2 -lSDL2_image -lSDL2_gfx -c main.c 
SDL_comboios.o: SDL_comboios.c comboios.h cores.h
	gcc -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/bin/sdl2-config -lm -lSDL2 -lSDL2_image -lSDL2_gfx -c SDL_comboios.c
comboios: main.o comboios.o ler.o SDL_comboios.o
	gcc -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/bin/sdl2-config main.o comboios.o ler.o SDL_comboios.o  -lm -lSDL2 -lSDL2_image -lSDL2_gfx -o comboios 

clean:     
	rm main.o comboios.o ler.o SDL_comboios-o -o comboios
