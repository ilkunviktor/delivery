#pragma once
#include "State.h"

uint_t ScoreFunc(const uint_t& turns, const uint_t& deliverTurn);

uint_t Score(const shared_ptr<State2>& stateLast, uint_t turnsTotal);