#include "RoutePlaner.h"



TrainDriver& RoutePlaner::getTrainDriverByIdx(int idx)
{
	for (auto& driver : drivers) {
		if (driver.second.getIdx() == idx) return driver.second;
	}
	return drivers.begin()->second;
}

Route RoutePlaner::getRouteByIdx(int idx)
{
	for (auto& driver : drivers) {
		if (driver.second.getIdx() == idx) return driver.second.getRoute();
	}
}

std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}


void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}


void RoutePlaner::makeTurn()
{
	resetTrainsLists();
	stageAffairs();
}

int RoutePlaner::countTrainsInPoint(int point)
{
	int count = 0;
	for (auto dr : drivers) {
		if (dr.second.postIdx == point) {
			++count;
		}
	}
	return count;
}


void RoutePlaner::stageAffairs()
{
	if (game_stage == 1)
	{
		upgradeIfPossible();
		resetRoutes();
		tryGoToSecondStage();
	}
	else if (game_stage == 2)
	{
		resetRoutes();
	}

}


void RoutePlaner::tryGoToSecondStage()
{
	if(Data_manager::getInstance().last_tick_ >server_tick_to_update || Data_manager::getInstance().tick > game_tick_to_update) game_stage = 2;
}


void RoutePlaner::tryGoToThirdStage()
{
	int top_trains = 0;
	for (auto driver : drivers)
	{
		if (Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx()).level == max_train_level) top_trains++;
	}
	if (top_trains == drivers.size() &&
		Data_manager::getInstance().getPlayer().getTown().level == max_town_level &&
		Data_manager::getInstance().getPlayer().getTown().armor >= 1000) game_stage = 3;
}



void RoutePlaner::resetRoutes()
{
	makeQueue();
	for (auto& driver : drivers)
	{
 	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (train.cooldown != 0)
		{
			Data_manager::getInstance().countOfCol++;
			driver.second.getRoute().path_seq.clear();
			driver.second.setStatus(true);
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
			if (driver.second.lastSpeed == driver.second.getSpeed() && driver.second.lastSpeed == 0) {
				driver.second.countOfWait += 1;
			}
			else {
				driver.second.lastSpeed = driver.second.getSpeed();
				driver.second.countOfWait = 0;
			}
			if (!(driver.second.last_ordered_line == driver.second.getLineToGo() && driver.second.last_ordered_speed == driver.second.getSpeed()))
			{
				if (Data_manager::getInstance().makeMove(driver.second.getIdx(), driver.second.getLineToGo(), driver.second.getSpeed()))
				{
					driver.second.last_ordered_line = driver.second.getLineToGo();
					driver.second.last_ordered_speed = driver.second.getSpeed();
				}
				else
				{
					driver.second.getRoute().path_seq.clear();
					driver.second.setStatus(true);
				}
			}
		}

	}

}

void RoutePlaner::makeQueue()
{
	for (auto& i : drivers) {
		if (i.second.getStatus() == true) continue;
		Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(i.second.getIdx());
		Graph_Line lineTrain = Data_manager::getInstance().getMapLayer0().getLineByIdx(train.line_idx);
		Graph_Point point = Data_manager::getInstance().getMapLayer0().getPoints()[i.second.getRoute().pathTop()];
		int trainToPoint = lengthToPoint(point, train);
		for (auto& j : drivers) {
			if (j.first <= i.first) continue;
			if (j.second.getStatus() == true) continue;
			Train tr = Data_manager::getInstance().getMapLayer1().getTrainByIdx(j.second.getIdx());
			Graph_Line lineTr = Data_manager::getInstance().getMapLayer0().getLineByIdx(tr.line_idx);
			Graph_Point pointTr = Data_manager::getInstance().getMapLayer0().getPoints()[j.second.getRoute().pathTop()];
			if ((tr.speed == 1 || tr.speed == 0) && tr.position == 0) {
				pointTr = Data_manager::getInstance().getMapLayer0().getPoints()[lineTr.points.first];
			}
			else if ((tr.speed == -1 || tr.speed == 0) && tr.position == lineTr.lenght) {
				pointTr = Data_manager::getInstance().getMapLayer0().getPoints()[lineTr.points.second];
			}
			if (pointTr.idx == point.idx) {
				int trToPoint = lengthToPoint(point, tr);
				if (trToPoint < trainToPoint) std::swap(i.second, j.second);
			}
		}
	}
}

int RoutePlaner::lengthToPoint(Graph_Point point, Train & train)
{
	Graph_Line trainLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(train.line_idx);
	int trainToPoint = -1;
	if (trainLine.points.second == point.idx) {
		trainToPoint = trainLine.lenght - train.position;
	}
	else if (trainLine.points.first == point.idx) {
		trainToPoint = train.position;
	}
	return trainToPoint;
}


std::vector<std::pair<int, int>> RoutePlaner::bestWayToStorage(int begin, Train & train)
{
	auto& trainDr = getTrainDriverByIdx(train.idx);
	auto town = Data_manager::getInstance().getPlayer().getTown();
	if (train.goods_type == 2) {
		return bestWayToHome(begin, train);
	}
	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	for (auto storage : Data_manager::getInstance().getMapLayer1().getStorages()) {
		if (begin == storage.second->point_idx) continue;
		routeSeq way = routeSeq();
		int count = countTrainsInPoint(storage.second->point_idx);
		if (count != 0)
		{
			if ((count + 1) * train.goods_capacity > storage.second->armor_capacity) continue;
		}
		if(train.goods_type != 3) way = reg.findWay(begin, storage.second->point_idx, train, 1);
		else way = reg.findWay(begin, storage.second->point_idx, train);
		if (way.size() == 0) continue;
		int safe_armor_capacity = 4 * reg.wayLength(way);
		int possible_to_take = std::min(storage.second->armor, train.goods_capacity);
		if (safe_armor_capacity - possible_to_take < bestDelta)
		{
			trainDr.postIdx = storage.second->point_idx;
			bestWay = way;
			bestDelta = safe_armor_capacity - possible_to_take;
		}
	}

	if (30 < reg.wayLength(bestWay) * 3 && train.goods != 0) // for this game. Very bad
	{
		trainDr.postIdx = begin;
		return routeSeq();
	}
	
	return bestWay;
}


