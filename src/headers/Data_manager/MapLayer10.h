#pragma once
#include "Game_object.h"
#include <vector>

class MapLayer10 :
	public Game_object
{
public:
	std::vector<std::pair<int, std::pair<int, int>>> points; //idx, x, y
	int idx = 0;
	std::pair<int, int> size; //x, y

	MapLayer10(int idx_, std::pair<int, int> size_, std::vector<std::pair<int, std::pair<int, int>>> points_) : idx(idx_), size(size_), points(points_) {};
	MapLayer10();

	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	~MapLayer10();
};

