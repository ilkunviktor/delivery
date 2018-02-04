#include "StdAfx.h"
#include "Reader.h"
#include "Solver.h"
#include "Writer.h"
#include "Utils.h"
#include "Score.h"
int main()
{
	// input
	string dataPath = "../../data/";
	string inputFilename = dataPath + "task.in";
	shared_ptr<Init> init = Read(inputFilename);
	// solve
	shared_ptr<Result> result;
	shared_ptr<State> stateLast;
	Solve(*init, result, stateLast);
	// output
	string timeCurrentStr = CalcTimeStr();
	string resultFileName = dataPath + "result_" + timeCurrentStr + ".out";
	Write(*result, resultFileName);
	// score
	uint_t score = Score(*init, *stateLast);
	cout << "score: " << score << endl;
	system("pause");

	return 0;
}
