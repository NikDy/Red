#pragma once
#include "Data_manager.h"
#include "Network_manager.h"
#include <queue>
#include <unordered_map>

class Regulator
{
private:

public:
	std::vector<int> findWay(int begin, int end);

	Regulator();
	~Regulator();
};

