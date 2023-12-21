CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SOURCES = main.c creation.c tris.c AVL.c Graphe.c Tas.c Selection.c
HEADERS = headers/const.h headers/structs.h headers/headers/creation.h headers/tris.h headers/AVL.h headers/Graphe.h headers/Tas.h headers/Selection.h hashmap.h
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