CC = gcc
CFLAGS = -Wall -O2 -I.
LDFLAGS = -lm

BUILD_DIR   = build
DATA_DIR    = data
RESULTS_DIR = results
PLOTS_DIR   = plots

SRCS = main.c                          \
       hash_functions/uint_hash.c      \
       hash_functions/float_hash.c     \
       hash_functions/string_hash.c    \
       hash_tables/chaining.c          \
       hash_tables/linear.c            \
       hash_tables/quadratic.c         \
       hash_tables/double_hash.c       \
       hash_tables/cuckoo.c            \
       perfect_hash/perfect.c          \
       tests/test_uint.c               \
       tests/test_float.c              \
       tests/test_string.c             \
       tests/test_tables_load.c        \
       tests/test_tables_ops.c         \
       tests/test_perfect.c

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

TARGET = lab_hash

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(DATA_DIR) $(RESULTS_DIR) $(PLOTS_DIR)

run: $(TARGET)
	@mkdir -p $(DATA_DIR) $(RESULTS_DIR)/functions $(RESULTS_DIR)/tables 
	@mkdir -p $(PLOTS_DIR)/functions $(PLOTS_DIR)/tables
	./$(TARGET)

plots: run
	python3 plot_results.py

all_plots: clean run plots

.PHONY: all clean run plots all_plots