#pragma once
#include "Data_manager.h"
#include <queue>
#include <unordered_map>

//регулятор должен принимать решения и отдовать приказы!!!
//не нужно строть маршруты для кого-то, он строит их для себя

//если объект должен общаться к своим полям, то используется this
// не reg.findWay() а this->findWay()
//если метод класса должен вызваться без создания объекта, то метод будет static


//не делайте варнинги, потому что они потом могут привести к печальному исходу
//лучше сразу их зафиксить это обычно не трудно

class Regulator
{
private:

public:
	std::vector<std::pair<int, int>> findWay(int begin, int end);
	int nearestMarket(int _lineIdx, int _position);
	std::map<int, std::pair<int, int>> makeTurn();
	std::pair<int, int> whereToGo(int _position, int _lineIdx, int pointToGo); //return speed and lineIdx, where to go

	Regulator();
	~Regulator();
};

