#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

#include "readGraph.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
int_vec leafBranch(CONST_NUM_BRANCH, 0);

int gaIndiv_t::mem_alloc_cnt = 0;
int state_t::mem_alloc_cnt = 0;
//int Vtop::mem_alloc_cnt = 0;

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
		
	void printEdge() {
		cout << "Edge " << bIndex
			 << " Top " << startTop 
			 << " End " << endTop << endl;
	}

	void addStartState(string state_) {
	// TODO	
	}

	void addEndState(string state_) {
	// TODO
	}

};

struct bNode_t {
	string state_val;

	int bIndex;
	int_vec outNodes;
	int_vec outEdges;
	int_vec inNodes;
	int_vec inEdges;

	bNode_t(int index_) :
		bIndex(index_) {
		outEdges = int_vec();
		outNodes = int_vec();
		inNodes = int_vec();
		inEdges = int_vec();
	}

	void addOutEdge(int edge_index_, int node_index_) {
		outNodes.push_back(node_index_);
		outEdges.push_back(edge_index_);
		if (outNodes.size() != outEdges.size()) {
			cout << "AF " << outNodes.size() << " " << outEdges.size() << endl;
			assert(0);
		}
	}

	void addInEdge(int edge_index_, int node_index_) {
		assert(inNodes.size() == inEdges.size());
		inNodes.push_back(node_index_);
		inEdges.push_back(edge_index_);
	}
};

struct brGraph_t {
	vector<bNode_t> bNodes;
	vector<bEdge_t> bEdges;
	int_vec nodeMap;
	int_vec edgeMap;
	int_vec branchType;
	// 0 - None, 1 - Edge, 2 - Node, 3 - Both

	brGraph_t() {
		nodeMap = int_vec(CONST_NUM_BRANCH, -1);
		edgeMap = int_vec(CONST_NUM_BRANCH, -1);
		branchType = int_vec(CONST_NUM_BRANCH, 0);
	}

	int addEdge(int index_, int start_, int end_) {
		assert(index_ < CONST_NUM_BRANCH);
//		if (//(branchType[index_] & 0x01) && 
//			(edgeMap[index_] == -1)) {
			bEdge_t newEdge(index_, start_, end_);
			bEdges.push_back(newEdge);
			edgeMap[index_] = bEdges.size() - 1;

			bNodes[nodeMap[start_]].addOutEdge(index_, end_);
			bNodes[nodeMap[end_]].addInEdge(index_, start_);
			return edgeMap[index_];
//		}
//		else
//			return false;
	}

	int addNode(int index_) {
		assert(index_ < CONST_NUM_BRANCH);
		if (//(branchType[index_] & 0x02) && 
			(nodeMap[index_] == -1)) {
			bNode_t newNode(index_);
			bNodes.push_back(newNode);
			nodeMap[index_] = bNodes.size() - 1;
		}
		return nodeMap[index_];
	}

	bEdge_t* getEdge(int index_) {
		int val = edgeMap[index_];
		if (val == -1)
			return NULL;
		
		return &bEdges[val];
	}

	bNode_t* getNode(int index_) {
		int val = nodeMap[index_];
		if (val == -1)
			return NULL;
		
		return &bNodes[val];
	}

	void printGraph() {
		
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

};
class Stage1_Param {

public:

	string fName; 

	int 	NUM_INDIV, INDIV_LEN;
	int 	NUM_TOP_INDIV;
	int 	NUM_GEN, POP_SIZE;

	vecIn_t			inputVec;
	int_vec			branchHit;

	state_pVec		stateList;	
	
	Stage1_Param() {
		branchHit = int_vec(CONST_NUM_BRANCH, 0);
		stateList = state_pVec();

	}

	~Stage1_Param() {
		branchHit = int_vec();
		for (int st = 0; (uint)st < stateList.size(); ++st) {
			if (stateList[st]) {
				delete stateList[st];
				stateList[st] = NULL;
			}
		}
//		for(state_pVec_iter it = stateList.begin(); it != stateList.end(); ++it)
//			if (*it) {
//				delete *it;
//				*it = NULL;
//			}
	}

};

class Stage2_Param : public Stage1_Param {
	
	state_t *endState;
	public:

	state_t 	*startState;
	int_vec		branchHit;
	int_vec		branchLeaf;
	set<int> 	favorSet;	

	brGraph_t	*branchGraph;

	Stage2_Param() {	
		startState = NULL;
		branchHit = int_vec(CONST_NUM_BRANCH, 0);
		branchLeaf = int_vec();
	}

