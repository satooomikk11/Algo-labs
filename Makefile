CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

SRCS = src/tester.c                        \
       src/sorting_utils.c                 \
       src/testing.c                       \
       src/sortings/insertion_sort.c       \
       src/sortings/bubble_sort.c          \
       src/sortings/selection_sort.c       \
       src/sortings/shell_sort.c           \
       src/sortings/heap_sort.c            \
       src/sortings/merge_sort.c           \
       src/sortings/quick_sort_partition.c \
       src/sortings/quick_sort_pivot.c     \
       src/sortings/introsort.c            \
       src/sortings/introspective.c        \
       src/sortings/radix_sort.c

all: generator qsort_check tester

generator: src/generator.c
	$(CC) $(CFLAGS) -o $@ $^

qsort_check: src/qsort_check.c
	$(CC) $(CFLAGS) -o $@ $^

tester: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f generator qsort_check tester
	rm -rf results/*.csv

test: all
	mkdir -p results small_tests big_tests test_most_dublicates
	chmod +x scripts/generate.sh
	./scripts/generate.sh
	./tester

plot:
	python3 scripts/plot_graphs.py

.PHONY: all clean test plot