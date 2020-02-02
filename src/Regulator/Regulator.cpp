#include "Regulator.h"
#include "RoutePlaner.h"

std::vector<std::pair<int, int>> Regulator::findWay(int begin, int end, Train& train, int type)
{
	auto& Graph = Data_manager::getInstance().getMapLayer0();
	auto& points = Graph.getPoints();

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> frontier;
	frontier.emplace(std::pair<int, int>(Graph.getPoints()[begin].idx, 0));
	std::unordered_map<int, int> came_from;
	came_from[begin] = begin;
	std::unordered_map<int, int> cost_so_far;
	cost_so_far[begin] = 0;
	int i = 1;
	while (!frontier.empty())
	{
		auto current = frontier.top().first;
		frontier.pop();
		for (auto& next : Graph.getPoints()[current].adjacency_list)
		{
			Graph_Line line = Graph.getLineByTwoPoints(current, next);
			train.line_idx = line.idx;
			train.speed = Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(current, next);
			if (train.speed == 1) train.position = 0;
			else train.position = line.lenght;
			if (current == begin && (checkLine(line, train, points[current]) == false || checkPoint(points[next], train, line) == false)) {
				continue;
			}
			int new_cost;
			if (nextPointWeight(current, next, type) != -1)
			{
				new_cost = cost_so_far[current] + nextPointWeight(current, next, type);
			}
			else
			{
				continue;
			}
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next])
			{
				cost_so_far[next] = new_cost;
				came_from[next] = current;
				frontier.emplace(std::pair<int, int>(next, new_cost));
			}
			i++;
		}
	}
	std::vector<std::pair<int, int>> path = std::vector<std::pair<int, int>>();
	auto current = end;
	if (came_from.find(current) == came_from.end()) return path;
	path.push_back(std::make_pair(current, cost_so_far[current]));
	while (current != begin) {
		current = came_from[current];
		path.push_back(std::make_pair(current, cost_so_far[current]));
	}
	std::reverse(path.begin(), path.end());

	//std::cout << "build route from " << path.begin()->first << " to " << path.back().first << std::endl;
	return path;
}


int Regulator::nextPointWeight(int current_point_idx, int next_point_idx, int train_type)
{
	auto markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto storages = Data_manager::getInstance().getMapLayer1().getStorages();
	if (train_type == 1) {
		for (auto market : markets) {
			if (next_point_idx == market.second->point_idx) {
				return -1;
			}
		}
	}
	else if (train_type == 2) {
		for (auto storage : storages) {
			if (next_point_idx == storage.second->point_idx) {
				return -1;
			}
		}
	}
	return Data_manager::getInstance().getMapLayer01().getLineByTwoPoints(current_point_idx, next_point_idx).lenght;
}



std::vector<std::pair<int, int>> Regulator::findWay(int begin, int end, int type)
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
		for (auto& next : Graph.getPoints()[current].adjacency_list)
		{
			Graph_Line line = Graph.getLineByTwoPoints(current, next);
			int new_cost;
			if (nextPointWeight(current, next, type) != -1)
			{
				new_cost = cost_so_far[current] + nextPointWeight(current, next, type);
			}
			else
			{
				continue;
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

bool Regulator::checkLine(Graph_Line line,Train& train, Graph_Point pointNow)
{
	int homeIdx = Data_manager::getInstance().getPlayer().getHome().idx;
	Train tr = nearestTrain(line, train);
	if (tr.idx != train.idx) {
		if (pointNow.idx == homeIdx && tr.speed != train.speed) {
			int trToPoint = lengthToPoint(pointNow, tr);
			if (trToPoint <= (line.lenght)) return true;
		}
		if (tr.speed != train.speed) return false;
		/*else {
			if (train.speed == 1 && tr.position < 2) return false;
			if (train.speed == -1 && line.lenght - tr.position < 2) return false;
		}*/
	}
	return true;
}

bool Regulator::checkPoint(Graph_Point point, Train& train, Graph_Line line)
{
	int trainToPoint = line.lenght;
	int pointNow = 0;
	if (point.idx == line.points.first) pointNow = line.points.second;
	else pointNow = line.points.first;
		for (auto tr : point.trains) {
			if (tr.idx != train.idx) {
				int trToPoint = lengthToPoint(point, tr);
				if (tr.line_idx == line.idx && tr.speed == train.speed) {
					if (tr.getPlayerIdx() == train.getPlayerIdx()) {
						Route dri = RoutePlaner::getInstance().getRouteByIdx(tr.idx);
						if (dri.path_seq.size() == 0 && trToPoint == 0 && tr.goods != 0) {
							return false;
						}
						else if (dri.path_seq.size() == 1 && trToPoint <= trainToPoint) {
							return false;
						}
						continue;
					}
					else continue;
				}
				//if (tr.getPlayerIdx() == train.getPlayerIdx() && pointNow == homeIdx) return false;
				if ((trainToPoint + 1) < trToPoint) continue;
				if (tr.getPlayerIdx() == train.getPlayerIdx() && trToPoint <= trainToPoint && trainToPoint > 2) {
					Route dri = RoutePlaner::getInstance().getRouteByIdx(tr.idx);
					if (dri.path_seq.size() == 1) {
						if (dri.path_seq[0] == pointNow) {
							return false;
						}
						else continue;
					}
					else if (dri.path_seq.size() >= 2) {
						if (dri.path_seq[0] == pointNow || dri.path_seq[1] == pointNow) {
							return false;
						}
						else continue;
					}
					else continue;
				}
				if (trToPoint == 0 && line.lenght != 1 && train.getPlayerIdx() != tr.getPlayerIdx()) continue;
				//if (trToPoint == 0 && tr.getPlayerIdx() == train.getPlayerIdx()) continue;
				return false;
			}
		}
	return true;
}


int Regulator::lengthToPoint(Graph_Point point, Train& train)
{
	Graph_Line trainLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(train.line_idx);
	int trainToPoint = -1;
	if (trainLine.points.second == point.idx) {
		trainToPoint = trainLine.lenght - train.position;
	}
	else if (trainLine.points.first == point.idx) {
		trainToPoint = train.position;
	}
	return trainToPoint;
}

Train Regulator::nearestTrain(Graph_Line line, Train& train)
{
	Train nearesTrain = Train(train);
	if (train.speed == 1) {
		nearesTrain.position = 10000;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (tr.position > train.position && tr.position < nearesTrain.position) {
				nearesTrain = tr;
			}
		}
	}
	else if (train.speed == -1) {
		nearesTrain.position = -1;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (tr.position < train.position && tr.position > nearesTrain.position) {
				nearesTrain = tr;
			}
		}
	}
	return nearesTrain;
}


Regulator::Regulator()
{
}


Regulator::~Regulator()
{
}
