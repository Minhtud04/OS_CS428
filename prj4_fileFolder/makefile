# Compiler
CC = gcc
# Flags: -Wall for warnings, -g for debugging
CFLAGS = -Wall -g

# Targets (two separate executables)
TARGETS = stat ls

all: $(TARGETS)

# Build stat from stat.c
stat: stat.o
	$(CC) $(CFLAGS) -o stat stat.o

# Build ls from ls.c
ls: ls.o
	$(CC) $(CFLAGS) -o ls ls.o

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean