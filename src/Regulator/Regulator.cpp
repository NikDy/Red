#include "Regulator.h"

std::vector<std::pair<int, int>> Regulator::findWay(int begin, int end, int type)
{
	auto& Graph = Data_manager::getInstance().getMapLayer0();
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto& storages = Data_manager::getInstance().getMapLayer1().getStorages();
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> frontier;
	frontier.emplace(std::pair<int, int>(Graph.getPoints()[begin].idx, 0));
	std::unordered_map<int, int> came_from;
	came_from[begin] = begin;
	std::unordered_map<int, int> cost_so_far;
	cost_so_far[begin] = 0;

	while (!frontier.empty())
	{
		auto current = frontier.top().first;
		frontier.pop();
		//if (current == end)
			//break;
		for (auto& next : Graph.getPoints()[current].adjacency_list)
		{
			int new_cost = cost_so_far[current] + Graph.getLineByTwoPoints(current, next).lenght;
			if(type == 1) {
				bool checkMarket = false;
				for (auto& market : markets) {
					if (next == market.second->point_idx) {
						checkMarket = true;
						break;
					}
				}
				if (checkMarket) {
					continue;
				} 
			} 
			else if (type == 2) {
				bool checkStorage = false;
				for (auto& storage : storages) {
					if (next == storage.second->point_idx) {
						checkStorage = true;
						break;
					}
				}
				if (checkStorage) {
					continue;
				}
			}
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next])
			{
				cost_so_far[next] = new_cost;
				came_from[next] = current;
				frontier.emplace(std::pair<int, int>(next, new_cost));
			}
		}
	}
	std::vector<std::pair<int, int>> path;
	auto current = end;
	path.push_back(std::make_pair(current, cost_so_far[current]));
	while (current != begin) {
		current = came_from[current];
		path.push_back(std::make_pair(current, cost_so_far[current]));
	}
	std::reverse(path.begin(), path.end());
	return path;
}

int Regulator::wayLength(std::vector<std::pair<int, int>> way)
{
	int lenght = 0;
	if (way.size() != 1) {
		lenght = (*way.rbegin()).second;
	}
	return lenght;
}

/*std::vector<int> Regulator::linesStatus() {	//not correct always, should think about somebody who is just standing on the line/standing on the first/second point of line
	std::map<int, Train>& trains = Data_manager::getInstance().getMapLayer1().getTrains();
	int numberOfLines = Data_manager::getInstance().getMapLayer0().getLines().size();
	std::vector<int> answer;
	int lineIdx = 0;
	int speed = 0;
	for (int i = 0; i < numberOfLines; i++) {
		answer.push_back(0);
	}
	for (auto train : trains) {
		lineIdx = train.second.getLineIdx();
		speed = train.second.getSpeed();
		if (answer[lineIdx] == 0) {
			answer[lineIdx] = speed;
		}
		else {
			if (!answer[lineIdx] == speed) {
				answer[lineIdx] = 2;
			}//else answer will be the same as it was
		}
	}
	return answer;
}

bool Regulator::storageOrMarket() { //true if market, false if storage 
	std::cout << "I'm inside storageOrMarket" << std::endl;
	auto& storages = Data_manager::getInstance().getMapLayer1().getStorages();
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	std::cout << "Town point idx " << town.point_idx << std::endl;
	int productsInTown = town.product;
	std::cout << "productsInTown " << town.product << std::endl;
	int populationInTownBeforeRoad = town.population;
	std::cout << "populationInTownBeforeRoad " << town.population << std::endl;
	int populationInTownThroughRoad=0;
	int difProducts = 0; //dif between eaten products? and products we can bring
	int lengthToStorage = 0;
	int lengthToMarket = 0;
	int wholePath = 0;
	for (auto storage : storages) {
		Storage _storage = *storage.second;
		std::cout << "Inside storage cycle. storage point idx is " << _storage.point_idx << std::endl;
		std::vector<std::pair<int, int>> pathToStorage = findWay(town.point_idx, _storage.point_idx, 1);
		lengthToStorage = wayLength(pathToStorage);
		std::cout << " market: " << _storage.point_idx << " lengthToMarket: " << lengthToStorage << std::endl;
		wholePath = lengthToStorage * 2;
		int turnCount = 1;
		int necessaryProdacts = 0;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		for (turnCount; turnCount < wholePath; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				std::cout << "inside plus 1" << std::endl;

			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				std::cout << "inside plus 2" << std::endl;

			}
		}
		difProducts = productsInTown  - necessaryProdacts;
		std::cout << "necessary products are " << necessaryProdacts << std::endl;
		std::cout << "dif products are " << difProducts << std::endl;

		if (difProducts > 0) {
			return false;
		}
	}
	return true;
}*/

/*std::map<int, std::pair<int, int>> Regulator::makeTurn() {
	Player& _player = Data_manager::getInstance().getPlayer();

	std::map<int, Train>& trains = _player.getTrains();
	std::map<int, std::shared_ptr<Market>>& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	std::map<int, std::pair<int, int>> turn;

	for (auto train : trains) {
		int lineIdx = train.second.getLineIdx();
		int position = train.second.getPosition();
		int speed = train.second.getSpeed();
		Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(lineIdx);
		int goods = train.second.getGoods();
		int goods_cap = train.second.getGoodsCapacity();
		std::vector<std::pair<int, int>> path;
		std::pair<int, int> speedNLine;
		if (goods == 0) {
			speedNLine = this->whereToGo(position, lineIdx, this->nearestMarket(lineIdx, position));
		}
		else {
			speedNLine = this->whereToGo(position, lineIdx, _player.getHome().idx);
		}
		std::pair<int, int> trainNSpeed(speedNLine.second, train.first);
		turn.emplace(speedNLine.first, trainNSpeed);
	}
	return turn;
}*/

