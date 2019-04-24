GOAL=star-space-fighter
CC=gcc
FLAGS=-lGL -lGLU -lglut -Wall -Wextra -Wno-unused

$(GOAL): main.o
	$(CC) $(FLAGS) -o $@ $^

main.o: main.c
	$(CC) $(FLAGS) -o $@ $^ -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o