void RoutePlaner::resetTrainsLists()
{
	products_drivers.clear();
	armor_drivers.clear();
	division_coef = std::stof(Data_manager::getInstance().config["stage_" + std::to_string(game_stage)]);
	max_town_level = std::stoi(Data_manager::getInstance().config["stage_" + std::to_string(game_stage) + "_town_max"]);
	int products_drivers_num = (int)(division_coef * drivers.size());
	for (auto driver : drivers)
	{
		if (driver.second.waitForOrder && products_drivers.size() < products_drivers_num)
		{
			products_drivers.emplace_back(driver.second);
		}
	}
	for (auto driver : drivers)
	{
		if (std::find(products_drivers.begin(), products_drivers.end(), driver.second) == products_drivers.end())
		{
			armor_drivers.emplace_back(driver.second);
		}
	}
}


bool RoutePlaner::buildRoutes(std::pair<const int, TrainDriver>& driver) {
	if (driver.second.getStatus()) {
		Train driven_train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (driven_train.cooldown != 0) {
			return false;
		}
		int route_start_point = getPointIdxByLineAndPosition(Data_manager::getInstance().getMapLayer0().getLineByIdx(driven_train.getLineIdx()),
			driven_train.getPosition());

		routeSeq way;
		if (driven_train.goods == driven_train.goods_capacity) {
			way = bestWayToHome(route_start_point, driven_train);
			driver.second.goodsType = 0;
		}
		else if (driven_train.goods < driven_train.goods_capacity)
		{
			if (std::find(products_drivers.begin(), products_drivers.end(), driver.second) == products_drivers.end())
			{
				way = bestWayToStorage(route_start_point, driven_train);
				driver.second.goodsType = 1;
			}
			else
			{
				way = bestWayToMarket(route_start_point, driven_train);
				driver.second.goodsType = 2;
			}
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
	train.inMarket = false;
	train.longway = false;
	getTrainDriverByIdx(train.idx).postIdx = -1;
	return reg.findWay(begin, Data_manager::getInstance().getPlayer().getTown().point_idx, train);
}


routeSeq RoutePlaner::bestWayToMarket(int begin, Train& train) {
	auto town = Data_manager::getInstance().getPlayer().getTown();

	routeSeq bestWay = routeSeq();
	int bestDelta = 10000000;
	if (train.goods_type == 3) {
		return bestWayToHome(begin, train);
	}
	auto& trainDr = getTrainDriverByIdx(train.idx);
	for (auto market : Data_manager::getInstance().getMapLayer1().getMarkets()) {
		if (begin == market.second->point_idx) continue;
		routeSeq way;
		int count = countTrainsInPoint(market.second->point_idx);
		if (count != 0)
		{
			if ((count + 1) * train.goods_capacity > market.second->product_capacity) continue;
		}
		if(train.goods_type != 2) way = reg.findWay(begin, market.second->point_idx, train, 2);
		else way = reg.findWay(begin, market.second->point_idx, train);
		if (way.size() == 0) continue;

		int possible_to_take = std::min(market.second->product, train.goods_capacity);
		int timeInPoint = (train.goods_capacity - possible_to_take) / (double)market.second->replenishment;
		int safe_product_capacity =
			(std::min((town.population + (2 * reg.wayLength(way)) / 20), town.population_capacity) + timeInPoint) *
			3 * reg.wayLength(way);
		if (safe_product_capacity - train.goods_capacity < bestDelta)
		{
			trainDr.postIdx = market.second->point_idx;
			bestWay = way;
			bestDelta = safe_product_capacity - train.goods_capacity;
		}
	}

	if (train.goods - train.goods_capacity < reg.wayLength(bestWay) && train.goods != 0)
	{
		trainDr.postIdx = begin;
		return routeSeq();
	}
	else
	{
		return bestWay;
	}
}


void RoutePlaner::upgradeIfPossible()
{
	auto& player = Data_manager::getInstance().getPlayer();

	int all_train_upgrade_cost = 0;
	for (auto train : player.getTrains()) 
	{
		if (train.second.level != 3) all_train_upgrade_cost += train.second.level * 40;
	}
	if (player.getTown().next_level_price <= player.getTown().armor &&
		player.getTown().next_level_price != 0) 
	{
		Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", player.getTown().idx), std::make_pair("trains", -1));
	}
	for (auto train : player.getTrains()) 
	{
		int point = getPointIdxByLineAndPosition(Data_manager::getInstance().getMapLayer0().getLineByIdx(train.second.getLineIdx()),
			train.second.getPosition());
		if (train.second.next_level_price <= player.getTown().armor &&
			train.second.next_level_price != 0 &&
			point == player.getHome().idx)
		{
			Data_manager::getInstance().tryUpgradeInGame(std::make_pair("posts", -1), std::make_pair("trains", train.second.idx));
			player.getTown().armor -= train.second.next_level_price;
		}
	}

}


RoutePlaner::RoutePlaner()
{
}

RoutePlaner::~RoutePlaner()
{
}

