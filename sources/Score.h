#pragma once
#include "State.h"

uint_big_t ScoreFunc(const uint_big_t& turns, const uint_big_t& deliverTurn);

uint_big_t Score(const Init& init, const State& stateLast);