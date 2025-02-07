CC = gcc
CFLAGS = -Wall -Wextra -std=c99 'pkg-config --cflags --libs gtk+-3.0'
LFLAGS = 
LIBS = 'pkg-config --cflags --libs gtk+-3.0'
SOURCES = jeopardy.c questions.c players.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = jeopardy.exe
.PHONY: clean help

jeopardy.exe : jeopardy.o questions.o players.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"
