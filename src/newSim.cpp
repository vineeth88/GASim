#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"
#include "graphLib.h"

#define GA_FIND_TOP_INDIV

#define PATH_HASH_ON
#define NEW_STAGE2_CORE
//#define OLD_STAGE2_CORE

typedef pair<int, int> nodePair_t;
typedef map<nodePair_t, string> pathMap_t;

//rstIn_t 	gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
int_vec 	IsBranchLeaf(CONST_NUM_BRANCH, 0);
int_vec		exclBranchList(CONST_NUM_BRANCH, 0);

vector<vecIn_t> BranchInputVec(CONST_NUM_BRANCH, string(""));
vector<string> BranchStateVal(CONST_NUM_BRANCH, string(""));
int_vec BranchNumTries(CONST_NUM_BRANCH, 0);

int gaIndiv_t::mem_alloc_cnt = 0;
int state_t::mem_alloc_cnt = 0;
//int Vtop::mem_alloc_cnt = 0;

class Stage1_Param {

	public:

		string fName; 

		int 	NUM_INDIV, INDIV_LEN;
		int 	NUM_TOP_INDIV;
		int 	NUM_GEN, POP_SIZE;
		int 	MAX_ROUNDS;

		vecIn_t			inputVec;
		int_vec			branchHit;
		state_pVec		stateList;	

		Stage1_Param() {
			branchHit = int_vec(CONST_NUM_BRANCH, 0);
			stateList = state_pVec();
		}

		~Stage1_Param() {
			branchHit.clear();
			inputVec.clear();

			for (int st = 0; (uint)st < stateList.size(); ++st) {
				if (stateList[st]) {
					delete stateList[st];
					stateList[st] = NULL;
				}
			}
			stateList.clear();

		}

};

class Stage2_Param : public Stage1_Param {

	public:

		int MAX_ROUNDS;
		int MAX_TRIES;

		state_t *	startState;
		Vtop *		cktVar;

		set<int> 	favorSet;
		set<int>	unCovered;

		int_vec		branch_index;
		int_vec 	branchHit;
		int_vec 	lastBranchHit;

		brGraph_t*	branchGraph;
		covGraph_t* covGraph;
		int 		curr_val, end_val;
		int			tar_edge, tar_node;
		
		string 		currPath;
		int start_node;

		Stage2_Param() {

			MAX_ROUNDS = 20;
			cktVar = NULL;
			branchHit = int_vec(CONST_NUM_BRANCH, 0);
			lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);
			favorSet.clear();
			unCovered.clear();
		}

		Stage2_Param(Stage1_Param* obj) {	
			NUM_GEN 	= obj->NUM_GEN;
			INDIV_LEN 	= obj->INDIV_LEN;
			POP_SIZE 	= obj->POP_SIZE;

			fName 		= obj->fName;

			branchHit 	= obj->branchHit;
			branch_index = (obj->stateList.back())->branch_index;

			cktVar = NULL;
			MAX_ROUNDS = 20;

			favorSet = set<int>();
			unCovered = set<int>();
			
		}

		//	~Stage2_Param() {
		//		if(cktVar) {
		//			delete cktVar;
		//			cktVar = NULL;
		//		}
		//		if (branchGraph) {
		//			delete branchGraph;
		//			branchGraph = NULL;
		//		}
		//	}

};

/* Stage 1 Functions */
void Stage1_GenerateVectors(Stage1_Param*);

void readParam(Stage1_Param*);
void readExclBranchList();

int computeBranches(vector<int>&);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

/* Stage 2 Functions */
void Stage2_GenerateVectors(Stage2_Param*);
void Stage2_Core(Stage2_Param*, int);
void Stage2_Core(Stage2_Param*);

void readParam(Stage2_Param*);
bool readTopNodes(covGraph_t&, brGraph_t&);
void readBranchGraph(brGraph_t&);

string getPathBFS(brGraph_t*, int, int);

int AddVector2Branch(int, vecIn_t);
int AddState2Branch(int, state_t*);

/* Standard subroutines	*/
void DisplayMap(stateMap_t&);
void DisplayMap(pathMap_t&);
void printVec(int_vec&);
void printSet(set<int>&);
void printCnt(int_vec&);
void PrintVectorSet(const vecIn_t& inputVec, bool printFlag = false);

int main(int argc, char* argv[]) {

#define random_seed
#ifdef random_seed
	uint seed = time(NULL);

//	seed = 1409956612;
	cout << seed << endl;
	srand(seed);
#endif

	char paramFile[100];
#ifndef _STAGE1_OLD_FN_
	cout << "New function" << endl;
	sprintf(paramFile, "%s.param", benchCkt);
#endif

	if (argc >= 2) {
		if (strcmp(argv[1], "-h") == 0) {
			cout << "Input format: " << endl
				<< argv[0] << " "
				<< argv[1] << " [<ckt>.param]" << endl;
			//			<< "Required files for b11/b12 : " 
			//			<< benchCkt << ".graph, " 
			//			<< benchCkt << ".bg " << endl;
		}
		else {
			sprintf(paramFile, "%s", argv[1]);
			cout << "Using parameters from " << paramFile << endl;
		}
	}		

	readExclBranchList();

	/* ************** START : STAGE 1 *************** */
	Stage1_Param *paramObj1 = new Stage1_Param;

	paramObj1->fName = string(paramFile);
	readParam(paramObj1);

	Stage1_GenerateVectors(paramObj1);
	PrintVectorSet(paramObj1->inputVec);

	cout << "Uncovered Branches: " << endl;
	int num_branch_uncovered = 0;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		if(exclBranchList[ind])
			paramObj1->branchHit[ind] = -10;

		if (paramObj1->branchHit[ind] == 0) {
			cout << ind << " ";
			num_branch_uncovered++;
		}
	}
	cout << endl << "% Coverage " 
		<< (float) (CONST_NUM_BRANCH - num_branch_uncovered) * 100.0f 
		/ (float) (CONST_NUM_BRANCH) 
		<< endl;

	cout << endl
		<< "Memory allocation details: " << endl
		<< "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		<< "state_t: " << state_t::mem_alloc_cnt << endl;

	//exit(0);

	/* *************** END : STAGE 1	*************** */
#if !defined(__b12)
	PrintVectorSet(paramObj1->inputVec, true);
	delete paramObj1;
	return 0;
#endif

	/*	************* START : STAGE 2 	*************** */
	covGraph_t covGraph;
	brGraph_t branchGraph;
	sprintf(covGraph.fName, "%s.graph", benchCkt);
	readGraph(covGraph);
	bool brFlag = readTopNodes(covGraph, branchGraph);

	if (!brFlag) {
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	}		

//#if !defined(__b12)
//	PrintVectorSet(paramObj1->inputVec, true);
//	delete paramObj1;
//	return 0;
//#endif

	vecIn_t TestVectorSet = paramObj1->inputVec;

	cout << "Stage 2 " << endl << endl;
	Stage2_Param *paramObj2 = new Stage2_Param(paramObj1);
	paramObj2->inputVec = paramObj1->inputVec;
	paramObj2->branchGraph = &branchGraph;
	paramObj2->covGraph = &covGraph;

	paramObj2->startState = paramObj1->stateList.back();
	paramObj2->stateList = paramObj1->stateList;
	paramObj1->stateList.clear();

	paramObj2->cktVar = new Vtop;
	SimMultiCycle(paramObj2->cktVar, paramObj1->inputVec);

	readParam(paramObj2);
	Stage2_GenerateVectors(paramObj2);

	//	cout << "Last state reached at the end of Stage 1" << endl;
	//	paramObj2->startState->printState();


	/* ***************** END : STAGE 2 ************** */
	PrintVectorSet(paramObj2->inputVec, true);

	cout << endl
		<< "Memory allocation details: " << endl
		<< "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		<< "state_t: " << state_t::mem_alloc_cnt << endl;

	exit(0);
	delete paramObj2;
	return 0;

}

