#include "RoutePlaner.h"



//Route RoutePlaner::buildNewMarketRoute(int begin, Train& train)
//{
//	auto markets = Data_manager::getInstance().getMapLayer1().getMarkets();
//	auto town = Data_manager::getInstance().getPlayer().getTown();
//	int way_length = 0;
//	int turns_before_population_decrease = town.product / ((town.population + (int)(way_length 
//		/ std::stof(Data_manager::getInstance().config["average_population_increase_period"]))) % town.population_capacity);
//	for (auto market : markets)
//	{
//		market.second->point_idx;
//	}
//}


std::vector<std::pair<int, int>> RoutePlaner::findWay(int begin, int end)
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
		if (current == end)
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


RoutePlaner::~RoutePlaner()
{
}

int RoutePlaner::wayLength(std::vector<std::pair<int, int>> way)
{
	int lenght = 0;
	for (auto point : way)
	{
		lenght += point.second;
	}
	return lenght;
}
