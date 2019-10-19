#include<bits/stdc++.h>
#include "network.h"

using namespace std;
#define PB push_back
string directly_copy_in_output = "";

// The whole network represted as a list of nodes
int network::addNode(Node& node){
    graph.PB(node);
    return 0;
}

int network::size(){
    return graph.size();
}

// get the index of node with a given name
int network::get_index(string val_name){
    for(int i=0;i<graph.size();i++){
      if(graph[i].get_name().compare(val_name)==0)
        return i;
    }
    return -1;
}

void network::print(){
  for (int i=0;i<graph.size();i++){
  cout <<i<<" --> ";
      graph[i].print();
  }
}

void network::print_format(string filename){
    ofstream file(filename);
    file << directly_copy_in_output;
    for(int i=0;i<graph.size();i++){
        Node* it = &graph[i];
        int v = 1;
        file << "probability (  "<<it->Node_Name<<" ";
        for(int i=0;i<it->Parents.size();i++){
            file<<" "<<it->Parents[i]<<" ";
            v++;
        }
        file <<") { //"<<v<<" variable(s) and "<<it->CPT.size()<<" values\n";
        file <<"\ttable ";
        for(int i=0;i<it->CPT.size();i++){
            file << std::fixed << std::setprecision(4) << (double)(it->CPT[i].first)/((it->CPT[i].second))<<" ";
        }
        file<<";\n}\n";
    }
}


// Given the network object and the values of observables, returns the probability using markovian blanket
vector<double> network::pMarkovian(int node_number, vector<string> &values){
    string temp_val = values[node_number];

    Node* node = &(graph[node_number]);

    vector<double> node_probabilities;

    vector<string> children_names;

    int numChildren = node->Children.size();

    for(int i=0; i<node->nvalues; ++i){
        values[node_number] = node->values[i];

        vector<string> parent_of_node_values;
        string node_value = values[node_number];

        vector<int> parents_of_node_indices = node->parent_indices;
        int num_parents_of_node = node->parent_indices.size();

        for(int k = 0; k<num_parents_of_node; ++k){
            parent_of_node_values.push_back(values[parents_of_node_indices[k]]);
        }

        int cpt_index_temp = node->calculate_cpt_index(node_value, parent_of_node_values, this);

        pdd node_prob_pair = node->CPT[cpt_index_temp];

        double node_prob = (node_prob_pair.first)/(node_prob_pair.second);



        double child_prob_product = 1;
        for(int j=0; j<numChildren; ++j){

            int child_absolute_position = node->Children[j];
            Node* child = &(graph[child_absolute_position]);
            string childValue = values[child_absolute_position];

            vector<string> parentValues;

            vector<int> parents_of_child_indices = child->parent_indices;

            int num_parents_of_children = parents_of_child_indices.size();

            for(int k = 0; k<num_parents_of_children; ++k){
                parentValues.push_back(values[parents_of_child_indices[k]]);
            }

            int cpt_index = child->calculate_cpt_index(childValue, parentValues, this);

            pdd child_prob_pair = child->CPT[cpt_index];

            double child_prob = (child_prob_pair.first)/(child_prob_pair.second);

            child_prob_product *= child_prob;

        }

        double prob = node_prob * child_prob_product;

        node_probabilities.push_back(prob);


    }


    values[node_number] = temp_val;

    return node_probabilities;


}
