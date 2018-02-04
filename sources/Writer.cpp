#include "StdAfx.h"
#include "Writer.h"

using namespace std;

void Write(const Result& result, const string& resultFilename)
{
	ofstream resultFile;
	resultFile.open(resultFilename);
	assert(resultFile.is_open());
	resultFile << "9" << endl;
	resultFile << "0 L 0 0 1" << endl;
	resultFile << "0 L 0 1 1" << endl;
	resultFile << "0 D 0 0 1" << endl;
	resultFile << "0 L 1 2 1" << endl;
	resultFile << "0 D 0 2 1" << endl;
	resultFile << "1 L 1 2 1" << endl;
	resultFile << "1 D 2 2 1" << endl;
	resultFile << "1 L 0 0 1" << endl;
	resultFile << "1 D 1 0 1" << endl;
	resultFile.close();
}