	Stage2_Param(Stage1_Param* obj) {	
		NUM_INDIV 	= obj->NUM_INDIV;	
		NUM_GEN 	= obj->NUM_GEN;
		INDIV_LEN 	= obj->INDIV_LEN;
		POP_SIZE 	= obj->POP_SIZE;
		
		fName 		= obj->fName;

		startState = NULL;
		branchHit = obj->branchHit;
		branchLeaf = int_vec();
	}

	~Stage2_Param() {
		if (startState) {
			delete startState;
			startState = NULL;
		}
		delete branchGraph;
	}

};

void Stage1_GenerateVectors(Stage1_Param*);
void readParam(Stage1_Param*);
void readParam(Stage2_Param*);

void readTopNodes(graph&, brGraph_t&);
void getTarget(Stage2_Param*, int, set<int>&);
int getPathBFS(brGraph_t*, int, int, string&);

void readBranchGraph(brGraph_t&);
void Stage2_GenerateVectors(Stage2_Param*);

int checkCoverage(const vecIn_t&);
void DisplayMap(stateMap_t&);
void printVec(int_vec&);
void printSet(set<int>&);
void printCnt(int_vec&);
void PrintVectorSet(const vecIn_t& inputVec, bool printFlag = false);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

int main(int argc, char* argv[]) {

//#define random_seed
#ifdef random_seed
    uint seed = time(NULL);
    cout << seed << endl;
    srand(seed);
#endif

	char paramFile[100];
	sprintf(paramFile, "%s.param", benchCkt);

	if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " [<ckt>.param]" << endl
				<< "Required files for b11/b12 : " 
				<< benchCkt << ".graph, " 
				<< benchCkt << ".bg " << endl;
        }
        else {
            sprintf(paramFile, "%s", argv[1]);
            cout << "Using parameters from " << paramFile << endl;
        }
	}		
	
   	Stage1_Param *paramObj1 = new Stage1_Param;
	paramObj1->NUM_GEN = 8;
	paramObj1->POP_SIZE = 256;
	paramObj1->INDIV_LEN = 20;
	paramObj1->NUM_INDIV = 5;
	paramObj1->NUM_TOP_INDIV = 4;

	paramObj1->fName = string(paramFile);
	readParam(paramObj1);
	
	Stage1_GenerateVectors(paramObj1);
	PrintVectorSet(paramObj1->inputVec);

	cout << "Uncovered Branches: " << endl;
	int num_branch_uncovered = 0;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		if(IsDefaultBranch(ind))
			paramObj1->branchHit[ind] = -5;

		if (paramObj1->branchHit[ind] == 0) {
			cout << ind << " ";
			num_branch_uncovered++;
		}
	}
	cout << endl << "% Coverage " << (float) (CONST_NUM_BRANCH - num_branch_uncovered) * 100.0f / (float) (CONST_NUM_BRANCH) << endl;

	if (num_branch_uncovered == 0) {
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	}
	
	#if defined(__b10)
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	#endif
	
	cout << endl
		 << "Memory allocation details: " << endl
		 << "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t: " << state_t::mem_alloc_cnt << endl;

//	delete paramObj1;	

	graph covGraph;
	brGraph_t branchGraph;
	sprintf(covGraph.fName, "%s.graph", benchCkt);
	readGraph(covGraph);
	readTopNodes(covGraph, branchGraph);

	cout << "Stage 2 " << endl << endl;
	Stage2_Param *paramObj2 = new Stage2_Param(paramObj1);;
	paramObj2->inputVec = paramObj1->inputVec;
	paramObj2->branchGraph = &branchGraph;
	paramObj2->startState = paramObj1->stateList.back();
	
//	cout << "States reached in Stage 1" << endl;
//	for (int ind = 0; (uint)ind < paramObj1->stateList.size(); ++ind) {
//		cout << paramObj1->inputVec.substr(ind*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS) 
//			 << endl;
//		paramObj1->stateList[ind]->printState();
//		cout << "--" << endl;
//	}

	cout << "Last state reached at the end of Stage 1" << endl;
	paramObj2->startState->printState();
	
