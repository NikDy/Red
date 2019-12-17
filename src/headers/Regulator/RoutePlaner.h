#pragma once
#pragma once
#include "Route.h"
#include "Data_manager.h"
#include "Regulator.h"
#include <unordered_map>
#include "TrainDriver.h"


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
	Route buildNewMarketRoute(int begin, Train& train);
	Route buildNewStorageRoute(int begin, Train& train);
	std::vector<std::pair<int, int>> bestWayToMarket(int begin, Train& train);
	bool storageOrMarket(int begin, Train& train);
	void buildRoutes();
	void addDriver(int _idx, TrainDriver _trainDriver);
	

private:
	std::map<int,TrainDriver> drivers; //int- driver's idx; true in town, false on road
	RoutePlaner();
	~RoutePlaner();

	
};