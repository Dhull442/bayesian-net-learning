#ifndef NETWORK_H
#define NETWORK_H 

#include<bits/stdc++.h>
#include "Graph_Node.h"

class Graph_Node;

// The whole network represted as a list of nodes
class network{
public:
	std::list <Graph_Node> Pres_Graph;
	std::string information;

	// unordered_map<string, int> name_index_mapping;

	int addNode(Graph_Node );

	int netSize();

  	// get the index of node with a given name
	int get_index(std::string );

	// get the node at nth index
  	std::list<Graph_Node>::iterator get_nth_node(int );

	//get the iterator of a node with a given name
	std::list<Graph_Node>::iterator search_node(std::string );

	void print();

	void print_format(std::string );
};

#endif