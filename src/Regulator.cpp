#include "Regulator.h"


std::vector<int> Regulator::findWay(int begin, int end)
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

	//return came_from;
	std::vector<int> path;
	auto current = end;
	path.push_back(current);
	while (current != begin) {
		current = came_from[current];
		path.push_back(current);
	}
	std::reverse(path.begin(), path.end());
	for (auto i : path) std::cout << i << " ";
	std::cout << std::endl;
	return path;
}


Regulator::Regulator()
{

}


Regulator::~Regulator()
{

}
