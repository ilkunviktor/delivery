#include "StdAfx.h"
#include "Result.h"

string Command::Str() const
{
	string actionStr;

	switch (action)
	{
		case Action::Load:
			actionStr = "L";
			break;
		case Action::Deliver:
			actionStr = "D";
			break;
		case Action::Unload:
			actionStr = "U";
			break;
		case Action::Wait:
			actionStr = "W";
			break;
	}

	string result = to_string(droneId) + " " + actionStr + " ";
	
	if (action == Action::Wait)
	{
		result += to_string(turnsCount);
	}
	else
	{
		result += to_string(objectId) + " " + to_string(productType) + " " + to_string(productCount);
	}

	return result;
}