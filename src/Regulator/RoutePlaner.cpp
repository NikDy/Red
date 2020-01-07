#include "RoutePlaner.h"



std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}


void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}


int RoutePlaner::needProducts(int length, int &population)
{
	int cooldown = 30;
	int necessaryProdacts = 0;
	int number_of_ticks_from_last = Data_manager::getInstance().last_tick_Refuges;
	int countRef = Data_manager::getInstance().count_Refuges;
	for (int turnCount = number_of_ticks_from_last; turnCount < length + number_of_ticks_from_last; turnCount++) {
		necessaryProdacts += population;
		if ((turnCount % cooldown == 0)) {
			if(countRef * cooldown < turnCount)  ++population;
		}
	}
	return necessaryProdacts;
}


std::vector<std::pair<int, int>> RoutePlaner::bestWayToStorage(int begin, Train & train)
{
	auto storages = Data_manager::getInstance().getMapLayer1().getStorages();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	if (train.goods == train.goods_capacity) {
		train.inStorage = false;
		return reg.findWay(begin, town.point_idx);
	}

	int populationInTownBeforeRoad = town.population;
	int populationInTownThroughRoad;
	int productInTrain = 0;
	int maxProducts = 0;
	int productsFromStorage = 0;
	int idx_market = 0;
	int necessaryProdacts = 0;
	int lengthPath = 0;

	std::vector<std::pair<int, int>> bestPath = std::vector<std::pair<int, int>>();
	for (auto storage : storages) {
		Town town_in = town;
		Storage _storage = *storage.second;
		if (begin == _storage.point_idx) continue;
		std::vector<std::pair<int, int>> path;
		if (train.inStorage == true) {
			path = reg.findWay(begin, _storage.point_idx);
		}
		else {
			path = reg.findWay(begin, _storage.point_idx, 1);
		}
		std::vector<std::pair<int, int>> pathToHome = reg.findWay(_storage.point_idx, town.point_idx);
		int lengthToStorage = reg.wayLength(path);
		int lengthToHome = reg.wayLength(pathToHome);
		lengthPath = lengthToHome + lengthToStorage;
		int turnCount = 1;
		populationInTownThroughRoad = populationInTownBeforeRoad;

		necessaryProdacts = needProducts(lengthPath, populationInTownThroughRoad);
		town_in.population = populationInTownThroughRoad;
		if (town_in.product > necessaryProdacts) {
			town_in.product = town.product - necessaryProdacts;
		}
		if (StorageToMarket(begin, train, town_in).size() == 0) continue;

		int actualProducts = 0;

		if (_storage.armor_capacity <= _storage.armor + lengthToStorage * _storage.replenishment) {
			actualProducts = _storage.armor_capacity;
		}
		else {
			actualProducts = _storage.armor + lengthToStorage * _storage.replenishment;
		}

		if (actualProducts > (train.goods_capacity - train.goods)) {
			actualProducts = train.goods_capacity - train.goods;
		}



		if (town.product > necessaryProdacts && actualProducts > maxProducts) {
			maxProducts = actualProducts;
			bestPath = path;
		}
	}

	if (bestPath.size() == 0) {
		if (begin != town.point_idx) {
			train.inStorage = false;
			bestPath = reg.findWay(begin, town.point_idx);
		}
	}
	else {
		train.inStorage = true;
	}

	return bestPath;
}


bool RoutePlaner::buildRoutes(std::pair<const int, TrainDriver> &driver) {
	if (driver.second.getStatus()) {
		Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (driven_train.cooldown != 0){
			std::cout << "Train: " << driven_train.idx << "collided" << std::endl;
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
			way = bestWayToMarket(route_start_point, driven_train);
		}
		if (way.size() == 0) return false;
		
		driver.second.setStatus(false);
		driver.second.setRoute(Route(way));
	}
	return true;
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

RoutePlaner::RoutePlaner() {


}

RoutePlaner::~RoutePlaner(){


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

	int maxProducts = 0;
	routeSeq bestWay = routeSeq();
	int bestStock = 10000000;
	for (auto market : Data_manager::getInstance().getMapLayer1().getMarkets()) {
		if (begin == market.second->point_idx) continue;
		routeSeq way;
		if (train.inMarket == false) {
			way = reg.findWay(begin, market.second->point_idx);
		}
		else {
			way = reg.findWay(begin, market.second->point_idx, 2);
		}
		if (way.size() == 0) continue;
		int safe_product_capacity =
				std::min((town.population + (2 * reg.wayLength(way)) / 25), town.population_capacity) * 
				2 * reg.wayLength(way) + 2 * reg.wayLength(way);
		int possible_to_take = std::min(market.second->product, train.goods_capacity);
		if (safe_product_capacity - possible_to_take < bestStock)
		{
			bestWay = way;
			bestStock = safe_product_capacity - possible_to_take;
		}
	}

	return bestWay;
}


std::vector<std::pair<int, int>> RoutePlaner::StorageToMarket(int begin, Train& train, Town& town) {
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	Regulator reg;

	if (train.goods == train.goods_capacity) {
		return reg.findWay(begin, town.point_idx);
	}

	int populationInTownBeforeRoad = town.population;
	int bestLength = 100000;
	int populationInTownThroughRoad;
	int productInTrain = 0;
	int maxProducts = 0;
	int productsFromMarket = 0;
	int idx_market = 0;
	int necessaryProdacts = 0;
	int lengthPath = 0;
	std::vector<std::pair<int, int>> bestPath = std::vector<std::pair<int, int>>();
	for (auto market : markets) {
		Market _market = *market.second;
		if (begin == _market.point_idx) continue;
		std::vector<std::pair<int, int>> path = reg.findWay(begin, _market.point_idx, 2);
		std::vector<std::pair<int, int>> pathToHome = reg.findWay(_market.point_idx, town.point_idx);
		int lengthToMarket = reg.wayLength(path);
		int lengthToHome = reg.wayLength(pathToHome);
		lengthPath = lengthToHome + lengthToMarket;
		int turnCount = 1;
		populationInTownThroughRoad = populationInTownBeforeRoad;

		necessaryProdacts = needProducts(lengthPath, populationInTownThroughRoad) + populationInTownThroughRoad * 10; //populationInTownThroughRoad * 10 just for fun. Need more products
		int actualProducts = 0;

		if (_market.product_capacity <= _market.product + lengthToMarket * _market.replenishment) {
			actualProducts = _market.product_capacity;
		}

		else {
			actualProducts = _market.product + lengthToMarket * _market.replenishment;
		}

		if (actualProducts > (train.goods_capacity - train.goods)) {
			actualProducts = train.goods_capacity - train.goods;
		}


		//productsFromMarket = actualProducts - necessaryProdacts;

		if (town.product > necessaryProdacts) {
			if (lengthPath < bestLength) {
				bestLength = lengthPath;
				maxProducts = productsFromMarket;
				bestPath = path;
			}
		}
	}

	return bestPath;
}