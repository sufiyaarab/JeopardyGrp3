// Compiler that will be used
CC = gcc //Uses the GNU C Compiler (GCC) to compile the project

//compiler flags
CFLAGS = -Wall -Wextra -std=c99 $(shell pkg-config --cflags gtk+-3.0)

//linker flags (are not used for this application)
LFLAGS = 

//Libraries to link
LIBS = $(shell pkg-config --libs gtk+-3.0)

//list of source files
SOURCES = jeopardy.c questions.c players.c

//list of object files
OBJECTS = $(subst .c,.o,$(SOURCES))

//exe output file
EXE = jeopardy.exe

//Define a phony target for cleaning up compiled files
.PHONY: clean

//Rule to build the final executable. It links the compiled object files into a single executable
$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LIBS)

//Rule to compile each .c file into a .o object file. '$<' is the source file being compiled, '$@' is the target object file output
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

//Rule to clean up the build artifacts
clean:
	rm -f $(OBJECTS) $(EXE) *~
