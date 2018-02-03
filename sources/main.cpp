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

	int rows = 0; // 1 ≤ number of rows ≤ 10000
	int columns = 0; // 1 ≤ number of columns ≤ 10000
	int drones = 0; // 1 ≤ D ≤ 1000
	int turns = 0; // 1 ≤ deadline of the simulation ≤ 1000000
	int payload = 0; // 1 ≤ maximum load of a drone ≤ 10000
	int productTypes = 0; // 1 ≤ P ≤ 10000
	vector<int> productWeights; // 1 ≤ weight ≤ maximum load of a drone 

	struct warehouse
	{
		int row = 0;
		int column = 0;
		vector<int> productsCounts;
	};

	inputFile >> rows >> columns >> drones >> turns >> payload;
	inputFile >> productTypes;

	for (int i = 0; i < productTypes; ++i)
	{
		int productWeight = 0;
		inputFile >> productWeight;
		productWeights.emplace_back(productWeight);
	}

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