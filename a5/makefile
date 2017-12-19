CC = gcc

SRCS = mygdb.c reader.c parser.c executor.c datastructures.c
OBJS = $(SRCS:.c=.o)

# Release build settings
EXE = mygdb
CFLAGS = -O0

.PHONY: all clean remake

# Default build
all: release

release: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -I/usr/include/libdwarf -o $(EXE) $^

$(RELDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(LCFLAGS) -o $@ $<

clean:
	rm -f $(EXE) $(OBJS)

remake: clean all
