#ifndef NETWORK_H
#define NETWORK_H

#include<bits/stdc++.h>
#include "Node.h"

class Node;

// The whole network represted as a list of nodes
class network{
public:
	std::vector< Node > graph;
	std::string information;

	int addNode(Node&);

	int size();

	// get the index of node with a given name
	int get_index(std::string);

	void print();

	void print_format(std::string );
};

#endif
