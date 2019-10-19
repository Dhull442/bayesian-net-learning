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
