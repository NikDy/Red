#pragma once
#include "Data_manager.h"
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
	bool checkFuel(Train _train);//tells us if we need to go home to refuel
	int findLength(int _lineIdx, int _position, int pointToGo);//return the length of the shortest path from our place to point, idk if we need it now, but it seemed to be a good idea
	std::vector<int> linesStatus(); //return status of all lines on the map, which tell us what is going on inside this line(0-nobody inside, 1-everybody in positive direction,-1 - everybody in negative direction, 2-all mixed
	std::vector<std::pair<int, int>> findWayWithoutLine(int begin, int end, int lineIdx); //returns shortest way without line which is busy, maybe should do vector of lineIdxes, which are busy at the moment

	Regulator();
	~Regulator();
};