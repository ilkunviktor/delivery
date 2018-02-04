#pragma once
#include "State.h"

uint_t CalcDistance(const Point& point1, const Point& point2);

void Solve(const Init& input, shared_ptr<Result>& result, shared_ptr<State>& stateLast);

