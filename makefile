# Compiler to use
CXX = g++ -std=c++0x

# Flags
CXXFLAGS = -Wall -g

# Executable name
OUT = main

all: $(OUT)

# Executable
$(OUT): main.o Graph_Node.o network.o 
	$(CXX) network.o Graph_Node.o main.o -o $(OUT) $(CXXFLAGS)

# Main
main.o: main.cpp network.h Graph_Node.h
	$(CXX) -c main.cpp -o $@

# Graph_Node
Graph_Node.o: Graph_Node.cpp Graph_Node.h network.h
	$(CXX) -c Graph_Node.cpp -o $@

# network
network.o: network.cpp network.h Graph_Node.h
	$(CXX) -c network.cpp -o $@

# Clean
clean:
	rm *.o
	rm $(OUT)
