#include "StdAfx.h"
#include "Utils.h"

string GetTimeStr()
{
	time_t t = time(nullptr);
	tm* stm = localtime(&t);
	stringstream ss;
	ss << put_time(stm, "%d-%m-%Y_%H-%M-%S");

	return ss.str();
}
