#include "readGraph.h"

using namespace std;

int readGraph(graph& graphCov, char fileName[100]) {
	ifstream graphFile;
	//char fileName[100];
	
	sprintf(graphCov.fName, "%s.graph", fileName);
	graphFile.open(fileName, ifstream::in);
	if (!graphFile) {
		cout << "Unable to open file " << fileName << endl;
		exit(-1);
	}

	cout << "Reading file: " << fileName << endl;
	
	int num_graphs = 0;
	uint root_node;
	int num_nodes = 0;
	graphFile >> num_graphs >> root_node >> num_nodes;
	cout << "Num graphs: " << num_graphs << endl 
		 << "Root node: " << root_node << endl
		 << "Num nodes: " << num_nodes << endl;

	//graph graphCov;
	graphCov.rootNode = root_node;
	graphCov.numNodes = num_nodes;
	
	int if_else_tally = 0;
	for (int index = 1; index <= num_nodes; index++) {
		
		uint nodeInd_, nParent_, nChild_;
		int bInd_;
		graphNode tmpNode;
		
		graphFile >> nodeInd_ >> bInd_;
		tmpNode.node_index = nodeInd_;
		tmpNode.branch_index = bInd_;

		graphFile >> nParent_;
		tmpNode.num_parent = nParent_;
		for (uint i = 0; i < nParent_; i++) {
			uint tmpIndex;
			graphFile >> tmpIndex;
			tmpNode.parent_index.push_back(tmpIndex);
		}

		bool ifelse_;
		graphFile >> ifelse_;
		tmpNode.if_else = ifelse_;
		
		if (ifelse_)
			if_else_tally--;
		else
			if_else_tally++;
		
		graphFile >> nChild_;
		tmpNode.num_child = nChild_;
		for (uint i = 0; i < nChild_; i++) {
			uint tmpIndex;
			graphFile >> tmpIndex;
			tmpNode.child_index.push_back(tmpIndex);
		}

		//tmpNode.printNode();
		graphCov.gNodes.push_back(tmpNode);
		graphCov.branchMap[bInd_] = index;

	}
	graphFile.close();

	cout << "IF/ELSE: " << if_else_tally << endl;
	return if_else_tally;
}

void graph::getDominators(uint index_, vector<uint> &domList) {
	
	uint curr_node = index_;
	domList.clear();
	
	cout << endl << "Dominators for " << index_ << ": " << endl;
	while (curr_node) {
		uint nParent = gNodes[curr_node].num_parent;
		if (nParent == 1) {
			curr_node = gNodes[curr_node].parent_index[0];
			domList.push_back(curr_node);
			cout << curr_node << " ";
		}
		else {
			vector< vector<uint> >pDomList(nParent);
			cout << endl << "Multiple parents: ";
			for (uint i = 0; i < nParent; ++i)
				getDominators(gNodes[curr_node].parent_index[i], pDomList[i]);
			
		}
	}
	cout << endl;
}
			

void getPath(const graph& graphCov, int target_branch, vector<uint>& target_path) {
	uint target_node_index = 0;
	for (uint i = 0; i <= graphCov.numNodes; i++)
		if (graphCov.gNodes[i].branch_index == target_branch)
			target_node_index = i;//graphCov.gNodes[i].node_index;

	uint curr_node = target_node_index;
	while (curr_node) {
		if (graphCov.gNodes[curr_node].branch_index != -1)
			target_path.push_back(graphCov.gNodes[curr_node].node_index);
		curr_node = graphCov.gNodes[curr_node].parent_index[0];
	}

//	cout << "Path for branch " << target_branch << "(" << target_node_index << ")" << endl;
//	printVal(target_path);

}

int readGraph(graph& graphCov) {
	ifstream graphFile;
	char fileName[100];
	
	sprintf(fileName, "%s", graphCov.fName);
	graphFile.open(fileName, ifstream::in);
	if (!graphFile) {
		cout << "Unable to open file " << fileName << endl;
		exit(-1);
	}

	cout << "Reading file: " << fileName << endl;
	
	int num_graphs = 0;
	uint root_node;
	int num_nodes = 0;
	graphFile >> num_graphs >> root_node >> num_nodes;
	cout << "Num graphs: " << num_graphs << endl 
		 << "Root node: " << root_node << endl
		 << "Num nodes: " << num_nodes << endl;

	//graph graphCov;
	graphCov.rootNode = root_node;
	graphCov.numNodes = num_nodes;
	
	int if_else_tally = 0;
	for (int index = 1; index <= num_nodes; index++) {
		
		uint nodeInd_, nParent_, nChild_;
		int bInd_;
		graphNode tmpNode;
		
		graphFile >> nodeInd_ >> bInd_;
		tmpNode.node_index = nodeInd_;
		tmpNode.branch_index = bInd_;

		graphFile >> nParent_;
		tmpNode.num_parent = nParent_;
		for (uint i = 0; i < nParent_; i++) {
			uint tmpIndex;
			graphFile >> tmpIndex;
			tmpNode.parent_index.push_back(tmpIndex);
		}

		bool ifelse_;
		graphFile >> ifelse_;
		tmpNode.if_else = ifelse_;
		
		if (ifelse_)
			if_else_tally--;
		else
			if_else_tally++;
		
		graphFile >> nChild_;
		tmpNode.num_child = nChild_;
		for (uint i = 0; i < nChild_; i++) {
			uint tmpIndex;
			graphFile >> tmpIndex;
			tmpNode.child_index.push_back(tmpIndex);
		}

		tmpNode.printNode();
		graphCov.gNodes.push_back(tmpNode);
		if (bInd_ != -1)
			graphCov.branchMap[bInd_] = index;
		
		char c = 1;
		while (c != ';') {
			graphFile >> c;
		}
	}
	graphFile.close();

	cout << "IF/ELSE: " << if_else_tally << endl;
	return if_else_tally;
}

void graph::getPath(int branch_, vector<int>& pathVec) {

	int target_node = branchMap[branch_];
	int curr_node = target_node;

	while (curr_node) {
		
		int bIndex = gNodes[curr_node].branch_index;
		if (bIndex != -1)
			pathVec.push_back(bIndex);
		
		curr_node = gNodes[curr_node].parent_index[0];

	}

}

int graph::getTopNode(int branch_) {

	int target_node = branchMap[branch_];
	int curr_node = target_node;

	int topNode = branch_, top2 = branch_;
	while (curr_node) {
		
		int bIndex = gNodes[curr_node].branch_index;
		if (bIndex != -1) {
			topNode = top2;
			top2 = bIndex;
		}
		
		curr_node = gNodes[curr_node].parent_index[0];

	}
	
	return topNode; 
}