void Stage2_GenerateVectors(Stage2_Param* paramObj2) {

	int_vec &branch_index = paramObj2->branch_index;
	int_vec &branchHit = paramObj2->branchHit;
	set<int> &favorSet = paramObj2->favorSet;
	//	set<int> &unCovered = paramObj2->unCovered;

	brGraph_t& branchGraph = *paramObj2->branchGraph;
	covGraph_t& covGraph = *paramObj2->covGraph;

	int_vec unCovered;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		if(IsDefaultBranch(br))
			branchHit[br] = -5;
		else if ((branchHit[br] == 0) && (IsBranchLeaf[br])) {
			bEdge_t* edge = branchGraph.getEdge(br);
			if (edge) 
				unCovered.push_back(br);
		}
	}

	int MAX_ROUNDS = paramObj2->MAX_ROUNDS;
	BranchNumTries = int_vec(CONST_NUM_BRANCH, 0);

#ifdef PATH_HASH_ON
	pathMap_t pathDB;
#endif

	for (int num_round = 0; num_round < MAX_ROUNDS; ++num_round) {

		cout << endl 
			<< "ROUND : " << num_round << " / " << MAX_ROUNDS
			<< endl << endl;
		favorSet.clear();

		/*	For hitting branch 86, 
			1. Target and hit branch 67
			2. Target and hit branch 68 next.
			3. Target and hit branch 86.	

			1)	Either this can be read from a file.
			2)	You cant target the same branch more than 10 times in a row 
			unless its the only branch left, or if the paths of the other 
			branches go through the targeted branch		*/

		if (unCovered.size() == 0)
			break;

		/*	edgeVec = The list of edges traversed in the last cycle	*/
		int_vec edgeVec;
		for (int_vec_iter br = branch_index.begin(); br != branch_index.end(); ++br) {
			if (IsBranchLeaf[*br])	{
				bEdge_t* curr_edge = branchGraph.getEdge(*br);
				if (curr_edge) 
					edgeVec.push_back(*br);
			}
		}

		/* 	curr_val = Selected edge with endpoints start_val -> end_val */
		int curr_val = edgeVec.back();
		bEdge_t* curr_edge = branchGraph.getEdge(curr_val);
		int start_val = curr_edge->startTop;
		int end_val = curr_edge->endTop;

		bNode_t* curr_node = branchGraph.getNode(end_val);
		assert(curr_node);

		string target_path = "Unreachable";
		int target_lvl = (2 << 20);
		int target_node = -1;
		int target_tries = paramObj2->MAX_TRIES;

		/*	- Find path from curr_node to all the start_nodes of the uncovered edges
			- Find the uncovered edge with the shortest path
			- Set the topNode(start_node) as the target_node */

		for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
			string path;

#ifdef PATH_HASH_ON
			nodePair_t curr = make_pair(end_val, *it);

			if (pathDB.find(curr) == pathDB.end()) {
				path = getPathBFS(&branchGraph, end_val, *it);
				pathDB.insert(make_pair(curr, path));
			//	cout << "Absent" << endl;
			}
			else {
				path = pathDB[curr];
			//	cout << "Present" << endl;
			}
#endif

#ifndef PATH_HASH_ON
			path = getPathBFS(&branchGraph, end_val, *it);
#endif

			int lvl = path.length();

#ifdef S2_DBG_PRINT
			cout << end_val << "-> " 
				<< *it << ": " 
				<< path << endl;
#endif
			if ((target_lvl > lvl) && (path.compare("Unreachable"))) {
				target_lvl = lvl;
				target_node = *it;
				target_path = path;
			}
		}

		//	DisplayMap(pathDB);

		cout << "Target	: " << target_node << endl
			<< "Level 	: " << target_lvl << endl
			<< "Path 	: " << target_path << endl;

		paramObj2->curr_val = curr_val;
		paramObj2->end_val = end_val;

		if (target_path.compare("Unreachable")) {
			BranchNumTries[target_node]++;
			int iter_val = end_val;

			bNode_t* iter_node = branchGraph.getNode(iter_val);
			assert(iter_node);

			#ifdef NEW_STAGE2_CORE
			paramObj2->currPath = target_path;
			Stage2_Core(paramObj2, iter_val);
			#endif

			#ifdef OLD_STAGE2_CORE 
			for (int st = 0; (uint) st < target_path.length(); ++st) {
				int brInd = target_path[st] - 48;
				int tar_edge = iter_node->outEdges[brInd];

				cout << " Path[" << st << "/" << target_path.length() << "]: " << tar_edge << endl;

				int_vec self_loop_br;
				for (int br = 0 ; br < iter_node->outNodes.size(); ++br) {
					if (iter_node->outNodes[br] == iter_val) {
						self_loop_br.push_back(iter_node->outEdges[br]);
					}
				}

				if (self_loop_br.size()) {
					cout << "Self-loop: ";
					printVec(self_loop_br);
					cout << endl;
				}

				/* Run stage 2	*/
				paramObj2->tar_edge = tar_edge;
				paramObj2->tar_node = iter_node->outNodes[brInd];

				Stage2_Core(paramObj2);
				int branch_hit = 20;
				for (int br = 0; br < iter_node->outEdges.size(); ++br) {
					int tmp_edge = iter_node->outEdges[br];
					if ((paramObj2->lastBranchHit[tmp_edge]) && 
							(iter_node->outNodes[br] != iter_val))
						branch_hit = 1;
				}
				if (branch_hit) {
					if(self_loop_br.size()) {
						while (branch_hit) {
							/*	Run stage 2	*/
							//Stage2_Core(paramObj2);
							if (paramObj2->lastBranchHit[tar_edge]) {
								branch_hit = 0;
								break;
							}
							for (int br = 0; br < iter_node->outEdges.size(); ++br) {
								int tmp_edge = iter_node->outEdges[br];
								if ((paramObj2->lastBranchHit[tmp_edge]) && 
										(iter_node->outNodes[br] != iter_val))
									branch_hit = 1;
							}
							branch_hit--;
							cout << "In branch_hit : " << branch_hit << endl;
						}
					}
					else {
						cout << "Currently Unreachable" << endl;
						break;
					}
				}

				iter_val = iter_node->outNodes[brInd];
				iter_node = branchGraph.getNode(iter_val);
				assert(iter_node);
			}

			#endif

			cout << endl << "END OF PATH" << endl;

			for (int br = 0; br < unCovered.size(); ++br) {
				if (unCovered[br] == target_node)
					unCovered[br] = -100;
				else if (branchHit[unCovered[br]])
					unCovered[br] = -100;
			}

			int_vec newVec;
			for (int_vec_iter br = unCovered.begin(); br != unCovered.end(); ++br) {
				if (*br != -100)
					newVec.push_back(*br);
			}
			if (branchHit[target_node] == 0)
				newVec.push_back(target_node);

			unCovered.clear();
			unCovered = newVec;
			newVec.clear();

			cout << "unCovered: " << endl;
			printVec(unCovered);
			cout << endl;
		}
		else {
			cout << endl
				<< " * * * * * * * * * * * * * " << endl
				<< "No reachable path. Simulating vector for next state." << endl;

			stateMap_t nxtStateMap = stateMap_t();
			map<keyVal_t, vecIn_t> vecMap;
			gaPopulation_t nxtStatePop(paramObj2->POP_SIZE, 1);

			state_pVec initPool;
			initPool.push_back(paramObj2->stateList.back());

			cout << "Start state: " << endl;
			initPool[0]->printState();
			cout << endl;

			nxtStatePop.initPopulation(initPool);

			Vtop *cktVar = paramObj2->cktVar;
			for (int ind = 0; ind < paramObj2->POP_SIZE; ++ind) {

				ResetCounters(cktVar);
				gaIndiv_t* indiv = nxtStatePop.indiv_vec[ind];
				indiv->simCkt(cktVar);

				int vt = 0;
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st, ++vt) {
					keyVal_t hash = (*st)->getHash();
					retVal_t ret = nxtStateMap.insert(make_pair(hash, *st));
					if (ret.second == false) {
						(ret.first->second)->hit_count++;
					}
					vecMap.insert(make_pair(hash, indiv->input_vec)); 
				}
			}

			vector<vecIn_t> nxtVec;
			vector<int> nxtPaths;
			for (stateMap_iter st = nxtStateMap.begin(); st != nxtStateMap.end(); ++st) {
				cout << "State: ";
				st->second->printState();
				cout << "Hits : " << st->second->hit_count << endl;
				cout << "Vector: " << vecMap[st->first] << endl;
				cout << endl;

				int_vec edgeVec;
				edgeVec.clear();
				state_t *curr = st->second;
				for (int_vec_iter br = curr->branch_index.begin(); 
						br != curr->branch_index.end(); ++br) {
					if (IsBranchLeaf[*br])	{
						bEdge_t* curr_edge = branchGraph.getEdge(*br);
						if (curr_edge) 
							edgeVec.push_back(*br);
					}
				}

				int curr_val = edgeVec.back();
				bEdge_t* curr_edge = branchGraph.getEdge(curr_val);
				int start_val = curr_edge->startTop;
				int end_val = curr_edge->endTop;

				bNode_t* curr_node = branchGraph.getNode(end_val);
				assert(curr_node);

				string target_path = "Unreachable";
				int target_lvl = (2 << 20);
				int target_node = -1;

				/*	- Find path from curr_node to all the start_nodes of the uncovered edges
					- Find the uncovered edge with the shortest path
					- Set the topNode(start_node) as the target_node */
				int num_paths;
				for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
					string path;
#ifdef PATH_HASH_ON
					nodePair_t curr_pair = make_pair(end_val, *it);
					if (pathDB.find(curr_pair) == pathDB.end()) {
						path = getPathBFS(&branchGraph, end_val, *it);
						pathDB.insert(make_pair(curr_pair, path));
//						cout << "Absent" << endl;
					}
					else {
						path = pathDB[curr_pair];
//						cout << "Present" << endl;
					}
#endif

#ifndef PATH_HASH_ON
					path = getPathBFS(&branchGraph, end_val, *it);
#endif

					int lvl = path.length();

					cout << *it << ": " << path << endl;
					if (path.compare("Unreachable")) {
						num_paths++;
						if (target_lvl > lvl) {
							target_lvl = lvl;
							target_node = *it;
							target_path = path;
						}
					}
				}

				if (target_path.compare("Unreachable")) {
					nxtVec.push_back(vecMap[st->first]);
					nxtPaths.push_back(num_paths);
				}
			}

			int max_num_paths = -1, max_path_index = -1;
			if (nxtPaths.size() == 0) {
				//PrintVectorSet(paramObj2->inputVec, true);
				//getDominator(covGraph, unCovered);
				assert(nxtPaths.size());
				/* Might need to backtrack or just exit in this case */
			}

			for (int np = 0; np < nxtPaths.size(); ++np) {
				if (nxtPaths[np] > max_num_paths) {
					max_num_paths = nxtPaths[np];
					max_path_index = np;
				}
			}

			cout << endl;

			(paramObj2->stateList.back())->setCktState(cktVar);
			SimOneCycle(cktVar, nxtVec[max_path_index]);
			state_t *tmpState = new state_t(cktVar, paramObj2->stateList.size());
			(paramObj2->stateList).push_back(tmpState);
			paramObj2->inputVec += nxtVec[max_path_index];

			cout << "Adding vector: " << nxtVec[max_path_index] << endl
				<< "State: " << endl;
			tmpState->printState();
			cout << endl; 

			paramObj2->lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);
			for (int_vec_iter it = tmpState->branch_index.begin();
					it != tmpState->branch_index.end(); ++it)
				paramObj2->lastBranchHit[*it]++;

			paramObj2->branch_index = tmpState->branch_index;
			for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
				paramObj2->branchHit[ind] += paramObj2->lastBranchHit[ind];
			printCnt(paramObj2->lastBranchHit);
			printCnt(paramObj2->branchHit);

