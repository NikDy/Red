#pragma once
#include "RoutePlaner.h"

class Decision {
public:
	Decision();
	~Decision();
	std::map<int, std::pair<int, int>> makeTurn();
	void Insertion();//insert all trains we need to insert into planer
};
