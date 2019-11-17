#pragma once
#include <list>
#include "Game_object.h"
#include <typeinfo>

class Graph_Point : Game_object
{
public:
	Game_object * clone();
	const std::type_info& getType();
	int idx = 0;
	int post_idx = 0;
	std::list<int> adjacency_list;

	Graph_Point(int idx_, int post_idx_);

	Graph_Point(int idx);

	~Graph_Point();
};

