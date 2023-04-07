CC = gcc
CFLAGS = -Wall -Wextra -O0 -g
LDFLAGS = -lm
TARGET = neural_network
TEST_TARGET = test_backpropagation
OBJS = neural_network.o value.o
TEST_OBJS = tests/test_backpropagation.o value.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

neural_network.o: neural_network.c value.h
	$(CC) $(CFLAGS) -c neural_network.c

value.o: value.c value.h
	$(CC) $(CFLAGS) -c value.c

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(LDFLAGS)

tests/test_backpropagation.o: tests/test_backpropagation.c value.h
	$(CC) $(CFLAGS) -c tests/test_backpropagation.c -o tests/test_backpropagation.o

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

