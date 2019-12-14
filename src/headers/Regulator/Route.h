#pragma once
#include <queue>
#include <vector>


class Route
{
public:
	Route();
	~Route();

	void buildPathQueue(std::vector<std::pair<int, int>>);
	int pathTop();
	void pathPop();

private:
	std::vector<int> path_seq;  //points of current route
	int route_lenght = 0;

};