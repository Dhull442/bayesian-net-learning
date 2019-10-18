#include <bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;


//String to directly copy in output before writing the probability tables
string directly_copy_in_output = "";

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

public:
	string Node_Name;  // Variable name
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;

		nvalues=n;
		values=vals;


	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }

		void print(){
			// cout << "{ "<<this.Node_Name<<","<<<<" -> " << "( Child: )"
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
				cout << CPT[i]<<",";
			}
			cout << endl;
		}



};


 // The whole network represted as a list of nodes
class network{
public:
	list <Graph_Node> Pres_Graph;
	string information;

	int addNode(Graph_Node node){
		Pres_Graph.push_back(node);
		return 0;
	}

	int netSize(){
		return Pres_Graph.size();
	}

  	// get the index of node with a given name
	int get_index(string val_name){
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
  	list<Graph_Node>::iterator get_nth_node(int n){
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
  list<Graph_Node>::iterator search_node(string val_name){
    list<Graph_Node>::iterator listIt;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
      if(listIt->get_name().compare(val_name)==0)
        return listIt;
    }
    cout<<"node not found\n";
    return listIt;
  }

	void print(){
		int i=0;
		for (list<Graph_Node>::iterator it = Pres_Graph.begin(); it != Pres_Graph.end(); ++it){
  		cout <<i<<" --> ";
			it->print();
			i++;

		}
	}

	void print_format(string filename){
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
				file << std::fixed << std::setprecision(4) << it->CPT[i]<<" ";
			}
			file<<";\n}\n";
		}
	}
	void process(string filename){
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
					vals.push_back(val);
				}
				cout << i <<" -- " <<vals.size() <<endl;
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
				while(temp.compare(")")!=0)
				{
					listIt1=Alarm.search_node(temp);
					listIt1->add_child(index);
					values.push_back(temp);
					ss>>temp;
				}
				listIt->set_Parents(values);
				getline (myfile,line);
				stringstream ss2;
				ss2.str(line);
				ss2>> temp;
				ss2>> temp;
				vector<float> curr_CPT;
				string::size_type sz;
				while(temp.compare(";")!=0)
				{
					float v = atof(temp.c_str());
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
	Alarm.process("resources_given/records.dat");
}
