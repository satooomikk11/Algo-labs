CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -Iinclude
LDFLAGS = -lm

SRC_DIR = src
INCLUDE_DIR = include

all: test_binary_heap test_binomial_heap

test_binary_heap: $(SRC_DIR)/test_binary_heap.c $(SRC_DIR)/binary_heap.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_binomial_heap: $(SRC_DIR)/test_binomial_heap.c $(SRC_DIR)/binomial_heap.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run: all
	@echo "Запуск тестов"
	@./test_binary_heap
	@./test_binomial_heap
	@echo "Построение графиков"
	@python3 scripts/plot_results.py

clean:
	rm -f test_binary_heap test_binomial_heap *.png

rebuild: clean all

.PHONY: all clean run rebuild