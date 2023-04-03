CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm
TARGET = neural_network
OBJS = neural_network.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

neural_network.o: neural_network.c
	$(CC) $(CFLAGS) -c neural_network.c

clean:
	rm -f $(OBJS) $(TARGET)

