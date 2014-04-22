#include "incl.h"
#include "verInclude.h"

void ReadInputVectors(vecIn_t&, int);
void PrintVectorSet(const vecIn_t&);

void ReadInputVectors(vecIn_t& inputVec, int numVec) {

	inputVec.clear();

	char fName[100];
	sprintf(fName, "%s.vec", vecFile);

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

void PrintVectorSet(const vecIn_t& inputVec) {

	cout << endl << "# Vectors: " << inputVec.length()/CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < inputVec.length(); i+= CONST_NUM_INPUT_BITS) {
		cout << inputVec.substr(i,CONST_NUM_INPUT_BITS) << endl;
	}

}

void printVec(const vector<int>& vec_) {
	for (
