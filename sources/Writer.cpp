#include "StdAfx.h"
#include "Writer.h"

using namespace std;

void Write(const Result& result, const string& resultFilename)
{
	ofstream resultFile;
	resultFile.open(resultFilename);
	assert(resultFile.is_open());
	resultFile << result.commandsCount << endl;
	for (auto &command : result.commands)
	{
		resultFile << command << endl;
	}
		
	resultFile.close();
}