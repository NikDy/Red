#pragma once
#include <list>
#include "Game_object.h"
#include <typeinfo>

class Graph_Point : public Game_object
{
public:

	int idx = 0;
	int post_idx = 0;
	std::list<int> adjacency_list;

	Graph_Point(int idx_, int post_idx_);
	Graph_Point(int idx);
	Game_object * clone();
	const std::type_info& getObjectType();
	~Graph_Point();
};

