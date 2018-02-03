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

	// 1 ≤ number of rows ≤ 10000
	// 1 ≤ number of columns ≤ 10000
	// 1 ≤ D drones ≤ 1000
	// 1 ≤ maximum load of a drone ≤ 10000
	// 1 ≤ P product types ≤ 10000
	// 1 ≤ W warehouses ≤ 10000
	// value of products by type in warehouse => 0 ≤ number of items ≤ 10000
	using uint_t = uint16_t; 
	// 1 ≤ deadline of the simulation ≤ 1000000
	using turns_t = uint32_t;

	uint_t rows = 0;
	uint_t columns = 0;
	uint_t drones = 0; 
	turns_t turns = 0;
	uint_t payload = 0;
	uint_t productTypesCount = 0;
	vector<uint_t> productWeights; // 1 ≤ weight ≤ maximum load of a drone
	uint_t warehousesCount = 0;

	struct Warehouse
	{
		uint_t row = 0;
		uint_t column = 0;
		vector<uint_t> productsCounts; // id => type, value => number of items
	};

	vector<Warehouse> warehouses;

	inputFile >> rows >> columns >> drones >> turns >> payload;
	inputFile >> productTypesCount;

	for (uint_t i = 0; i < productTypesCount; ++i)
	{
		uint_t productWeight = 0;
		inputFile >> productWeight;
		productWeights.emplace_back(productWeight);
	}

	inputFile >> warehousesCount;

	for (uint_t i = 0; i < warehousesCount; ++i)
	{
		Warehouse warehouse;
		inputFile >> warehouse.row >> warehouse.column;

		for (uint_t j = 0; j < productTypesCount; ++j)
		{
			uint_t productTypeCount = 0;
			inputFile >> productTypeCount;
			warehouse.productsCounts.emplace_back(productTypeCount);
		}

		warehouses.emplace_back(warehouse);
	}

	inputFile.close();

	// solve


	//output
	ofstream resultFile;
	resultFile.open(dataPath + "result.out");
	resultFile << "Writing this to a file 1.\n";
	resultFile.close();

	return 0;
}