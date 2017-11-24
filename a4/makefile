# Compiler flags
CC = gcc
CFLAGS = -c

# Project files
SRCS_MTP = matmult_p.c reader.c printer.c shmops.c
OBJS_MTP = $(SRCS_MTP:.c=.o)
EXE_MTP  = matmult_p

SRCS_MUL = multiply.c
OBJS_MUL = $(SRCS_MTP:.c=.o)
EXE_MUL = multiply

SRCS_FMT = matformatter.c printer.c
OBJS_FMT = $(SRCS_FMT:.c=.o)
EXE_FMT = matformatter

OBJS_ALL = $(OBJS_MTP) $(OBJS_MUL) $(OBJS_FMT)

# Debug build settings
DBGDIR = debug
DBGEXE_MTP = $(DBGDIR)/$(EXE_MTP)
DBGEXE_MUL = $(DBGDIR)/$(EXE_MUL)
DBGEXE_FMT = $(DBGDIR)/$(EXE_FMT)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS_ALL))
DBGCFLAGS = -g -O0 -DDEBUG

# Release build settings
RELDIR = release
RELEXE_MTP = $(EXE_MTP)
RELEXE_MUL = $(EXE_MUL)
RELEXE_FMT = $(EXE_FMT)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS_ALL))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: debug release

# Debug rules
debug: $(DBGEXE_MTP) $(DBGEXE_MUL) $(DBGEXE_FMT)

$(DBGEXE_MTP): $(OBJS_MTP)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_MTP) $(OBJS_MTP)

$(DBGEXE_MUL): $(OBJS_MUL)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_MUL) $(OBJS_MUL)

$(DBGEXE_FMT): $(OBJS_FMT)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_FMT) $(OBJS_FMT)

$(DBGDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $<

# Release rules
release: $(RELEXE_MTP) $(RELEXE_MUL) $(RELEXE_FMT)

$(RELEXE_MTP): $(OBJS_MTP)
	$(CC) $(RELCFLAGS) -o $(RELEXE_MTP) $(OBJS_MTP)

$(RELEXE_MUL): $(OBJS_MUL)
	$(CC) $(RELCFLAGS) -o $(RELEXE_MUL) $(OBJS_MUL)

$(RELEXE_FMT): $(OBJS_FMT)
	$(CC) $(RELCFLAGS) -o $(RELEXE_FMT) $(OBJS_FMT)

$(RELDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $@ $<

# Other rules
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE_MTP) $(RELEXE_MUL) $(RELEXE_FMT) $(OBJS_ALL) $(DBGOBJS) \
		$(DBGEXE_MTP) $(DBGEXE_MUL) $(DBGEXE_FMT)
