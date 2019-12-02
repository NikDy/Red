#include "Regulator.h"


std::vector<std::pair<int, int>> Regulator::findWay(int begin, int end)
{
	auto& Graph = Data_manager::getInstance().getMapLayer0();
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
		if(current == end)
			break;
		for (auto& next : Graph.getPoints()[current].adjacency_list)
		{
			int new_cost = cost_so_far[current] + Graph.getLineByTwoPoints(current, next).lenght;
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


int Regulator::nearestMarket(int _lineIdx, int _position) { 
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	int min = 0;
	int length = 10000000;
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(_lineIdx);
	int _point = 0;
	bool case3 = false;
	if (_position == 0) {
		 _point = line.points.first;
	}
	else if (_position == line.lenght) {
		 _point = line.points.first;
	}
	else {
		case3 = true;
	}
	int length1 = 0;
	for (auto market : markets) {
		Market _market = *market.second;
		if (!case3) {
			std::vector<std::pair<int, int>> path = this->findWay(_point, _market.point_idx);
			length1 = path[path.size() - 1].second;
		}
		else {
			std::vector<std::pair<int, int>> path1 = this->findWay(line.points.first, _market.point_idx);
			if (path1.size()  > 1) {
				if (path1[1].first == line.points.second) {
					length1 = path1[path1.size() - 1].second + line.lenght - _position;
				}
				else {
					length1 = path1[path1.size() - 1].second + _position;
				}
			}
			std::vector<std::pair<int, int>> path2 = this->findWay(line.points.second, _market.point_idx);
			int length2 = 10000000;
			if (path2.size() > 1) {
				if (path2[1].first == line.points.first) {
					length2 = path2[path1.size() - 1].second + _position;
				}
				else {
					length2 = path2[path2.size() - 1].second + line.lenght - _position;
				}
			}
			if (length1 > length2) {
				length1 = length2;
			}
			
		}
		if (length1 < length) {
			length = length1;
			min = _market.point_idx;
		}
	}
	//std::cout << min << std :: endl;
	return min;
}

std::pair<int, int> Regulator::whereToGo(int _position, int _lineIdx, int pointToGo) {
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(_lineIdx);
	int speed = 0;
	int lineToGo = 0;
	if (_position == 0) {
		int point = line.points.first;
		std::vector<std::pair<int, int>> path = this->findWay(point, pointToGo);
		line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(point, path[1].first);
		if (line.points.first == point) {
			speed = 1;
		}
		else {
			speed = -1;
		}
		lineToGo = line.idx;
		//std::cout << "line to go:" << lineToGo << " speed:" << speed << std::endl;
		return std::make_pair(lineToGo, speed);
	}
	else if (_position == line.lenght) {
		int point = line.points.second;
		std::vector<std::pair<int, int>> path = this->findWay(point, pointToGo);
		line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(point, path[1].first);
		if (line.points.first == point) {
			speed = 1;
		}
		else {
			speed = -1;
		}
		lineToGo = line.idx;
		//std::cout << "line to go:" << lineToGo << " speed:" << speed << std::endl;
		return std::make_pair(lineToGo, speed);
	}
	else {
		std::vector<std::pair<int, int>> path1 = this->findWay(line.points.first, pointToGo);
		int length1 = 0;
		int koef1 = 1;
		if (path1.size() > 1) {
			if (path1[1].first == line.points.second) {
				length1 = path1[path1.size() - 1].second + line.lenght - _position;
				koef1 = -1;
			}
			else {
				length1 = path1[path1.size() - 1].second + _position;
			}
		}
		std::vector<std::pair<int, int>> path2 = this->findWay(line.points.second, pointToGo);
		int length2 = 0;
		int koef2 = 1;
		if (path2.size() > 1) {
			if (path2[1].first == line.points.first) {
				length2 = path2[path1.size() - 1].second + _position;
				koef2 = -1;
			}
			else {
				length2 = path2[path2.size() - 1].second + line.lenght - _position;
			}
		}
		if (length1 <= length2) {
			speed = -1*koef1;
		}
		else {
			speed = 1*koef2;
		}
		//std::cout << "line to go:" << lineToGo << " speed:" << speed << std::endl;
		return std::make_pair(_lineIdx, speed);
	}

}

std::map<int, std::pair<int, int>> Regulator::makeTurn() { 
	//std::cout << "I got inside" << std::endl;
	Player& _player = Data_manager::getInstance().getPlayer();

	std::map<int, Train>& trains = _player.getTrains();
	std::map<int, std::shared_ptr<Market>>& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	std::map<int, std::pair<int, int>> turn;
	int lineIdx = 0;
	int position = 0;
	int speed = 0;
	int goods = 0;
	int goods_cap = 0;

	for (auto train : trains) {
		lineIdx = train.second.getLineIdx();
		position = train.second.getPosition();
		speed = train.second.getSpeed();
		Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(lineIdx);
		goods = train.second.getGoods();
		goods_cap = train.second.getGoodsCapacity();
		std::vector<std::pair<int, int>> path;
		std::pair<int, int> speedNLine;
		if (goods == goods_cap) {
			speedNLine = this->whereToGo(position, lineIdx, _player.getHome().post_idx);
		}
		else {
			speedNLine = this->whereToGo(position, lineIdx, this->nearestMarket(lineIdx, position));
		}
		//std::cout << "lineIdx:" << speedNLine.first << " speed: " << speedNLine.second << " trainIdx" << train.first;
		std::pair<int, int> trainNSpeed(speedNLine.second, train.first);
		turn.emplace(speedNLine.first, trainNSpeed);
	}
	return turn;
}


Regulator::Regulator()
{

}


Regulator::~Regulator()
{

}