//	vector<int> unCovered;
//	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
//		if(IsDefaultBranch(ind))
//			paramObj1->branchHit[ind] = -5;
//
//		if (paramObj1->branchHit[ind] == 0) {
//			if (leafBranch[ind]) {
//				bEdge_t* edge = branchGraph.getEdge(ind);
//				unCovered.push_back(edge->startTop);
//			}
//		}
//	}

	set<int> unCoveredEdges;
	set<int> unCovered;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		if(IsDefaultBranch(ind))
			paramObj1->branchHit[ind] = -5;

		if (paramObj1->branchHit[ind] == 0) {
			if (leafBranch[ind]) {
				bEdge_t* edge = branchGraph.getEdge(ind);
				if (edge) {
					unCovered.insert(edge->startTop);
					unCoveredEdges.insert(ind);
				}
			}
		}
	}

	cout << "Edges: " << endl;
	for (int_vec_iter br = paramObj2->startState->branch_index.begin();
			br != paramObj2->startState->branch_index.end(); ++br) {
		if (leafBranch[*br]) {
			paramObj2->branchLeaf.push_back(*br);
			cout << *br << " ";
			
			bEdge_t * curr_edge = branchGraph.getEdge(*br);
			if(cur_edge) {
				int start_val = cur_edge->startTop;
				int end_val = cur_edge->endTop;
				cout << "(" << start_val << "-" << end_val << ")" << endl;

				bNode_t * end_node = branchGraph.getNode(end_val);
				assert(end_node);

				for (int_vec_iter it = end_node->outEdges.begin(); 
						it != end_node->outEdges.end(); ++it) {
					if (paramObj2->branchHit[*it] == 0) {
						cout << "Target = " << *it << endl;
					}
				}
						
				cout << "Edges (" << end_val << "): ";
				printVec((*end_node).outEdges);
				cout << endl;
				cout << "Nodes (" << end_val << "): ";
				printVec((*end_node).outNodes);
				cout << endl;

				map<int, string> pathVec;
				map<int, int> levelVec;

				//for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
				int min_lvl = (2 << 10), min_ind = -1;
				for (set<int>::iterator it = unCovered.begin(); it != unCovered.end(); ++it) {
					string path;
					int lvl = getPathBFS(&branchGraph, end_val, *it, path);
					cout << *it << ": " << lvl << " levels .. " << path << endl;

					if ((min_lvl > lvl) && (path.compare("Unreachable"))) {
						min_lvl = lvl;
						min_ind = *it;
					}

					pathVec.insert(make_pair(*it, path));
					levelVec.insert(make_pair(*it, lvl));
				}

				int target = min_ind;
				string path = pathVec[target];
				cout << "Target = " << target << endl
					 << "Path = " << path << endl;

				vector<int> brPath (path.length());
				bNode_t* curr_node = end_node;
				for (int i = 0; i < path.length(); ++i) {
					int ind = path[i] - 48;
					brPath[i] = curr_node->outEdges[ind];
					int tmp = curr_node->outNodes[ind];
					
					cout << brPath[i] << " ";
					curr_node = branchGraph.getNode(tmp);
					assert(curr_node != NULL);
				}
			}
			else
				cout << "-";
			cout << endl;
		}
			
	}

	exit(0);

	readParam(paramObj2);
	for (int i = 0; i < 100; i++) {
		cout << endl << "Iteration " << i << endl;
		Stage2_GenerateVectors(paramObj2);
	}

	printCnt(paramObj2->branchHit);
	PrintVectorSet(paramObj2->inputVec);

	delete paramObj2;
//	#endif 

	return 0;
}

