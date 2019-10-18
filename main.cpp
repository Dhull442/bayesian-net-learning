#include "network.h"
#include "Graph_Node.h"


#include <bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

//String to directly copy in output before writing the probability tables


extern string directly_copy_in_output;

class training_input_reader{

public:

	string filename;
	// ith element gives the position of missing element in ith row
	vector<int> missing_values_position;

	training_input_reader(string filename){
		this->filename = filename;

		process();
	}

	void process(){
		ifstream dat(filename);
		int i=0;
		if(dat.is_open()){
			while(!dat.eof()){
				string line;
				getline(dat,line);
				stringstream ss(line);
				vector< string > vals;
				string val;
				while(ss>>val){
					assert(val.size() >= 2);
					val = val.substr(1, val.size() - 2);
					// if (val == "?"){

					// }
					vals.push_back(val);
				}
				// cout << i <<" -- " <<vals.size() <<endl;
			}
		}
		dat.close();
	}

};

network read_network(){
	network Alarm;
	string line;
	int find=0;
	string info = "";
	ifstream myfile("resources_given/alarm.bif");
	bool to_continue_directly_copying = true;
	string temp;
	string name;
	vector<string> values;
	srand(time(0));

	map<string, int> parent_name_index_map; 

	if (myfile.is_open()){
		while (!myfile.eof()){
			stringstream ss;
			getline (myfile,line);
			ss.str(line);
			ss>>temp;
			bool is_probability = temp.compare("probability")==0;
			bool to_copy_line = !is_probability && to_continue_directly_copying;

			if(to_copy_line)
				directly_copy_in_output += line + "\n";

			if(is_probability){
				to_continue_directly_copying = false;
			}

			if(temp.compare("variable")==0){
				ss>>name;
				getline (myfile,line);
				if (to_copy_line)
					directly_copy_in_output += line + "\n";
				stringstream ss2;
				ss2.str(line);
				for(int i=0;i<4;i++){
					ss2>>temp;
				}
				values.clear();
				while(temp.compare("};")!=0){
					values.push_back(temp);
					ss2>>temp;
				}
				Graph_Node new_node(name,values.size(),values);
				
				int pos=Alarm.addNode(new_node);
				
				//Updating the map
				parent_name_index_map[name] = pos;
			}
			else if(is_probability){
				ss>>temp;
				ss>>temp;
				list<Graph_Node>::iterator listIt;
				list<Graph_Node>::iterator listIt1;
				listIt=Alarm.search_node(temp);
				int index=Alarm.get_index(temp);
				ss>>temp;
				values.clear();
				//Stores the indices of parents of node
				vector<int> indices_values;
				while(temp.compare(")")!=0)
				{
					listIt1=Alarm.search_node(temp);
					listIt1->add_child(index);
					values.push_back(temp);
					indices_values.push_back(parent_name_index_map[temp]);
					ss>>temp;
				}
				listIt->set_Parents(values);
				listIt->set_parent_indices(indices_values);
				getline (myfile,line);
				stringstream ss2;
				ss2.str(line);
				ss2>> temp;
				ss2>> temp;
				vector<double> curr_CPT;
				string::size_type sz;
				while(temp.compare(";")!=0)
				{
					double v = stod(temp);
					if(v<0){
						// v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						// srand (static_cast <unsigned> (time(0)));
						v = abs(cos(100*sin(rand())));
						// cout << time(0)<<endl;
					}
					curr_CPT.push_back(v);
					ss2>>temp;
				}
				listIt->set_CPT(curr_CPT);
			}
		}
		if(find==1)
			myfile.close();
	}
	return Alarm;
}



int main()
{
	network Alarm;
	Alarm=read_network();

// Example: to do something
	// cout<<"Perfect! Hurrah! \n";
	Alarm.print_format("resources_given/solved_alarm.bif");
	// Alarm.print();
	training_input_reader training_data_object("resources_given/records.dat");
}
