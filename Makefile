CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pthread -Iinclude
BIN_DIR = bin
BUILD_DIR = build
TARGETS = $(BIN_DIR)/ants
COMMON_OBJS = \
	$(BUILD_DIR)/cli.o \
	$(BUILD_DIR)/node.o \
	$(BUILD_DIR)/tree.o \
	$(BUILD_DIR)/ant.o \
	$(BUILD_DIR)/simulation.o \
	$(BUILD_DIR)/timer.o

.PHONY: all clean

all: $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR)/ants: $(BUILD_DIR)/main.o $(COMMON_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BUILD_DIR)/main.o $(COMMON_OBJS)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) src/*.o ants src/main
