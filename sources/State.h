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
	vector<shared_ptr<Warehouse>> warehouses;
	deque<shared_ptr<OrderSub>> ordersSubPending;
	uint_t ordersProductsPendingId = 0;
	uint_t ordersDelivered = 0;
	uint_t turnsCurrent = 0;
};