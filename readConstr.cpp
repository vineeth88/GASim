#include "incl.h"
#include "verInclude.h"
#include <cstring>

using namespace std;

enum relOp_t {
	dc = 0,
	eq,	ne,
	lt, gt
};

const int X_VAL = -1;

class brCond : public state_var {
	public:

	vector<int> inputVec;
	vector<relOp_t> relOper;

	brCond() {
		inputVec = vector<int>(CONST_NUM_INPUT_BITS, -1);
		relOper = vector<relOp_t>(CONST_NUM_VARS, dc);
		state_var();
	}

	inline uint& operator[](int index) {
		return state_var::operator[](index);
	}

};

template <typename T>
T str2num (const string &str) {
	stringstream ss (str);
	T result;
	return (ss >> result) ? result : 0;
}

int readConstr(vector<brCond>& vecBrCond, char fileName[100]) {
	
	ifstream consFile;

	consFile.open(fileName, ifstream::in);
	if (!consFile) {
		cout << "Unable to open file " << fileName << endl;
		exit(-1);
	}

	cout << "Reading " << fileName << endl;
	
	string junk;
	getline(consFile, junk);

	int numInputs, numStateVars;
	consFile >> numInputs >> numStateVars;// >> junkC;
	
	assert(numInputs == CONST_NUM_INPUT_BITS);
	assert(numStateVars == CONST_NUM_VARS);
	
	if(vecBrCond.size() != CONST_NUM_BRANCH)
		vecBrCond = vector<brCond>(CONST_NUM_BRANCH);
	
	for (int index = 0; index < CONST_NUM_BRANCH; ++index) {
		string inChar;
		brCond &currBrCnd = vecBrCond[index];
		consFile >> inChar;
		cout << "Branch " << inChar << endl;

		cout << "Inputs: "; 
		for (int i = 0; i < numInputs; i++) {
			consFile >> inChar;
			if (!inChar.compare("X")) {
//			if (!strcmp(inChar,"X")) {
				currBrCnd.inputVec[i] = -1;
				cout << "X";
			}
			else {
				currBrCnd.inputVec[i] = inChar[1] - 48;
				cout << currBrCnd.inputVec[i];
			}

		}
		cout << endl;
	
		cout << "Branch: ";
		for (int j = 0; j < numStateVars; j++) {
			consFile >> inChar;
//			cout << inChar;
			if (inChar.compare("X")) {
				switch (inChar[0]) {
					case '=':	
						cout << " eq ";
						currBrCnd.relOper[j] = eq;
						break;

					case '!':	
						cout << " ne ";
						currBrCnd.relOper[j] = ne;
						break;

					case '<':	
						cout << " lt ";
						currBrCnd.relOper[j] = lt;
						break;

					case '>':	
						cout << " gt ";
						currBrCnd.relOper[j] = gt;
						break;

					default:	
						cout << " dc ";
						currBrCnd.relOper[j] = dc;
						break;
				}
				inChar.erase(inChar.begin());
				uint num = (uint) str2num<int>(inChar);
				currBrCnd[j] = num;
	//			cout << j << " " << inChar << ", ";
			}	
		}
		cout << endl;

		while ((inChar.compare(";")))
			consFile >> inChar;
		
		for (int j = 0; j < numStateVars; j++) {
			if ( currBrCnd.relOper[j]) {
				cout << j << " " << currBrCnd.relOper[j] << " " << currBrCnd[j];
				if (j < numStateVars - 1)
					cout << ",";
			}
		}

		cout << endl;
			
	}

	consFile.close();
	
	return 0;
}


int main(int argc, char** argv) {
	
	char fName[] = "b01.cf";
	vector<brCond> vecBrCond;

	if (argc < 2) {
		cout << "Reading " << fName << endl;
		readConstr(vecBrCond, fName);
	}
	else {
		cout << "Reading " << argv[1] << endl;
		readConstr(vecBrCond, argv[1]);
	}

	return 0;
}
