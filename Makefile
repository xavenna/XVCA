# based on code from https://stackoverflow.com/a/20830354
# but modified to fit project needs
# Kind of a kludgy mess at the moment
CFLAGS = -Wall -fexceptions -std=c++17
INCD = -Iinc/
CC = g++

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADER = $(wildcard include/*.h)

.PHONY: all clean asm xvca drive remake

# Default build
all: asm xvca drive

ASMEXE = assembler.exe
ASMOBJS = assembler.o util.o assembler-util.o file-utils.o # I don't think this can be automatically generated

XVEXE = xvca.exe
XVOBJS = xvca.o util.o

DREXE = drivemgr.exe
DROBJS = drivemgr.o util.o file-utils.o drive.o

# Assembler
asm: $(ASMEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.cpp=.d)

$(ASMEXE): $(ASMOBJS)
	$(CC) -o assembler.exe $^
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCD) -c $< -o $@

# XVCA core
xvca: $(XVEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.cpp=.d)

$(XVEXE) : $(XVOBJS)
	$(CC) -o $(XVEXE) $^
%.o : %.cpp
	$(CC) $(CFLAGS) $(INCD) -c $< -o $@

# Drive Manager
drive: $(DREXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.cpp=.d)

$(DREXE) : $(DROBJS)
	$(CC) -o $(DREXE) $^
%.o : %.cpp
	$(CC) $(CFLAGS) $(INCD) -c $< -o $@

#
# Other rules
#
#prep:
#	@mkdir -p $(DBGDIR) $(RELDIR) $(HDIR)

remake: clean all

clean:
	rm -f $(XVEXE) $(XVOBJS) $(ASMEXE) $(ASMOBJS) $(DREXE) $(DROBJS)
