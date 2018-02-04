#include "StdAfx.h"
#include "Score.h"

uint_big_t ScoreFunc(const uint_big_t& turns, const uint_big_t& deliverTurn)
{
	uint_big_t score = (uint_big_t)ceil(100 * (double)(turns - deliverTurn) / (double)turns);

	return score;
}

uint_big_t Score(const State& stateLast)
{
	uint_big_t result = 0;

	for (const auto& order : stateLast.ordersCurrent)
	{
		if (order.state == OrderState::Delivered)
		{
			result += ScoreFunc(stateLast.turnsCurrent, order.deliverTurn);
		}
	}

	return result;
}