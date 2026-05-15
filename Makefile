CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm
TARGET = test_runner
SRCS = test_runner.c dynamic_array_stack.c linked_list_stack.c
OBJS = $(SRCS:.c=.o)
DEPS = stack_struct.h dynamic_array_stack.h linked_list_stack.h ctest.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all
