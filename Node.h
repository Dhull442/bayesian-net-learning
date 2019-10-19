#ifndef Node_H
#define Node_H

#include <vector>
#include <string>
#include "network.h"

typedef std::pair<double,double> pdd;

class network;

// Our graph consists of a list of nodes where each node is represented as follows:
class Node{

public:
	std::string Node_Name;  // Variable name
	std::vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	std::vector<std::string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	std::vector<std::string> values; // Categories of possible values
	std::vector<pdd> CPT; // conditional probability table as a 1-d array . Written as (numerator, denominator)

	std::vector<int> parent_indices; // Parents of a particular node - these are index of nodes in graph.
	std::unordered_map<std::string, int> value_to_int_mapping; // Mapping value strings to integer for easier calculations

	// Constructor- a node is initialised with its name and its categories
	Node(std::string, int, std::vector<std::string>);
	std::string get_name();
	std::vector<int> get_children();
	std::vector<std::string> get_Parents();
	std::vector<pdd> get_CPT();
	int get_nvalues();
	std::vector<std::string> get_values();
	void set_CPT(std::vector<pdd>&);
	void set_Parents(std::vector<std::string>&);
	void set_parent_indices(std::vector<int>&);
	// add another node in a graph as a child of this node
	int add_child(int);
	// Given the values of parents and the network object, return the relevant cpt_index
	int calculate_cpt_index(std::string,std::vector<std::string>&,network&);
	// Given the network object and the values of observables, returns the probability using markovian blanket
	void print();
};

#endif
