CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Isrc -Ilib

LDFLAGS =

SRC = \
	src/main.c \
	src/database.c \
	src/terminal.c \
	src/ui.c \
	lib/sqlite3.c

TARGET = typehanzi

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run