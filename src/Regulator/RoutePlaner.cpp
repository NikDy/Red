#include "RoutePlaner.h"




Route RoutePlaner::buildNewMarketRoute(int begin, Train& train) { //now is only for 1 market road without checking fuel and capacity
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
		lengthToMarket = reg.wayLength(path);
		std::cout << "lengthToMarket Is " << lengthToMarket << std::endl;
		int turnCount = 1;
		int necessaryProdacts = populationInTownBeforeRoad;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		std::cout << "population througt " << populationInTownThroughRoad << std::endl;
		for (turnCount; turnCount < 2*lengthToMarket ; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				std::cout << "inside plus 1" << std::endl;
			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				std::cout << "inside plus 2" << std::endl;

			}
		}
		std::cout << "products in markt are " << _market.product << std::endl;
		std::cout << "necessary products are " << necessaryProdacts << std::endl;
		int actualProducts = 0;

		if (_market.product_capacity <= _market.product + lengthToMarket) { //checking market capacity
			actualProducts = _market.product_capacity;
		}
		else {
			actualProducts = _market.product + lengthToMarket;
		}

		if (actualProducts > train.goods_capacity) {
			actualProducts = train.goods_capacity;
		}


		productsFromMarket = actualProducts - necessaryProdacts;
		std::cout << "products from Market are " << productsFromMarket << std::endl;

		if (productsFromMarket>maxProducts) {
			maxProducts = productsFromMarket;
			max = _market.point_idx;
		}
	}
	std::cout << "Outside market cycle. The best market poin idx is " << max << std::endl;
	std::vector<std::pair<int, int>> minPath = reg.findWay(begin, max);
	Route route;
	route.buildPathQueue(minPath);
	return route;
}

std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}

Route RoutePlaner::buildNewStorageRoute(int begin, Train& train) { //for one storage without checking fuel and capasity
	std::cout << "I'm inside buildStorageRoute" << std::endl;
	auto storages = Data_manager::getInstance().getMapLayer1().getStorages();
	auto markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;
	int productsInTown = town.product;
	std::cout << "products in town " << town.product << std::endl;
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
	std::cout << "I'm before storage cycle buildStorageRoute" << std::endl;
	for (auto storage : storages) {
		std::cout << "I'm in storage cycle buildStorageRoute" << std::endl;
		Storage _storage = *storage.second;
		std::vector<std::pair<int, int>> pathToStorage = reg.findWay(begin, _storage.point_idx, 1);
		lengthToStorage = reg.wayLength(pathToStorage);
		std::cout << " storage: " << _storage.point_idx << " lengthToStorage: " << lengthToStorage << std::endl;
		wholePath = lengthToStorage * 2;
		int necessaryProdacts = 0;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		
		for (int turnCount = 1; turnCount != wholePath; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				std::cout << "inside plus 1" << std::endl;
			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				std::cout << "inside plus 2" << std::endl;

			}
		}
		std::cout << "necessary products are " << necessaryProdacts << std::endl;
		difProducts = productsInTown - necessaryProdacts;
		armorFromStorage = _storage.armor;
		if (difProducts > 0 &&armorFromStorage>maxArmor) {
				maxArmor = armorFromStorage;
				max = _storage.point_idx;
		}
	}
	std::cout << "Outside storage cycle. The best storage point idx is " << max << std::endl;
	std::vector<std::pair<int, int>> minPath = reg.findWay(begin, max, 1);
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
	//auto _drivers = getInstance().getDrivers();
	
	for (auto& driver : getInstance().getDrivers()) {
		if (driver.second.getStatus()) {
			std::cout << "driver with true status idx: " << driver.first << std::endl;
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
			if (storageOrMarket(point, train)) {
				std::cout << "I'm inside MarketRoadBuilder" << std::endl;
				std::cout << "point to start idx is " <<point<< std::endl;
				route=buildNewMarketRoute(point, train);
				//std::cout << "first point of road is " <<route.pathTop() <<std::endl;
			}
			else {
				std::cout << "I'm inside StorageRoadBuilder" << std::endl;
				std::cout << "point to start idx is " << point << std::endl;
				route=buildNewStorageRoute(point, train);
			}
			driver.second.setStatus(false);
			driver.second.setRoute(route);
			//std::cout << "Show first point of driver road: " << driver.second.getRoute().pathTop()<<std::endl;			
		}
	}

}


RoutePlaner::RoutePlaner() {


}

RoutePlaner::~RoutePlaner(){


}


