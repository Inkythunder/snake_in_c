build: snake.c linked_list_generic.c linked_list_generic.h
	gcc -I raylib snake.c linked_list_generic.c raylib/libraylib.a -lm -g -o snake

default: build
