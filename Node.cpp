#include<bits/stdc++.h>
#include "Node.h"
using namespace std;

typedef pair<double,double> pdd;

// Our graph consists of a list of nodes where each node is represented as follows:



// Constructor- a node is initialised with its name and its categories
Node::Node(string name,int n,vector<string> vals)
{
    Node_Name=name;
    nvalues=n;
    values=vals;
    for(int i=0; i<n; ++i){
        value_to_int_mapping[vals[i]] = i + 1;
    }

}
string Node::get_name()
{
    return Node_Name;
}
vector<int> Node::get_children()
{
    return Children;
}
vector<string> Node::get_Parents()
{
    return Parents;
}
vector<pdd> Node::get_CPT()
{
    return CPT;
}
int Node::get_nvalues()
{
    return nvalues;
}
vector<string> Node::get_values()
{
    return values;
}
void Node::set_CPT(vector<pdd>& new_CPT)
{
    CPT.clear();
    CPT=new_CPT;
}
void Node::set_Parents(vector<string>& Parent_Nodes)
{
    Parents.clear();
    Parents=Parent_Nodes;
}
void Node::set_parent_indices(vector<int>& Parent_Nodes)
{
    parent_indices.clear();
    parent_indices=Parent_Nodes;
}
// add another node in a graph as a child of this node
int Node::add_child(int new_child_index )
{
    for(int i=0;i<Children.size();i++)
    {
        if(Children[i]==new_child_index)
            return 0;
    }
    Children.push_back(new_child_index);
    return 1;
}

// Given the values of parents and the network object, return the relevant cpt_index
int Node::calculate_cpt_index(string node_value, vector<string> &parent_values, network &network_object){
    int cpt_index = 1;
    int num_parents = parent_values.size();
    int temp = 1;

    for(int i=num_parents-1; i>=0; --i){
        int parent_index_in_network = parent_indices[i];

        Node* parent = &(network_object.graph[parent_index_in_network]);
        int curr_value = parent->value_to_int_mapping[parent_values[i]];
        int max_value = parent->nvalues;

        cpt_index += temp*(curr_value-1);

        temp *= max_value;



    }

    int curr_value = value_to_int_mapping[node_value];
    int max_value = nvalues;

    cpt_index += temp*(curr_value-1);

    return cpt_index - 1; // Subtracting 1 because the actual index should start from 0


}






void Node::print(){
    cout << "{ "<<this->Node_Name<<","<<" -> " << "( Child: )";
    cout << Node_Name<<" "<<Children.size()<<"->";
    for(int i=0;i<Children.size();i++){
        cout <<Children[i]<<",";
    }
    cout << " "<< Parents.size()<<"->";
    for(int i=0;i<Parents.size();i++){
        cout << Parents[i]<<",";
    }
    cout << " "<<nvalues << "->";
    for(int i=0;i<values.size();i++){
        cout << values[i]<<",";
    }
    cout << " ";
    for(int i=0;i<CPT.size();i++){
        cout<< CPT[i].first<<",";
    }
    cout << endl;
}