#ifdef OLD_STAGE2_CORE
			for( set<int>::iterator br = favorSet.begin(); br != favorSet.end(); ++br) {
				if (paramObj2->lastBranchHit[*br] != 0)
					paramObj2->favorSet.erase(*br);
			}
			cout << "Elements in favorSet after = ";
			printSet(paramObj2->favorSet);
			cout << endl;
#endif
			//		getchar();
			cout << " * * * * * * * * * * * * * * * * * * * * " << endl << endl;
		}
	}

#ifdef S2_DBG_PRINT
	int i = 0;
	for (state_pVec_iter st = paramObj2->stateList.begin();
			st != paramObj2->stateList.end(); ++st, ++i) {
		cout << paramObj2->inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << endl;
		(*st)->printState(1);
		cout << endl << endl;
	}
#endif

	return;
}

#ifdef NEW_STAGE2_CORE

void Stage2_Core(Stage2_Param* paramObj, int start_node) {

	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN = paramObj->INDIV_LEN/CONST_NUM_INPUT_BITS;
	
	brGraph_t* branchGraph = paramObj->branchGraph;
	Vtop *cktVar = paramObj->cktVar;
	int_vec &branchHit = paramObj->branchHit;
	int_vec &lastBranchHit = paramObj->lastBranchHit;
	
	lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);

	state_pVec startPool;
	state_t* start_state = paramObj->stateList.back();
	start_state->printState(1);
	startPool.push_back(start_state);

	start_state->setCktState(cktVar);
	printCktState(cktVar);
	ResetCounters(cktVar);

	int TAR_EDGE_FITNESS	= -200;
	int TAR_NODE_FITNESS	= -100;
	int SELF_LOOP_FITNESS 	= -50;
	int OUT_EDGE_FITNESS	= 100;
	int UNIMPT_FITNESS		= 0;

	/*	Run GA	*/
	gaPopulation_t stage2Pop(POP_SIZE, VEC_LEN);
	stage2Pop.initPopulation(startPool);
	
	fitness_t prev_gen_fitness = (2 << 24), curr_gen_fitness = (2 << 25);
	string path = paramObj->currPath;
	//NUM_GEN = 1;
	for (int gen = 0; gen < NUM_GEN; ++gen) {
		
		cout << endl << "GEN " << gen << endl;

		fitness_t curr_gen_max_fit = - (2 << 20), curr_gen_min_fit = (2 << 20);
		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage2Pop.indiv_vec[ind];

			/* Reset masking for b12 */
			#if defined(__b12)
			for (int x = 0; x < indiv->input_vec.length(); x += 5) 
				indiv->input_vec[x] = '0';
			#endif

			indiv->simCkt(cktVar);
			
			int path_idx = 0, exit_state_loop = 0;
			int curr_node = start_node;
			int indiv_idx = 0;
			int_vec state_fit_vec;
			fitness_t prev_fit = 0;

			for (state_pVec_iter st = indiv->state_list.begin();
					st != indiv->state_list.end(); ++st) {

				/* Compute	- target edge [nxt_edge]
							- target node [nxt_node]
							- set of branches in self loop	*/

				bNode_t* curr_node_obj = branchGraph->getNode(curr_node);
				assert(curr_node_obj);

				int nxt_node = curr_node_obj->outNodes[path[path_idx] - '0'];
				int nxt_edge = curr_node_obj->outEdges[path[path_idx] - '0'];

				set<int> self_loop_br, out_node_br;
				for (int bt = 0; (uint) bt < curr_node_obj->outEdges.size(); ++bt) { 
					int br = curr_node_obj->outEdges[bt];
					if (br == nxt_edge)
						continue;

					if (curr_node_obj->outNodes[bt] == curr_node)
						self_loop_br.insert(br);
					else
						out_node_br.insert(br);
				}
				
				int state_fit = 0;
				for (int_vec_iter br = (*st)->branch_index.begin(); 
						br != (*st)->branch_index.end(); ++br) {
					
					/* Add vector to branch	*/

					if(IsBranchLeaf[*br] == 0)
						continue;

					bEdge_t* curr_edge_obj = branchGraph->getEdge(*br); 
					if (curr_edge_obj == NULL)
						continue;
					
					/* If correct edge [nxt_edge] was taken */
					if (*br == nxt_edge) {
//						state_fit += (TAR_EDGE_FITNESS) * (path_idx + 1);
						state_fit += prev_fit + TAR_EDGE_FITNESS;
						path_idx++;
						curr_node = nxt_node;
						break;
					}
						
					/* if correct node [nxt_node] was reached */
					else if (curr_edge_obj->endTop == nxt_node) {
//						state_fit += (TAR_NODE_FITNESS) * (path_idx + 1);
						state_fit += prev_fit + TAR_NODE_FITNESS;
						path_idx++;
						curr_node = nxt_node;
						break;
					}

					/* If self loop was taken */
					else if (self_loop_br.find(*br) != self_loop_br.end()) {
						state_fit += prev_fit + SELF_LOOP_FITNESS;
						break;
					}

					/* Else if any other edge was taken */
					else if (out_node_br.find(*br) != out_node_br.end()) {
						indiv_idx = path_idx;	
						exit_state_loop = true;
						state_fit += prev_fit + OUT_EDGE_FITNESS;
						break;
					}

					/* If the branch does not matter */
					else {
						state_fit += UNIMPT_FITNESS;
					}
				}	

				state_fit_vec.push_back(state_fit);
				prev_fit = state_fit;

				if ((exit_state_loop) || (path_idx == path.length())){
//					cout << "Indiv " << ind << " digressed @ index " 
//						 << path_idx << "after" << state_fit_vec.size()
//						 << endl;
					break;
				}

			}	// END State
			
//			cout << "Indiv " << ind << " reached " << state_fit_vec.size()
//				 << " vectors"  << endl
//				 << "Fitness: ";
//			printVec(state_fit_vec);
//			cout << endl;
//			indiv->printIndiv(0);

			indiv->fitness = 0;			
			int max_ind = -1, max_fit = (2 << 20);
			for (int it = 0; (uint)it < state_fit_vec.size(); ++it) {
				if (max_fit >= state_fit_vec[it]) {
					max_ind = it;
					max_fit = state_fit_vec[it];
				}
				indiv->fitness += state_fit_vec[it];
			}
			if (max_ind != -1)
				indiv->max_index = max_ind;

			if (curr_gen_fitness >= indiv->fitness)
				curr_gen_fitness = indiv->fitness;

			if (curr_gen_min_fit >= indiv->fitness)
				curr_gen_min_fit = indiv->fitness;

			if (curr_gen_max_fit <= indiv->fitness)
				curr_gen_max_fit = indiv->fitness;

#ifdef S2_DBG_PRINT
			cout << indiv->fitness << " : " 
				<< indiv->max_index + 1 << " vectors " 
				<< endl << endl;
#endif
		}	// END Indiv

