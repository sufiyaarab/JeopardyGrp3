CC = gcc
CFLAGS = -Wall -Wextra -std=c99 $(shell pkg-config --cflags gtk+-3.0)
LFLAGS = 
LIBS = $(shell pkg-config --libs gtk+-3.0)
SOURCES = jeopardy.c questions.c players.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = jeopardy.exe

.PHONY: clean

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXE) *~
