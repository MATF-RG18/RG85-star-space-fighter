GOAL=star-space-fighter
CC=gcc
FLAGS= -std=c11 -lGL -lGLU -lglut -lm -Wall -Wextra -Wno-unused -O3

$(GOAL): main.o funkcije.o image.o
	$(CC) $(FLAGS) -o $@ $^

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

