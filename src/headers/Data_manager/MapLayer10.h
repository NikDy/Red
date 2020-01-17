#pragma once
#include "Game_object.h"
#include <vector>
#include <map>

class MapLayer10 :
	public Game_object
{
public:
	std::map<int, std::pair<int, int>> points; //idx, x, y
	int idx = 0;
	std::pair<int, int> size; //x, y

	MapLayer10(int idx_, std::pair<int, int> size_) : idx(idx_), size(size_) {};
	MapLayer10();

	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	~MapLayer10();
};

