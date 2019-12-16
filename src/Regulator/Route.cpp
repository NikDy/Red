#include "Route.h"
#include <iostream>

Route::Route()
{
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
	for (auto point = points_n_weigths.end() - 1; point != points_n_weigths.begin(); point--)
	{
		path_seq.push_back(point->first);
		route_lenght += point->second;
	}

}

int Route::pathTop()
{
	std::cout << "i'm inside pathTop() " << std::endl;
	std::cout << "path seq begin is " << *path_seq.begin()<< std::endl;


	return *path_seq.begin();
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