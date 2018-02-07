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

deque<shared_ptr<OrderSub>> CreateOrdersSub(const shared_ptr<Order>& order)
{
	// orders sub priority here
	deque<shared_ptr<OrderSub>> result;

	// split orders by 
	// 1. nearest warehouse  
	// 2. needed items
	map<uint_t, uint_t> items = order->items; // copy

	for (auto&& warehouseIt : order->warehousesNearest)
	{
		shared_ptr<Warehouse> warehouse = warehouseIt.second;
	
		for (auto&& item : items)
		{
			while (item.second > 0 && 
				warehouse->productsCounts[item.first] > 0)
			{
				// order sub can contain one product
				// we will analyze later merging of orders sub to load several products from 1 warehouse in 1 turn
				const uint_t productCountSub = 1; 

				warehouse->productsCounts[item.first] -= productCountSub; // change global warehouse !
				item.second -= productCountSub;

				shared_ptr<OrderSub> orderSub = make_shared<OrderSub>();
				orderSub->orderId = order->id;
				orderSub->productType = item.first;
				orderSub->warehouseId = warehouse->id;

				result.emplace_back(orderSub);
			}
		}
	}

	// reverse orders sub from far to nearest warehouse
	reverse(result.begin(), result.end());

	return result;
}

void Solve2(const Init& input, shared_ptr<Result>& result, shared_ptr<State2>& stateLast)
{
	// init
	result = make_shared<Result>();
	stateLast = make_shared<State2>();

	// create drones
	for (uint_t droneId = 0; droneId < input.dronesCount; ++droneId)
	{
		shared_ptr<Drone2> droneInit = make_shared<Drone2>();
		droneInit->id = droneId;
		droneInit->location = input.warehouses[0]->location;
		stateLast->drones.emplace_back(droneInit);
	}

	// copy orders
	for (const auto& order : input.orders)
	{
		shared_ptr<Order> orderPtr = make_shared<Order>(order);
		stateLast->orders.emplace_back(orderPtr);
	}

	// orders preparation, finding:
	for (uint_t orderId = 0; orderId < stateLast->orders.size(); ++orderId)
	{
		// 1. productsWeightTotal
		shared_ptr<Order> order = stateLast->orders[orderId];

		for (const auto& item : order->items)
		{
			order->productsWeightTotal += input.productWeights[item.first] * item.second;
		}

		// 2. nearest warehouses
		for (const auto& warehouse : input.warehouses)
		{
			uint_t distance = CalcDistance(order->location, warehouse->location);
			order->warehousesNearest.insert({distance, warehouse});
		}

		// 3. distanceForDeliverMin
		// find warehouses for flight
		vector<shared_ptr<Warehouse>> warehousesForFlight;
		uint_t itemsCount = order->productsCount;
		map<uint_t, uint_t> items = order->items; // copy

		for (auto&& warehouseIt : order->warehousesNearest)
		{
			shared_ptr<Warehouse> warehouse = warehouseIt.second;

			for (auto&& item : items)
			{
				if (item.second > 0 &&
					warehouse->productsCounts[item.first] > 0)
				{
					if (warehousesForFlight.empty())
					{
						warehousesForFlight.push_back(warehouse);

					}
					else if(warehousesForFlight[warehousesForFlight.size() - 1] != warehouse) // if not added
					{
						warehousesForFlight.push_back(warehouse);
					}

					const uint_t productCountSub = min(item.second, warehouse->productsCounts[item.first]);
					item.second -= productCountSub;
					itemsCount -= productCountSub;
				}
			}

			// check empty items order
			if (itemsCount == 0)
			{
				break;
			}
		}

		assert(!warehousesForFlight.empty());

		// find deliver distance from order location to all nearest warehouse of order. from near to far warehouse
		order->distanceForDeliverMin = 0;
		Point locationLast = order->location;

		for (const auto& warehouse : warehousesForFlight)
		{
			order->distanceForDeliverMin += CalcDistance(locationLast, warehouse->location);
			locationLast = warehouse->location;
		}
	}

	// orders priority here
	deque<shared_ptr<Order>> ordersSorted;
	copy(stateLast->orders.begin(), stateLast->orders.end(), back_inserter(ordersSorted));
	sort(ordersSorted.begin(), ordersSorted.end(),
		[](const shared_ptr<Order>& order1, const shared_ptr<Order>& order2) -> bool
	{
		// close to 0 => order1 => result true
		// close to 1 => order2 => result false
		// sum of factors => 1
		float productWeightFactor = 0.7f;
		float distanceMinFactor = 0.3f;

#pragma warning "add neural network for this factors. also different factors per file"

		float productsWeightCompare = (float)order1->productsWeightTotal /
			(float)(order1->productsWeightTotal + order2->productsWeightTotal);
		float deliverDistanceMinCompare = (float)order1->distanceForDeliverMin / 
			(float)(order1->distanceForDeliverMin + order2->distanceForDeliverMin);
		
		float compareTotal = productWeightFactor * productsWeightCompare + distanceMinFactor * deliverDistanceMinCompare;

		return compareTotal < 0.5f;
	});
	
	// create orders sub with order priority
	for (auto&& orderSorted : ordersSorted)
	{
		deque<shared_ptr<OrderSub>> ordersProduct = CreateOrdersSub(orderSorted);
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
				uint_t weightCurrent = 0;
				uint_t orderId = -1; // max of uint_t

				while (!stateLast->ordersSubPending.empty())
				{
					shared_ptr<OrderSub> orderSub = *stateLast->ordersSubPending.begin();
					uint_t orderWeight = input.productWeights[orderSub->productType];

					if (ordersSubForDrone.empty())
					{
						orderId = orderSub->orderId;
					}

					if (!(!ordersSubForDrone.empty() && orderId != orderSub->orderId) && // if not same order then break
						weightCurrent + orderWeight <= input.payload)
					{
						// add order sub
						ordersSubForDrone.emplace_back(orderSub);
						weightCurrent += orderWeight;
						stateLast->ordersSubPending.pop_front();
					}
					else
					{
						break;
					}
				}

				// create drone pending commands, only load and deliver (for one order)
				if (!ordersSubForDrone.empty())
				{
					// create load commands
					deque<shared_ptr<Command>> commandsLoad;

					for (const auto& ordersSub : ordersSubForDrone)
					{
						shared_ptr<Command> command = make_shared<Command>();
						command->droneId = drone->id;
						command->action = Action::Load;
						command->objectId = ordersSub->warehouseId;
						command->productType = ordersSub->productType;
						command->productCount = 1;
						Point warehouseLocation = input.warehouses[command->objectId]->location;
						uint_t moveDistance = drone->location == warehouseLocation ? 0 : CalcDistance(drone->location, warehouseLocation);
						command->turnsCount = moveDistance + 1; // + 1 for loading

						drone->location = warehouseLocation;
						commandsLoad.emplace_back(command);
					}

					// analyze commands for merging. 
					// for load several products of one type from one warehouse in one turn OR
					// for deliver several products one type for one order in one turn
					auto commandsMerging = [](deque<shared_ptr<Command>>& commands)
					{
						shared_ptr<Command> commandLast;

						for (uint_t i = 0; i < commands.size(); ++i)
						{
							shared_ptr<Command> commandCurrent = commands[i];

							if (commandLast)
							{
								if (commandLast->productType == commandCurrent->productType &&
									commandLast->objectId == commandCurrent->objectId &&
									commandCurrent->turnsCount == 1 // only command with 1 turn
									)
								{
									// merge commands
									commandLast->productCount += commandCurrent->productCount;

									commands.erase(commands.begin() + i);
									--i;
								}
								else
								{
									commandLast = commandCurrent;
								}
							}
							else
							{
								commandLast = commandCurrent;
							}
						}
					};

					// merging
					commandsMerging(commandsLoad);

					drone->commandsPending.insert(drone->commandsPending.end(), 
						commandsLoad.begin(), commandsLoad.end());

					// create delivery commands
					deque<shared_ptr<Command>> commandsDeliver;
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
						commandsDeliver.emplace_back(command);
					}

					// merging
					commandsMerging(commandsDeliver);

					drone->commandsPending.insert(drone->commandsPending.end(),
						commandsDeliver.begin(), commandsDeliver.end());
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