#include "TrainDriver.h"

TrainDriver::TrainDriver(int _idx) {
	idx = _idx;
}


TrainDriver::~TrainDriver() {

}

int TrainDriver::getIdx() {
	return idx;
}

void TrainDriver::setIdx(int _idx) {
	idx = _idx;
}

int TrainDriver::getSpeed() {
	return speed;
}

void TrainDriver::setSpeed(int _speed) {
	speed = _speed;
}

int TrainDriver::getLineToGo() {
	return lineToGo;
}

void TrainDriver::setLineToGo(int _lineToGo) {
	lineToGo = _lineToGo;
}

bool TrainDriver::getStatus() {
	return status;
}

void TrainDriver::setStatus(bool _status) {
	status = _status;
}

Route TrainDriver::getRoute() {
	return route;
}

void TrainDriver::setRoute(Route _route) {
	route = _route;
}

void TrainDriver::foundSpeedNLine( ) { //to found speedToSet
	std::cout << "I'm inside foundSpeedNLine" << std::endl;
	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idx);
	int curLineIdx = train.getLineIdx();
	std::cout << "Cur Line Idx is " <<curLineIdx<< std::endl;

	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(curLineIdx);
	int position = train.getPosition();
	std::cout << "Train Position is " << position << std::endl;
	if (position == 0 || position == curLine.lenght) {
		if (route.onePoint()) {
			std::cout << "Only one point in route" << std::endl;
			route.pathPop();
			setStatus(true);
			return;
		}

		int firstPoint = (route.pathTop());//first point of carrent route
		std::cout << "i'm after pathTop() " << std::endl;
		std::cout << "first point of the route is " << firstPoint << std::endl;
		route.pathPop();
		std::cout << "i'm after pathPop() "  << std::endl;

		int secondPoint = route.pathTop();//second point of current route
		std::cout << "second point of the route is " << secondPoint << std::endl;

		Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(firstPoint, secondPoint);
		setLineToGo(line.idx);
		if (firstPoint == line.points.first) {
			setSpeed(1);
		}
		else {
			setSpeed(-1);
		}
	}
}
