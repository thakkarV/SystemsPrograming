# Compiler flags
CC = gcc
CFLAGS = -c

# Project files
SRCS_MMP = matmult_p.c reader.c printer.c shmops.c datastructures.c
OBJS_MMP = $(SRCS_MMP:.c=.o)
DBGOBJS_MMP = $(addprefix $(DBGDIR)/, $(OBJS_MMP))
EXE_MMP  = matmult_p

SRCS_MUL = multiply.c
OBJS_MUL = $(SRCS_MUL:.c=.o)
DBGOBJS_MUL = $(addprefix $(DBGDIR)/, $(OBJS_MUL))
EXE_MUL = multiply

SRCS_FMT = matformatter.c printer.c reader.c datastructures.c
OBJS_FMT = $(SRCS_FMT:.c=.o)
DBGOBJS_FMT = $(addprefix $(DBGDIR)/, $(OBJS_FMT))
EXE_FMT = matformatter

SRCS_MMT = matmult_t.c reader.c printer.c threads.c datastructures.c
OBJS_MMT = $(SRCS_MMT:.c=.o)
DBGOBJS_MMT = $(addprefix $(DBGDIR)/, $(OBJS_MMT))
EXE_MMT = matmult_t

OBJS_ALL = $(OBJS_MMP) $(OBJS_MUL) $(OBJS_FMT) $(OBJS_MMT)

# Debug build settings
DBGDIR = debug
DBGEXE_MMP = $(DBGDIR)/$(EXE_MMP)
DBGEXE_MUL = $(DBGDIR)/$(EXE_MUL)
DBGEXE_FMT = $(DBGDIR)/$(EXE_FMT)
DBGEXE_MMT = $(DBGDIR)/$(EXE_MMT)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS_ALL))
DBGCFLAGS = -g -O0 -DDEBUG

# Release build settings
RELEXE_MMP = $(EXE_MMP)
RELEXE_MUL = $(EXE_MUL)
RELEXE_FMT = $(EXE_FMT)
RELEXE_MMT = $(EXE_MMT)
RELOBJS = $(OBJS_ALL)
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug release remake

# default build
all: debug release

###
### Debug rules
###
debug: $(DBGEXE_MMP) $(DBGEXE_MUL) $(DBGEXE_FMT) $(DBGEXE_MMT)

$(DBGEXE_MMP): $(DBGOBJS_MMP)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_MMP) $(DBGOBJS_MMP)

$(DBGEXE_MUL): $(DBGOBJS_MUL)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_MUL) $(DBGOBJS_MUL)

$(DBGEXE_FMT): $(DBGOBJS_FMT)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_FMT) $(DBGOBJS_FMT)

$(DBGEXE_MMT): $(DBGOBJS_MMT)
	$(CC) $(DBGCFLAGS) -o $(DBGEXE_MMT) $(DBGOBJS_MMT)

$(DBGDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $<

###
### Release rules
###
release: $(RELEXE_MMP) $(RELEXE_MUL) $(RELEXE_FMT) $(RELEXE_MMT)

$(RELEXE_MMP): $(OBJS_MMP)
	$(CC) $(RELCFLAGS) -o $(RELEXE_MMP) $(OBJS_MMP)

$(RELEXE_MUL): $(OBJS_MUL)
	$(CC) $(RELCFLAGS) -o $(RELEXE_MUL) $(OBJS_MUL)

$(RELEXE_FMT): $(OBJS_FMT)
	$(CC) $(RELCFLAGS) -o $(RELEXE_FMT) $(OBJS_FMT)

$(RELEXE_MMT): $(OBJS_MMT)
	$(CC) $(RELCFLAGS) -o $(RELEXE_MMT) $(OBJS_MMT)

$(RELDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $@ $<

remake: clean all

clean:
	rm -f $(RELEXE_MMP) $(RELEXE_MUL) $(RELEXE_FMT) $(RELEXE_MMT) \
		  $(DBGEXE_MMP) $(DBGEXE_MUL) $(DBGEXE_FMT) $(DBGEXE_MMT) \
		  $(OBJS_ALL) $(DBGOBJS) \
