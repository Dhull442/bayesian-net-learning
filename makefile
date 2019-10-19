# Compiler to use
CXX = g++ -std=c++0x

# Flags
CXXFLAGS = -Wall -g -O3

# Executable name
OUT = main

all: $(OUT)

# Executable
$(OUT): main.o Node.o network.o
	$(CXX) network.o Node.o main.o -o $(OUT) $(CXXFLAGS)

# Main
main.o: main.cpp network.h Node.h
	$(CXX) -c main.cpp -o $@

# Graph_Node
Node.o: Node.cpp Node.h network.h
	$(CXX) -c Node.cpp -o $@

# network
network.o: network.cpp network.h Node.h
	$(CXX) -c network.cpp -o $@

# Clean
clean:
	rm *.o
	rm $(OUT)
