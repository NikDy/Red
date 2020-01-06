#include "Decision.h"

Decision::Decision() {


}

Decision::~Decision() {


}

void Decision::Insertion() {
	Player& _player = Data_manager::getInstance().getPlayer();
	std::map<int, Train>& trains = _player.getTrains();
	for (auto train : trains) {
		int idx = train.second.getIdx();
		TrainDriver driver = TrainDriver(idx);
		if (driver.getRoute().isEmpty()) {
			RoutePlaner::getInstance().addDriver(idx, driver);
		}
	}
}

std::map<int, std::pair<int, int>> Decision:: makeTurn() {
	RoutePlaner::getInstance().buildRoutes();

	std::map<int, std::pair<int, int>> turn;
	for (auto& driver : RoutePlaner::getInstance().getDrivers()) {
		Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(driver.second.getIdx());
		if (train.cooldown != 0) 
		{ 
			driver.second.getRoute().path_seq.clear();
			driver.second.onWay = true;
			continue;
		}
		if (driver.second.foundSpeedNLine(driver.second) == false) continue;
		turn.emplace(driver.second.getIdx(), std::make_pair(driver.second.getSpeed(), driver.second.getLineToGo()));
		
	}
	return turn;

}