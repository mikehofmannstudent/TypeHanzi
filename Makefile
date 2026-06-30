# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Compiler flags
CFLAGS = -Wall -Wextra -I$(SRC_DIR)

# Libraries
LIBS =

# Target
TARGET = $(BIN_DIR)/ime_query

# Sources
SRCS = \
	$(SRC_DIR)/query.c \
	$(SRC_DIR)/sqlite3.c

# Object files
OBJS = \
	$(BUILD_DIR)/query.o \
	$(BUILD_DIR)/sqlite3.o

all: directories $(TARGET)

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/query.o: $(SRC_DIR)/query.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sqlite3.o: $(SRC_DIR)/sqlite3.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run directories