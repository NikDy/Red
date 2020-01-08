#include "Regulator.h"

std::vector<std::pair<int, int>> Regulator::findWay(int begin, int end, int type)
{
	auto& Graph = Data_manager::getInstance().getMapLayer0();
	auto& points = Graph.getPoints();
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
		for (auto& next : Graph.getPoints()[current].adjacency_list)
		{
			Graph_Line line = Graph.getLineByTwoPoints(current, next);
			int speed = Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(current, next);
			if (current == begin && checkLine(line, speed) == false) {
				continue;
			}
			int new_cost = cost_so_far[current] + line.lenght;
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
	if (came_from.find(current) == came_from.end()) return path;
	path.push_back(std::make_pair(current, cost_so_far[current]));
	while (current != begin) {
		current = came_from[current];
		path.push_back(std::make_pair(current, cost_so_far[current]));
	}
	std::reverse(path.begin(), path.end());

	std::cout << "build route from " << path.begin()->first << " to " << path.back().first << std::endl;
	return path;
}


int Regulator::wayLength(std::vector<std::pair<int, int>> way)
{
	int lenght = 0;
	if (way.size() == 0) return 0;
	if (way.size() != 1) {
		lenght = (*way.rbegin()).second;
	}
	return lenght;
}

bool Regulator::checkLine(Graph_Line line, int speed)
{
	for (auto tr : line.trains) {
		if (tr.speed != speed) {
			return false;
		}
		else {
			if(speed == 1 && tr.position < 3) return false;
			if(speed == -1 && line.lenght - tr.position < 3) return false;
		}
	}
	return true;
}




Regulator::Regulator()
{
}


Regulator::~Regulator()
{
}
