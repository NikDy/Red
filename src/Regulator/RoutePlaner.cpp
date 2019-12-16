#include "RoutePlaner.h"




Route RoutePlaner::buildNewMarketRoute(int begin, Train&) { //now is only for 1 market road without checking fuel and capacity
	auto markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;
	std::cout << "First point idx of route is " << begin << std::endl;

	int productsInTown = town.product;
	int populationInTownBeforeRoad = town.population;
	int populationInTownThroughRoad;
	int max = 0;
	int maxProducts=0; //max amount of products we can bring to home minus eaten products
	int productsFromMarket = 0;
	int lengthToMarket = 0;

	for (auto market : markets) {
		Market _market = *market.second;
		std::cout << "Inside market cycle. storage point idx is " << _market.point_idx << std::endl;
		std::vector<std::pair<int, int>> path = reg.findWay(begin, _market.point_idx);
		lengthToMarket =reg.wayLength(path);
		//std::cout << "lengthToMarket Is " << lengthToMarket << std::endl;
		int turnCount = 1;
		int necessaryProdacts = populationInTownBeforeRoad;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		//std::cout << "population througt " << populationInTownThroughRoad << std::endl;
		for (turnCount; turnCount < 2*lengthToMarket ; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			//std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				//std::cout << "inside plus 1" << std::endl;
			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				//std::cout << "inside plus 2" << std::endl;

			}
		}
		//std::cout << "products in markt are " << _market.product << std::endl;
		//std::cout << "necessary products are " << necessaryProdacts << std::endl;
		productsFromMarket = _market.product - necessaryProdacts;
		//std::cout << "products from Market are " << productsFromMarket << std::endl;

		if (productsFromMarket>maxProducts) {
			maxProducts = productsFromMarket;
			max = _market->point_idx;
		}
	}
	std::cout << "Outside market cycle. The best market poin idx is " << max << std::endl;
	std::vector<std::pair<int, int>> minPath = reg.findWay(begin, max);
	Route route;
	route.buildPathQueue(minPath);
	return route;
}

Route RoutePlaner::buildNewStorageRoute(int begin, Train&) { //for one storage without checking fuel and capasity
	auto& storages = Data_manager::getInstance().getMapLayer1().getStorages();
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;
	int productsInTown = town.product;
	int populationInTownBeforeRoad = town.population;
	int populationInTownThroughRoad;
	int max = 0;
	int maxArmor = 0;
	int difProducts = 0; //dif between eaten products? and products we can bring
	int plusProducts = 0;//products to increase products in town
	bool canWeGo = false;//can we go or no to those storage and market and our food will nnot be empty after
	int _point = 0;
	int armorFromStorage = 0;
	int lengthToStorage = 0;
	int lengthToMarket = 0;
	int wholePath = 0;
	for (auto storage : storages) {
		Storage _storage = *storage.second;
		std::vector<std::pair<int, int>> pathToStorage = reg.findWay(begin, _storage.point_idx);
		lengthToStorage = reg.wayLength(pathToStorage);
		for (auto market : markets) {
			Market _market = *market.second;
			std::vector<std::pair<int, int>> pathToMarket = reg.findWay(begin, _market.point_idx);
			lengthToMarket = reg.wayLength(pathToMarket);
			wholePath = lengthToMarket * 4 + lengthToStorage * 2;
			int turnCount = 0;
			int necessaryProdacts = 0;
			populationInTownThroughRoad = populationInTownBeforeRoad;
			for (turnCount; turnCount < wholePath; turnCount++) {
				necessaryProdacts += populationInTownThroughRoad;
				if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
					populationInTownThroughRoad++;
				}
				if (turnCount % 60 == 0) {
					populationInTownThroughRoad += 2;
				}
			}
			difProducts = productsInTown + _market.product - necessaryProdacts;
			plusProducts = _market.product - (lengthToMarket * 2 *populationInTownThroughRoad);//нужно посчитать отдельно кол-во возможно съеденной еды (todo) пока так
			if (difProducts > 0 && plusProducts > 0) {
				canWeGo = true;
				break;
			}
		}

		if (armorFromStorage > maxArmor) {
			maxArmor = armorFromStorage;
			max = _storage.point_idx;
		}
	}
	std::vector<std::pair<int, int>> minPath = reg.findWay(begin, max);
	Route route;
	route.buildPathQueue(minPath);
	return route;

}

void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}

void RoutePlaner::buildRoutes() {
	std::cout << "I'm inside buildRoutes" << std::endl;
	Regulator  reg;
	for (auto driver : drivers) {
		if (driver.second.getStatus()) {
			Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
			//std::cout << "Train idx is " <<train.getIdx()<< std::endl;
			int point = 0;
			int lineIdx = train.getLineIdx();
			//std::cout << "Current line idx is " << train.getLineIdx() << std::endl;
			int position = train.getPosition();
			//std::cout << "Current Position is " << train.getPosition() << std::endl;
			Route route;
			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(lineIdx);
			//std::cout << "Current Line length is " << line.lenght << std::endl;
			if (position == line.lenght) {
				point = line.points.second;
			}
			else {
				point = line.points.first;
			}
			if (reg.storageOrMarket()) {
				std::cout << "I'm inside MarketRoadBuilder" << std::endl;
				route=buildNewMarketRoute(point, train);
				//std::cout << "first point of road is " <<route.pathTop() <<std::endl;
			}
			else {
				std::cout << "I'm inside StorageRoadBuilder" << std::endl;
				route=buildNewStorageRoute(point, train);
			}
			driver.second.setStatus(false);
			driver.second.setRoute(route);
			//std::cout << "Show first point of driver road: " << driver.second.getRoute().pathTop()<<std::endl;
			driver.second.foundSpeedNLine(driver.second);
			std::cout << "his speed and line are (inside routeBuilder) " << driver.second.getSpeed() << " and " << driver.second.getLineToGo() << std::endl;
		}
	}

}


RoutePlaner::RoutePlaner() {


}

RoutePlaner::~RoutePlaner(){


}