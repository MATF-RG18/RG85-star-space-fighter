GOAL=star-space-fighter
CC=gcc
FLAGS= -lm
OGL=-lGL -lGLU -lglut

$(GOAL): main.o funkcije.o image.o
	$(CC) -o $@ $^ $(FLAGS) $(OGL)

main.o: main.c funkcije.h funkcije.c
	$(CC) -o $@ $< -c

funkcije.o: funkcije.c funkcije.h
	$(CC) -o $@ $< -c
	
image.o: image.c
	$(CC) -o $@ $^ -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o