//		cout << "Max: " << curr_gen_max_fit
//			<< " Min: " << curr_gen_min_fit << endl;
		bool gaTerminate = false;

		if ((curr_gen_min_fit == curr_gen_max_fit) ||
				(gen == NUM_GEN - 1) ||
				(prev_gen_fitness < curr_gen_fitness))
			gaTerminate = true;
			
		if (gaTerminate) {
			cout << "Terminating after GEN " << gen << endl;
			break;
		}
		else {
			stage2Pop.gaEvolve();
			prev_gen_fitness = curr_gen_fitness;
			curr_gen_fitness = (2 << 25);
		}

	}	// END Generation

#ifndef GA_FIND_TOP_INDIV
	std::sort(stage2Pop.indiv_vec.begin(), stage2Pop.indiv_vec.end(), compFitness);
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
#endif

#ifdef GA_FIND_TOP_INDIV
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
	for (gaIndiv_pVec_iter gt = stage2Pop.indiv_vec.begin(); 
			gt != stage2Pop.indiv_vec.end(); ++gt) {
		if (compFitness(*gt, indiv))
			indiv = *gt;
	}
#endif
	
	cout << endl << "After GA(Core) : " << endl;
	indiv->printIndiv(0);
//	cout << endl << indiv->max_index + 1 << " vectors copied" << endl;

	paramObj->inputVec += indiv->input_vec.substr(0, 
					CONST_NUM_INPUT_BITS * (indiv->max_index + 1));

	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
		if (st == NULL) {
			cout << "GG" << endl;
			break;
		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			lastBranchHit[*it]++;

		paramObj->stateList.push_back(indiv->state_list[ind]);
		indiv->state_list[ind] = NULL;
	}

	paramObj->branch_index = paramObj->stateList.back()->branch_index;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
		branchHit[ind] += lastBranchHit[ind];

	cout << "Returning from Stage2_Core" << endl;
	printCnt(paramObj->branchHit);

}

#endif 

#ifdef OLD_STAGE2_CORE
void Stage2_Core(Stage2_Param* paramObj) {

	cout << endl 
		<< "GA Stage 2: " << endl
		<< "Previous Coverage: " << endl;
	printCnt(paramObj->branchHit);
	cout << endl;

	cout << "Current params: " << endl
		<< "Edge: " << paramObj->curr_val << endl
		<< "Node: " << paramObj->end_val << endl;

	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN = paramObj->INDIV_LEN/CONST_NUM_INPUT_BITS;
	
	brGraph_t *branchGraph = paramObj->branchGraph;
	set<int> &favorSet = paramObj->favorSet;
	int_vec &branchHit = paramObj->branchHit;
	int_vec &lastBranchHit = paramObj->lastBranchHit;
	Vtop *cktVar = paramObj->cktVar;

	lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);

	state_pVec startPool;
	state_t* start_state = paramObj->stateList.back();
	start_state->printState(1);
	startPool.push_back(start_state);

	start_state->setCktState(cktVar);
	printCktState(cktVar);
	ResetCounters(cktVar);

