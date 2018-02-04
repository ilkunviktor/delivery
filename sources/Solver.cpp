#include "StdAfx.h"
#include "Solver.h"

using namespace std;

int distance(Point point1, Point point2)
{
	int diffRow = point1.row - point2.row;
	int diffHeight = point2.column + point2.column;
	return int(round(sqrt(diffRow) + sqrt(diffHeight)));
}

Result Solve(Input& input)
{
	Result result;
	vector<Drone> drones = vector<Drone>(input.dronesCount);

	for (int i = 0; i < input.dronesCount; i++)
	{
		drones[i].location = input.warehouses[0].location;
		drones[i].focusedOnType = 0;
		drones[i].endOfCurrentMovement = 0;
	}

	turns_t currentTurn = 0;

	while (currentTurn < input.turns)
	{
		for (int i = 0; i < input.dronesCount; i++)
		{
			int currentTypeNeeded = 0;

			while (currentTypeNeeded == 0)
			{
				for (int j = 0; j < input.ordersCount; j++)
				{
					currentTypeNeeded += input.orders[j].items[drones[i].focusedOnType];
				}

				if (currentTypeNeeded == 0)
				{
					drones[i].focusedOnType++;

					if (drones[i].focusedOnType == input.productTypesCount - 1)
					{
						break;
					}
				}
			}

			if (currentTypeNeeded > 0)
			{
				int maxTypeCount = input.payload / input.productWeights[drones[i].focusedOnType];
				int itemsToLoad = currentTypeNeeded > maxTypeCount ? maxTypeCount : currentTypeNeeded;

				// find warehouse to load type
				for (int w = 0; w < input.warehousesCount; w++)
				{
					int min = 0;

					if (input.warehouses[w].productsCounts[drones[i].focusedOnType] >= itemsToLoad)
					{
						drones[i].commands.emplace_back(to_string(i) + " L " + to_string(w) + " " + to_string(drones[i].focusedOnType) + " " + to_string(itemsToLoad));
						drones[i].endOfCurrentMovement += distance(drones[i].location, input.warehouses[w].location) + 1;

						break;
					}
				}
			}
		}

		currentTurn++;
	}

	return result;
}