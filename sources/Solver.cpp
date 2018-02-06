#include "StdAfx.h"
#include "Solver.h"

uint_t CalcDistance(const Point& point1, const Point& point2)
{
	int diffRow = point2.row - point1.row;
	int diffHeight = point2.column - point1.column;
	uint_t dist = (uint_t)ceil(sqrt(diffRow * diffRow + diffHeight * diffHeight));

	return dist;
}

/*
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
}
*/
/////////////////////////

deque<shared_ptr<OrderSub>> CreateOrdersSub(vector<shared_ptr<Warehouse>>& warehouses, const shared_ptr<Order>& order)
{
	// orders sub priority here
	deque<shared_ptr<OrderSub>> result;

	// sort nearest warehouse to order
	multimap<uint_t, shared_ptr<Warehouse>> warehousesNearest; // key => distance

	for (auto&& warehouse : warehouses)
	{
		uint_t distance = CalcDistance(order->location, warehouse->location);
		warehousesNearest.insert({distance, warehouse});
	}

	for (const auto& item : order->items)
	{
		for (uint_t i = 0; i < item.second; ++i)
		{
			shared_ptr<OrderSub> orderSub = make_shared<OrderSub>();
			orderSub->orderId = order->id;
			orderSub->productType = item.first;

			// find product in nearest warehouse and sub from them this product
			bool warehouseSet = false;

			for (auto it = warehousesNearest.begin(); it != warehousesNearest.end(); ++it)
			{
				shared_ptr<Warehouse> warehouseNearest = it->second;

				if (warehouseNearest->productsCounts[orderSub->productType] > 0)
				{
					--warehouseNearest->productsCounts[orderSub->productType];
					orderSub->warehouseId = warehouseNearest->id;
					warehouseSet = true;

					break;
				}
			}

			assert(warehouseSet);

			result.emplace_back(orderSub);
		}
	}

	return result;
}

