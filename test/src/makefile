CXX = g++
CXXFLAGS = -std=c++0x -g -L../gtest/lib -lgtest -lgtest_main -lpthread
INCS = -I../../src -I../gtest/include
BUILD = ../../build/
OBJS = $(BUILD)ring_buffer_test.o $(BUILD)main_test_all.o

all: testAll

testAll: ring_buffer_test.o main_test_all.o
	$(CXX) $(INCS) $(OBJS) $(CXXFLAGS) -o $(BUILD)testAll 

ring_buffer_test.o: 
	$(CXX) $(INCS) $(CXXFLAGS) -c ring_buffer_test.cpp -o $(BUILD)ring_buffer_test.o

main_test_all.o: 
	$(CXX) $(INCS) $(CXXFLAGS) -c main_test_all.cpp -o $(BUILD)main_test_all.o

clean:
	rm $(BUILD)testAll $(BUILD)*.o
