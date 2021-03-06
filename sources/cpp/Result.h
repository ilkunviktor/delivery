﻿#pragma once
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

	uint_t turnsCurrent = 0;
};

struct Result
{
	vector<shared_ptr<Command>> commands;
};