CC = gcc
CFLAGS = -O2 -Wall -Iinclude
BUILD_DIR = build

SRC = src/main.c src/sieve.c src/timing.c
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/sieve.o $(BUILD_DIR)/timing.o
TARGET = $(BUILD_DIR)/lab01

all: $(TARGET)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)/*