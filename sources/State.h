#pragma once
#include "Init.h"
#include "Result.h"

struct Drone
{
	Point location;
	uint_t focusedOnType = 0;
	uint_big_t endOfCurrentMovement = 0;
	vector<Command> commands;
};

struct State
{
	vector<Order> ordersCurrent;
};