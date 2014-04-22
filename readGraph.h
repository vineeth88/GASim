#include "incl.h"

using namespace std;

class graphNode {
	public:
	int branch_index;
	uint node_index;
	
	uint num_parent;
	vector<uint> parent_index;
	uint num_child;
	vector<uint> child_index;
	
	bool if_else;

	// condition to be added here

	graphNode() {
		branch_index = -2;
		node_index = 0;
		num_parent = num_child = 0;
		parent_index = vector<uint>();
		child_index = vector<uint>();
	}

	graphNode(int branch_, uint index_, bool if_else_) {
		branch_index = branch_;
		node_index = index_;
		num_parent = num_child = 0;
		parent_index = vector<uint>();
		child_index = vector<uint>();
	}

	void printNode() {
		cout << "Node# " << node_index 
			 << " [" << branch_index << "] " << endl
			 << num_parent <<" Parents: ";
		for (vector<uint>::iterator it = parent_index.begin(); 
			it != parent_index.end(); it++)
			cout << *it << " ";
		cout << endl;
	
		if (if_else)
			cout << "Else branch" << endl;
		else
			cout << "If branch" << endl;
	
		cout << num_child <<" Children: ";
		for (vector<uint>::iterator it = child_index.begin(); 
			it != child_index.end(); it++)
			cout << *it << " ";
		cout << endl << "--------------" << endl;
	}

};

class graph {
	
	public:

	uint numNodes;
	vector<graphNode> gNodes;
	uint rootNode;
	set<uint> leafNodes;

	graph() {
		// The root node is 0. Therefore nodes should start from gNodes[1]
		gNodes = vector<graphNode>(1);
		numNodes = 0;
		rootNode = 0;
	}

	graph(uint num_, uint root_) {
		numNodes = num_;
		rootNode = root_;
		gNodes = vector<graphNode>(numNodes);
	}

	bool isLeafNode(uint index_);
	void getDominators(uint, vector<uint>&);
};

int readGraph(graph&, char[]);

