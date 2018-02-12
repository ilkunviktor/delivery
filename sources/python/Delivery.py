import os, sys
from collections import Counter, OrderedDict
from recordclass import recordclass
from math import *

taskPath = '../../data/task/'
resultPath = '../../data/result'
Point = recordclass('Point', 'x y')
Warehouse = recordclass('Warehouse', 'id point productsCounts')
Order = recordclass('Order', 'id point productsCountTotal items state deliverTurn productsWeightTotal ' +
	'warehousesNearest deliverTurnsMin productsCountPending productsCountDelivered')
Command = recordclass('Command', 'droneId action objectId productType productCount turnsCount turnsCurrent')
Drone = recordclass('Drone', 'id point turnsWaited commandsPending')

def Distance(p1, p2):
	diffX = p2[0] - p1[0]
	diffY = p2[1] - p1[1]
	return ceil(sqrt(diffX * diffX + diffY * diffY))

def Equal(p1, p2):
	return p1[0] == p2[0] and p1[1] == p2[1];

for fn in os.listdir(taskPath):
	# read
	f = open(taskPath + fn)
	rows, columns, dronesCount, turns, payload = map(int, f.readline().split())
	productTypeCount = int(f.readline())
	productWeights = list(map(int, f.readline().split()))
	warehousesCount = int(f.readline())
	warehouses = []

	for i in range(warehousesCount):
		point = list(map(int, f.readline().split()))
		productsCounts = list(map(int, f.readline().split()))
		warehouses.append(Warehouse(i, point, productsCounts))

	ordersCount = int(f.readline())
	orders = []

	for i in range(ordersCount):
		point = list(map(int, f.readline().split()))
		productsCountTotal = int(f.readline())
		items = dict(Counter(map(int, f.readline().split())))
		orders.append(Order(i, point, productsCountTotal, items, 'pending',
			0, 0, OrderedDict(), sys.maxsize, 0, 0))

	# solve
	drones = [Drone(i, warehouses[0].point, 0, []) for i in range(dronesCount)]

	for order in orders:
		order.productsWeightTotal = sum([(productWeights[type] * count) for type, count in order.items.items()])
		order.warehousesNearest = OrderedDict([(Distance(w.point, order.point), w) for w in warehouses])


	ordersDelivered = 0
	turnsCurrent = 0
	ordersPending = orders

	while ordersDelivered < len(orders) and	turnsCurrent < turns:
		turnsCurrent += 1

		for d in drones:
			if not d.commandsPending and ordersPending:
				for o in ordersPending:
					itemsCount = o.productsCountTotal - o.productsCountPending - o.productsCountDelivered
					items = dict(o.items)
					loadTurns = 0
					unloadedItemTypes = set()
					points = []
					
					for dist, w in order.warehousesNearest.items():
						productsCounts = list(w.productsCounts)

						for type, count in items.items():
							if count > 0 and productsCounts[type] > 0:
								if not points:
									points.append(w.point)
									loadTurns += 1
									unloadedItemTypes.add(type)
								elif not Equal(points[-1], w.point):
									points.append(w.point)
									loadTurns += 1
								
								productCountSub = min(count, productsCounts[type])
								items[type] -= productCountSub
								productsCounts[type] -= productCountSub
								itemsCount -= productCountSub

						if itemsCount == 0:
							break

					assert points
					points.insert(0, o.point)
					points.append(d.point)
					o.deliverTurnsMin = sum([Distance(points[i], points[i+1]) for i in range(len(points)-1)])
					o.deliverTurnsMin += loadTurns + len(unloadedItemTypes)

				ordersSorted = list(ordersPending)
				
				def orders_cmp(o1, o2):
					weight = float(o1.productsWeightTotal) / (o1.productsWeightTotal + o2.productsWeightTotal)
					distance = float(o1.deliverTurnsMin) / (o1.deliverTurnsMin + o2.deliverTurnsMin)

					return 0.38 * weight + 0.62 * distance < 0.5;

				ordersSorted.sort(cmp = orders_cmp)

				a = 2
	'''					
# print('original: %r\n slice: %r\n list(): %r\n copy: %r\n deepcopy: %r'
#     % (a, b, c, d, e))

	'''

	'''

					assert(!pointsForFlight.empty());

					pointsForFlight.push_back(drone->location); // fly from drone location

					// find deliver distance from order location to all nearest warehouse of order. 
					// from near to far warehouse
					// and to drone
					order->deliverTurnsMin = 0;
					Point locationLast = order->location;

					for (const auto& location : pointsForFlight)
					{
						order->deliverTurnsMin += CalcDistance(locationLast, location);
						locationLast = location;
					}

					order->deliverTurnsMin += loadTurns;
					order->deliverTurnsMin += (uint_t)unloadedItemTypes.size();
				}

				// orders priority here
				deque<shared_ptr<Order>> ordersSorted;
				copy(stateLast->ordersPending.begin(), stateLast->ordersPending.end(), back_inserter(ordersSorted));
					
				sort(ordersSorted.begin(), ordersSorted.end(),
					[](const shared_ptr<Order>& order1, const shared_ptr<Order>& order2) -> bool
				{
					// close to 0 => order1 => result true
					// close to 1 => order2 => result false
					// sum of factors => 1
					float productWeightFactor = 0.38f;
					float distanceMinFactor = 0.62f;

//add neural network for this factors. also different factors per file
					
					float productsWeightCompare = (float)order1->productsWeightTotal /
						(float)(order1->productsWeightTotal + order2->productsWeightTotal);
					float deliverDistanceMinCompare = (float)order1->deliverTurnsMin /
						(float)(order1->deliverTurnsMin + order2->deliverTurnsMin);

					float compareTotal = productWeightFactor * productsWeightCompare + distanceMinFactor * deliverDistanceMinCompare;

					return compareTotal < 0.5f;
				});

				// most fast Order
				shared_ptr<Order> order = ordersSorted.front();

				// for every drone we find the best suborders of ONE order
				deque<shared_ptr<OrderSub>> ordersSubPending = CreateOrdersSub(order);

				// try to fill drone of orders sub of SAME order. And fill them to max payload of drone
				if (!ordersSubPending.empty())
				{
					deque<shared_ptr<OrderSub>> ordersSubForDrone;
					uint_t weightCurrent = 0;
					uint_t orderId = -1; // max of uint_t

					while (!ordersSubPending.empty())
					{
						shared_ptr<OrderSub> orderSub = *ordersSubPending.begin();
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
							ordersSubPending.pop_front();
						}
						else
						{
							break;
						}
					}

					if (ordersSubPending.empty())
					{
						// drone take all ordersSub of order
						// remove from pending order
						for (uint_t id = 0; id < stateLast->ordersPending.size(); ++id)
						{
							if (stateLast->ordersPending[id]->id == order->id)
							{
								stateLast->ordersPending.erase(stateLast->ordersPending.begin() + id);
								break;
							}
						}
					}

					// create drone pending commands, only load and deliver (for one order)
					if (!ordersSubForDrone.empty())
					{
						// change global warehouses
						for (const auto& ordersSub : ordersSubForDrone)
						{
							shared_ptr<Warehouse> warehouse = stateLast->warehouses[ordersSub->warehouseId];
							warehouse->productsCounts[ordersSub->productType] -= ordersSub->productCount;
							order->productsCountPending += ordersSub->productCount;
							order->items[ordersSub->productType] -= ordersSub->productCount;
						}

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
							Point warehouseLocation = stateLast->warehouses[command->objectId]->location;
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
						order->productsCountPending -= commandPending->productCount;
						order->productsCountDelivered += commandPending->productCount;

						if (order->productsCountDelivered == order->productsCountTotal)
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
			'''
