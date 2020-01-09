#include "RoutePlaner.h"



std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}


void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}



void RoutePlaner::makeTurn() {

	upgradeIfPossible();

	std::map<int, std::pair<int, int>> turn;
	for (auto& driver : drivers) {
		Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (train.cooldown != 0)
		{
			Data_manager::getInstance().countOfCol++;
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
		if (check == true) {
			if (!Data_manager::getInstance().makeMove(driver.second.getIdx(), driver.second.getLineToGo(), driver.second.getSpeed())) {
				driver.second.getRoute().path_seq.clear();
				driver.second.setStatus(true);
				driver.second.goodsType = 0;
			}
		}

	}

}



std::vector<std::pair<int, int>> RoutePlaner::bestWayToStorage(int begin, Train & train)
{
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	for (auto storage : Data_manager::getInstance().getMapLayer1().getStorages()) {
		if (begin == storage.second->point_idx) continue;
		routeSeq way;
		if(train.goods_type != 3) way = reg.findWay(begin, storage.second->point_idx, train, 1);
		else way = reg.findWay(begin, storage.second->point_idx, train);
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
			//std::cout << "Train: " << driven_train.idx << " collided" << std::endl;
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

	int expected_product_income = 0;
	int expected_armor_income = 0;
	for (auto driver : drivers)
	{
		if (driver.second.goodsType == 1)
		{
			Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
			expected_product_income += driven_train.goods_capacity;
		}
		else if (driver.second.goodsType == 2)
		{
			Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
			expected_armor_income += driven_train.goods_capacity;
		}
	}
	int possible_avaible_product = expected_product_income + town.product;
	routeSeq best_way_to_storage = bestWayToStorage(begin, train);
	routeSeq best_way_to_market = bestWayToMarket(begin, train);
	int way_to_storage_and_market = reg.wayLength(best_way_to_market) + reg.wayLength(best_way_to_storage);
	int safe_product_capacity =
		std::min((town.population + (2 * way_to_storage_and_market) / 25), town.population_capacity) *
		2 * way_to_storage_and_market + 2 * way_to_storage_and_market;
	int way_to_market_len = reg.wayLength(best_way_to_market);
	//if (((best_way_to_storage.size() != 0 && _driver.goodsType != 1 && safe_product_capacity - expected_food_income <= 0)
	//|| safe_product_capacity > town.product_capacity) && town.level != 3)
	if(((expected_armor_income + town.armor) < 220 && town.level < 3) 
		|| ((expected_armor_income + town.armor) < 80  && town.level == 3)
		|| train.goods_type == 3)
	{
		_driver.goodsType = 2;
		return best_way_to_storage;
	}
	else
	{
		_driver.goodsType = 1;
		return best_way_to_market;
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
	return reg.findWay(begin, Data_manager::getInstance().getPlayer().getTown().point_idx, train);
}


routeSeq RoutePlaner::bestWayToMarket(int begin, Train& train) {
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	for (auto market : Data_manager::getInstance().getMapLayer1().getMarkets()) {
		if (begin == market.second->point_idx) continue;
		routeSeq way;
		if(train.goods_type != 2) way = reg.findWay(begin, market.second->point_idx, train, 2);
		else way = reg.findWay(begin, market.second->point_idx, train);
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

	if (train.goods - train.goods_capacity < reg.wayLength(bestWay) && train.goods != 0) 
	{
		return bestWayToHome(begin, train);
	}
	else
	{
		return bestWay;
	}
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

	int all_train_upgrade_cost = 0;
	for (auto train : player.getTrains()) {
		if (train.second.level != 3) all_train_upgrade_cost += train.second.level * 40;
		int point = getPointIdxByLineAndPosition(Data_manager::getInstance().getMapLayer0().getLineByIdx(train.second.getLineIdx()),
			train.second.getPosition());
		if (train.second.next_level_price <= player.getTown().armor &&
			train.second.next_level_price != 0 &&
			point == player.getHome().idx &&
			reg.wayLength(bestWayToStorage(point, train.second)) * 2 < player.getTown().armor) { //multiply by 4 just for safe interval
			Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", -1), std::make_pair("trains", train.second.idx));
			player.getTown().armor -= train.second.next_level_price;
		}
	}
	if (player.getTown().next_level_price <= player.getTown().armor &&
		player.getTown().next_level_price != 0 && 
		(all_train_upgrade_cost > player.getTown().armor_capacity || all_train_upgrade_cost == 0)) {
		Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", player.getTown().idx), std::make_pair("trains", -1));
	}
}

