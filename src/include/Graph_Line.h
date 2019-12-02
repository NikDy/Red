#pragma once
#include <utility>


class Graph_Line
{
public:
	int idx = 0;
	int lenght = 0;
	std::pair<int, int> points = std::pair<int, int>(0, 0);

	Graph_Line(int idx_, int lenght_, std::pair<int, int> points_);

	~Graph_Line();
};

