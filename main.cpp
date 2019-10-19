#include "network.h"
#include "Node.h"


#include <bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

typedef std::pair<double,double> pdd;

//String to directly copy in output before writing the probability tables
extern string directly_copy_in_output;

class training_input_reader{

public:

	string filename;
	network network_to_be_trained;

	// ith element gives the position of missing element in ith row, if no value missing, then gives -1
	vector<int> missing_values_position;

	training_input_reader(string filename, network& network_to_be_trained){
		this->filename = filename;
		this->network_to_be_trained = network_to_be_trained;

		initialise_network();
	}

	void initialise_network_cpt_by_one_sample(vector<string> sample, double sample_weight){
		// ! Incomplete

		int n = sample.size();
		for(int i=0; i<n; ++i){

			Node* node = &network_to_be_trained.graph[i];

			int num_parents = node->parent_indices.size();
			vector<string> parent_values(num_parents);
			for(int j=0; j<num_parents; ++j){

				int index_of_jth_parent = node->parent_indices[j];

				parent_values[j] = sample[index_of_jth_parent];

			}



			vector<string> values_of_node = node->values;
			
			int node_value_number = node->value_to_int_mapping[sample[i]];

			int n2 = values_of_node.size();

			for(int j=0; j<n2; ++j){

				int cpt_index = node->calculate_cpt_index(values_of_node[j], parent_values, network_to_be_trained);

				(node->CPT[cpt_index]).second += sample_weight;

				if(node_value_number == j + 1){
					(node->CPT[cpt_index]).first += sample_weight;
				}
			}


		}


	}

	void initialise_network(){	// Instead of random initialisation, initialised using a uniform prior

		// ! Incomplete
		ifstream dat(filename);
		int i=0;
		if(dat.is_open()){
			while(!dat.eof()){
				string line;
				getline(dat,line);
				bool is_some_value_missing = false;
				stringstream ss(line);
				vector< string > vals;
				string val;
				while(ss>>val){
					assert(val.size() >= 2);
					val = val.substr(1, val.size() - 2);
					if (val == "?"){
						missing_values_position.push_back(vals.size());
						is_some_value_missing = true;
					}
					vals.push_back(val);	// Question mark is also pushed back, so as to maintain the ordering
				}

				if(!is_some_value_missing){
					missing_values_position.push_back(-1);
				}


				if(is_some_value_missing){

					
				}

				// for(int i=0; i<vals.size(); ++i){


				// }
				// cout << i <<" -- " <<vals.size() <<endl;
			}
		}
		dat.close();
	}

};

void read_network(network& Alarm,string filename){
	string line;
	int find=0;
	string info = "";
	ifstream myfile(filename);
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
				Node new_node(name,values.size(),values);

				int pos=Alarm.addNode(new_node);

				//Updating the map
				parent_name_index_map[name] = pos;
			}
			else if(is_probability){
				ss>>temp;
				ss>>temp;
				// listIt=Alarm.search_node(temp);
				int index=Alarm.get_index(temp);
				ss>>temp;
				values.clear();
				//Stores the indices of parents of node
				vector<int> indices_values;
				while(temp.compare(")")!=0)
				{
					// listIt1=Alarm.search_node(temp);
					int index1 = Alarm.get_index(temp);
					Alarm.graph[index1].add_child(index);
					values.push_back(temp);
					indices_values.push_back(parent_name_index_map[temp]);
					ss>>temp;
				}
				Alarm.graph[index].set_Parents(values);
				Alarm.graph[index].set_parent_indices(indices_values);
				int numvalues = Alarm.graph[index].nvalues;
				getline (myfile,line);
				stringstream ss2;
				ss2.str(line);
				ss2>> temp;
				ss2>> temp;
				vector<pdd> curr_CPT;
				string::size_type sz;
				while(temp.compare(";")!=0)
				{
					double v = stod(temp);
					// if(v<0){
					// 	// v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					// 	// srand (static_cast <unsigned> (time(0)));
					// 	v = 0.5;
					// 	// cout << time(0)<<endl;
					// }
					double numerator = 1;
					double denominator  = (double)numvalues;
					curr_CPT.push_back(pdd(numerator, denominator));	//Values given are random only,
										//hence we make choose intial values from a uniform prior
					ss2>>temp;
				}
				Alarm.graph[index].set_CPT(curr_CPT);
			}
		}
		if(find==1)
			myfile.close();
	}
}

int main(int argc, char** argv){
	assert(argc==3);
	network Alarm;
	string bifFile(argv[1]);
	read_network(Alarm,bifFile);
	string datFile(argv[2]);
	Alarm.print_format("solved_alarm.bif");
	// Alarm.print();
	// training_input_reader training_data_object("resources_given/records.dat", Alarm);
}
