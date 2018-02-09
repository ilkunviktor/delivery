#pragma once
#include "Init.h"
#include "Result.h"

struct Drone
{
	Point location;
	uint_t focusedOnType = 0;
	uint_t endOfCurrentMovement = 0;
	vector<Command> commands;
};

struct State
{
	vector<Order> ordersCurrent;
	uint_t turnsCurrent = 0;
};

/////////////

struct OrderSub // contain one product
{
	uint_t orderId = 0;
	uint_t warehouseId = 0;
	uint_t productType = 0;

	// order sub can contain one product
	// we will analyze later merging of orders sub to load several products from 1 warehouse in 1 turn
	static const uint_t productCount = 1;
};

struct Drone2
{
	uint_t id = 0;
	Point location; // not actual, last location after deliver
	uint_t turnsWaited = 0;

	deque<shared_ptr<Command>> commandsPending;
};

struct State2
{
	vector<shared_ptr<Drone2>> drones;
	deque<shared_ptr<Order>> orders; // by id
	deque<shared_ptr<Warehouse>> warehouses; // by id
	deque<shared_ptr<Order>> ordersPending;
	uint_t ordersDelivered = 0;
	uint_t turnsCurrent = 0;
};