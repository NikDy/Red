#include "RoutePlaner.h"




Route RoutePlaner::buildNewMarketRoute(int begin, Train& train) { //now is only for 1 market road without checking fuel and capacity

	std::vector<std::pair<int, int>> way = bestWayToMarket(begin, train);

	Route route;
	route.buildPathQueue(way);
	return route;
}

std::map<int, TrainDriver>& RoutePlaner::getDrivers() {
	return drivers;
}

/*Route RoutePlaner::buildNewStorageRoute(int begin, Train& train) { //for one storage without checking fuel and capasity
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

}*/

void RoutePlaner::addDriver(int _idx, TrainDriver _trainDriver) {
	 drivers.emplace(_idx, _trainDriver);
}

/*std::vector<std::pair<int, int>> RoutePlaner::pathToMarket()
{
	return std::vector<std::pair<int, int>>();
}*/

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
		inStorage = false;
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
		if (inStorage == true) {
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


		int productsFromStorage = actualProducts;

		if (town.product > necessaryProdacts) {
			maxProducts = productsFromStorage;
			bestPath = path;
		}
	}

	if (bestPath.size() == 0) {
		if (begin != town.point_idx) {
			inStorage = false;
			bestPath = reg.findWay(begin, town.point_idx);
		}
	}
	else {
		inStorage = true;
	}

	return bestPath;
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
			} // market or storage
			std::vector<std::pair<int, int>> way = std::vector<std::pair<int, int>>();
			if (longway == false) {
				if (Data_manager::getInstance().stopUpdate == false) {
					if(inMarket == false) way = bestWayToStorage(point, train);
					if (way.size() == 0) {
						if(inStorage == false) way = bestWayToMarket(point, train);
					}
				}
				else {
					way = bestWayToMarket(point, train);
				}
			}
			else {
				way = bestWayToMarket(point, train);
			}
			route.buildPathQueue(way);
			/*
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
			}*/
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


std::vector<std::pair<int, int>> RoutePlaner::bestWayToMarket(int begin, Train& train) {
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	auto& town = Data_manager::getInstance().getPlayer().getTown();
	Regulator reg;

	if (train.goods == train.goods_capacity) {
		inMarket = false;
		longway = false;
		return reg.findWay(begin, town.point_idx);
	}

	int populationInTownBeforeRoad = town.population;
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
		std::vector<std::pair<int, int>> path;
		if (inMarket == true) {
			path = reg.findWay(begin, _market.point_idx);
		}
		else {
			path = reg.findWay(begin, _market.point_idx, 2);
		}
		std::vector<std::pair<int, int>> pathToHome = reg.findWay(_market.point_idx, town.point_idx);
		int lengthToMarket = reg.wayLength(path);
		int lengthToHome = reg.wayLength(pathToHome);
		lengthPath = lengthToHome + lengthToMarket;
		int turnCount = 1;
		populationInTownThroughRoad = populationInTownBeforeRoad;

		if (longway == false) {
			necessaryProdacts = needProducts(lengthPath, populationInTownThroughRoad);
		}
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
		productsFromMarket = actualProducts - necessaryProdacts;


		if (productsFromMarket>maxProducts && town.product > necessaryProdacts) {
			maxProducts = productsFromMarket;
			bestPath = path;
		}
	}

	if(bestPath.size() == 0) {
		if (begin != town.point_idx) {
			bestPath = reg.findWay(begin, town.point_idx);
			inMarket = false;
		}
		else {
			longway = true;
			bestPath = bestWayToMarket(begin, train);
		}
	}
	else {
		inMarket = true;
	}

	return bestPath;
}

/*bool RoutePlaner::storageOrMarket(int begin, Train& train) { //true if market, false if storage 
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
	std::vector<std::pair<int, int>> pathToMarket = bestWayToMarket(begin, train); // need min way to market for his life 
	for (auto storage : storages) {
		Storage _storage = *storage.second;
		std::cout << "Inside storage cycle. storage point idx is " << _storage.point_idx << std::endl;
		std::vector<std::pair<int, int>> pathToStorage = reg.findWay(town.point_idx, _storage.point_idx, 1);
		lengthToStorage = reg.wayLength(pathToStorage) + reg.wayLength(pathToMarket);
		std::cout << " market: " << _storage.point_idx << " lengthToMarket: " << lengthToStorage << std::endl;
		wholePath += lengthToStorage * 2;
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
}*/


std::vector<std::pair<int, int>> RoutePlaner::StorageToMarket(int begin, Train& train, Town& town) {
	auto& markets = Data_manager::getInstance().getMapLayer1().getMarkets();
	Regulator reg;

	if (train.goods == train.goods_capacity) {
		longway = false;
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

		necessaryProdacts = needProducts(lengthPath, populationInTownThroughRoad) + 30; //just for fun. Need more products
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