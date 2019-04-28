GOAL=star-space-fighter
CC=gcc
FLAGS= -lm -Wall -Wextra -Wno-unused -O3
OGL=-lGL -lGLU -lglut

$(GOAL): main.o funkcije.o image.o
	$(CC) $(FLAGS) $(OGL) -o $@ $^

main.o: main.c funkcije.h funkcije.c
	$(CC) $(FLAGS) -o $@ $< -c

funkcije.o: funkcije.c funkcije.h
	$(CC) $(FLAGS) -o $@ $< -c
	
image.o: image.c
	$(CC) $(FLAGS) -o $@ $^ -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o

