#include "Decision.h"

Decision::Decision() {


}

Decision::~Decision() {


}



std::map<int, std::pair<int, int>> Decision:: makeTurn() {

	std::map<int, std::pair<int, int>> turn;
	for (auto& driver : RoutePlaner::getInstance().getDrivers()) {
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
			if (RoutePlaner::getInstance().buildRoutes(driver) == false) {
				check = false;
				break;
			}
			if (driver.second.foundSpeedNLine() == true) {
				check = true;
				break;
			}
		}
		if(check = true) turn.emplace(driver.second.getIdx(), std::make_pair(driver.second.getSpeed(), driver.second.getLineToGo()));	
	}
	return turn;

}