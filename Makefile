CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

SOURCES = main.c creation.c
HEADERS = const.h structs.h creation.h
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