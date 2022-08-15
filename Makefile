# This makefile was created with help from the following stackoverflow answer:
# https://stackoverflow.com/a/23418196

LLIB = -lncurses
CXX = g++
CPPFLAGS = -Wall -Wextra -fexceptions -std=c++17

ASMEXE = assembler
XVEXE = xvca
DREXE = drivemgr

ifeq ($(OS),Windows_NT)
XVEXE += .exe
DREXE += .exe
ASMEXE += .exe
else
LFLAGS +=  -no-pie
endif

ifeq (1, $(REL))
  CPPFLAGS += -O2 -s -DNDEBUG
  OBJ_DIR = ./obj/release
else
  CPPFLAGS += -g -O0
  OBJ_DIR = ./obj/debug
endif

SRCS = $(wildcard src/*.cpp)
df = $(OBJ_DIR)/$(*F)
AUTODEPS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
OBJS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

ASMOBJS := $(addprefix $(OBJ_DIR)/,assembler.o util.o assembler-util.o file-utils.o)

XVOBJS = $(addprefix $(OBJ_DIR)/,emulator.o xvca.o util.o cpu.o register-group.o display-adapter.o file-utils.o emulate-loop.o drive.o memory-group.o)

DROBJS = $(addprefix $(OBJ_DIR)/,drivemgr.o util.o file-utils.o drive.o encoding.o)

.PHONY : all clean tilde debug release remake asm xvca drive

all: asm xvca drive

drive: $(DREXE)

asm: $(ASMEXE)

ifeq ($(XVEXE),xvca)

else
xvca: $(XVEXE)
endif 
$(ASMEXE): $(ASMOBJS)
	$(CXX) $(LFLAGS) -o $@ $^

$(DREXE): $(DROBJS)
	$(CXX) $(LFLAGS) -o $@ $^

$(XVEXE): $(XVOBJS)
	$(CXX) $(LFLAGS) -o $@ $^ $(LLIB)

$(OBJ_DIR)/%.o: src/%.cpp
	@$(CXX) -MM -MP -MT $(df).o -MT $(df).d $(CPPFLAGS) $< > $(df).d
	$(CXX) $(CPPFLAGS) -c $< -o $@

-include $(AUTODEPS)

remake: clean all

clean :
	rm -f obj/debug/*.o obj/release/*.o obj/debug/*.d obj/release/*.d ./xvca ./drivemgr ./asm
tilde :
	rm *~
