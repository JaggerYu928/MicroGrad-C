CC = gcc
CFLAGS = -Wall -Wextra -O0 -g
LDFLAGS = -lm
TARGET = neural_network
TEST_TARGET = test_backpropagation
TEST_NEURON_TARGET = test_neuron
TEST_LAYER_TARGET = test_layer
OBJS = neural_network.o value.o neuron.o layer.o
TEST_OBJS = tests/test_backpropagation.o value.o
TEST_NEURON_OBJS = tests/test_neuron.o value.o neuron.o
TEST_LAYER_OBJS = tests/test_layer.o value.o neuron.o layer.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

neural_network.o: neural_network.c value.h layer.h
	$(CC) $(CFLAGS) -c neural_network.c

value.o: value.c value.h
	$(CC) $(CFLAGS) -c value.c

neuron.o: neuron.c neuron.h
	$(CC) $(CFLAGS) -c neuron.c

layer.o: layer.c layer.h
	$(CC) $(CFLAGS) -c layer.c

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(LDFLAGS)

$(TEST_NEURON_TARGET): $(TEST_NEURON_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_NEURON_TARGET) $(TEST_NEURON_OBJS) $(LDFLAGS)

$(TEST_LAYER_TARGET): $(TEST_LAYER_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_LAYER_TARGET) $(TEST_LAYER_OBJS) $(LDFLAGS)

tests/test_backpropagation.o: tests/test_backpropagation.c value.h
	$(CC) $(CFLAGS) -c tests/test_backpropagation.c -o tests/test_backpropagation.o

tests/test_neuron.o: tests/test_neuron.c value.h neuron.h
	$(CC) $(CFLAGS) -c tests/test_neuron.c -o tests/test_neuron.o

tests/test_layer.o: tests/test_layer.c value.h neuron.h layer.h
	$(CC) $(CFLAGS) -c tests/test_layer.c -o tests/test_layer.o

test: $(TEST_TARGET)
	./$(TEST_TARGET)

testneuron: $(TEST_NEURON_TARGET)
	./$(TEST_NEURON_TARGET)

testlayer: $(TEST_LAYER_TARGET)
	./$(TEST_LAYER_TARGET)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TEST_NEURON_OBJS) $(TEST_LAYER_OBJS) $(TARGET) $(TEST_TARGET) $(TEST_NEURON_TARGET) $(TEST_LAYER_TARGET)

# 添加這個目標以運行Valgrind
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

memcheckneuron: $(TEST_NEURON_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_NEURON_TARGET)

memchecklayer: $(TEST_LAYER_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_LAYER_TARGET)