void Stage2_GenerateVectors(Stage2_Param* paramObj) {

	cout << "GA Stage 2: " << endl
		 << "Stage 1 Coverage: " << endl;
	printCnt(paramObj->branchHit);
	cout << endl;
	
	int NUM_GEN	= 2; //paramObj->NUM_GEN;
	int POP_SIZE = 64; //paramObj->POP_SIZE;
	int VEC_LEN = paramObj->INDIV_LEN/CONST_NUM_INPUT_BITS;

	cout << "Vec Length: " << VEC_LEN << endl;
	brGraph_t *branchGraph = paramObj->branchGraph;

	Vtop *cktVar;
	cktVar = new Vtop;
	
//	(paramObj->startState)->setCktState(cktVar);
	SimMultiCycle(cktVar, paramObj->inputVec);
	printCktState(cktVar);

	state_pVec startPool;
	state_t* start_state = new state_t (cktVar, 0);
	startPool.push_back(start_state);

	cout << "Last leaf node reached" << endl;
//	state_t *st = paramObj->startState;
//	state_t *st = start_state;
	
	set<int> favorSet = paramObj->favorSet;
	int curr_edge;
//	for (int_vec_iter it = st->branch_index.begin();
//			it != st->branch_index.end(); ++it) {
//		if (leafBranch[*it] == 0) 
//			continue;
//
//		cout << "Branch " << *it << ": ";
//
//		int edgeVal = branchGraph->edgeMap[*it];
//		cout << edgeVal << endl;
//		if(edgeVal == -1)
//			continue;
//		
//		curr_edge = *it;
//		getTarget(paramObj, *it, favorSet);
//		
//		cout //<< "Target: " << target_edge << endl
//			 << "Branches favorable: ";  
//		printSet(favorSet);
//		cout << endl;
//	}
	
	for (int_vec_iter it = paramObj->branchLeaf.begin();
			it != paramObj->branchLeaf.end(); ++it) {
		if (leafBranch[*it] == 0) 
			continue;

		cout << "Branch " << *it << ": ";

		int edgeVal = branchGraph->edgeMap[*it];
		cout << edgeVal << endl;
		if(edgeVal == -1)
			continue;
		
		curr_edge = *it;
		getTarget(paramObj, *it, favorSet);
		
		cout //<< "Target: " << target_edge << endl
			 << "Branches favorable: ";  
		printSet(favorSet);
		cout << endl;
	}
	
	// Run GA

	gaPopulation_t stage2Pop(POP_SIZE, VEC_LEN);
	stage2Pop.initPopulation(startPool);
	
	int CONST_IMPT_BRANCH = 50;
	int CONST_TARGET_BRANCH = -100;
	int CONST_UNIMPT_BRANCH = 200;

	bool gaTerminate = false;
	fitness_t prev_gen_fitness = (2 << 29), curr_gen_fitness = (2 << 30);

	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage2Pop.indiv_vec[ind];
			indiv->simCkt(cktVar);

//			indiv->printIndiv(0);

//			if (maxCov < indiv->num_branch)
//				maxCov = indiv->num_branch;
//			
//			avgCov += indiv->num_branch;

			int_vec state_fit_vec(indiv->state_list.size(), 0);
			int ind = 0;

//			cout << "States: " << endl;
			for (state_pVec_iter st = indiv->state_list.begin();
					st != indiv->state_list.end(); ++st, ind++) {
				
				bool branch_p = false;
				int branch_val = -1;
				int state_fit = 0;
				for (int_vec_iter br = (*st)->branch_index.begin(); 
						br != (*st)->branch_index.end() && !branch_p; ++br) {
					if (leafBranch[*br] == 0)
						continue;
//					cout << *br <<" ";
					if (favorSet.find(*br) != favorSet.end()) {
						state_fit += CONST_IMPT_BRANCH;
//						cout << CONST_UNIMPT_BRANCH ;
						if (*br != curr_edge) {
							state_fit += CONST_TARGET_BRANCH;
//							cout << CONST_UNIMPT_BRANCH;
							branch_p = true;
							branch_val = *br;
						}
//						cout << " ";
					}
					else {
						state_fit += CONST_UNIMPT_BRANCH;
//						cout << CONST_UNIMPT_BRANCH << " ";
					} 

					if (branch_p) {
						cout << "Branch " << branch_val << " reached in " << ind << " vectors" << endl;
						gaTerminate = true;
					}
//					cout << "; ";
				}
//				if (branch_p) {
//					cout << "Branch " << branch_val << " reached in " << ind << " vectors" << endl;
//					gaTerminate = true;
//				}
				state_fit_vec[ind] = state_fit;
			}

			cout << "Fitness: ";
			printVec(state_fit_vec);
			cout << endl;

			indiv->fitness = 0;			
			int max_ind = -1, max_fit = (2 << 20);
//			cout << "max_fitness = " << max_fit << endl;
			for (int it = 0; (uint)it < indiv->state_list.size(); ++it) {
				if (max_fit >= state_fit_vec[it]) {
					max_ind = it;
					max_fit = state_fit_vec[it];
				}
				indiv->fitness += state_fit_vec[it];
			}
			indiv->fitness /= indiv->state_list.size();
			if (max_ind != -1)
				indiv->max_index = max_ind;

			if (curr_gen_fitness > indiv->fitness)
				curr_gen_fitness = indiv->fitness;

			cout << "Fitness(" << indiv->index << "): " << indiv->fitness << endl;
		}
	
		gaTerminate = gaTerminate || (gen == NUM_GEN-1) || (prev_gen_fitness < curr_gen_fitness);
		if (!gaTerminate) {
			stage2Pop.gaEvolve();
			cout << "Evolved" << endl;
		}

		prev_gen_fitness = curr_gen_fitness;
		curr_gen_fitness = (2 << 30);
	}
	
	cout << "After GA : " << endl;
	std::sort(stage2Pop.indiv_vec.begin(), stage2Pop.indiv_vec.end(), compFitness);
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
	indiv->printIndiv(1);
	cout << endl << indiv->max_index + 1 << " vectors copied" << endl;
	paramObj->inputVec += indiv->input_vec.substr(0, CONST_NUM_INPUT_BITS * (indiv->max_index + 1));

	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			paramObj->branchHit[*it]++;
	}

	vector<int> delSet;
	for (set<int>::iterator br = favorSet.begin();
			br != favorSet.end(); ++br) {
		if (paramObj->branchHit[*br] == 0)
			delSet.push_back(*br);
	}
	cout << "Branches uncovered: ";
	printVec(delSet);
	cout << endl;
	
	paramObj->favorSet.clear();
	for (int_vec_iter br = delSet.begin();
			br != delSet.end(); ++br) {
		paramObj->favorSet.insert(*br);
	}
	cout << "Elements in favorSet = ";
	printSet(paramObj->favorSet);
	cout << endl;

	paramObj->branchLeaf.clear();
	state_t *st = indiv->state_list[indiv->max_index];
	for (int_vec_iter br = st->branch_index.begin();
			br != st->branch_index.end(); ++br) {
		if (leafBranch[*br])
			paramObj->branchLeaf.push_back(*br);
	}

	delete start_state;
	cout << endl;

}

