CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -pthread

TARGET = ants

SRCS = main.c node.c tree.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c tree.h node.h
	$(CC) $(CFLAGS) -c main.c

tree.o: tree.c tree.h node.h
	$(CC) $(CFLAGS) -c tree.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run