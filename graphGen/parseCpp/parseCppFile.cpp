#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <set>

#include <cstdlib>
using namespace std;

int main (int argc, char* argv[]) {
	
	char cktName[100] = "Vb01";

	if (argc >= 2)
		sprintf(cktName, "%s", argv[1]);
	else {
		cerr << "No input file provided" << endl
			 << "Input format: " << endl
			 << argv[0] << " <ckt>" << endl;
		exit(-1);
	}
			
	
	char fName[100];
	sprintf(fName, "%s.cpp", cktName);

	ifstream inFile;
	inFile.open(fName, ios::in);
	if(!inFile) {
		cout << "Unable to open file: "<< fName << endl;
		exit(0);
	}
	string currLine;
	int lineNo = 0, numLevels = 0, orgLineNo = 0;

	char outName[100];
	sprintf(outName, "%s_f.cpp", argv[1]);

	ofstream outFile;
	outFile.open(outName, ios::out);
	if(!outFile) {
		cout << "Unable to open file: "<< outName << endl;
		exit(0);
	}
	
	vector<int> ifStack;
	while (inFile) {
		string tmpLine;
		getline(inFile, tmpLine);
		orgLineNo++;

		if (tmpLine.find("#") != string::npos) {
			outFile << tmpLine << endl;
			cout << orgLineNo << ": #define" << endl; 
			continue;
		}
		else if (tmpLine.find("//") != string::npos) { 
		/*	Special case when it can be commented after a statement	*/
			tmpLine.clear();
			cout << orgLineNo << ": //Comment" << endl;
			outFile << endl;
			continue;
		}
		else if (tmpLine.find("/*") != string::npos) {
			cout << orgLineNo ;
			while (tmpLine.find("*/") == string::npos) {
				getline(inFile, tmpLine);
				orgLineNo++;
			}
			cout << " - " << orgLineNo << "/*Comment*/" << endl;
			continue;
		}
		else if ((tmpLine.find("if") != string::npos) 
			|| (tmpLine.find("else") != string::npos)
			|| (tmpLine.find("for") != string::npos)) {
		/*	Verilator does not have while loops. So don't have to include it */
			if (currLine.length()) {
				outFile << currLine << endl;
				currLine.clear();
			}
//			cout << endl << orgLineNo;
			if (tmpLine.find("{") != string::npos) {
				outFile << tmpLine << endl;
			}
			else {
				currLine += tmpLine;
				while ((tmpLine.find("{") == string::npos)
					  && (tmpLine.find(";") == string::npos)) {
					getline(inFile, tmpLine);
					orgLineNo++;

					stringstream tmpStream;
					string tmpStr;
					tmpStream << tmpLine;
					while (tmpStream) {
						tmpStream >> tmpStr;
			//			cout << tmpStr << ":";
			//			currLine += " ";
						currLine += tmpStr;
						tmpStr.clear();
					}
				}
//				cout << " - " << orgLineNo << " if/else" << endl;
				cout << orgLineNo << ": " << currLine << endl; 
				outFile << currLine << endl;
				currLine.clear();
			}
		}
		else {
		
			/* Beginning of Functions */ 
			if (tmpLine.find("{") != string::npos) {
				outFile << currLine << " "; 
				currLine.clear();
				int ind = tmpLine.find("{") + 1;
				outFile << tmpLine.substr(0,ind) << endl;
				tmpLine = tmpLine.substr(ind, string::npos);
				//continue;
			}
			
			if (tmpLine.find("}") != string::npos) {
				outFile << currLine << " ";
				currLine.clear();
				int ind = tmpLine.find("}");
				if (ind == 0) {
					outFile << "}" << endl;
					continue;
				}
				outFile << tmpLine.substr(0,ind-1) << endl << "	}" << endl;
				tmpLine = tmpLine.substr(ind+1, string::npos);
				//continue;
			}
			
			if (tmpLine.find(";") != string::npos) {

				if (currLine.length() == 0)
					currLine += tmpLine;
				else {
					stringstream tmpStream;
					string tmpStr;
					tmpStream << tmpLine;
					while (tmpStream) {
						tmpStream >> tmpStr;
			//			cout << tmpStr << ":";
			//			currLine += " ";
						currLine += tmpStr;
						tmpStr.clear();
					}
				}
				outFile << currLine << endl;
				currLine.clear();
			}
			else if (currLine.length() == 0)
				currLine += tmpLine;
			else {
				stringstream tmpStream;
				string tmpStr;
				tmpStream << tmpLine;
				while (tmpStream) {
					tmpStream >> tmpStr;
//					cout << tmpStr << ":";
			//		currLine += " ";
					currLine += tmpStr;
					tmpStr.clear();
				}
			}
		}
	}
	outFile << currLine << endl;
	inFile.close();
	outFile.close();
	cout << "Wrote into " << outName << endl
		 << "Reparsing for {} " << endl;

	sprintf(fName, "%s_f.cpp", cktName);
	inFile.open(fName, ios::in);
	if(!inFile) {
		cout << "Unable to open file: "<< fName << endl;
		exit(0);
	}
	
	sprintf(outName, "%s_p.cpp", cktName);
	outFile.open(outName, ios::out);
	if(!inFile) {
		cout << "Unable to open file: "<< outName << endl;
		exit(0);
	}

	lineNo = 0;
	while (inFile) {
		string tmpLine;
		getline(inFile, tmpLine);
		lineNo++;
		if (tmpLine.find("}") == string::npos) 
			outFile << tmpLine << endl;
		else {
//			cout << "Line: " << lineNo << endl 
//				 << tmpLine << endl
//				 << "-----------" << endl;
			size_t pos = tmpLine.find_first_of("}");
			size_t prev_pos = 0;
//			cout << "pos = " << pos << endl;
			while (pos != string::npos) {
				string pre = tmpLine.substr(prev_pos, pos+1-prev_pos);
//				string post = tmpLine.substr(pos+1);
//				cout << pre << ":" << endl;
//				cout << post << endl;
				
				outFile << pre << endl; 
				tmpLine[pos] = '*';
				prev_pos = pos + 1;
				pos = tmpLine.find_first_of("}", prev_pos);
				
//				cout << "pos = " << pos << endl;
			}
			string post = tmpLine.substr(prev_pos);
//			cout << post << endl;
			outFile << post << endl;
		}
	}

	inFile.close();
	outFile.close();

	char command[100];
	sprintf(command, "rm %s", fName);
	system(command);
	cout << "Deleted " << fName << endl;
	cout << "Wrote into " << outName << endl;


	return 0;
}
