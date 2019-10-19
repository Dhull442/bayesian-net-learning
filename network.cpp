#include<bits/stdc++.h>
#include "network.h"

using namespace std;

string directly_copy_in_output = "";


// The whole network represted as a list of nodes


int network::addNode(Graph_Node node){
    Pres_Graph.push_back(node);
    return 0;
}

int network::netSize(){
    return Pres_Graph.size();
}

// get the index of node with a given name
int network::get_index(string val_name){
    list<Graph_Node>::iterator listIt;
    int count=0;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        if(listIt->get_name().compare(val_name)==0)
            return count;
        count++;
    }
    return -1;
}

// get the node at nth index
list<Graph_Node>::iterator network::get_nth_node(int n){
    list<Graph_Node>::iterator listIt;
    int count=0;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        if(count==n)
            return listIt;
        count++;
    }
    return listIt;
}

//get the iterator of a node with a given name
list<Graph_Node>::iterator network::search_node(string val_name){
    list<Graph_Node>::iterator listIt;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
    if(listIt->get_name().compare(val_name)==0)
        return listIt;
    }
    cout<<"node not found\n";
    return listIt;
}

void network::print(){
    int i=0;
    for (list<Graph_Node>::iterator it = Pres_Graph.begin(); it != Pres_Graph.end(); ++it){
    cout <<i<<" --> ";
        it->print();
        i++;

    }
}

void network::print_format(string filename){
    ofstream file(filename);
    time_t now = time(0);
    char* dt = ctime(&now);

    file << directly_copy_in_output;


    for (list<Graph_Node>::iterator it = Pres_Graph.begin(); it != Pres_Graph.end(); ++it){
        // file << "noth"<<endl;
        int v = 1;
        file << "probability (  "<<it->Node_Name<<" ";
        for(int i=0;i<it->Parents.size();i++){
            file<<" "<<it->Parents[i]<<" ";
            v ++;
        }
        file <<") { //"<<v<<" variable(s) and "<<it->CPT.size()<<" values\n";
        file <<"\ttable ";
        for(int i=0;i<it->CPT.size();i++){
            file << std::fixed << std::setprecision(4) << (double)(it->CPT[i].first)/((it->CPT[i].second))<<" ";
        }
        file<<";\n}\n";
    }
}