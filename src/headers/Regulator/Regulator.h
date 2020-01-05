#pragma once
#include "Data_manager.h"
#include <queue>
#include <unordered_map>


class Regulator
{
private:

public:
	
	std::vector<int> linesStatus(); //return status of all lines on the map, which tell us what is going on inside this line(0-nobody inside, 1-everybody in positive direction,-1 - everybody in negative direction, 2-all mixed
	bool storageOrMarket(); //true if market, false if storage 
	std::map<int, std::pair<int, int>> makeTurn();
	std::vector<std::pair<int, int>> findWay(int begin, int end, int type = 0);
	static int wayLength(std::vector<std::pair<int, int>>);

	bool checkLine(Graph_Line line, int speed);
	//idk if we will ned them after new logic
	//int nearestMarket(int _lineIdx, int _position);
	//std::pair<int, int> whereToGo(int _position, int _lineIdx, int pointToGo); //return speed and lineIdx, where to go

	Regulator();
	~Regulator();
};