#pragma once
#include "Graph_Line.h"
#include "Data_manager.h"

#include <iostream>
#include <queue>
#include <vector>


class Route
{
public:
	Route();
	Route(std::vector<std::pair<int, int>>);
	~Route();

	void buildPathQueue(std::vector<std::pair<int, int>>);
	int pathTop();
	void pathPop();
	bool onePoint();
	bool isEmpty();
	void showRoute();
	std::vector<int> path_seq;  //points of current route
	int route_lenght = 0;

};