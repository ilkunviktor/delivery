#include "StdAfx.h"
#include "Solver.h"
#include "Reader.h"
#include "Utils.h"
#include "Writer.h"

int main()
{
	// input
	string dataPath = "../../data/";
	string inputFilename = dataPath + "task.in";
	Init input = Read(inputFilename);
	// solve
	Result result = Solve(input);
	// output
	string timeCurrentStr = GetTimeStr();
	string resultFileName = dataPath + "result_" + timeCurrentStr + ".out";
	Write(result, resultFileName);
	// score
	//...

	return 0;
}