Regulator::Regulator()
{

}

Regulator::~Regulator()
{

}








//idk if we will need them after
/*int Regulator::nearestMarket(int _lineIdx, int _position) { //return an idx of nearst market from our position on the map
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	int min = 0;
	int minLength = 10000000; //min length to the market
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(_lineIdx);
	int _point = 0;
	bool case3 = false; // the case if we are inside the line
	if (_position == 0) {
		 _point = line.points.first;
	}
	else if (_position == line.lenght) {
		 _point = line.points.first;
	}
	else {
		case3 = true;
	}
	int lengthToMarket = 0;
	for (auto market : markets) {
		Market _market = *market.second;
		if (!case3) { //if we are at some point
			std::vector<std::pair<int, int>> path = findWay(_point, _market.point_idx);
			lengthToMarket = path[path.size() - 1].second;
		}
		else { //if we are inside the line
			std::vector<std::pair<int, int>> pathFromFirstPoint = findWay(line.points.first, _market.point_idx);
			int lengthFromFirstPoint = 0; //length from position through the first point of line
			if (pathFromFirstPoint.size() == 1) { //this means that the first point of line is our destination
				lengthFromFirstPoint = _position;
			}
			else { //our first point is'n the pointToGo
				if (pathFromFirstPoint[1].first == line.points.second) {
					lengthFromFirstPoint = pathFromFirstPoint[pathFromFirstPoint.size() - 1].second + line.lenght - _position;
				}
				else {
					lengthFromFirstPoint = pathFromFirstPoint[pathFromFirstPoint.size() - 1].second + _position;
				}
			}
			std::vector<std::pair<int, int>> pathFromSecondPoint = findWay(line.points.second, _market.point_idx);
			int lengthFromSecondPoint = 0;
			if (pathFromSecondPoint.size() == 1) { //this means that the second point of line is our destination
				lengthFromSecondPoint = line.lenght - _position;
			}
			else {
				if (pathFromSecondPoint[1].first == line.points.first) {
					lengthFromSecondPoint = pathFromSecondPoint[pathFromSecondPoint.size() - 1].second + _position;
				}
				else {
					lengthFromSecondPoint = pathFromSecondPoint[pathFromSecondPoint.size() - 1].second + line.lenght - _position;
				}
			}
			if (lengthFromFirstPoint > lengthFromSecondPoint) {
				lengthToMarket = lengthFromSecondPoint;
			}
			else {
				lengthToMarket = lengthFromFirstPoint;
			}
			
		}
		if (lengthToMarket < minLength) {
			minLength = lengthToMarket;
			min = _market.point_idx;
		}
	}
	return min;
}*/

/*std::pair<int, int> Regulator::whereToGo(int _position, int _lineIdx, int pointToGo) { //the shortest way from our position to given point
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(_lineIdx);
	int speed = 0;
	int lineToGo = 0;
	if (_position == 0) { //if we are at first point of line
		int point = line.points.first;
		std::vector<std::pair<int, int>> path = findWay(point, pointToGo);
		line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(point, path[1].first);
		if (line.points.first == point) {
			speed = 1;
		}
		else {
			speed = -1;
		}
		lineToGo = line.idx;
		return std::make_pair(lineToGo, speed);
	}
	else if (_position == line.lenght) { //if we are at the second point of line
		int point = line.points.second;
		std::vector<std::pair<int, int>> path = findWay(point, pointToGo);
		line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(point, path[1].first);
		if (line.points.first == point) {
			speed = 1;
		}
		else {
			speed = -1;
		}
		lineToGo = line.idx;
		return std::make_pair(lineToGo, speed);
	}
	else { //if we are inside line
		std::vector<std::pair<int, int>> pathFromFirstPoint = findWay(line.points.first, pointToGo);
		int lengthFromFirstPoint = 0; //length from position through the first point of line
		int koef1 = 1;
		if (pathFromFirstPoint.size() == 1) { //this means that the first point of line is our destination
			lengthFromFirstPoint = _position;
		}
		else { //our first point is'n the pointToGo
			if (pathFromFirstPoint[1].first == line.points.second) { //if our path goes through the second point
				lengthFromFirstPoint = pathFromFirstPoint[pathFromFirstPoint.size() - 1].second + line.lenght - _position;
				koef1 = -1; //in this case we should change our speed in oposite direction
			}
			else { //if it doesn't
				lengthFromFirstPoint = pathFromFirstPoint[pathFromFirstPoint.size() - 1].second + _position;
			}
		}
		//all the same with second point
		std::vector<std::pair<int, int>> pathFromSecondPoint = findWay(line.points.second, pointToGo);
		int lengthFromSecondPoint = 0;
		int koef2 = 1;
		if (pathFromSecondPoint.size() == 1) { //this means that the second point of line is our destination
			lengthFromSecondPoint =line.lenght-_position;
		}
		else {
			if (pathFromSecondPoint[1].first == line.points.first) {
				lengthFromSecondPoint = pathFromSecondPoint[pathFromSecondPoint.size() - 1].second + _position;
				koef2 = -1;
			}
			else {
				lengthFromSecondPoint = pathFromSecondPoint[pathFromSecondPoint.size() - 1].second + line.lenght - _position;
			}
		}
		if (lengthFromFirstPoint <= lengthFromSecondPoint) {
			speed = -1*koef1;
		}
		else {
			speed = 1*koef2;
		}
		lineToGo = line.idx;
		return std::make_pair(lineToGo, speed);
	}

}*/