std::vector<std::pair<int, int>> RoutePlaner::bestWayToMarket(int begin, Train& train) { //now is only for 1 market road without checking fuel and capacity
	auto markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;
	std::cout << "First point idx of route is " << begin << std::endl;

	int productsInTown = town.product;
	int populationInTownBeforeRoad = town.population;
	int populationInTownThroughRoad;
	int max = 0;
	int maxProducts = 0; //max amount of products we can bring to home minus eaten products
	int productsFromMarket = 0;
	int lengthToMarket = 0;

	for (auto market : markets) {
		Market _market = *market.second;
		std::cout << "Inside market cycle. storage point idx is " << _market.point_idx << std::endl;
		std::vector<std::pair<int, int>> path = reg.findWay(begin, _market.point_idx);
		lengthToMarket = reg.wayLength(path);
		std::cout << "lengthToMarket Is " << lengthToMarket << std::endl;
		int turnCount = 1;
		int necessaryProdacts = populationInTownBeforeRoad;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		std::cout << "population througt " << populationInTownThroughRoad << std::endl;
		for (turnCount; turnCount < 2 * lengthToMarket; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				std::cout << "inside plus 1" << std::endl;
			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				std::cout << "inside plus 2" << std::endl;

			}
		}
		std::cout << "products in markt are " << _market.product << std::endl;
		std::cout << "necessary products are " << necessaryProdacts << std::endl;
		int actualProducts = 0;

		if (_market.product_capacity <= _market.product + lengthToMarket) { //checking market capacity
			actualProducts = _market.product_capacity;
		}
		else {
			actualProducts = _market.product + lengthToMarket;
		}

		if (actualProducts > train.goods_capacity) {
			actualProducts = train.goods_capacity;
		}


		productsFromMarket = actualProducts - necessaryProdacts;
		std::cout << "products from Market are " << productsFromMarket << std::endl;

		if (productsFromMarket>maxProducts) {
			maxProducts = productsFromMarket;
			max = _market.point_idx;
		}
	}
	std::cout << "Outside market cycle. The best market poin idx is " << max << std::endl;
	std::vector<std::pair<int, int>> minPath = reg.findWay(begin, max);
	return minPath;
}

bool RoutePlaner::storageOrMarket(int begin, Train& train) { //true if market, false if storage 
	Regulator reg;
	std::cout << "I'm inside storageOrMarket" << std::endl;
	auto& storages = Data_manager::getInstance().getMapLayer1().getStorages();
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto town = Data_manager::getInstance().getPlayer().getTown();
	std::cout << "Town point idx " << town.point_idx << std::endl;
	int productsInTown = town.product;
	std::cout << "productsInTown " << town.product << std::endl;
	int populationInTownBeforeRoad = town.population;
	std::cout << "populationInTownBeforeRoad " << town.population << std::endl;
	int populationInTownThroughRoad = 0;
	int difProducts = 0; //dif between eaten products? and products we can bring
	int lengthToStorage = 0;
	int lengthToMarket = 0;
	int wholePath = 0;
	std::vector<std::pair<int, int>> pathToMarket = bestWayToMarket(begin, train);
	for (auto storage : storages) {
		Storage _storage = *storage.second;
		std::cout << "Inside storage cycle. storage point idx is " << _storage.point_idx << std::endl;
		std::vector<std::pair<int, int>> pathToStorage = reg.findWay(town.point_idx, _storage.point_idx, 1);
		lengthToStorage = reg.wayLength(pathToStorage) + reg.wayLength(pathToMarket);
		std::cout << " market: " << _storage.point_idx << " lengthToMarket: " << lengthToStorage << std::endl;
		wholePath = lengthToStorage * 2;
		int turnCount = 1;
		int necessaryProdacts = 0;
		populationInTownThroughRoad = populationInTownBeforeRoad;
		for (turnCount; turnCount < wholePath; turnCount++) {
			necessaryProdacts += populationInTownThroughRoad;
			std::cout << turnCount << " necessaryProdacts are" << necessaryProdacts << std::endl;
			if ((turnCount % 30 == 0) && (turnCount % 60 != 0)) {
				populationInTownThroughRoad++;
				std::cout << "inside plus 1" << std::endl;

			}
			if (turnCount % 60 == 0) {
				populationInTownThroughRoad += 2;
				std::cout << "inside plus 2" << std::endl;

			}
		}
		difProducts = productsInTown - necessaryProdacts;
		std::cout << "necessary products are " << necessaryProdacts << std::endl;
		std::cout << "dif products are " << difProducts << std::endl;

		if (difProducts > 0) {
			return false;
		}
	}
	return true;
}