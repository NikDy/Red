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

	TrainDriver& getTrainDriverByIdx(int idx);
	Route getRouteByIdx(int idx);
	std::map<int, TrainDriver>& getDrivers();
	void loadDrivers();

	void makeTurn();

	int countTrainsInPoint(int point);
private:
	Regulator reg;
	int getPointIdxByLineAndPosition(Graph_Line line, int pos);

	int lengthToPoint(Graph_Point point, Train & train);

	bool buildRoutes(std::pair<const int, TrainDriver>& driver);
	routeSeq bestWayToMarket(int begin, Train& train);
	routeSeq bestWayToStorage(int begin, Train& train);
	routeSeq bestWayToHome(int begin, Train& train);

	void stageAffairs();
	void tryGoToSecondStage();
	void tryGoToThirdStage();
	void resetRoutes();

	void addDriver(int _idx, TrainDriver _trainDriver);
	void upgradeIfPossible();
	void resetTrainsLists();

	void makeQueue();

	int server_tick_to_update = std::stoi(Data_manager::getInstance().config["server_tick_to_update"]);
	int game_tick_to_update = std::stoi(Data_manager::getInstance().config["game_tick_to_update"]);
	int game_stage = std::stoi(Data_manager::getInstance().config["game_stage"]);
	int max_town_level = std::stoi(Data_manager::getInstance().config["stage_" + std::to_string(game_stage) + "_town_max"]);
	int max_train_level = std::stoi(Data_manager::getInstance().config["stage_" + std::to_string(game_stage) + "_train_max"]);
	int average_market_way = std::stoi(Data_manager::getInstance().config["average_market_way"]);
	int average_storage_way = std::stoi(Data_manager::getInstance().config["average_storage_way"]);
	float division_coef = 1;
	std::list<TrainDriver> products_drivers;
	std::list<TrainDriver> armor_drivers;
	std::map<int,TrainDriver> drivers; //int- driver's idx; true in town, false on road
	RoutePlaner();
	~RoutePlaner();
};