int getPathBFS(brGraph_t* brGraph, int start, int target, string& path) {
	
	cout << "Finding path from " << start << " -> " << target << endl;
	map<int, string> label;
	map<int, int> level;
	int max_level = -1;

	vector<int> bfsQueue;
	bfsQueue.push_back(start);
	level.insert(make_pair(start, 0));
	label.insert(make_pair(start, ""));

	const int THRESH_LEVEL = 10;
	for (int qInd = 0; qInd < bfsQueue.size(); ++qInd) {

		int front = bfsQueue[qInd];
		bNode_t* curr = brGraph->getNode(front);

//		cout << "F: " << front << endl;
		for (int eInd = 0; eInd < curr->outNodes.size(); ++eInd) {
			int node_ = curr->outNodes[eInd];
			
			char ch[2];
			ch[0] = eInd + 48; ch[1] = '\0';
			string tmpLabel = label[front] + string(ch);
			label.insert(make_pair(node_, tmpLabel));

			int tmpLevel = level[front] + 1;
			level.insert(make_pair(node_, tmpLevel));

			if (node_ == target) {
				path = tmpLabel;
				return tmpLevel;
			}

			if (max_level < tmpLevel)
				max_level = tmpLevel;

			bfsQueue.push_back(node_);

//			cout << "Queued " << node_ 
//				 << " Level: " << tmpLevel
//				 << " Label: " << tmpLabel
//				 << " Max_Lvl: " << max_level << endl;
		}

		if (max_level > THRESH_LEVEL) {
			path = "Unreachable";
			return -1;
		}

	}

}



//void getTarget(Stage2_Param* paramObj, int currEdge, vector<int>& favorSet, vector) {
void getTarget(Stage2_Param* paramObj, int currEdge, set<int>& favorSet) {
	
	brGraph_t* branchGraph = paramObj->branchGraph;
		
	int edge_ind = branchGraph->edgeMap[currEdge];
	bEdge_t* curr = &(branchGraph->bEdges[edge_ind]);
	
	int start_node = curr->startTop;
	int end_node = curr->endTop;

	int next_state = branchGraph->nodeMap[end_node];
	if (next_state == -1) {
		cout << "No data for " << end_node << endl;
		return;
	}

	int_vec *reachable_nodes = &(branchGraph->bNodes[next_state].outNodes);
	int_vec *out_edges = &(branchGraph->bNodes[next_state].outEdges);

	for (int brInd = 0; (uint) brInd < out_edges->size(); ++brInd) {
		int br = (*out_edges)[brInd];
		cout << "Edge " << br;
		if (paramObj->branchHit[br] == 0) {
			favorSet.insert(br);
//			favorSet.push_back(br);
			cout << " not reached" << endl;
		}
		else {
			cout << " reached" << endl;
			if (br == currEdge)
				favorSet.insert(br);
//				favorSet.push_back(br);
		}
	}
}

