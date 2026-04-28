CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=199309L -Iinclude
LDFLAGS = -pthread

TARGET = ants

SRCS = src/main.c src/node.c src/tree.c src/ant.c src/simulation.c src/cli.c src/timer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

src/main.o: src/main.c include/cli.h include/simulation.h include/timer.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/node.o: src/node.c include/node.h
	$(CC) $(CFLAGS) -c src/node.c -o src/node.o

src/tree.o: src/tree.c include/tree.h include/node.h
	$(CC) $(CFLAGS) -c src/tree.c -o src/tree.o

src/ant.o: src/ant.c include/ant.h include/simulation.h include/tree.h include/node.h
	$(CC) $(CFLAGS) -c src/ant.c -o src/ant.o

src/simulation.o: src/simulation.c include/simulation.h include/ant.h include/tree.h include/node.h
	$(CC) $(CFLAGS) -c src/simulation.c -o src/simulation.o

src/cli.o: src/cli.c include/cli.h
	$(CC) $(CFLAGS) -c src/cli.c -o src/cli.o

src/timer.o: src/timer.c include/timer.h
	$(CC) $(CFLAGS) -c src/timer.c -o src/timer.o

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run