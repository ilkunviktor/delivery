#include "StdAfx.h"
#include "Validation.h"
#include "Utils.h"

using namespace std;

int main()
{
	// Types
	// 1 ≤ number of rows ≤ 10000
	// 1 ≤ number of columns ≤ 10000
	// 1 ≤ D drones ≤ 1000
	// 1 ≤ maximum load of a drone ≤ 10000
	// 1 ≤ P product types ≤ 10000
	// 1 ≤ W warehouses ≤ 10000
	// value of products by type in warehouse => 0 ≤ number of items ≤ 10000
	// 1 ≤ C custom orders ≤ 10000
	// 1 ≤ L ordered products count in 1 order <= 10000
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

	struct Point
	{
		uint_t row = 0;
		uint_t column = 0;
	};

	struct Warehouse
	{
		Point location;
		vector<uint_t> productsCounts; // id => product type, value => number of items
	};

	vector<Warehouse> warehouses;
	uint_t ordersCount;

	struct Order
	{
		Point location;
		uint_t productsCount;
		map<uint_t, uint_t> items; // key => product type, value => count
	};

	vector<Order> orders;

	// input
	std::string dataPath = "../../data/";
	ifstream inputFile;
	inputFile.open(dataPath + "task.in");
	assert(inputFile.is_open());

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
		inputFile >> warehouse.location.row;
		inputFile >> warehouse.location.column;

		for (uint_t j = 0; j < productTypesCount; ++j)
		{
			uint_t productTypeCount = 0;
			inputFile >> productTypeCount;
			warehouse.productsCounts.emplace_back(productTypeCount);
		}

		warehouses.emplace_back(warehouse);
	}

	inputFile >> ordersCount;

	for (uint_t i = 0; i < ordersCount; ++i)
	{
		Order order;
		inputFile >> order.location.row;
		inputFile >> order.location.column;
		inputFile >> order.productsCount;

		for (uint_t j = 0; j < order.productsCount; ++j)
		{
			uint_t productType = 0;
			inputFile >> productType;
			++order.items[productType];
		}

		orders.emplace_back(order);
	}

	inputFile.close();

	// solve


	// output
	string timeCurrentStr = GetTimeStr();
	string resultFileName = dataPath + "result_" + timeCurrentStr + ".out";
	ofstream resultFile;
	resultFile.open(resultFileName);
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

	// validation
	Validate(resultFileName);

	return 0;
}