#ifdef S2_DBG_PRINT
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		cout << "Branch " << br << ":\t " << BranchInputVec[br] 
			//<< "\t " << BranchNumTries[br]
			<< endl << BranchStateVal[br] << endl;
	}
	cout << endl;
#endif

	cout << "Elements in favorSet before = { ";
	printSet(paramObj->favorSet);
	cout << " } " <<  endl;

	/*	Run GA	*/
	gaPopulation_t stage2Pop(POP_SIZE, VEC_LEN);
	stage2Pop.initPopulation(startPool);

	int WT_TAR_EDGE = -200;
	int WT_TAR_NODE = -100;
	int WT_SELF_EDGE = -50;
	int WT_OUT_EDGE = 100;
	int WT_UNIMPT	= 200;

	int tar_edge = paramObj->tar_edge;
	int tar_node = paramObj->tar_node;

	bEdge_t* tar_edge_obj = branchGraph->getEdge(tar_edge);
	assert(tar_edge_obj);
	if (tar_edge_obj->endTop != tar_node) {
		cout << "Edge " << tar_edge 
			<< " ends in "	<< tar_edge_obj->endTop
			<< " not in " 	<< tar_node << endl;
	}
	assert(tar_edge_obj->endTop == tar_node);

	bNode_t* tar_node_obj = branchGraph->getNode(tar_node);
	assert(tar_edge_obj);

	bool gaTerminate = false;
	fitness_t prev_gen_fitness = (2 << 24), curr_gen_fitness = (2 << 25);

	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		fitness_t curr_gen_max_fit = - (2 << 25), curr_gen_min_fit = (2 << 25);
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage2Pop.indiv_vec[ind];

			/* Reset masking for b12 */
#if defined(__b12)
			for (int x = 0; x < indiv->input_vec.length(); x += 5) 
				indiv->input_vec[x] = '0';
#endif

			indiv->simCkt(cktVar);

			int_vec state_fit_vec;

			//			cout << "States: " << endl;
			int branch_val = -1;
			bool exit_state = false;
			int sv = 0;
			for (state_pVec_iter st = indiv->state_list.begin();
					st != indiv->state_list.end(); ++st, ++sv) {

				int state_fit = 0;
				vecIn_t inp = indiv->input_vec.substr(sv*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
				for (int_vec_iter br = (*st)->branch_index.begin(); 
						br != (*st)->branch_index.end() && !exit_state; ++br) {

					AddVector2Branch(*br, inp);

					if (IsBranchLeaf[*br] == 0)
						continue;

					bEdge_t* curr_edge_obj = branchGraph->getEdge(*br);
					if (curr_edge_obj == NULL)
						continue;

					if (*br == tar_edge) {
						state_fit += WT_TAR_EDGE;
						exit_state = true;
						branch_val = *br;
						gaTerminate = true;
					}
					else if (curr_edge_obj->endTop == tar_edge_obj->endTop) {
						state_fit += WT_TAR_NODE;
						branch_val = *br;
						exit_state = true;
					}
					else {
						int curr_node = curr_edge_obj->startTop;
						bNode_t* curr_node_obj = branchGraph->getNode(curr_node);

						bool unimpt_branch = true;
						for (int bt = 0; (uint) bt < curr_node_obj->outEdges.size(); ++bt) { 
							if (curr_node_obj->outEdges[bt] == *br) {
								unimpt_branch = false;
								if (curr_node_obj->outNodes[bt] == curr_node) {
									state_fit += WT_SELF_EDGE;
									exit_state = false;
								}
								else {
									state_fit += WT_OUT_EDGE;
									exit_state = true;
									branch_val = *br;
								}
							}
						}

						if(unimpt_branch)
							state_fit += WT_UNIMPT;
					}

				}

				state_fit_vec.push_back(state_fit);
				if (exit_state) {
#ifdef S2_DBG_PRINT
					cout << branch_val << " reached in " << state_fit_vec.size() << " vectors" << endl;
#endif
					break;
				}

			}

#ifdef S2_DBG_PRINT
			cout << "Fitness: ";
			printVec(state_fit_vec);
			cout << endl;
#endif

			indiv->fitness = 0;			
			int max_ind = -1, max_fit = (2 << 20);
			for (int it = 0; (uint)it < state_fit_vec.size(); ++it) {
				if (max_fit >= state_fit_vec[it]) {
					max_ind = it;
					max_fit = state_fit_vec[it];
				}
				indiv->fitness += state_fit_vec[it];
			}
			//			indiv->fitness /= indiv->state_list.size();
			if (max_ind != -1)
				indiv->max_index = max_ind;
			//			indiv->max_index = state_fit_vec.size() - 1;

			if (curr_gen_fitness >= indiv->fitness)
				curr_gen_fitness = indiv->fitness;

			if (curr_gen_min_fit >= indiv->fitness)
				curr_gen_min_fit = indiv->fitness;

			if (curr_gen_max_fit <= indiv->fitness)
				curr_gen_max_fit = indiv->fitness;

#ifdef S2_DBG_PRINT
			cout << "Fitness(" << indiv->index << "): " 
				<< indiv->fitness << " : " 
				<< indiv->max_index + 1 << " vectors " 
				<< endl << endl;
#endif
		}	

		cout << "Max: " << curr_gen_max_fit
			<< " Min: " << curr_gen_min_fit << endl;
		if (curr_gen_max_fit == curr_gen_min_fit)
			gaTerminate = true;

		gaTerminate = gaTerminate || (gen == NUM_GEN-1) || (prev_gen_fitness < curr_gen_fitness);
		if (!gaTerminate) {
			stage2Pop.gaEvolve();
			cout << "Evolved" << endl;
		}

		prev_gen_fitness = curr_gen_fitness;
		curr_gen_fitness = (2 << 25);
	}

	cout << "After GA : " << endl;

#ifndef GA_FIND_TOP_INDIV
	std::sort(stage2Pop.indiv_vec.begin(), stage2Pop.indiv_vec.end(), compFitness);
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
#endif

#ifdef GA_FIND_TOP_INDIV
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
	for (gaIndiv_pVec_iter gt = stage2Pop.indiv_vec.begin(); 
			gt != stage2Pop.indiv_vec.end(); ++gt) {
		if (compFitness(*gt, indiv))
			indiv = *gt;
	}
#endif
	indiv->printIndiv(1);

	cout << endl << indiv->max_index + 1 << " vectors copied" << endl;

	paramObj->inputVec += indiv->input_vec.substr(0, CONST_NUM_INPUT_BITS * (indiv->max_index + 1));
	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
		if (st == NULL) {
			cout << "GG" << endl;
			break;
		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			//paramObj->branchHit[*it]++;
			lastBranchHit[*it]++;

		paramObj->stateList.push_back(indiv->state_list[ind]);
		indiv->state_list[ind] = NULL;
	}

	paramObj->branch_index = paramObj->stateList.back()->branch_index;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
		branchHit[ind] += lastBranchHit[ind];
#ifdef S2_DBG_PRINT
#ifdef S2_DBG_PRINT_EXTRA
	printCnt(lastBranchHit);
#endif
	printCnt(paramObj->branchHit);
#endif

	for( set<int>::iterator br = favorSet.begin(); br != favorSet.end(); ++br) {
		if (lastBranchHit[*br] != 0)
			favorSet.erase(*br);
	}
