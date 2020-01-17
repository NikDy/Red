#pragma once
#include <list>
#include "Game_object.h"
#include <typeinfo>
#include <vector>
#include "Train.h"

class Graph_Point : public Game_object
{
public:
	bool pointBusy = false;
	int trainBusy = 0;
	std::vector<Train> trains = std::vector<Train>();
	int idx = 0;
	int post_idx = 0;
	std::list<int> adjacency_list;

	Graph_Point();
	Graph_Point(int idx_, int post_idx_);
	Graph_Point(int idx);
	std::shared_ptr<Game_object> getObjectPtr();
	const std::type_info& getObjectType();
	~Graph_Point();
};

