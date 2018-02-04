#include "StdAfx.h"
#include "Solver.h"
#include "Validator.h"
#include "Reader.h"
#include "Utils.h"
#include "Writer.h"


using namespace std;

int main()
{
	// input
	string dataPath = "../../data/";
	string inputFilename = dataPath + "task.in";
	Input input = Read(inputFilename);
	// solve
	Result result = Solve(input);
	// output
	string timeCurrentStr = GetTimeStr();
	string resultFileName = dataPath + "result_" + timeCurrentStr + ".out";
	Write(result, resultFileName);
	// validation
	Validate(resultFileName);
	// score
	//...

	return 0;
}
