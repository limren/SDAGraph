CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SOURCES = main.c creation.c tris.c AVL.c Graph.c
HEADERS = const.h structs.h creation.h tris.h AVL.h Graph.h hashmap.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main


.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -g -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)