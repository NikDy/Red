#include "TrainDriver.h"

TrainDriver::TrainDriver(int _idx) {
	idx = _idx;
}


TrainDriver::~TrainDriver() {

}

int& TrainDriver::getIdx() {
	return idx;
}

void TrainDriver::setIdx(int _idx) {
	idx = _idx;
}

int& TrainDriver::getSpeed() {
	return speed;
}

void TrainDriver::setSpeed(int _speed) {
	speed = _speed;
}

int& TrainDriver::getLineToGo() {
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

Route& TrainDriver::getRoute() {
	return route;
}

void TrainDriver::setRoute(Route _route) {
	route = _route;
}

void TrainDriver::foundSpeedNLine(TrainDriver& driver) { //to found speedToSet
	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idx);
	int curLineIdx = train.getLineIdx();

	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(curLineIdx);
	int position = train.getPosition();

	//driver.getRoute().showRoute();

	if (driver.getRoute().onePoint()) {
		if (position == 1 && driver.getSpeed() == -1) {
			setStatus(true);
			driver.getRoute().pathPop();
		}
		if (curLine.lenght - position == 1 && driver.getSpeed() == 1) {
			setStatus(true);
			driver.getRoute().pathPop();
		}
	}
	else if (position == 0 || position == curLine.lenght) {
		int firstPoint = driver.getRoute().pathTop();//first point of carrent route
		driver.getRoute().pathPop();
		int secondPoint = driver.getRoute().pathTop();//second point of current route

		Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(firstPoint, secondPoint);
		setLineToGo(line.idx);
		if (firstPoint == line.points.first) {
			setSpeed(1);
		}
		else {
			setSpeed(-1);
		}
		if (driver.getRoute().onePoint()) {
			if (line.lenght == 1) {
				setStatus(true);
				driver.getRoute().pathPop();
			}
		}
	}
	//driver.getRoute().showRoute();

}
