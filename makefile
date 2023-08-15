CC = gcc
CFLAGS = -Wall -pedantic -pthread

DEPS = 	common.h \
		shared_data.h \
		thread_operations.h \
		utilities.h

OBJ = 	a2.o \
		shared_data.o \
		thread_operations.o \
		utilities.o

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
