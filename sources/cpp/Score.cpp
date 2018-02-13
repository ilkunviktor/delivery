#include "StdAfx.h"
#include "Score.h"

uint_t ScoreFunc(const uint_t& simulationTurns, const uint_t& deliverTurn)
{
	uint_t score = (uint_t)ceil(100 * (double)(simulationTurns - deliverTurn) / (double)simulationTurns);

	return score;
}

uint_t Score(const shared_ptr<State2>& stateLast, uint_t turnsTotal)
{
	uint_t result = 0;

	for (const auto& order : stateLast->orders)
	{
		if (order->state == OrderState::Delivered)
		{
			result += ScoreFunc(turnsTotal, order->deliverTurn);
		}
	}

	return result;
}