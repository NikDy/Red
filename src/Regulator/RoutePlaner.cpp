#include "RoutePlaner.h"



std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}


void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}


std::vector<std::pair<int, int>> RoutePlaner::bestWayToStorage(int begin, Train & train)
{
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	for (auto storage : Data_manager::getInstance().getMapLayer1().getStorages()) {
		if (begin == storage.second->point_idx) continue;
		routeSeq way = reg.findWay(begin, storage.second->point_idx, 1);
		if (way.size() == 0) continue;
		int safe_armor_capacity = 4 * reg.wayLength(way);
		int possible_to_take = std::min(storage.second->armor, train.goods_capacity);
		if (safe_armor_capacity - possible_to_take < bestDelta)
		{
			bestWay = way;
			bestDelta = safe_armor_capacity - possible_to_take;
		}
	}

	return bestWay;
}


bool RoutePlaner::buildRoutes(std::pair<const int, TrainDriver> &driver) {
	if (driver.second.getStatus()) {
		Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (driven_train.cooldown != 0){
			std::cout << "Train: " << driven_train.idx << " collided" << std::endl;
			return false;
		}
		int route_start_point = getPointIdxByLineAndPosition(Data_manager::getInstance().getMapLayer0().getLineByIdx(driven_train.getLineIdx()),
								driven_train.getPosition());
		
		routeSeq way;
		if (driven_train.goods == driven_train.goods_capacity) {
			way = bestWayToHome(route_start_point, driven_train);
		}
		else if (driven_train.goods < driven_train.goods_capacity)
		{
			way = wayToMostActualPost(route_start_point, driver.second);
		}
		if (way.size() == 0) return false;
		
		driver.second.setStatus(false);
		driver.second.setRoute(Route(way));
	}
	return true;
}


routeSeq RoutePlaner::wayToMostActualPost(int begin, TrainDriver& _driver)
{
	auto town = Data_manager::getInstance().getPlayer().getTown();
	auto train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(_driver.getIdx());
	Regulator reg;

	int expected_food_income = 0;
	for (auto driver : drivers)
	{
		if (driver.second.goodsType == 1)
		{
			Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
			expected_food_income += driven_train.goods_capacity;
		}
	}
	routeSeq best_way_to_storage = bestWayToStorage(begin, train);
	int safe_product_capacity =
		std::min((town.population + (2 * reg.wayLength(best_way_to_storage)) / 25), town.population_capacity) *
		2 * reg.wayLength(best_way_to_storage) + 2 * reg.wayLength(best_way_to_storage);
	if (best_way_to_storage.size() != 0 && _driver.goodsType != 1 && safe_product_capacity - expected_food_income <= 0)
	{
		_driver.goodsType = 2;
		return best_way_to_storage;
	}
	else
	{
		_driver.goodsType = 1;
		return bestWayToMarket(begin, train);
	}
}


int RoutePlaner::getPointIdxByLineAndPosition(Graph_Line line, int pos)
{
	if (pos == line.lenght) {
		return line.points.second;
	}
	else if (pos == 0) {
		return line.points.first;
	}
	else {
		return -1;
	}
}


routeSeq RoutePlaner::bestWayToHome(int begin, Train& train)
{
	Regulator reg;
	train.inMarket = false;
	train.longway = false;
	return reg.findWay(begin, Data_manager::getInstance().getPlayer().getTown().point_idx);
}


routeSeq RoutePlaner::bestWayToMarket(int begin, Train& train) {
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	for (auto market : Data_manager::getInstance().getMapLayer1().getMarkets()) {
		if (begin == market.second->point_idx) continue;
		routeSeq way = reg.findWay(begin, market.second->point_idx, 2);
		if (way.size() == 0) continue;
		int safe_product_capacity =
				std::min((town.population + (2 * reg.wayLength(way)) / 25), town.population_capacity) * 
				2 * reg.wayLength(way) + 2 * reg.wayLength(way);
		int possible_to_take = std::min(market.second->product, train.goods_capacity);
		if (safe_product_capacity - possible_to_take < bestDelta)
		{
			bestWay = way;
			bestDelta = safe_product_capacity - possible_to_take;
		}
	}

	return bestWay;
}


RoutePlaner::RoutePlaner()
{
}

RoutePlaner::~RoutePlaner()
{
}