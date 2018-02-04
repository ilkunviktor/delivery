#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>

using namespace std;

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

struct drone
{
	Point location;
	int focusedOnType;
	int endOfCurrentMovement;
	vector<std::string> commands;
};


int distance(Point point1, Point point2)
{
	int diffRow = point1.row - point2.row;
	int diffHeight = point2.column + point2.column;
	return int(round(sqrt(diffRow) + sqrt(diffHeight)));
}


int main()
{
	uint_t rows = 0;
	uint_t columns = 0;
	uint_t dronesCount = 0; 
	turns_t turns = 0;
	uint_t payload = 0;
	uint_t productTypesCount = 0;
	vector<uint_t> productWeights; // 1 ≤ weight ≤ maximum load of a drone
	uint_t warehousesCount = 0;


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
	bool b = inputFile.is_open();
	assert(inputFile.is_open());

	inputFile >> rows >> columns >> dronesCount >> turns >> payload;
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
	

	vector<drone> drones = vector<drone>(dronesCount);
	for (int i=0; i<dronesCount; i++)
	{
		drones[i].location = warehouses[0].location;
		drones[i].focusedOnType = 0;
		drones[i].endOfCurrentMovement = 0;
	}

	turns_t currentTurn = 0;
	while (currentTurn < turns)
	{
		for (int i = 0; i < dronesCount; i++)
		{
			int currentTypeNeeded = 0;
			while (currentTypeNeeded == 0)
			{
				for (int j = 0; j < ordersCount; j++)
				{
					currentTypeNeeded += orders[j].items[drones[i].focusedOnType];
				}

				if (currentTypeNeeded == 0)
				{
					drones[i].focusedOnType++;
					if (drones[i].focusedOnType == productTypesCount - 1)
					{
						break;
					}
				}
			}

			if (currentTypeNeeded > 0)
			{				
				int maxTypeCount = payload / productWeights[drones[i].focusedOnType];				
				int itemsToLoad = currentTypeNeeded > maxTypeCount ? maxTypeCount : currentTypeNeeded;

				// find warehouse to load type
				for (int w = 0; w < warehousesCount; w ++)
				{
					int min = 0;
					if (warehouses[w].productsCounts[drones[i].focusedOnType] >= itemsToLoad)
					{
						drones[i].commands.emplace_back(to_string(i) + " L " + to_string(w) + " " + to_string(drones[i].focusedOnType) + " " + to_string(itemsToLoad));
						drones[i].endOfCurrentMovement += distance(drones[i].location, warehouses[w].location) + 1;						
						break;
					}
				}


			}
		}

		currentTurn++;
	}											

	//output
	ofstream resultFile;
	resultFile.open(dataPath + "result.out");
	resultFile << "Writing this to a file 1.\n";
	resultFile.close();

	return 0;
}