#ifdef S2_DBG_PRINT
	cout << "Elements in favorSet after = ";
	printSet(paramObj->favorSet);
	cout << endl;
#endif

}
#endif

string getPathBFS(brGraph_t* brGraph, int start, int target) {

#ifdef S2_DBG_PRINT
	cout << "Finding path from " << start << " -> " << target << endl;
#endif
	map<int, string> label;
	map<int, int> level;

	typedef pair< map<int, string>::iterator, bool> retStr;

	int max_level = -1;
	string path = "";
	vector<int> bfsQueue;
	bfsQueue.push_back(start);
	level.insert(make_pair(start, 0));
	label.insert(make_pair(start, ""));

	const int THRESH_LEVEL = 20;
	const int MAX_QUEUE_SIZE = 1000;
	for (int qInd = 0; (uint) qInd < bfsQueue.size(); ++qInd) {

		int front = bfsQueue[qInd];
		bNode_t* curr = brGraph->getNode(front);
		assert(curr);

		//		cout << "F: " << front << endl;
		for (int eInd = 0; (uint) eInd < curr->outNodes.size(); ++eInd) {
			int node_ = curr->outNodes[eInd];

			retStr ret;
			char ch[2];
			ch[0] = eInd + 48; ch[1] = '\0';
			string tmpLabel = label[front] + string(ch);
			ret = label.insert(make_pair(node_, tmpLabel));

			int tmpLevel = level[front] + 1;
			if (ret.second) {
				level.insert(make_pair(node_, tmpLevel));
			}
			//			if (node_ == target) {
			//				path = tmpLabel;
			//				return tmpLevel;
			//			} 

			int edge_ = curr->outEdges[eInd];
			if (edge_ == target) {
				//				path = tmpLabel;
				//				return tmpLevel;
				return tmpLabel;
			}

			if (max_level < tmpLevel)
				max_level = tmpLevel;

			if (ret.second) {
				bfsQueue.push_back(node_);
			}

			//			cout << "Queued " << node_ 
			//				 << " Level: " << tmpLevel
			//				 << " Label: " << tmpLabel
			//				 << " Max_Lvl: " << max_level << endl;
		}

		//		cout << "Queue (" << bfsQueue.size() << ")" << endl;
		//		printVec(bfsQueue);
		//		cout << endl;

		if ((max_level > THRESH_LEVEL) || (qInd > MAX_QUEUE_SIZE)) {
			cout << "Lvl: " << max_level << "/" << THRESH_LEVEL << "\t"
				<< "Q: " << qInd << "/" << MAX_QUEUE_SIZE << endl;
			path = "Unreachable";
			return path;
		}

	}

	path = "Unreachable";
	return path;
}

bool readTopNodes(covGraph_t& graphCov, brGraph_t& branchGraph) {

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
			IsBranchLeaf[branch_] = 1;
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
		return false;
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
	branchGraph.printGraph();

	return true;
}

void Stage1_GenerateVectors(Stage1_Param* paramObj) {

	// Parameters
	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN	= paramObj->INDIV_LEN / CONST_NUM_INPUT_BITS;
	//	int NUM_INDIV = paramObj->NUM_INDIV;
	//	int TOP_INDIV = paramObj->NUM_TOP_INDIV;
	int MAX_ROUNDS = paramObj->MAX_ROUNDS;

	/* TODO :
	   Need to set the following weights according to the ckt parameters
	   -	Depth of the ckt (Max branches that can be reached in a single cycle)
	 */

	double WT_FIT_STATE = 0.0;
	double WT_FIT_COV = 0.3;
	double WT_FIT_BRANCH = 0.7;

	int WT_COV = 500;
	int WT_NEW_BRANCH = 2000;

	cout << "GA Stage 1: " << endl
		<< POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;

	Vtop *cktVar = new Vtop;

	//	int_vec branch_counters(CONST_NUM_BRANCH, 0);
	int_vec branch_counters = exclBranchList;

	SimMultiCycle(cktVar, 0);
	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
		if (GetCoverage(cktVar, br))
			branch_counters[br] = -5;

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

	stateMap_t currStateMap, glStateMap;
	currStateMap.clear();
	int prevMaxCov = 0;

	gaIndiv_pVec topIndiv_vec;
	bool gaTerminate = false;
	//	int last_gen = 0;
	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		bool improv_cov = false; 
		bool new_state = false;

		int maxCov = 0, avgCov = 0;
		int_vec maxInd;

		for (int ind = 0; ind < POP_SIZE; ++ind) {

			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];

			/* Reset masking for b12 */
			//			#if defined(__b12)
			//			for (int x = 0; x < indiv->input_vec.length(); x += 5) 
			//				indiv->input_vec[x] = '0';
			//			#endif

			indiv->simCkt(cktVar);

			//			indiv->printIndiv(1);

			/* Add states to stateMap	*/
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st) {
				state_t *curr = *st;
				keyVal_t hash_val_ = curr->getHash();
				retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));

				curr->hit_count = 1;
				/*	If the key for the state is present	=>	New state was not found
					If the value for the key = NULL	
					=> 	The state was found in the previous cycle
					Therefore, set the new found state as the value for that key.
					Else => The state was found in this cycle
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

			indiv->num_branch = computeBranches(indiv->branch_cov);

			/* Computing max and avg branch coverage */
			if (maxCov < indiv->num_branch)
				maxCov = indiv->num_branch;

			avgCov += indiv->num_branch;

		}

		cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
			<< "Max Coverage: " << maxCov << endl;

		avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
		improv_cov = (maxCov >= prevMaxCov);

		/* Fittest Individuals i.e. individuals with maximum coverage	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];
			if (indiv->num_branch == maxCov) {
				cout << ind << " ";
				maxInd.push_back(ind);
			}

		}
		cout << endl;	

		/* Compute Fitness	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

			/* Fitness for #branches covered */
			fitness_t fitness_cov = (avgCov - indiv->num_branch) * WT_COV;

			/* Fitness for branch coverage	*/
			fitness_t fitness_branch = 0;

			/* Fitness for states reached 	*/
			fitness_t fitness_state = 0;

			/* Combining all fitness values	*/
			indiv->fitness 
				= (fitness_cov * WT_FIT_COV) 
				+ (fitness_branch * WT_FIT_BRANCH)
				+ (fitness_state * WT_FIT_STATE);
		}	


		/* Terminate GA Condition:
		   - gen == NUM_GEN
		   - coverage[gen] <= coverage[gen-1]
		   - no new states reached	*/

		gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

		if (!gaTerminate) {

			for (stateMap_iter st = currStateMap.begin(); 
					st != currStateMap.end(); ++st) 
				currStateMap[st->first] = NULL;

#ifndef GA_FIND_TOP_INDIV
			//			for (int ind = 0; ind < POP_SIZE; ++ind) {
			//				(stage0Pop.indiv_vec[ind])->printIndiv(1);
			//			}
			std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
			cout << "Fitness: " << (stage0Pop.indiv_vec[0])->fitness << "\t | " << (stage0Pop.indiv_vec[0])->index << endl;
			gaIndiv_t *tmp = new gaIndiv_t(*(stage0Pop.indiv_vec[0]));
#endif

#ifdef GA_FIND_TOP_INDIV
			gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

			//			for (int ind = 0; ind < POP_SIZE; ++ind) {
			//				gaIndiv_t *gt = stage0Pop.indiv_vec[ind];
			////				gt->printIndiv(1);
			//				if (compCoverage(gt, indiv))
			//					indiv = gt;
			//			}

			for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
					gt != stage0Pop.indiv_vec.end(); ++gt) {
				if (compCoverage(*gt, indiv))
					indiv = *gt;
			}
			cout << "Fitness: " << indiv->fitness << "\t | " << indiv->index << endl;
			gaIndiv_t *tmp = new gaIndiv_t(*indiv);
