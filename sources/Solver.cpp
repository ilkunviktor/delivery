#include "StdAfx.h"
#include "Solver.h"

uint_t distance(Point point1, Point point2)
{
	int diffRow = point1.row - point2.row;
	int diffHeight = point2.column + point2.column;
	uint_t dist = (uint_t)ceil(sqrt(diffRow) + sqrt(diffHeight));

	return dist;
}

Result Solve(Init input)
{
	
	Result result;

	Drone droneInit;
	droneInit.location = input.warehouses[0].location;
	vector<Drone> drones(input.dronesCount, droneInit);

	turns_t turnCurrent = 0;

	while (turnCurrent < input.turns)
	{
		for (uint_t i = 0; i < input.dronesCount; ++i)
		{
			if (drones[i].endOfCurrentMovement == turnCurrent)
			{
				uint_t productCountByType = 0;

				while (productCountByType == 0)
				{
					for (uint_t j = 0; j < input.ordersCount; j++)
					{
						productCountByType += input.orders[j].items[drones[i].focusedOnType];
					}

					if (productCountByType == 0)
					{
						drones[i].focusedOnType++;

						if (drones[i].focusedOnType == input.productTypesCount)
						{
							drones[i].endOfCurrentMovement = input.turns;

							break;
						}
					}
				}

				if (productCountByType > 0)
				{
					uint_t maxTypeCount = input.payload / input.productWeights[drones[i].focusedOnType];
					uint_t droneLoadedItems = min(productCountByType, maxTypeCount);

					// find warehouse to load type
					for (uint_t w = 0; w < input.warehousesCount; w++)
					{
						if (input.warehouses[w].productsCounts[drones[i].focusedOnType] >= 0)
						{
							droneLoadedItems = min(input.warehouses[w].productsCounts[drones[i].focusedOnType], droneLoadedItems);
							// add load command
							Command command;
							command.droneId = i;
							command.action = Action::Load;
							command.objectId = w;
							command.productType = drones[i].focusedOnType;
							command.productCount = droneLoadedItems;
							drones[i].commands.emplace_back(command);
							drones[i].endOfCurrentMovement += distance(&drones[i].location,	&input.warehouses[w].location) + 1;
							drones[i].location.row = input.warehouses[w].location.row;
							drones[i].location.column = input.warehouses[w].location.column;

							break;
						}
					}

					// find nearest orders to deliver
					vector<uint_t> ordersToDeliver;

					for (uint_t j = 0; j < input.ordersCount; ++j)
					{
						if (input.orders[j].items[drones[i].focusedOnType] > 0)
						{
							uint_t deliveredItems = min(
								input.orders[j].items[drones[i].focusedOnType], droneLoadedItems);
							droneLoadedItems -= deliveredItems;
							input.orders[j].items[drones[i].focusedOnType] = input.orders[j].items[drones[i].focusedOnType] - deliveredItems;
							// add deliver command
							Command command;
							command.droneId = i;
							command.action = Action::Deliver;
							command.objectId = j;
							command.productType = drones[i].focusedOnType;
							command.productCount = deliveredItems;
							drones[i].commands.emplace_back(command);
							drones[i].endOfCurrentMovement += distance(drones[i].location, input.orders[j].location) + 1;

							if (droneLoadedItems == 0)
							{
								break;
							}
						}
					}
				}
			}			
		}

		turnCurrent++;
	}

	for (auto&& drone : drones)
	{
		result.commands.insert(result.commands.begin(), drone.commands.begin(), drone.commands.end());
	}

	result.commandsCount = (uint_t)result.commands.size();

	return result;
}