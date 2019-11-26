#pragma once
#include "Data_manager.h"
#include "Network_manager.h"
#include <queue>

class Regulator
{
private:

public:
	std::vector<int> findWay(int begin, int end);

	Regulator();
	~Regulator();
};

