#pragma once
#include <utility>
#include "Game_object.h"
#include <typeinfo>

class Graph_Line : public Game_object
{
public:
	int idx = 0;
	int lenght = 0;
	std::pair<int, int> points = std::pair<int, int>(0, 0);

	Graph_Line(int idx_, int lenght_, std::pair<int, int> points_);
	Game_object * clone();
	const std::type_info& getObjectType();
	~Graph_Line();
};

