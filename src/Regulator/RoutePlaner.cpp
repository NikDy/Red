#include "RoutePlaner.h"



std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}


void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}



std::map<int, std::pair<int, int>> RoutePlaner::makeTurn() {

	upgradeIfPossible();

	std::map<int, std::pair<int, int>> turn;
	for (auto& driver : drivers) {
		Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (train.cooldown != 0)
		{
			driver.second.getRoute().path_seq.clear();
			driver.second.setStatus(true);
			driver.second.goodsType = 0;
			continue;
		}
		bool check = false;
		while (true) {
			if (buildRoutes(driver) == false) {
				check = false;
				break;
			}
			if (driver.second.foundSpeedNLine() == true) {
				check = true;
				break;
			}
		}
		if (check == true) turn.emplace(driver.second.getIdx(), std::make_pair(driver.second.getSpeed(), driver.second.getLineToGo()));
	}
	return turn;

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


void RoutePlaner::loadDrivers()
{
	for (auto train : Data_manager::getInstance().getPlayer().getTrains()) {
		int idx = train.second.getIdx();
		TrainDriver driver = TrainDriver(idx);
		RoutePlaner::getInstance().addDriver(idx, driver);
	}
}


routeSeq RoutePlaner::wayToMostActualPost(int begin, TrainDriver& _driver)
{
	auto town = Data_manager::getInstance().getPlayer().getTown();
	auto train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(_driver.getIdx());
	Regulator reg;

	int expected_food_income = town.product;
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

	if (((best_way_to_storage.size() != 0 && _driver.goodsType != 1 && safe_product_capacity - expected_food_income <= 0)
		|| safe_product_capacity > town.product_capacity) && town.level != 3)
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

void RoutePlaner::upgradeIfPossible()
{
	auto& player = Data_manager::getInstance().getPlayer();
	int armor_in_towm = player.getTown().armor;

	for (auto train : player.getTrains()) {
		int point = getPointIdxByLineAndPosition(Data_manager::getInstance().getMapLayer0().getLineByIdx(train.second.getLineIdx()),
			train.second.getPosition());
		if (train.second.next_level_price <= player.getTown().armor && train.second.next_level_price != 0 && point == player.getHome().idx) {
			Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", -1), std::make_pair("trains", train.second.idx));
			player.getTown().armor -= train.second.next_level_price;
		}
	}
	if (player.getTown().next_level_price <= player.getTown().armor && player.getTown().next_level_price != 0) {
		Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", player.getTown().idx), std::make_pair("trains", -1));
	}
}