void Solve2(const Init& input, shared_ptr<Result>& result, shared_ptr<State2>& stateLast)
{
	// init
	result = make_shared<Result>();
	stateLast = make_shared<State2>();

	for (const auto& order : input.orders)
	{
		shared_ptr<Order> orderPtr = make_shared<Order>(order);
		stateLast->orders.emplace_back(orderPtr);
	}

	for (const auto& warehouse : input.warehouses)
	{
		shared_ptr<Warehouse> warehousePtr = make_shared<Warehouse>(warehouse);
		stateLast->warehouses.emplace_back(warehousePtr);
	}

	// create drones
	for (uint_t droneId = 0; droneId < input.dronesCount; ++droneId)
	{
		shared_ptr<Drone2> droneInit = make_shared<Drone2>();
		droneInit->id = droneId;
		droneInit->location = stateLast->warehouses[0]->location;
		stateLast->drones.emplace_back(droneInit);
	}

	// orders priority here
	deque<shared_ptr<Order>> ordersSorted;
	copy(stateLast->orders.begin(), stateLast->orders.end(), back_inserter(ordersSorted));
	sort(ordersSorted.begin(), ordersSorted.end(),
		[&input](const shared_ptr<Order>& order1, const shared_ptr<Order>& order2) -> bool
	{
		return order1->productsWeightTotal < order2->productsWeightTotal;
	});
	
	// create orders sub
	for (auto&& orderSorted : ordersSorted)
	{
		deque<shared_ptr<OrderSub>> ordersProduct = CreateOrdersSub(stateLast->warehouses, orderSorted);
		stateLast->ordersSubPending.insert(
			stateLast->ordersSubPending.end(), ordersProduct.begin(), ordersProduct.end());
	}

	while (stateLast->ordersDelivered < stateLast->orders.size() &&
		stateLast->turnsCurrent < input.turns)
	{
		++stateLast->turnsCurrent;

		for (auto&& drone : stateLast->drones)
		{
			// try to fill drone of orders sub of SAME order. And fill them to max payload of drone
			if (drone->commandsPending.empty() && !stateLast->ordersSubPending.empty())
			{
				deque<shared_ptr<OrderSub>> ordersSubForDrone;
				uint_t weightFree = input.payload;
				uint_t orderId = -1; // max of uint_t

				while (!stateLast->ordersSubPending.empty())
				{
					shared_ptr<OrderSub> orderSub = *stateLast->ordersSubPending.begin();
					uint_t orderWeight = input.productWeights[orderSub->productType];

					if (!ordersSubForDrone.empty())
					{
						shared_ptr<OrderSub> orderSubPrevious = *ordersSubForDrone.begin();
						orderId = orderSubPrevious->orderId;
					}

					if ((!ordersSubForDrone.empty() && orderId == orderSub->orderId) || // same order
						weightFree >= orderWeight)
					{
						// add order sub
						ordersSubForDrone.emplace_back(orderSub);
						weightFree -= orderWeight;
						stateLast->ordersSubPending.pop_front();
					}
					else
					{
						break;
					}
				}

				// create drone pending commands, only load and deliver
				if (!ordersSubForDrone.empty())
				{
					// create load commands
					for (const auto& ordersSub : ordersSubForDrone)
					{
						shared_ptr<Command> command = make_shared<Command>();
						command->droneId = drone->id;
						command->action = Action::Load;
						command->objectId = ordersSub->warehouseId;
						command->productType = ordersSub->productType;
						command->productCount = 1;
						Point warehouseLocation = stateLast->warehouses[command->objectId]->location;
						uint_t moveDistance = drone->location == warehouseLocation ? 0 : CalcDistance(drone->location, warehouseLocation);
						command->turnsCount = moveDistance + 1; // + 1 for loading

						drone->location = warehouseLocation;
						drone->commandsPending.emplace_back(command);
					}

#pragma message ("todo: add analyze load commands for merging product counts!")

					// create delivery commands
					Point orderLocation = stateLast->orders[orderId]->location;

					for (const auto& ordersSub : ordersSubForDrone)
					{
						shared_ptr<Command> command = make_shared<Command>();
						command->droneId = drone->id;
						command->action = Action::Deliver;
						command->objectId = orderId;
						command->productType = ordersSub->productType;
						command->productCount = 1;
						uint_t deliverDistance = drone->location == orderLocation ? 0 : CalcDistance(drone->location, orderLocation);
						command->turnsCount = deliverDistance + 1; // + 1 for deliver

						drone->location = orderLocation;
						drone->commandsPending.emplace_back(command);
					}

#pragma message ("todo: add analyze deliver commands for merging product counts!")

				}
			}

			// drone lifecycle: wait 0..* => load 1..* => deliver 1..* (for 1 same order) => wait 0..* ...

			if (drone->commandsPending.empty())
			{
				++drone->turnsWaited;
			}
			else
			{
				// result wait command
				if (drone->turnsWaited > 0)
				{
					shared_ptr<Command> command = make_shared<Command>();
					command->droneId = drone->id;
					command->action = Action::Wait;
					command->turnsCount = drone->turnsWaited;
					result->commands.push_back(command);

					drone->turnsWaited = 0;
				}

				// result load or delivery command
				shared_ptr<Command> commandPending = *drone->commandsPending.begin();
				++commandPending->turnsCurrent;

				if (commandPending->turnsCurrent == commandPending->turnsCount)
				{
					result->commands.push_back(commandPending);
					drone->commandsPending.pop_front();

					// check deliver order (fill for scoring)
					if (commandPending->action == Action::Deliver)
					{
						shared_ptr<Order> order = stateLast->orders[commandPending->objectId];
						order->productsCount -= commandPending->productCount;

						if (order->productsCount == 0)
						{
							order->state = OrderState::Delivered;
							order->deliverTurn = stateLast->turnsCurrent;
							++stateLast->ordersDelivered;
						}
					}
				}
			}
		}
	}
}