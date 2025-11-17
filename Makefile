# Compiler to use
CC = gcc

# Compiler flags
# -Iinclude: tells the compiler to look for header files in the 'include' directory
# -Wall -Wextra: enables common warnings (good practice)
# -g: adds debugging information
CFLAGS = -Iinclude -Wall -Wextra -g

# The name of the final executable
TARGET = student_system

# List of source files using wildcard for the 'src' and 'src/features/*' directories
# The backslash '\' character is used to escape the space in "Authentication System.c" 
SRCS = $(wildcard src/*.c) \
       $(wildcard src/features/*/*.c)

# Generate a list of object files from the source files (replaces .c with .o)
# Note: GNU Make automatically handles spaces in filenames in the OBJS list.
OBJS = $(SRCS:.c=.o)

# Detect OS and shell type automatically
ifeq ($(OS),Windows_NT)
    ifneq (,$(findstring /, $(SHELL)))
        # Using Unix-like shell on Windows (Git Bash, MSYS, etc.)
        RM = rm -f
        NULL = 2>/dev/null
        SEP = /
    else
        # Using cmd.exe
        SHELL := cmd.exe
        .SHELLFLAGS := /C
        RM = del /Q
        NULL = 2>nul
        SEP = \\
    endif
else
    # Linux/macOS
    RM = rm -f
    NULL = 2>/dev/null
    SEP = /
endif


# Default target (what runs when you just type 'make')
all: $(TARGET)

# Rule to link the object files into the final executable
# $@ is the target name, $^ is the list of prerequisites (all object files)
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

	$(RM) $(subst /,$(SEP),$(OBJS)) $(NULL)

	@echo "Build complete! Run with ./${TARGET}"

# Generic rule to compile .c files to .o files
# $< is the source file, $@ is the object file
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove generated files
clean:
	@echo "Cleaning up..."
	$(RM) $(subst /,$(SEP),$(OBJS)) $(TARGET) $(TARGET).exe $(NULL)

# Phony targets (targets that aren't actual files)
.PHONY: all clean