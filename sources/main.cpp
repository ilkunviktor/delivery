#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

int main()
{
	// input
	std::string dataPath = "../../data/";
	ifstream inputFile;
	inputFile.open(dataPath + "task.in");
	bool b = inputFile.is_open();
	assert(inputFile.is_open());

	int rows = 0;
	int columns = 0;
	int drones = 0;
	int turns = 0;
	int payload = 0;
	int productTypes = 0;
	vector<int> productWeights;

	struct warehouse
	{
		int row = 0;
		int column = 0;
		vector<int> productsCounts;
	};

	inputFile >> rows >> columns >> drones >> turns >> payload;


	inputFile.close();

	// solve

	vector<warehouse> warehouses;

	//output
	ofstream resultFile;
	resultFile.open(dataPath + "result.out");
	resultFile << "Writing this to a file 1.\n";
	resultFile.close();

	return 0;
}