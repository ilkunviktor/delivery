#pragma once
#include "Init.h"
#include "Result.h"

struct Drone
{
	Point location;
	uint_t focusedOnType = 0;
	turns_t endOfCurrentMovement = 0;
	vector<Command> commands;
};
