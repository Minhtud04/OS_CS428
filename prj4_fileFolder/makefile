CC = gcc
CFLAGS = -Wall -g

TARGETS = stat ls find

all: $(TARGETS)

stat: stat.o
	$(CC) $(CFLAGS) -o stat stat.o

ls: ls.o
	$(CC) $(CFLAGS) -o ls ls.o

find: find.o
	$(CC) $(CFLAGS) -o find find.o


# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean