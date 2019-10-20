#include "network.h"
#include "Node.h"


#include <bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

typedef std::pair<double,double> pdd;
#define PB push_back
//String to directly copy in output before writing the probability tables
extern string directly_copy_in_output;

class training_input_reader{

public:

	string filename;
	network* network_to_be_trained;
	vector< vector<string> > datf;

	// ith element gives the position of missing element in ith row, if no value missing, then gives -1
	vector<int> missing_values_position;

	training_input_reader(string filename, network& network_to_be_trained){
		this->filename = filename;
		this->network_to_be_trained = &network_to_be_trained;
		initialise_network();
	}


	void update_net(vector< string > sample, double wt){
		
		int n = sample.size();
		for(int i=0; i<n; ++i){
			// cout<<"Bye1"<<endl;
			// cout<<"Bye"<<endl;
			Node* node = &(network_to_be_trained->graph[i]);
			// cout<<i<<endl;
			int num_parents = node->parent_indices.size();
			
			vector<string> parent_values(num_parents);
			for(int j=0; j<num_parents; ++j){

				int index_of_jth_parent = node->parent_indices[j];

				parent_values[j] = sample[index_of_jth_parent];

			}
			
			// cout<<node->values.size()<<endl;
			vector<string> values_of_node = node->values;
			// cout<<node->values[node->values.size()-1]<<endl;
			// cout<<sample[i]<<endl;
			// cout<<node->value_to_int_mapping[sample[i]]<<endl;
			
			int node_value_number = node->value_to_int_mapping[sample[i]];

			int n2 = values_of_node.size();

			// cout<<"Hi"<<endl;
			for(int j=0; j<n2; ++j){
				// cout<<"Hi2"<<endl;
				// cout<<values_of_node[j]<<endl;

				int cpt_index = node->calculate_cpt_index(values_of_node[j], parent_values, *network_to_be_trained);
				cout<<cpt_index<<endl;
				// int cpt_index = 0;

				// cout<<(node->CPT[cpt_index]).first<<" "<<(node->CPT[cpt_index]).second<<endl;

				(node->CPT[cpt_index]).second += wt;

				if(node_value_number == j + 1){
					(node->CPT[cpt_index]).first += wt;
				}
			}
			// cout<<endl;
			
		}

		
	}

	void train_loop(){
		for(int i=0;i<missing_values_position.size();i++){
			// cout<<i<<endl;
			if(missing_values_position[i]==-1){
				
				update_net(datf[i],1);
			}
			else{
				cout<<"Hi"<<endl;
				vector< double > tmp = network_to_be_trained->pMarkovian(missing_values_position[i],datf[i]);
				cout<<"Hi2"<<endl;
				vector<string> sample = datf[i];
				// for(int l=0; l<sample.size(); ++l){
				// 		cout<<sample[l]<<" 0 ";
				// }
				// cout<<endl;
				assert(network_to_be_trained->graph[missing_values_position[i]].values.size() == tmp.size());
				for(int j=0;j<tmp.size();j++){
					sample[missing_values_position[i]] = network_to_be_trained->graph[missing_values_position[i]].values[j];
					int wt = tmp[j];
				
					// cout<<wt<<endl;
					
					update_net(sample,wt);
					
				}
			}
			
		}
	}

	void initialise_network_cpt_by_one_sample(vector<string> sample, double sample_weight){
		// ! Incomplete

		


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
					// val = val.substr(1, val.size() - 2);
					if (val == "\"?\""){
						missing_values_position.PB(vals.size());
						is_some_value_missing = true;
					}
					vals.PB(val);	// Question mark is also pushed back, so as to maintain the ordering
				}
				if(!is_some_value_missing){
					missing_values_position.PB(-1);
				}
				datf.PB(vals);
			}
		}
		dat.close();
	}
	void print(){
		network_to_be_trained->print_format("solved_alarm.bif");
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
	int node_counter = 0;
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

				Alarm.addNode(new_node);

				//Updating the map
				parent_name_index_map[name] = node_counter;
				++node_counter;
			}
			else if(is_probability){
				ss>>temp;
				ss>>temp;
				// listIt=Alarm.search_node(temp);
				int index=parent_name_index_map[temp];
				ss>>temp;
				values.clear();
				//Stores the indices of parents of node
				vector<int> indices_values;
				while(temp.compare(")")!=0)
				{
					// listIt1=Alarm.search_node(temp);
					int index1 = parent_name_index_map[temp];
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
	training_input_reader t(datFile,Alarm);
	int i=0;

	cout<<"Here"<<endl;
	for(int i=0; i<Alarm.graph.size(); ++i){
		Alarm.graph[i].print();
		cout<<endl;
	}

	while(i<10){
		// Update whole
		// cout<<"Hi"<<endl;
		t.train_loop();
		// cout<<"Bye"<<endl;
		// cout<<i<<endl;
		++i;
	}
	t.print();
}
