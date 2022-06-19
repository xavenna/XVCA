# based on code from https://stackoverflow.com/a/20830354
# but modified to fit project needs
CFLAGS = -Wall -fexceptions -std=c++11
INCD = -Iinc/
CC = g++

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADER = $(wildcard include/*.h)

.PHONY: all clean asm xvca remake

# Default build
all: asm xvca

ASMEXE = assembler.exe
ASMOBJS = assembler.o util.o

XVEXE = xvca.exe
XVOBJS = xvca.o util.o

asm: $(ASMEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(DBGCFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.c=.d)

$(ASMEXE): $(ASMOBJS)
	$(CC) -o assembler.exe $^
%.o: %.cpp
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(INCD) -c $< -o $@

#
# Release rules
#
release: $(RELEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(RELCFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.c=.d)

$(XVEXE) : $(XVOBJS)
	$(CC) -o $(XVEXE) $^
%.o : %.cpp
	$(CC) $(CFLAGS) $(INCD) -c $< -o $@

#
# Other rules
#
#prep:
#	@mkdir -p $(DBGDIR) $(RELDIR) $(HDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
