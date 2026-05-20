CC = gcc
CFLAGS = -O2 -Wall -Wextra -Iinclude
LDFLAGS = 

PROJECT_DIR := $(shell pwd)
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
DATA_DIR = data
RESULTS_DIR = results
PLOTS_DIR = plots

SOURCES = $(SRC_DIR)/naive_tree.c   \
          $(SRC_DIR)/avl_tree.c     \
          $(SRC_DIR)/treap.c        \
          $(SRC_DIR)/splay_tree.c   \
          $(SRC_DIR)/rb_tree.c      \
          $(SRC_DIR)/b_tree.c       \
          $(SRC_DIR)/skip_list.c    \
          $(SRC_DIR)/main.c

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/test_trees

.PHONY: all clean run plot dirs

dirs:
	@mkdir -p $(BIN_DIR) $(DATA_DIR) $(RESULTS_DIR) $(PLOTS_DIR)

all: dirs $(TARGET)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) | dirs
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET) | dirs
	@echo "Running tests..."
	@cd $(PROJECT_DIR) && ./$(TARGET)

plot:
	@echo "Generating plots..."
	@cd $(PROJECT_DIR) && python3 scripts/plot_results.py

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(DATA_DIR)
	@rm -rf $(RESULTS_DIR)
	