void readTopNodes(graph& graphCov, brGraph_t& branchGraph) {

	int_vec leafNodes;
	int_vec topNodes;
	for (int i = 1; (unsigned) i <= graphCov.numNodes; ++i) {
		if ((graphCov.gNodes[i]).branch_index == -1)
			continue;

		int branch_ = (graphCov.gNodes[i]).branch_index;
		int parent_ = graphCov.getTopNode(branch_);

		if(IsDefaultBranch(branch_))
			continue;

		if (branch_ == parent_)
			topNodes.push_back(parent_);

		if (graphCov.gNodes[i].num_child == 0) {
			leafNodes.push_back(branch_);
			leafBranch[branch_] = 1;
		}
	}

	cout << "Leaf nodes : "; 
	printVec(leafNodes);
	cout << endl;

	cout << "Top nodes : "; 
	printVec(topNodes);
	cout << endl;
	
	for (int_vec_iter it = leafNodes.begin(); 
			it != leafNodes.end(); ++it)
		branchGraph.branchType[*it]++;

	for (int_vec_iter it = topNodes.begin(); 
			it != topNodes.end(); ++it)
		branchGraph.branchType[*it] += 2;

	char fName[100];
	sprintf(fName, "%s.bg", benchCkt);
	
	ifstream brFile;
	brFile.open(fName, ios::in);
	if (brFile == NULL) {
		cout << "Unable to open file " << fName << endl;
		return;
	}
	
	for (int lNum = 0; brFile && (uint) lNum < topNodes.size(); ++lNum) {
		string tmpStr;
		getline(brFile, tmpStr);
//		cout << tmpStr << " l:" << tmpStr.length() << endl;

		stringstream ss;
		ss << tmpStr;

		int node_index;
		ss >> node_index;

		int num;
		ss >> num;

		int_vec edge_vec;
		while (ss) {
			edge_vec.push_back(num);

			int topNode = graphCov.getTopNode(num);

			branchGraph.addNode(node_index);
			branchGraph.addNode(topNode);
			branchGraph.addEdge(num, topNode, node_index);

//			cout << branchGraph.addNode(node_index) << " ";
//			cout << branchGraph.addNode(topNode) << " ";
//			cout << branchGraph.addEdge(num, topNode, node_index) << " ";
//			cout << topNode << "--" << num << "-->" << node_index << endl;

			ss >> num;
		}

//		cout << endl;
	}
	
	brFile.close();
//	branchGraph.printGraph();

    return;
}

void Stage1_GenerateVectors(Stage1_Param* paramObj) {
	
	// Parameters
	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN	= paramObj->INDIV_LEN / CONST_NUM_INPUT_BITS;
	int NUM_INDIV = paramObj->NUM_INDIV;
	int TOP_INDIV = paramObj->NUM_TOP_INDIV;
	
	double WT_FIT_STATE = 0.7;
	double WT_FIT_BRANCH = 0.3;
	int WT_FIT_COV = 500;

	cout << "GA Stage 1: " << endl
		 << POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;
	
	Vtop *cktVar = new Vtop;

	int_vec branch_counters(CONST_NUM_BRANCH, 0);

	SimMultiCycle(cktVar, 0);
	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
		if (GetCoverage(cktVar, br))
			branch_counters[br] = 5;

	state_t *rstState;
	rstState = new state_t(cktVar, 0);
	rstState->pIndiv = NULL;

	state_pVec startPool;
	startPool.push_back(rstState);
	
	cout << "Reset state: " << endl;
	rstState->printState();
	cout << endl;

	gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
	/*	Init population with one of the states in the pool 
		of start states and string of random vectors	*/
	stage0Pop.initPopulation(startPool);

	stateMap_t currStateMap;
	currStateMap.clear();
	int prevMaxCov = 0;

	bool gaTerminate = false;
	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {
		
//		cout << "States allocated: " << state_t::mem_alloc_cnt << endl;
//		cout << "Indiv allocated: " << gaIndiv_t::mem_alloc_cnt << endl;

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		bool improv_cov = false; 
		bool new_state = false;

		int maxCov = 0, avgCov = 0;
		int_vec maxInd;

		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
			indiv->simCkt(cktVar);

			/* Add states to stateMap	*/
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st) {
				state_t *curr = *st;
				keyVal_t hash_val_ = curr->getHash();
				retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));
				
				curr->hit_count = 1;
				/*	If the key for the state is present 
					=>	New state was not found
					If the value for the key = NULL	
						=> The state was found in the previous cycle
						Therefore, set the new found state as the value for that key.
					Else
						=> The state was found in this cycle
						Therefore, just increment the count of that state		*/
				if (ret.second == false) {
					if (ret.first->second)
						(ret.first->second)->hit_count++;
					else 
						(ret.first->second) = curr;
				}
				else {
					new_state = true;
				}
			}

			/* Add branch hits to currBranchCov */
			for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
				currBranchCov[br] += indiv->branch_cov[br];
			}

			/* Computing max and avg branch coverage */
			if (maxCov < indiv->num_branch)
				maxCov = indiv->num_branch;
			
			avgCov += indiv->num_branch;

		}
		
		cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
			 << "Max Coverage: " << maxCov << endl;

		avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
		improv_cov = (maxCov >= prevMaxCov);

		/* Fittest Individuals
			i.e. individuals with maximum coverage	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];
			if (indiv->num_branch == maxCov) {
				cout << ind << " ";
				maxInd.push_back(ind);
			}

		}
		cout << endl;	

//			for (int_vec_iter it = maxInd.begin();
//					it != maxInd.end(); ++it)
//				stage0Pop.indiv_vec[*it]->printIndiv(1);
//			cout << endl;

		/* Compute Fitness	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

			/* Fitness for #branches covered */
			fitness_t fitness_cov = avgCov - indiv->num_branch;

			/* Fitness for branch coverage	*/
			fitness_t fitness_branch = 0;

			/* Fitness for states reached 	*/
			fitness_t fitness_state = 0;

			/* Combining all fitness values	*/
			indiv->fitness 
				= (fitness_cov * WT_FIT_COV) 
				+ (fitness_branch * WT_FIT_BRANCH)
				+ (fitness_state * WT_FIT_STATE);
