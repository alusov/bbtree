CC=g++
CFLAGS=-c -Wall -std=c++11
BUILD=../build/

all: bbtree treevis

bbtree: bbtree.o
	$(CC) $(BUILD)bbtree.o -o $(BUILD)bbtree

bbtree.o: bbtree.cpp
	$(CC) $(CFLAGS) bbtree.cpp -o $(BUILD)bbtree.o

treevis: treevis.o
	$(CC) $(BUILD)treevis.o -o $(BUILD)treevis

treevis.o: bbtree.cpp
	$(CC) $(CFLAGS) tree_visualizer.cpp -o $(BUILD)treevis.o

clean:
	rm $(BUILD)*.o
