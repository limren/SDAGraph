CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

SOURCES = main.c creation.c tris.c AVL.c
HEADERS = const.h structs.h creation.h tris.h AVL.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main


.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)