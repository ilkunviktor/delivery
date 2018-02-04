#pragma once

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

struct Drone
{
	Point location;
	int focusedOnType;
	turns_t endOfCurrentMovement;
	vector<string> commands;
};

struct Order
{
	Point location;
	uint_t productsCount;
	map<uint_t, uint_t> items; // key => product type, value => count
};

struct Input
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
	vector<Order> orders;
};