//				cout << "Fitness (" << ind << ") " << indiv->fitness 
//					 << " [" << fitness_cov 
//					 << ", " << fitness_branch
//					 << ", " << fitness_state << "]" << endl 
//					 << endl;
		}	
		

		/* Terminate GA Condition:
			- gen == NUM_GEN
			- coverage[gen] <= coverage[gen-1]
			- no new states reached	*/
		
		gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

		if (!gaTerminate) {

//				cout << "States found " << gen << endl;
			for (stateMap_iter st = currStateMap.begin(); 
					st != currStateMap.end(); ++st) {
//					cout << (st->first) << endl;
				currStateMap[st->first] = NULL;
//					(st->second) = NULL;
			}
//			DisplayMap(currStateMap);

			stage0Pop.gaEvolve();
			
			prevMaxCov = maxCov;
		}
		else
			cout << "Terminating after gen " << gen << endl;

	}
		
	/* Mine for interesting individuals and states 	*/
	cout << "States reached after Round 0" << endl;
//	DisplayMap(currStateMap);
//	cout << endl;

	cout << "States allocated: " << state_t::mem_alloc_cnt << endl;

	std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
//		cout << "Population after Round 0" << endl;
//		int pInd = 0;
//		for (gaIndiv_pVec_iter indiv = stage0Pop.indiv_vec.begin();
//				indiv != stage0Pop.indiv_vec.end(); ++indiv) {
//			cout << pInd << endl;
//			(*indiv)->printIndiv(0);
//			cout << (*indiv)->num_branch << " branches with "
//				 << (*indiv)->max_index + 1 << " vectors" << endl;
//			//getchar();
//			pInd++;
//		}
	
	gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

	paramObj->inputVec = (indiv->input_vec).substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));
	paramObj->branchHit = int_vec(CONST_NUM_BRANCH, 0);
	SimMultiCycle(cktVar,0);
	GetBranchCounters(cktVar, paramObj->branchHit);
	paramObj->stateList = state_pVec(indiv->max_index+1, NULL);
	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
//		if (st == NULL) {
//			cout << "NULL state found at " << ind << endl;
//			assert(0);
//		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			paramObj->branchHit[*it]++;
		//paramObj->stateList[ind] = new state_t(*indiv->state_list[ind]);
		paramObj->stateList[ind] = indiv->state_list[ind];
		indiv->state_list[ind] = NULL;
	}

	cout << "Branches covered after Stage 1: " << endl;
	printCnt(paramObj->branchHit);

	delete rstState;
	delete cktVar;
}