#endif

			topIndiv_vec.push_back(tmp);

			stage0Pop.gaEvolve();

			prevMaxCov = maxCov;
		}
		else
			cout << "Terminating after gen " << gen << endl;

	}

	cout << "States allocated: " << state_t::mem_alloc_cnt << endl;

	for (gaIndiv_pVec_iter gt = topIndiv_vec.begin();
			gt != topIndiv_vec.end(); ++gt) {
		gaIndiv_t *tmp = *gt;
		tmp->state_list = state_pVec(tmp->vec_length, NULL);
		tmp->simCkt(cktVar);
	}

	/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
#ifndef GA_FIND_TOP_INDIV
	std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);

	topIndiv_vec.push_back(stage0Pop.indiv_vec[0]);
	stage0Pop.indiv_vec[0] = NULL;
#endif

#ifdef GA_FIND_TOP_INDIV
	gaIndiv_t *tmp = stage0Pop.indiv_vec[0];
	int max_idx = 0, idx = 0;
	for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
			gt != stage0Pop.indiv_vec.end(); ++gt, ++idx) {
		if (compCoverage(*gt, tmp)) {
			tmp = *gt;
			max_idx = idx;
		}
	}

	topIndiv_vec.push_back(stage0Pop.indiv_vec[max_idx]);
	stage0Pop.indiv_vec[max_idx] = NULL;
#endif

	/* Sorting to calculate the best indiv among the N generations */
	std::sort(topIndiv_vec.begin(), topIndiv_vec.end(), compCoverage);
	gaIndiv_t *indiv = topIndiv_vec[0];

	gaIndiv_pVec_iter gt = topIndiv_vec.begin(); 
	gt++;
	for (;gt != topIndiv_vec.end(); ++gt) {
		delete *gt;
		*gt = NULL;
	}

	topIndiv_vec.clear();

	/* ***** ROUND 1:MAX_ROUNDS	***** */

	/*	Need to add the final state to statePool only if its unique	
		If not unique, the indiv with greater coverage should be added
		If all coverages are same, the indiv with lesser max_index should be added
	 */

	vecIn_t startVec;
	startPool.clear();

	ResetCounters(cktVar);
	SimMultiCycle(cktVar, 0);
	indiv->simCkt(cktVar);

	//	indiv->printIndiv(1);
	for (int st = 0; st <= indiv->max_index; ++st) {	
		state_t *curr = indiv->state_list[st];
		keyVal_t hash_val_ = curr->getHash();
		glStateMap.insert(make_pair(hash_val_, curr));

		for (int_vec_iter bt = curr->branch_index.begin();
				bt != curr->branch_index.end(); ++bt)
			branch_counters[*bt]++;

	}
	state_t* st = indiv->state_list[indiv->max_index];
	startVec = indiv->input_vec.substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));
	startPool.push_back(st);

	cout << "After Round 0: " << endl;
	st->printState();
	cout << " --------------- " << endl;

	printCnt(branch_counters);
	cout << " --------------- " << endl;

	bool Rnd_terminate = false;

	bool all_done = true;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		if (!IsDefaultBranch(br) && (branch_counters[br] == 0))
			all_done = false;
	}

	for (int round = 1; !all_done && round < MAX_ROUNDS && !Rnd_terminate; ++round) {

		gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
		/*	Init population with one of the states in the pool 
			of start states and string of random vectors	*/
		stage0Pop.initPopulation(startPool);

		stateMap_t currStateMap;
		//stateMap_t currStateMap = glStateMap;
		int prevMaxCov = 0;

		bool gaTerminate = false;
		for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

			cout << "GEN " << gen << endl << endl;

			bool improv_cov = false; 
			bool new_state = false;

			int maxCov = 0, avgCov = 0;
			int_vec maxInd;

			for (int ind = 0; ind < POP_SIZE; ++ind) {

				ResetCounters(cktVar);
				gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
				indiv->simCkt(cktVar);

				//				indiv->printIndiv(1);

				/* Add states to stateMap	*/
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st) {
					state_t *curr = *st;
					keyVal_t hash_val_ = curr->getHash();
					retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));

					curr->hit_count = 1;
					/*	If the key for the state is present	=>	New state was not found

						If the value for the key = NULL	
						=> The state was found in the previous cycle. 
						Therefore, set the new found state as the value for that key.

						Else => The state was found in this cycle
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

				indiv->num_branch = computeBranches(indiv->branch_cov);

				/* Computing max and avg branch coverage */
				if (maxCov < indiv->num_branch)
					maxCov = indiv->num_branch;

				avgCov += indiv->num_branch;

			}

			cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
				<< "Max Coverage: " << maxCov << endl;

			avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
			improv_cov = (maxCov >= prevMaxCov);

			/* Compute Fitness	*/
			for (int ind = 0; ind < POP_SIZE; ++ind) {

				gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

				/* Fitness for #branches covered */
				fitness_t fitness_cov = (avgCov - indiv->num_branch) * WT_COV;

				/* Fitness for branch coverage	*/
				fitness_t fitness_branch = 0;
				for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
					if (indiv->branch_cov[br] && (branch_counters[br] == 0) 
							&& (exclBranchList[br] == 0))
						fitness_branch -= WT_NEW_BRANCH;
				}

				/* Fitness for states reached 	*/
				fitness_t fitness_state = 0;

				/* Combining all fitness values	*/
				indiv->fitness 
					= (fitness_cov * WT_FIT_COV) 
					+ (fitness_branch * WT_FIT_BRANCH)
					+ (fitness_state * WT_FIT_STATE);

				//				cout << "Fitness (" << ind << ") " 
				//					 << indiv->fitness 
				//					 << " [" << fitness_cov 
				//					 << ", " << fitness_branch 
				//					 << "]" << endl << endl;
			}	


			/* Terminate GA Condition:
			   - gen == NUM_GEN
			   - coverage[gen] <= coverage[gen-1]
			   - no new states reached	*/

			gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

			if (!gaTerminate) {

				for (stateMap_iter st = currStateMap.begin(); 
						st != currStateMap.end(); ++st) 
					currStateMap[st->first] = NULL;

				stage0Pop.gaEvolve();

				prevMaxCov = maxCov;
			}
			else {
				cout << "Terminating after gen " << gen << endl;
				if (prevMaxCov < maxCov)
					prevMaxCov = maxCov;
			}

		}

#ifndef GA_FIND_TOP_INDIV
		/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
		std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compFitness);
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
#endif

#ifdef GA_FIND_TOP_INDIV
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
		for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
				gt != stage0Pop.indiv_vec.end(); ++gt) {
			if (compFitness(*gt, indiv))
				indiv = *gt;
		}
