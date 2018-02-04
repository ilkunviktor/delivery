#pragma once
#include "Init.h"

struct Command
{
	uint_t droneId = 0;
	Action action = Action::Load;
	uint_t objectId = 0;
	uint_t productType = 0;
	uint_t productCount = 0;
	uint_t turnsCount = 0;

	string Str() const;
};

struct Result
{
	uint_t commandsCount = 0;
	vector<Command> commands;
};