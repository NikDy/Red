#pragma once
#include "Data_manager.h"
#include "Network_manager.h"
#include <queue>
#include <unordered_map>

class Regulator
{
private:

public:
	std::vector<std::pair<int, int>> findWay(int begin, int end);
	int nearestMarket(int _lineIdx, int _position);
	std::map<int, std::pair<int, int>> makeTurn();
	std::pair<int, int> whereToGo(int _position, int _lineIdx, int pointToGo); //return speed and lineIdx, where to go

	Regulator();
	~Regulator();
};

