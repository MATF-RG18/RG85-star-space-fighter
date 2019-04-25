GOAL=star-space-fighter
CC=gcc
FLAGS= -std=c99 -lGL -lGLU -lglut -lm -Wall -Wextra -Wno-unused

$(GOAL): main.o funkcije.o
	$(CC) $(FLAGS) -o $@ $^

main.o: main.c
	$(CC) $(FLAGS) -o $@ $^ -c

funkcije.o: funkcije.c
	$(CC) $(FLAGS) -o $@ $^ -c


.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o

