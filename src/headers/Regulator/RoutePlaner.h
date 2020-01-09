#pragma once
#pragma once
#include "Route.h"
#include "Data_manager.h"
#include "Regulator.h"
#include "TrainDriver.h"

#include <unordered_map>

typedef std::pair<Market, std::vector<std::pair<int, int>>> wayToMarket;
typedef std::vector<std::pair<int, int>> routeSeq;

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

	bool buildRoutes(std::pair<const int, TrainDriver> &driver);
	void loadDrivers();

	void makeTurn();

	void upgradeIfPossible();

private:
	Regulator reg;
	int getPointIdxByLineAndPosition(Graph_Line line, int pos);

	routeSeq wayToMostActualPost(int begin, TrainDriver& train);
	routeSeq bestWayToMarket(int begin, Train& train);
	routeSeq bestWayToStorage(int begin, Train& train);
	routeSeq bestWayToHome(int begin, Train& train);

	void addDriver(int _idx, TrainDriver _trainDriver);

	std::map<int,TrainDriver> drivers; //int- driver's idx; true in town, false on road
	RoutePlaner();
	~RoutePlaner();
	
};