#pragma once
#pragma once
#include "Route.h"
#include "Data_manager.h"
#include "Regulator.h"
#include "TrainDriver.h"

#include <unordered_map>



class RoutePlaner
{
public:
	static RoutePlaner& getInstance()
	{
		static RoutePlaner instance;
		return instance;
	}
	RoutePlaner(RoutePlaner const&) = delete;
	RoutePlaner operator=(RoutePlaner const&) = delete;//singleton

	std::map<int, TrainDriver>& getDrivers();
	std::vector<std::pair<int, int>> bestWayToMarket(int begin, Train& train);

	bool buildRoutes(std::pair<const int, TrainDriver> &driver);

	void addDriver(int _idx, TrainDriver _trainDriver);

	int needProducts(int length, int &population);

	std::vector<std::pair<int, int>> bestWayToStorage(int begin, Train& train);
	std::vector<std::pair<int, int>> StorageToMarket(int begin, Train& train, Town& town);

private:
	int getPointIdxByLineAndPosition(Graph_Line line, int pos);

	std::map<int,TrainDriver> drivers; //int- driver's idx; true in town, false on road
	RoutePlaner();
	~RoutePlaner();
	
};