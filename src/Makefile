CC=g++
CFLAGS=-c -Wall -std=c++0x
BUILD=../build/

all: bbtree

bbtree: bbtree.o
	$(CC) $(BUILD)bbtree.o -o $(BUILD)bbtree

bbtree.o: bbtree.cpp
	$(CC) $(CFLAGS) bbtree.cpp -o $(BUILD)bbtree.o

clean:
	rm $(BUILD)*.o
