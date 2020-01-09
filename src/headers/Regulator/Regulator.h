#pragma once
#include "Data_manager.h"
#include <queue>
#include <unordered_map>


class Regulator
{
private:

public:
	
	std::vector<std::pair<int, int>> findWay(int begin, int end, Train& train, int type = 0);
	static int wayLength(std::vector<std::pair<int, int>>);

	Regulator();
	~Regulator();
private:
	bool checkLine(Graph_Line line, int speed);
	bool checkPoint(Graph_Point point, Train& train, Graph_Line line);
	int lengthToPoint(Graph_Point point, Train & train);
};