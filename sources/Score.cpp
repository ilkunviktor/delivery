#include "StdAfx.h"
#include "Score.h"

uint_t ScoreFunc(const uint_t& simulationTurns, const uint_t& deliverTurn)
{
	uint_t score = (uint_t)ceil(100 * (double)(simulationTurns - deliverTurn) / (double)simulationTurns);

	return score;
}

uint_t Score(const State& stateLast)
{
	uint_t result = 0;

	for (const auto& order : stateLast.ordersCurrent)
	{
		if (order.state == OrderState::Delivered)
		{
			result += ScoreFunc(stateLast.turnsCurrent, order.deliverTurn);
		}
	}

	return result;
}

uint_t Score(const State2& stateLast)
{
	uint_t result = 0;

	for (uint_t i = 0; i < stateLast.ordersDelivered; ++i)
	{
		result += ScoreFunc(stateLast.turnsCurrent, stateLast.orders[i].deliverTurn);
	}

	return result;
}