int checkCoverage(const vecIn_t& inputVec) {
	
	Vtop* top = new Vtop;

	int numVec = inputVec.length()/CONST_NUM_INPUT_BITS;
	int_vec brCoverage(CONST_NUM_BRANCH, 0);
	set<int> brSet;
	
	// Add reset branches to the set 
	int_vec resetBranch;
	GetResetBranch(top, resetBranch);
	for (int_vec_iter it = resetBranch.begin();
			it != resetBranch.end(); ++it) {
		brSet.insert(*it);
	}

	int index;

	for (int i = 0; i < numVec; ++i) {
		string inputStr = inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << inputStr << endl;
		SimOneCycle(top, inputStr);
//		printCktState(top);

		int_vec currBranch;
		GetCoverage(top, currBranch);
		for (int_vec_iter it = currBranch.begin();
				it != currBranch.end(); ++it) {
			brCoverage[*it]++;
			brSet.insert(*it);
		}
	
		if(brSet.size() == CONST_NUM_BRANCH-1) {
			index = i;
			cout << "#Vectors = " << index << endl;
			break;
		}

	}
	cout << "Coverage: " << GetCoverage(top, true) << endl;
	delete top;

	return index;
}

void readParam(Stage1_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_0 	: 4 generations" << endl
			 << "POP_SIZE_0	: 256 individuals" << endl
			 << "INDIV_LEN_0: 20 bits" << endl
			 << "NUM_INDIV_0: 2 rounds" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
		paramObj->NUM_INDIV = 2;
	}	
	else {
		while(paramIn) {
			string curr;
			getline(paramIn, curr);

			if (curr.find("NUM_GEN_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN = " << val << endl;
					paramObj->NUM_GEN = val;
				}
				else 
					paramObj->NUM_GEN = 4;
			}
			else if (curr.find("POP_SIZE_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_0 = " << val << endl;
					paramObj->POP_SIZE = val;
				}
				else 
					paramObj->POP_SIZE = 256;
			}
			else if (curr.find("INDIV_LEN_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_0 = " << val << endl;
					paramObj->INDIV_LEN = val;
				}
				else 
					paramObj->INDIV_LEN = 20;
			}
			else if (curr.find("NUM_INDIV_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_INDIV_0 = " << val << endl;
					paramObj->NUM_INDIV = val;
				}
				else 
					paramObj->NUM_INDIV = 2;
			}

		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

void readParam(Stage2_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_1 	: 4 generations" << endl
			 << "POP_SIZE_1	: 256 individuals" << endl
			 << "INDIV_LEN_1: 20 bits" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
	}	
	else {
		while(paramIn) {
			string curr;
			getline(paramIn, curr);

			if (curr.find("NUM_GEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN_1 = " << val << endl;
					paramObj->NUM_GEN = val;
				}
				else 
					paramObj->NUM_GEN = 4;
			}
			else if (curr.find("POP_SIZE_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_1 = " << val << endl;
					paramObj->POP_SIZE = val;
				}
				else 
					paramObj->POP_SIZE = 256;
			}
			else if (curr.find("INDIV_LEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_1 = " << val << endl;
					paramObj->INDIV_LEN = val;
				}
				else 
					paramObj->INDIV_LEN = 20;
			}
		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

void DisplayMap(stateMap_t& stateMap) {
	for (stateMap_iter it = stateMap.begin(); it!= stateMap.end(); ++it) {

		cout << it->first << "\t";
		if (it->second) {
			cout << it->second->hit_count << endl;
			(it->second)->printState();
		}
		else 
			cout << "NULL" << endl;

		cout << endl;
	}
}

void printVec(int_vec& vec_) {
	for (int_vec_iter vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

void printSet(set<int>& vec_) {
	for (set<int>::iterator vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

void printCnt(int_vec& vec_) {
	for (uint v = 0; v < vec_.size(); ++v)
		cout << v << "(" << vec_[v] << ") ";
	cout << endl;
}

void PrintVectorSet(const vecIn_t& inputVec, bool printFlag ) {

	ofstream vecOut;
	char fName[100];

	if (printFlag) {
		sprintf(fName, "%s_%d.vec", benchCkt, (int)time(NULL) % 10000);
		
		cout << "Writing vectors into file -> " << fName << endl; 
		vecOut.open(fName, ios::out);
		
		vecOut << CONST_NUM_INPUT_BITS << endl;
	}
		
	cout << endl << "# Vectors: " << inputVec.length()/CONST_NUM_INPUT_BITS << endl;

	for (uint i = 0; i < inputVec.length(); i++) {
		cout << inputVec[i];
		if(printFlag)
			vecOut << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0) {
			cout << endl;
			if(printFlag)
				vecOut << endl;
		}
	}
	
	if(printFlag) {
		vecOut << "END" << endl;
		vecOut.close();
	}
}

bool compCoverage(gaIndiv_t* A, gaIndiv_t* B) {
	
	if (A->num_branch != B->num_branch)
		return (A->num_branch > B->num_branch);
	else 
		return (A->max_index < B->max_index);
	
}