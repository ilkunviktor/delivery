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

struct OrderProduct
{
	uint_t orderId = 0;
	uint_t warehouseId = 0;
	uint_t productType = 0;
};

enum class DroneState
{
	Pending,
	Active
};

struct Drone2
{
	Point location;
	Action action = Action::Wait;
	DroneState state = DroneState::Pending;
	deque<OrderProduct> ordersProducts;
	uint_t targetActionTurns = 0;
	uint_t actionCurrentTurns = 0;

	vector<Command> commands;
};

struct State2
{
	vector<Drone2> drones;
	deque<Order> orders;
	vector<Warehouse> warehouses;
	deque<OrderProduct> ordersProductsPending;
	uint_t ordersProductsPendingId = 0;
	uint_t ordersDelivered = 0;
	uint_t turnsCurrent = 0;
};