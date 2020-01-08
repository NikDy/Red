#pragma once
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