#include "StdAfx.h"
#include "Reader.h"

using namespace std;

Input Read(const string& inputFilename)
{
	Input input;

	ifstream inputFile;
	inputFile.open(inputFilename);
	bool b = inputFile.is_open();
	assert(inputFile.is_open());

	inputFile >> input.rows >> input.columns >> input.dronesCount >> input.turns >> input.payload;
	inputFile >> input.productTypesCount;

	for (uint_t i = 0; i < input.productTypesCount; ++i)
	{
		uint_t productWeight = 0;
		inputFile >> productWeight;
		input.productWeights.emplace_back(productWeight);
	}

	inputFile >> input.warehousesCount;

	for (uint_t i = 0; i < input.warehousesCount; ++i)
	{
		Warehouse warehouse;
		inputFile >> warehouse.location.row;
		inputFile >> warehouse.location.column;

		for (uint_t j = 0; j < input.productTypesCount; ++j)
		{
			uint_t productTypeCount = 0;
			inputFile >> productTypeCount;
			warehouse.productsCounts.emplace_back(productTypeCount);
		}

		input.warehouses.emplace_back(warehouse);
	}

	inputFile >> input.ordersCount;

	for (uint_t i = 0; i < input.ordersCount; ++i)
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

		input.orders.emplace_back(order);
	}

	inputFile.close();

	return input;
}