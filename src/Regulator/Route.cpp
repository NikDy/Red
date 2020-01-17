#include "Route.h"
#include <iostream>
#include "Graph_Line.h"
#include "Data_manager.h"

Route::Route()
{
}

Route::Route(std::vector<std::pair<int, int>> seq)
{
	buildPathQueue(seq);
}

Route::~Route()
{
}

void Route::buildPathQueue(std::vector<std::pair<int, int>> points_n_weigths)
{
	for (auto point = points_n_weigths.begin(); point != points_n_weigths.end(); point++)
	{
		path_seq.push_back(point->first);
		route_lenght += point->second;
	}
}

int Route::pathTop()
{
	return *path_seq.begin();
}

void Route::showRoute() {
	for (std::vector<int>::iterator it =path_seq.begin(); it != path_seq.end(); ++it)
		std::cout << *it<<std::endl;
}

void Route::pathPop()
{
	path_seq.erase(path_seq.begin());
}

bool Route::onePoint() {
	if (path_seq.size() == 1) {
		return true;
	}
	else {
		return false;
	}
}



bool Route::isEmpty() {
	if (path_seq.size() == 0) {
		return true;
	}
	else {
		return false;
	}
}