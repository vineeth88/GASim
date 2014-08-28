#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <set>

#include <cstdlib>

using namespace std;

struct cond_t;

int str2int (string);
void graphViz(char[], vector<cond_t>&);

struct cond_t {
	int lineNo;
	int index;
	int coverage;
	int if_else;

	vector<int> parent;
	vector<int> child;

	cond_t(int lineNo_, int index_, int coverage_, int if_else_) :
		lineNo(lineNo_), index(index_),
		coverage(coverage_), if_else(if_else_) {}
	
	string print() {
		stringstream ss;
		ss << index << " " 
		   << coverage << " "
//		   << "(" << lineNo << ") " 
		   << parent.size() << " ";
		for (vector<int>::iterator it = parent.begin();
				it != parent.end(); ++it)
			ss << *it << " ";
		ss << if_else << " "
		   << child.size() << " ";
		for (vector<int>::iterator it = child.begin();
				it != child.end(); ++it)
			ss << *it << " ";
		ss << ";";
		return ss.str();
	}
	
	void addParent(int num_) {
		parent.push_back(num_);
	}

	void addChild(int num_) {
		child.push_back(num_);
	}

};

int main (int argc, char* argv[]) {
	
	char cktName[100] = "Vb01";

	if (argc >= 2)
		sprintf(cktName, "%s", argv[1]);

	char fName[100];
	sprintf(fName, "%s.cond", cktName);

	ifstream inFile;
	inFile.open(fName, ios::in);
	string currLine;
	int lineNo = 0, numLevels = 0;

	vector<int> ifStack;
	int cIndex = 0;
	int condFlag = 0;

	vector<cond_t> condList;
	vector<int> condTree;
	condTree.push_back(0);
	int num_graphs = 0, root_node = 0;

	while (inFile) {
		getline(inFile, currLine);
		lineNo++;
//		cout << currLine << endl;
		if(currLine.find("if") != string::npos) {
			cIndex++;

			condFlag = cIndex;
			condList.push_back(cond_t(lineNo, cIndex, -1, 0));
//			(condList.back()).addParent(ifStack.back());
			
//			if (cIndex >= 1)
				condList[cIndex-1].addParent(condTree.back());

//			cout << condTree.back() << " <- " << cIndex << endl;
			if (condTree.back() != 0)
				condList[condTree.back()-1].addChild(cIndex);
			else	
				num_graphs++;

//			for (int i = 0; i < numLevels; i++)
//				cout << "  ";
//			cout << lineNo << ": if() " << numLevels << endl;

			ifStack.push_back(lineNo);
//			numLevels++;
		}
		else if (currLine.find("else") != string::npos) {
//			numLevels++;
			cIndex++;
//			for (int i = 0; i < numLevels; i++)
//				cout << "  ";
//			cout << lineNo << ": else() " << numLevels 
//				 << " <-> " << ifStack.back() << endl;
			ifStack.pop_back();

			condFlag = cIndex;
			condList.push_back(cond_t(lineNo, cIndex, -1, 1));

//			if (cIndex >= 1)
				condList[cIndex-1].addParent(condTree.back());
//			cout << condTree.back() << " <- " << cIndex << endl;
			if (condTree.back() != 0)
				condList[condTree.back()-1].addChild(cIndex);
		}

		if (currLine.find("{") != string::npos) {
			numLevels++;
			condTree.push_back(cIndex);
		}
		else if (currLine.find("}") != string::npos) {
			numLevels--;
			condTree.pop_back();
		}

		//if (lineNo == condFlag+1) {
		size_t covPos = currLine.find("Vcoverage[");
		if (covPos != string::npos) {
			size_t covEnd = currLine.find("]",covPos+1);
			covEnd -= covPos+10;
			string cov_value = currLine.substr(covPos+10, covEnd);
//			cout << cov_value << endl;

//			if(cIndex >= 1)	
//				condList[cIndex-1].coverage = str2int(cov_value);
			if (cIndex >= 1) {
				if (condTree.back() == cIndex)
					condList[cIndex-1].coverage = str2int(cov_value);
				else if(condTree.size())
					condList[condTree.back()-1].coverage = str2int(cov_value);
			}
		}
//		else 
//			cout << "bIndex = -1" << endl;
	}
	
	ofstream outFile;
	char outName[100];
	sprintf(outName, "%s.graph", cktName);
	outFile.open(outName, ios::out);
	
	if (!outFile) {
		cout << "Unable to open file " << outName << endl;
		exit(0);
	}

	outFile << num_graphs << endl
		 << root_node << endl
		 << condList.size() << endl;
	for (vector<cond_t>::iterator it = condList.begin();
			it != condList.end(); ++it)	{
		outFile << it->print() << endl;
	}

	outFile.close();
	cout << "Wrote into " << outName << endl;
	
	if (argc == 3) {
		graphViz(cktName, condList);
		cout << "Generated graphviz - " << cktName << ".gv" << endl;
	}
	return 0;
}

int str2int (string str) {
	stringstream ss;
	ss << str;
	int num;
	ss >> num;

	return num;
}

void graphViz(char cktName[], vector<cond_t>& graph) {
	
	ofstream outFile;
	char fName[100];

	sprintf(fName,"%s.gv",cktName);
	outFile.open(fName, ios::out);

	if(!outFile) {
		cout << "Unable to write graph into file " << fName << endl;
		return;
	}
	
	outFile << "digraph " << cktName << "{" << endl
			<< "0 [label=\"\"];" << endl;

	for (vector<cond_t>::iterator it = graph.begin();
			it != graph.end(); ++it)	{
			if (it->coverage == -1)
				outFile << it->index << " [label=\"\"];" << endl;
			else
				outFile << it->index 
						<< " [label=\"" << it->coverage 
						<< "\"];" << endl;

		for (vector<int>::iterator xt = it->parent.begin();
				xt != it->parent.end(); ++xt) 
			outFile << *xt << "->" << it->index << endl;

	}

	outFile << "}";
	outFile.close();

	return;
}

