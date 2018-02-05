#include "StdAfx.h"
#include "Solver.h"

uint_t CalcDistance(const Point& point1, const Point& point2)
{
	int diffRow = point1.row - point2.row;
	int diffHeight = point2.column + point2.column;
	uint_t dist = (uint_t)ceil(sqrt(diffRow) + sqrt(diffHeight));

	return dist;
}

void Solve(const Init& input, shared_ptr<Result>& result, shared_ptr<State>& stateLast)
{
	result = make_shared<Result>();
	stateLast = make_shared<State>();

	stateLast->ordersCurrent = input.orders;

	Drone droneInit;
	droneInit.location = input.warehouses[0].location;
	vector<Drone> drones(input.dronesCount, droneInit);

	while (stateLast->turnsCurrent < input.turns)
	{
		for (uint_t i = 0; i < input.dronesCount; ++i)
		{
			if (drones[i].endOfCurrentMovement == stateLast->turnsCurrent)
			{
				uint_t productCountByType = 0;

				while (productCountByType == 0)
				{
					for (uint_t j = 0; j < input.ordersCount; j++)
					{
						productCountByType += stateLast->ordersCurrent[j].items[drones[i].focusedOnType];
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
							drones[i].endOfCurrentMovement += CalcDistance(drones[i].location, input.warehouses[w].location) + 1;
							drones[i].location.row = input.warehouses[w].location.row;
							drones[i].location.column = input.warehouses[w].location.column;

							break;
						}
					}

					// find nearest orders to deliver
					for (uint_t j = 0; j < input.ordersCount; ++j)
					{
						if (stateLast->ordersCurrent[j].items[drones[i].focusedOnType] > 0)
						{
							Order orderCurrent = stateLast->ordersCurrent[j];
							uint_t focusedType = drones[i].focusedOnType;
							uint_t itemWeight = orderCurrent.items[focusedType];
							uint_t deliveredItems = min(itemWeight, droneLoadedItems);
							droneLoadedItems -= deliveredItems;
							stateLast->ordersCurrent[j].items[drones[i].focusedOnType] = stateLast->ordersCurrent[j].items[drones[i].focusedOnType] - deliveredItems;

							// check order complete
							bool orderComplete = true;

							for (const auto& item : stateLast->ordersCurrent[j].items)
							{
								if (item.second)
								{
									orderComplete = false;

									break;
								}
							}

							if (orderComplete)
							{
								stateLast->ordersCurrent[j].state = OrderState::Delivered;
								stateLast->ordersCurrent[j].deliverTurn = stateLast->turnsCurrent;
							}

							// add deliver command
							Command command;
							command.droneId = i;
							command.action = Action::Deliver;
							command.objectId = j;
							command.productType = drones[i].focusedOnType;
							command.productCount = deliveredItems;
							drones[i].commands.emplace_back(command);
							drones[i].endOfCurrentMovement += CalcDistance(drones[i].location, stateLast->ordersCurrent[j].location) + 1;

							if (droneLoadedItems == 0)
							{
								break;
							}
						}
					}
				}
			}			
		}

		stateLast->turnsCurrent++;
	}

	for (auto&& drone : drones)
	{
		result->commands.insert(result->commands.begin(), drone.commands.begin(), drone.commands.end());
	}

	result->commandsCount = (uint_t)result->commands.size();
}

/////////////////////////

deque<OrderProduct> CreateOrdersProducts(const Order& order)
{
	// order products priority 
	deque<OrderProduct> result;

	for (const auto& item : order.items)
	{
		for (uint_t i = 0; i < item.second; ++i)
		{
			OrderProduct orderProduct;
			orderProduct.orderId = order.id;
			orderProduct.productType = item.first;

			assert(false); // add nearest warehouse and sub from them this item

			result.emplace_back(orderProduct);
		}
	}

	return result;
}

void Solve2(const Init& input, shared_ptr<Result>& result, shared_ptr<State2>& stateLast)
{
	// init
	result = make_shared<Result>();
	stateLast = make_shared<State2>();

	stateLast->orders.insert(stateLast->orders.end(),
		input.orders.begin(), input.orders.end());
	stateLast->warehouses.insert(stateLast->warehouses.end(),
		input.warehouses.begin(), input.warehouses.end());
	
	Drone2 droneInit;
	droneInit.location = stateLast->warehouses[0].location;
	stateLast->drones.resize(input.dronesCount, droneInit);

	// orders priority
	sort(stateLast->orders.begin(), stateLast->orders.end(),
		[&input](const Order& order1, const Order& order2) -> bool
	{
		return order1.productsWeightTotal < order2.productsWeightTotal;
	});

	for (uint_t i = 0; i < stateLast->orders.size(); ++i)
	{
		deque<OrderProduct> ordersProduct = CreateOrdersProducts(stateLast->orders[i]);
		stateLast->ordersProductsPending.insert(
			stateLast->ordersProductsPending.end(), ordersProduct.begin(), ordersProduct.end());
	}

	while (stateLast->ordersDelivered < stateLast->orders.size() ||
		stateLast->turnsCurrent < input.turns)
	{
		for (auto&& drone : stateLast->drones)
		{
			if (!stateLast->ordersProductsPending.empty())
			{
				if (drone.state != DroneState::Active)
				{

				}
			}

			++drone.actionCurrentTurns;
		}

		++stateLast->turnsCurrent;
	}
}