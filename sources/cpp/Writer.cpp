#include "StdAfx.h"
#include "Writer.h"
#include "Result.h"

void Write(const Result& result, const string& resultFilename)
{
	ofstream resultFile;
	resultFile.open(resultFilename);
	assert(resultFile.is_open());
	resultFile << result.commands.size() << endl;

	for (auto&& command : result.commands)
	{
		resultFile << command->Str() << endl;
	}
		
	resultFile.close();
}