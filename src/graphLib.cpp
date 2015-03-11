#include "graphLib.h"

using namespace std;

gNode_t::gNode_t() {
	branch_index = -2;
	node_index = 0;
	num_parent = num_child = 0;
	parent_index = vector<uint>();
	child_index = vector<uint>();
}

gNode_t::gNode_t(int branch_, uint index_, bool if_else_) {
	branch_index = branch_;
	node_index = index_;
	num_parent = num_child = 0;
	parent_index = vector<uint>();
	child_index = vector<uint>();
}

void gNode_t::printNode() {
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

covGraph_t::covGraph_t() {
	// The root node is 0. Therefore nodes should start from gNodes[1]
	gNodes = vector<gNode_t>(1);
	numNodes = 0;
	rootNode = 0;

	branchMap = vector<int>(CONST_NUM_BRANCH);
}

covGraph_t::covGraph_t(uint num_, uint root_) {
	numNodes = num_;
	rootNode = root_;
	gNodes = vector<gNode_t>(numNodes);

	branchMap = vector<int>(CONST_NUM_BRANCH);
}

gNode_t& covGraph_t::operator [](int index_) {
	assert((index_ > 0) && ((uint)index_ < gNodes.size()));
	return gNodes[index_];
}

void covGraph_t::getDominators(uint index_, vector<uint> &domList) {
	
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
			

void getPath(const covGraph_t& graphCov, int target_branch, vector<uint>& target_path) {
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

}

int readGraph(covGraph_t& graphCov) {
	ifstream graphFile;
	char fileName[100];
	
	sprintf(fileName, "%s", graphCov.fName);
	graphFile.open(fileName, ifstream::in);
	if (!graphFile) {
		cout << "Unable to open file " << fileName << endl;
		//exit(-1);
		return (1 << 15);
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
		gNode_t tmpNode;
		
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

//		tmpNode.printNode();
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

void covGraph_t::getPath(int branch_, vector<int>& pathVec) {

	int target_node = branchMap[branch_];
	int curr_node = target_node;

	while (curr_node) {
		
		int bIndex = gNodes[curr_node].branch_index;
		if (bIndex != -1)
			pathVec.push_back(bIndex);
		
		curr_node = gNodes[curr_node].parent_index[0];

	}

}

int covGraph_t::getTopNode(int branch_) {

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


void bEdge_t::printEdge() {
	cout << "Edge " << bIndex
		 << " Top " << startTop 
		 << " End " << endTop << endl;
}

bNode_t::bNode_t(int index_) :
	bIndex(index_) {
	outEdges = int_vec();
	outNodes = int_vec();
	inNodes = int_vec();
	inEdges = int_vec();
}

void bNode_t::addOutEdge(int edge_index_, int node_index_) {
	outNodes.push_back(node_index_);
	outEdges.push_back(edge_index_);
	if (outNodes.size() != outEdges.size()) {
		cout << "AF " << outNodes.size() << " " << outEdges.size() << endl;
		assert(0);
	}
}

void bNode_t::addInEdge(int edge_index_, int node_index_) {
	assert(inNodes.size() == inEdges.size());
	inNodes.push_back(node_index_);
	inEdges.push_back(edge_index_);
}

brGraph_t::brGraph_t() {
	nodeMap = int_vec(CONST_NUM_BRANCH, -1);
	edgeMap = int_vec(CONST_NUM_BRANCH, -1);
	branchType = int_vec(CONST_NUM_BRANCH, 0);
}

int brGraph_t::addEdge(int index_, int start_, int end_) {
	assert(index_ < CONST_NUM_BRANCH);
	bEdge_t newEdge(index_, start_, end_);
	bEdges.push_back(newEdge);
	edgeMap[index_] = bEdges.size() - 1;

	bNodes[nodeMap[start_]].addOutEdge(index_, end_);
	bNodes[nodeMap[end_]].addInEdge(index_, start_);
	return edgeMap[index_];
}

int brGraph_t::addNode(int index_) {
	assert(index_ < CONST_NUM_BRANCH);
	if (nodeMap[index_] == -1) {
		bNode_t newNode(index_);
		bNodes.push_back(newNode);
		nodeMap[index_] = bNodes.size() - 1;
	}
	return nodeMap[index_];
}

bEdge_t* brGraph_t::getEdge(int index_) {
	int val = edgeMap[index_];
	if (val == -1)
		return NULL;
	
	return &bEdges[val];
}

bNode_t* brGraph_t::getNode(int index_) {
	int val = nodeMap[index_];
	if (val == -1)
		return NULL;
	
	return &bNodes[val];
}

void brGraph_t::printGraph() {
	
	ofstream outFile;
	char gName[100];

	sprintf(gName, "%s.gv", benchCkt);
	outFile.open(gName, ios::out);

	if(!outFile) {
		cout << "Unable to write graph into file " << gName << endl;
		return;
	}

	outFile << "digraph " << benchCkt << "{" << endl;
	for (vector<bNode_t>::iterator it = bNodes.begin();
			it != bNodes.end(); ++it)    {
		outFile << it->bIndex
				<< " [label=\"" << it->bIndex
				<< "\"];" << endl;
	}

	for (vector<bEdge_t>::iterator it = bEdges.begin();
			it != bEdges.end(); ++it)    {
		outFile << it->startTop << "->" << it->endTop 
				<< " [label=\"" << it->bIndex << "\"];"
				<< endl;
	}

	outFile << "}";
	outFile.close();

}
