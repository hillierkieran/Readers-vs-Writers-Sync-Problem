CC = gcc
CFLAGS = -Wall -pedantic -pthread

DEPS = 	

OBJ = a2.o

all: a2

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean run

clean: 
	rm -f *~ *.o $(OBJ) a2

run:
	./a2
