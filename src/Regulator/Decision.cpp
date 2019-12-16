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
			std::cout << "I'm in insertion. Driver idx is " << driver.getIdx() << std::endl;
		}
	}
}

std::map<int, std::pair<int, int>> Decision:: makeTurn() {
	std::cout << "I'm inside MakeTurn" << std::endl;
	RoutePlaner::getInstance().buildRoutes();
	std::cout << "I'm after buildRoutes" << std::endl;
	auto drivers = RoutePlaner::getInstance().drivers;
	std::map<int, std::pair<int, int>> turn;
	std::cout << "I'm before last cycle in Decision makeTurn" << std::endl;
	for (auto driver : drivers) {
		std::cout << "I'm inside last cycle in Decision makeTurn" << std::endl;
		std::cout << "driver idx is " <<driver.second.getIdx() << std::endl;
		driver.second.foundSpeedNLine();
		std::cout << "his speed and line are " << driver.second.getSpeed()<<" and "<< driver.second.getLineToGo()<< std::endl;
		turn.emplace(driver.second.getLineToGo(), std::make_pair(driver.second.getSpeed(), driver.second.getIdx()));
		
	}
	return turn;

}