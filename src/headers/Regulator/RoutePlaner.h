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
	
	Route buildNewMarketRoute(int begin, Train&);
	Route buildNewStorageRoute(int begin, Train&);
	
	void buildRoutes();
	void addDriver(int _idx, TrainDriver _trainDriver);
	std::map<int, TrainDriver> drivers; //int- driver's idx; bool- driver's status (true in town, false on road)

private:
	RoutePlaner();
	~RoutePlaner();

	
};