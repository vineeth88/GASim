#include "incl.h"
#include "verInclude.h"
#include "vType.h"

using namespace std;

class gNode_t {
	public:
	int branch_index;
	uint node_index;
	
	uint num_parent;
	vector<uint> parent_index;
	uint num_child;
	vector<uint> child_index;
	
	bool if_else;

	gNode_t();
	gNode_t(int, uint, bool);
	void printNode();
};

class covGraph_t {
	
	public:
	
	char fName[100];

	uint numNodes;
	vector<gNode_t> gNodes;
	uint rootNode;

	vector<int> branchMap;

	covGraph_t();
	covGraph_t(uint, uint);
	gNode_t& operator [](int);

	void getDominators(uint, vector<uint>&);
	void getPath(int, vector<int>&);
	int getTopNode(int);

};

int readGraph(covGraph_t&);

struct brGraph_t;
struct bNode_t;
struct bEdge_t;

struct bEdge_t {
	string startStr, endStr;
	int bIndex;	
	int startTop, endTop;

	bEdge_t(int index_) : 
		bIndex(index_){ };

	bEdge_t(int index_, int start_, int end_) :
		bIndex(index_), startTop(start_), endTop(end_) { };

	void printEdge();

//	void addStartState(string state_) {
//	// TODO	
//	}
//
//	void addEndState(string state_) {
//	// TODO
//	}
};

struct bNode_t {
	string state_val;

	int bIndex;
	int_vec outNodes;
	int_vec outEdges;
	int_vec inNodes;
	int_vec inEdges;

	bNode_t(int);
	void addOutEdge(int, int);
	void addInEdge(int, int);

};

struct brGraph_t {
	vector<bNode_t> bNodes;
	vector<bEdge_t> bEdges;
	int_vec nodeMap;
	int_vec edgeMap;
	int_vec branchType;
	// 0 - None, 1 - Edge, 2 - Node, 3 - Both

	brGraph_t();
	int addEdge(int, int, int);
	int addNode(int);
	bEdge_t* getEdge(int);
	bNode_t* getNode(int);
	void printGraph();

};
