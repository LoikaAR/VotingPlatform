# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include

# List of source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Output binary
TARGET := $(BIN_DIR)/texture_synth

# Default target
all: $(TARGET)

# Link source files directly to create executable
$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Clean up
clean:
	rm -f $(BIN_DIR)/texture_synth

.PHONY: all clean
