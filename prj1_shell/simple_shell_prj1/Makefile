CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -I src 
LDFLAGS =  

VPATH = src src/builtins src/command src/parse src/pipeline
SOURCES = main.c built_in.c command_t.c parser.c pipeline_t.c
OBJECTS = $(SOURCES:.c=.o) 
EXEC = min-shell

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# main.o: main.c parse/parser.h
# built_in.o: built_in.c builtins/built_in.h  # Path from VPATH
# command_t.o: command_t.c command/command_t.h
# parser.o: parser.c parse/parser.h command/command_t.h
# pipeline_t.o: pipeline_t.c pipeline/pipeline_t.h builtins/built_in.h command/command_t.h parse/parser.h

test_parser: test_parser.o  # Link rule similar
	$(CC) $< $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f *.o $(EXEC) test_parser

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC)