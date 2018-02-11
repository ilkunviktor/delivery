#include "StdAfx.h"
#include "Reader.h"
#include "Solver.h"
#include "Writer.h"
#include "Utils.h"
#include "Score.h"
#include "Neural.h"

int main()
{
	g_neuralFactors.itemsRation = 0.5f;

	string taskPath = "../../data/task";
	string resultPath = "../../data/result";
	uint_t scoreTotal = 0;
	int64_t millisecondsTotal = 0;

	// iterate over tasks
	for (const auto& taskPath : experimental::filesystem::v1::directory_iterator(taskPath))
	{
		// timing
		chrono::time_point<std::chrono::system_clock> timeStart =
			chrono::system_clock::now();
		// input
		stringstream ss;
		ss << taskPath;
		string inputFilename = ss.str();

		size_t taskStrStart = inputFilename.find_last_of('\\');
		size_t taskStrEnd = inputFilename.find_last_of('.');
		string taskStr = inputFilename.substr(taskStrStart + 1, taskStrEnd - taskStrStart - 1);

		shared_ptr<Init> init = Read(inputFilename);
		// solve
		shared_ptr<Result> result;
		//shared_ptr<State> stateLast;
		//Solve(*init, result, stateLast);
		shared_ptr<State2> stateLast;
		Solve2(*init, result, stateLast);
		// output
		//string timeCurrentStr = CalcTimeStr();
		//string resultFileName = resultPath + "/" + taskStr + "_" + timeCurrentStr + ".out";
		string resultFileName = resultPath + "/" + taskStr + ".out";
		Write(*result, resultFileName);
		// score
		uint_t score = Score(stateLast, init->turns);
		scoreTotal += score;
		// timing
		chrono::time_point<std::chrono::system_clock> timeEnd =
			chrono::system_clock::now();
		chrono::milliseconds millisec = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);
		millisecondsTotal += millisec.count();
		cout << "file: " << inputFilename << endl;
		cout << "score: " << score << endl;
		cout << "time(ms): " << to_string(millisec.count()) << endl << endl;
	}

	cout << "scoreTotal: " << scoreTotal << endl;
	cout << "millisecondsTotal: " << millisecondsTotal << endl << endl;
	system("pause");

	return 0;
}