#endif

		cout << "Fittest indiv after round " << round << endl;
		indiv->printIndiv(1);

		int_vec curr_branch_cnt (CONST_NUM_BRANCH, 0);
		bool new_start_state = false;
		for (int st = 0; st <= indiv->max_index; ++st) {	
			state_t *curr = indiv->state_list[st];
			keyVal_t hash_val_ = curr->getHash();
			retVal_t ret = glStateMap.insert(make_pair(hash_val_, curr));

			if (ret.second == true) {
				cout << "X";
				new_start_state = true;
			}
			else {
				cout << "-";
			}
			for (int_vec_iter bt = curr->branch_index.begin();
					bt != curr->branch_index.end(); ++bt)
				curr_branch_cnt[*bt]++;

		}

		cout << endl;

		bool new_branch_ = false;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
			if ((curr_branch_cnt[br]) && (branch_counters[br] == 0))
				new_branch_ = true;
			branch_counters[br] += curr_branch_cnt[br];
		}

		bool all_done_2 = true;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
			if (!IsDefaultBranch(br) && (branch_counters[br] == 0)) {
				all_done_2 = false;
				break;
			}
		}

		Rnd_terminate = !(new_start_state || new_branch_) || (round == MAX_ROUNDS-1) || (all_done_2);

		startVec += indiv->input_vec.substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));

		for (state_pVec_iter st = startPool.begin(); st != startPool.end(); ++st)
			if (*st == NULL) {
				delete *st;
				*st = NULL;
			}

		startPool.clear();
		state_t* st = indiv->state_list[indiv->max_index];
		indiv->state_list[indiv->max_index] = NULL;

		startPool.push_back(st);

		cout << "After Round " << round << " : " << endl;
		st->printState();
		cout << " --------------- " << endl;

		printCnt(branch_counters);
		cout << " --------------- " << endl;

		if (Rnd_terminate)
			cout << "Exiting after round " << round << endl;
	}		

	rstState->setCktState(cktVar);
	SimMultiCycle(cktVar, startVec);
	int_vec branch_cov;
	GetBranchCounters(cktVar, branch_cov);

	cout << "Final coverage: " << endl;
	printCnt(branch_cov);

	gaIndiv_t *indiv_ = new gaIndiv_t(0, 
			startVec.length()/CONST_NUM_INPUT_BITS,
			rstState);
	indiv_->input_vec = startVec;
	indiv_->simCkt(cktVar);

	paramObj->inputVec = startVec;
	paramObj->branchHit = branch_cov;
	paramObj->stateList = indiv_->state_list;

	//	cout << "Branches covered after Stage 1: " << endl;
	//	printCnt(paramObj->branchHit);

	delete rstState;
	delete cktVar;
}

int AddVector2Branch(int branch_index, vecIn_t inp) {
	assert(inp.length() == CONST_NUM_INPUT_BITS);
	if (BranchInputVec[branch_index].compare("") == 0) {
		BranchInputVec[branch_index] = inp;
		return 0;
	}
	else {
		vecIn_t& brInp = BranchInputVec[branch_index];
		bool t = false;
		for (int ind = 0; ind < CONST_NUM_INPUT_BITS; ++ind) {
			if (brInp[ind] == 'X')
				continue;
			else if (brInp[ind] != inp[ind]) {
				brInp[ind] = 'X';
				t = true;
			}
		}
		if (t)
			return -1;
		else
			return 1;
	}
}

int AddState2Branch(int branch_index, state_t* st) {
	if (BranchStateVal[branch_index].compare("") == 0) {
		BranchStateVal[branch_index] = st->state_val;
		return 0;
	}
	else {
		string &brSt = BranchStateVal[branch_index];
		string inp = st->state_val;
		bool t = false;
		for (int ind = 0; ind < CONST_NUM_STATE_BITS; ++ind) {
			if (brSt[ind] == 'X')
				continue;
			else if (brSt[ind] != inp[ind]) {
				brSt[ind] = 'X';
				t = true;
			}
		}
		if (t)
			return -1;
		else
			return 1;
	}
}

/* Read the excluded branch list from ckt.excl or from interface file	*/
void readExclBranchList () {

	ifstream fileIn;
	char fName[80];
	sprintf(fName, "%s.excl", benchCkt);
	fileIn.open(fName, ios::in);

	if(fileIn) {
		cout << "Reading excluded branch list from "
			<< fName << endl;
	}
	else {
		cout << "Using default branch list from " 
			<< benchCkt << "Int.h" << endl;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br){ 
			if (IsDefaultBranch(br)) {
				exclBranchList[br] = -10;
				cout << br << " ";
			}
		}
		cout << endl;
		return;
	}

	while(fileIn) {	
		stringstream ss;
		string curr;
		getline(fileIn, curr);

		ss << curr;
		int br;
		ss >> br;
		while(ss) {
			cout << br << " ";
			exclBranchList[br] = -10;
			ss >> br;
		}
		cout << endl;
	}

	printCnt(exclBranchList);
	return;
}

/* Compute branch coverage of individual - branches in exclBranchList */
int computeBranches(vector<int>& branch_cov) {

	assert(branch_cov.size() == (uint)CONST_NUM_BRANCH);
	int num_branch = 0;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		if (branch_cov[br] && !exclBranchList[br])
			num_branch ++;
	}
	return num_branch;
}

/* Read parameters for Stage 1 from ckt.param	*/
void readParam (Stage1_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			<< "Using default values. " << endl
			<< "NUM_GEN_0 	: 4 generations" << endl
			<< "POP_SIZE_0	: 256 individuals" << endl
			<< "INDIV_LEN_0: 20 bits" << endl
			<< "MAX_ROUNDS_0: 10 rounds" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
		paramObj->NUM_INDIV = 2;
		paramObj->MAX_ROUNDS = 10;
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
			else if (curr.find("MAX_ROUNDS_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_ROUNDS = " << val << endl;
					paramObj->MAX_ROUNDS = val;
				}
				else 
					paramObj->MAX_ROUNDS = 10;
			}

		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

/* Read parameters for Stage 1 from ckt.param	*/
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
		paramObj->MAX_ROUNDS = 100;
		paramObj->MAX_TRIES = 20;
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
			else if (curr.find("MAX_ROUNDS") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_ROUNDS = " << val << endl;
					paramObj->MAX_ROUNDS = val;
				}
				else 
					paramObj->MAX_ROUNDS = 20;
			}
			else if (curr.find("MAX_TRIES") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_TRIES = " << val << endl;
					paramObj->MAX_TRIES = val;
				}
				else 
					paramObj->MAX_TRIES = 20;
			}
		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

/* Functions to print contents of state maps, vector<int> and set<int>	*/
void DisplayMap(pathMap_t& pathDB) {
	typedef pathMap_t::iterator pathMap_iter;
	for (pathMap_iter it = pathDB.begin(); it!= pathDB.end(); ++it) {

		cout << (it->first).first << " -> " 
			<< (it->first).second << endl;
		cout << it->second << endl;

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

/* Print branches and corresponding counter values	*/
void printCnt(int_vec& vec_) {
	int num_branch = 0;
	for (uint v = 0; v < vec_.size(); ++v) {
		cout << v << "(" << vec_[v] << ") ";
		if (vec_[v] == 0)
			num_branch++;
	}
	cout << endl << "#Branch " << CONST_NUM_BRANCH - num_branch << endl;
}

/* Print the test vector set inputVec
   %2 = True	=>	Print to file ckt_(rand).vec	*/
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

/*	Comparator function for sort() in Stage 1 - Round 0	*/
bool compCoverage(gaIndiv_t* A, gaIndiv_t* B) {

	if (A->num_branch != B->num_branch)
		return (A->num_branch > B->num_branch);
	else 
		return (A->max_index < B->max_index);

}

