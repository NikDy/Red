#pragma once
#include "Data_manager.h"
#include <queue>
#include <unordered_map>


class Regulator
{
private:

public:
	
	std::vector<std::pair<int, int>> findWay(int begin, int end, int type = 0);
	static int wayLength(std::vector<std::pair<int, int>>);

	bool checkLine(Graph_Line line, int speed);

	Regulator();
	~Regulator();
};