CC = gcc

SRCS = mygdb.c reader.c parser.c executor.c dwarf_utils.c datastructures.c
OBJS = $(SRCS:.c=.o)

# Release build settings
EXE = mygdb
CFLAGS = -O0 -I/usr/include/libdwarf

.PHONY: all clean remake

# Default build
all: release

release: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -ldwarf -o $(EXE) $^

$(RELDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXE) $(OBJS)

remake: clean all
