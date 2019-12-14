#pragma once
#include "Route.h"
#include "Data_manager.h"
#include <unordered_map>


class RoutePlaner
{
public:
	static Route buildNewMarketRoute(int begin, Train&);
	static std::vector<std::pair<int, int>> findWay(int begin, int end);

private:
	RoutePlaner();
	~RoutePlaner();

	static int wayLength(std::vector<std::pair<int, int>>);
};