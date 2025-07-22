# This makefile was created with help from the following stackoverflow answer:
# https://stackoverflow.com/a/23418196

CXX = g++
CPPFLAGS = -Wall -Wextra -fexceptions -std=c++17 -DXV_DEBUG

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
  CPPFLAGS += -g -O0 -DXV_DEBUG
  OBJ_DIR = ./obj/debug
endif

SRCS = $(wildcard src/*.cpp)
df = $(OBJ_DIR)/$(*F)
AUTODEPS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
OBJS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

ASMOBJS := $(addprefix $(OBJ_DIR)/,assembler.o util.o assembler-util.o file-utils.o asm2machine.o)

XVOBJS = $(addprefix $(OBJ_DIR)/,emulator.o xvca.o util.o cpu.o register-group.o display-adapter.o file-utils.o emulate-loop.o drive.o memory-group.o flags.o keyboard-adapter.o drive-adapter.o adapter-group.o disassemble.o)

DROBJS = $(addprefix $(OBJ_DIR)/,drivemgr.o util.o file-utils.o drive.o encoding.o)

DRIVEFILES = $(wildcard bootdr/*)

.PHONY : all clean tilde debug release remake asm emu drive boot

all: asm emu drive

drive: $(DREXE)

asm: $(ASMEXE)

emu: $(XVEXE)

$(ASMEXE): $(ASMOBJS)
	$(CXX) $(LFLAGS) -o $@ $^

$(DREXE): $(DROBJS)
	$(CXX) $(LFLAGS) -o $@ $^

$(XVEXE): $(XVOBJS)
	$(CXX) $(LFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: src/%.cpp
	@$(CXX) -MM -MP -MT $(df).o -MT $(df).d $(CPPFLAGS) $< > $(df).d
	$(CXX) $(CPPFLAGS) -c $< -o $@

-include $(AUTODEPS)

boot: boot.x boot.xdr

boot.x : asm/bootloader.asm
	./assembler -n -j0 -iasm/bootloader.asm -oboot.x

boot.xdr: bsec.x $(DRIVEFILES)
	./drivemgr -p bootdr/ boot.xdr
	./drivemgr -b boot.xdr bsec.x

bsec.x: asm/boot-sector.asm
	./assembler -n -j9000 -iasm/boot-sector.asm -obsec.x

remake: clean all

clean :
	rm -f obj/debug/*.o obj/release/*.o obj/debug/*.d obj/release/*.d ./xvca ./drivemgr ./assembler
tilde :
	rm *~
