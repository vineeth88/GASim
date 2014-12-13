#include "verInclude.h"
#include "incl.h"
#include "verilated.h"

char vecFile[100];
void ReadInputVectors(vecIn_t&, int);
int printCnt(int_vec& vec);

int state_t::mem_alloc_cnt = 0;

int main(int argc, char* argv[]) {

	Vtop *cktVar = new Vtop;
	int_vec branch_counters(CONST_NUM_BRANCH, 0);

	if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " <ckt>.vec" << endl;
        }
        else {
            sprintf(vecFile, "%s", argv[1]);
            cout << "Reading " << vecFile << endl;
        }
	}		

//	SimMultiCycle(cktVar, 0);
//	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
//		if (GetCoverage(cktVar, br))
//			branch_counters[br] = 5;
	
	vecIn_t inputVec;
	ReadInputVectors(inputVec,-1);
	SimMultiCycle(cktVar, inputVec);
//	int len_vec = inputVec.length() / CONST_NUM_INPUT_BITS;
//	cout << "Length: " << len_vec << endl << endl;
//	state_pVec stateList(len_vec, NULL);
//	for (int ind = 0; ind < len_vec; ++ind) {
//		vecIn_t inp = inputVec.substr(ind*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
//		SimOneCycle(cktVar, inp);
//		stateList[ind] = new state_t(cktVar, ind);
//		ResetCounters(cktVar);
//	}
//	int i = 0;
//	for (state_pVec_iter st = stateList.begin();
//			st != stateList.end(); ++st, ++i) {
//		cout << inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
//		cout << endl;
//		(*st)->printState(1);
//		cout << endl << endl;
//		delete *st;
//	}
	GetBranchCounters(cktVar, branch_counters);
	int num = printCnt(branch_counters);
	cout << num << " branches reached" << endl;

}

void ReadInputVectors(vecIn_t& inputVec, int numVec) {

    inputVec.clear();

    char fName[100];
    sprintf(fName, "%s", vecFile);

    ifstream filePtr;
    filePtr.open(fName, ios::in);
    if (filePtr == NULL) {
        cerr << "Error opening " << fName << endl;
        exit(-1);
    }

    int numBit;
    filePtr >> numBit;

    assert(numBit == CONST_NUM_INPUT_BITS);

    uint vecInd = 0;
    for (vecInd = 0; vecInd < (uint) numVec ; vecInd++) {
        string str;
        filePtr >> str;

        if(str.compare("END")==0)
            break;

        assert(str.length() == (uint) CONST_NUM_INPUT_BITS);
        inputVec += str;
    }

    cout << "Read " << vecInd << " vectors!" << endl;
    filePtr.close();

}

int printCnt(int_vec& vec) {
	
	int num = 0;
	for (uint v = 0; v < vec.size(); ++v) {
		cout << v << "(" << vec[v] << ") ";
		if (vec[v])
			num++;
	}
	cout << endl;
	return num;
}

