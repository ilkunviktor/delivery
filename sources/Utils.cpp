#include "StdAfx.h"
#include "Utils.h"

using namespace std;

string GetTimeStr()
{
	time_t t = time(nullptr);
	struct tm stm = *localtime(&t);
	stringstream ss;
	ss << put_time(&stm, "%d-%m-%Y_%H-%M-%S");

	return ss.str();
}
