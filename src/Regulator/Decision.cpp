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
		driver.second.foundSpeedNLine(driver.second);
		
		turn.emplace(driver.second.getLineToGo(), std::make_pair(driver.second.getSpeed(), driver.second.getIdx()));
		
	}
	return turn;

}