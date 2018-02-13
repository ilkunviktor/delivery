import os, sys, functools, time
from collections import Counter, deque
from recordclass import recordclass
from math import sqrt, ceil

GetTimeCurrentMs = lambda: int(round(time.time() * 1000))

taskPath = '../../data/task/'
resultPath = '../../data/result'
scoreTotal = 0
millisecondsTotal = 0

Point = recordclass('Point', 'x y')
Warehouse = recordclass('Warehouse', 'id point productsCounts')
Order = recordclass('Order', 'id point productsCountTotal items state deliverTurn productsWeightTotal ' +
	'warehousesNearest deliverTurnsMin productsCountPending productsCountDelivered')
Command = recordclass('Command', 'droneId action objectId productType productCount turnsCount turnsCurrent')
Drone = recordclass('Drone', 'id point turnsWaited commandsPending')
OrderSub = recordclass('OrderSub', 'orderId warehouseId productType productCount')

def Distance(p1, p2):
	diffX = p2[0] - p1[0]
	diffY = p2[1] - p1[1]
	return ceil(sqrt(diffX * diffX + diffY * diffY))

def Equal(p1, p2):
	return p1[0] == p2[0] and p1[1] == p2[1];

for fn in os.listdir(taskPath):
	millisecondsStart = GetTimeCurrentMs()
	# read
	f = open(taskPath + fn)
	assert f

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
	orders = deque()

	for i in range(ordersCount):
		point = list(map(int, f.readline().split()))
		productsCountTotal = int(f.readline())
		items = dict(Counter(map(int, f.readline().split())))
		orders.append(Order(i, point, productsCountTotal, items, 'pending',
			0, 0, list(), sys.maxsize, 0, 0))
	
	f.close()

	# solve
	commandsAll = []
	drones = [Drone(i, warehouses[0].point, 0, deque()) for i in range(dronesCount)]

	for order in orders:
		order.productsWeightTotal = sum([(productWeights[type] * count) for type, count in order.items.items()])
		order.warehousesNearest = sorted([(Distance(w.point, order.point), w) for w in warehouses], key = lambda x: x[0])

	ordersDelivered = 0
	turnsCurrent = 0
	ordersPending = list(orders)

	def OrdersCmp(o1, o2):
		weight = float(o1.productsWeightTotal) / (o1.productsWeightTotal + o2.productsWeightTotal)
		distance = float(o1.deliverTurnsMin) / (o1.deliverTurnsMin + o2.deliverTurnsMin)

		return -1 if 0.38 * weight + 0.62 * distance < 0.5 else 1

	def CreateOrdersSub(o):
		result = deque()
		items = dict(o.items)

		for dist, w in order.warehousesNearest:
			productsCounts = list(w.productsCounts)

			for type, count in items.items():
				while items[type] > 0 and productsCounts[type] > 0:
					oSub = OrderSub(o.id, w.id, type, 1)
					result.append(oSub)
					productsCounts[type] -= oSub.productCount
					items[type] -= oSub.productCount
					
		result.reverse()

		return result

	def CommandsMerging(commands):
		commandLast = None

		for i in range(len(commands)):
			commandCurrent = commands[i]

			if commandLast:
				if (commandLast.productType == commandCurrent.productType and
					commandLast.objectId == commandCurrent.objectId and
					commandCurrent.turnsCount == 1):
					commandLast.productCount += commandCurrent.productCount
					del commands[i]
					i -= 1
				else:
					commandLast = commandCurrent

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
					
					for dist, w in o.warehousesNearest:
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
									unloadedItemTypes.add(type)
								
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
				ordersSorted.sort(key = functools.cmp_to_key(OrdersCmp))
				order = ordersSorted[0]
				ordersSubPending = CreateOrdersSub(order)
				
				if ordersSubPending:
					ordersSubForDrone = deque()
					weightCurrent = 0;
					orderId = -1

					while ordersSubPending:
						orderSub = ordersSubPending[0]
						orderWeight = productWeights[orderSub.productType]

						if not ordersSubForDrone:
							orderId = orderSub.orderId

						if (not(ordersSubForDrone and orderId != orderSub.orderId) and
							weightCurrent + orderWeight <= payload):
							ordersSubForDrone.append(orderSub)
							weightCurrent += orderWeight
							ordersSubPending.popleft()
						else:
							break

						if not ordersSubPending:
							for i in range(len(ordersPending)):
								if(ordersPending[i].id == order.id):
									ordersPending = ordersPending[:i] + ordersPending[i+1:]
									break

						if ordersSubForDrone:
							for orderSub in ordersSubForDrone:
								warehouses[orderSub.warehouseId]
								order.productsCountPending += orderSub.productCount
								order.items[orderSub.productType] -= orderSub.productCount

							commandsLoad = deque()

							for orderSub in ordersSubForDrone:
								warehousePoint = warehouses[orderSub.warehouseId].point
								moveDistance = 0 if Equal(d.point, warehousePoint) else Distance(d.point, warehousePoint)
								d.point = warehousePoint
								commandsLoad.append(Command(d.id, 'L', orderSub.warehouseId, orderSub.productType,
									orderSub.productCount, moveDistance + 1, 0))
							
							CommandsMerging(commandsLoad)
							d.commandsPending.extend(commandsLoad)
							commandsDeliver = deque()
							orderPoint = orders[orderId].point

							for orderSub in ordersSubForDrone:
								deliverDistance = 0 if Equal(d.point, orderPoint) else Distance(d.point, orderPoint)
								d.point = orderPoint
								commandsDeliver.append(Command(d.id, 'D', orderId, orderSub.productType,
									   orderSub.productCount, deliverDistance + 1, 0))
						
							CommandsMerging(commandsDeliver)
							d.commandsPending.extend(commandsDeliver)

			if not d.commandsPending:
				d.turnsWaited += 1
			else:
				if d.turnsWaited > 0:
					commandsAll.append(Command(d.id, 'W', 0, 0, 0, d.turnsWaited, 0))
					d.turnsWaited = 0

				commandPending = d.commandsPending[0]
				commandPending.turnsCurrent += 1

				if commandPending.turnsCurrent == commandPending.turnsCount:
					commandsAll.append(commandPending)
					d.commandsPending.popleft()

					if commandPending.action == 'D':
						order = orders[commandPending.objectId]
						order.productsCountPending -= commandPending.productCount
						order.productsCountDelivered += commandPending.productCount

						if order.productsCountDelivered == order.productsCountTotal:
							order.state = 'delivered'
							order.deliverTurn = turnsCurrent
							ordersDelivered += 1

	# result
	fnSplit = fn.split('.')
	resultFileName = resultPath + "/" + fnSplit[0] + ".out"
	f = open(resultFileName, 'w')
	assert f

	f.write(str(len(commandsAll)) + '\n')

	for c in commandsAll:
		result = '%d %s ' % (c.droneId, c.action)

		if c.action == 'W':
			result += str(c.turnsCount)
		else:
			result += '%d %d %d' % (c.objectId, c.productType, c.productCount)

		f.write(result + '\n')

	f.close()

	# score
	score = 0

	for o in orders:
		if o.state == 'delivered':
			score += ceil(float(turns - o.deliverTurn) * 100 / turns)

	scoreTotal += score
	millisecondsTask = GetTimeCurrentMs() - millisecondsStart
	millisecondsTotal += millisecondsTask
	print('file: %s\nscore: %d\nmillisecondsTask: %d\n' % (fn, score, millisecondsTask))

print('scoreTotal: %d\nmillisecondsTotal: %d' % (scoreTotal, millisecondsTotal))