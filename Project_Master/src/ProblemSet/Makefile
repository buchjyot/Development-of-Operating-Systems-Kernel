# Project: Project1
CFLAGS= -Wall -std=gnu99 -pedantic -Werror -g

EXECS = operatingSystem memtest

# Executable Target
all: $(EXECS)

# C program and their dependency
operatingSystem: operatingSystem.o \
shell.o \
displayPrompt.o \
freeAllocatedMemory.o \
getCurserLocation.o \
parseArguments.o \
readInputString.o \
reportError.o \
cmd_date.o \
cmd_echo.o \
cmd_exit.o \
cmd_help.o \
cmd_malloc.o \
cmd_free.o \
cmd_memorymap.o \
cmd_freeerrorcode.o \
executeCommand.o \
memory.o \
processControlUtils.o \
initializeMainMemory.o \
util.o \
strcmp_fcn.o \

memtest: memtest.o \
memory.o \
processControlUtils.o \
reportError.o \
initializeMainMemory.o \
util.o

# Clean target
clean :
	rm -f $(EXECS) *.o *.dev *.win *.layout

