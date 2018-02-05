#pragma once
#include "State.h"

uint_t ScoreFunc(const uint_t& turns, const uint_t& deliverTurn);

uint_t Score(const State& stateLast);

uint_t Score(const State2& stateLast);