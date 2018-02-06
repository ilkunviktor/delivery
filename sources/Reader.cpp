#include "StdAfx.h"
#include "Reader.h"
#include "Solver.h"

shared_ptr<Init> Read(const string& inputFilename)
{
	shared_ptr<Init> in(new Init);

	ifstream inputFile;
	inputFile.open(inputFilename);
	assert(inputFile.is_open());

	inputFile >> in->rows >> in->columns >> in->dronesCount >> in->turns >> in->payload;
	inputFile >> in->productTypesCount;

	for (uint_t i = 0; i < in->productTypesCount; ++i)
	{
		uint_t productWeight = 0;
		inputFile >> productWeight;
		in->productWeights.emplace_back(productWeight);
	}

	inputFile >> in->warehousesCount;

	for (uint_t i = 0; i < in->warehousesCount; ++i)
	{
		shared_ptr<Warehouse> warehouse = make_shared<Warehouse>();
		warehouse->id = i;
		inputFile >> warehouse->location.row;
		inputFile >> warehouse->location.column;

		for (uint_t j = 0; j < in->productTypesCount; ++j)
		{
			uint_t productTypeCount = 0;
			inputFile >> productTypeCount;
			warehouse->productsCounts.emplace_back(productTypeCount);
		}

		in->warehouses.emplace_back(warehouse);
	}

	inputFile >> in->ordersCount;

	for (uint_t i = 0; i < in->ordersCount; ++i)
	{
		Order order;
		order.id = i;
		inputFile >> order.location.row;
		inputFile >> order.location.column;
		inputFile >> order.productsCount;

		for (uint_t j = 0; j < order.productsCount; ++j)
		{
			uint_t productType = 0;
			inputFile >> productType;
			++order.items[productType];
		}

		in->orders.emplace_back(order);
	}

	inputFile.close();

	return in;
}