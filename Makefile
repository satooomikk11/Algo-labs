CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lm
TARGET = build/tester

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS = $(SRC_DIR)/fenwick_tree.c       \
       $(SRC_DIR)/segment_tree.c       \
       $(SRC_DIR)/sparse_table.c       \
       $(SRC_DIR)/tester.c

OBJS = $(BUILD_DIR)/fenwick_tree.o     \
       $(BUILD_DIR)/segment_tree.o     \
       $(BUILD_DIR)/sparse_table.o     \
       $(BUILD_DIR)/tester.o

.PHONY: all clean run

all: $(BUILD_DIR) $(RESULTS_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(RESULTS_DIR):
	mkdir -p $(RESULTS_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

run: $(TARGET)
	./$(TARGET)


clean:
	rm -rf $(BUILD_DIR)
