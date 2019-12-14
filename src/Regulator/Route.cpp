#include "Route.h"

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
	for (auto point = points_n_weigths.end(); point != points_n_weigths.begin(); point--)
	{
		path_seq.push_back(point->first);
		route_lenght += point->second;
	}
}

int Route::pathTop()
{
	return *path_seq.begin();
}

void Route::pathPop()
{
	path_seq.erase(path_seq.begin());
}
