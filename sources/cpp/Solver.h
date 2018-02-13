#pragma once
#include "State.h"


uint_t CalcDistance(const Point& point1, const Point& point2);

deque<shared_ptr<OrderSub>> CreateOrdersSub(const shared_ptr<Order>& order);

void Solve2(const Init& input, shared_ptr<Result>& result, shared_ptr<State2>& stateLast);

