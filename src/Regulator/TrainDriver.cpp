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
	if (_status == true) wait = false;
	status = _status;
}



Route& TrainDriver::getRoute() {
	return route;
}

void TrainDriver::setRoute(Route _route) {
	route = _route;
}

bool TrainDriver::foundSpeedNLine() { //to found speedToSet
	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idx);
	int curLineIdx = train.getLineIdx();
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	auto& lines = Data_manager::getInstance().getMapLayer0().getLines();
	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(curLineIdx);
	int position = train.getPosition();

	//driver.getRoute().showRoute();
	
	if (getRoute().path_seq.size() == 0) {
		setStatus(true);
		return false;
	}

		if (getRoute().onePoint()) {
			if ((position == 1 && getSpeed() == -1) || (curLine.lenght - position == 1 && getSpeed() == 1)) {
				setStatus(true);
				getRoute().pathPop();
			} 
			else if ((position == 0 && getSpeed() == -1) || (curLine.lenght - position == 0 && getSpeed() == 1)) {
				setStatus(true);
				getRoute().pathPop();
			}
			else {
				checkAndSetRout();
			}
		}
		else if ((position == 0 || position == curLine.lenght) && wait == false) {
			int firstPoint = getRoute().pathTop();//first point of carrent route
			getRoute().pathPop();
			int secondPoint = getRoute().pathTop();//second point of current route

			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(firstPoint, secondPoint);
			setLineToGo(line.idx);
			
			setSpeed(Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(firstPoint, secondPoint));
			if (isNextLineInRouteAvailable(line, getSpeed()) == false || checkPoint(points[secondPoint], train) == false) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			} 
			else {
				lines[line.points].trains.push_back(train);
			}
			if (getRoute().onePoint()) {
				if (line.lenght == 1) {
					setStatus(true);
					getRoute().pathPop();
				}
			}
		}
		else {
			if (checkAndSetRout() == false) return false;
		}
		return true;
	//driver.getRoute().showRoute();
}



bool TrainDriver::isNextLineInRouteAvailable(Graph_Line line, int speed)
{
	for (auto tr : line.trains) {
		if (tr.speed != speed) {
			return false;
		}
		else {
			if (speed == 1 && tr.position < 3) {
				setSpeed(0);
				wait = true;
			}
			if (speed == -1 && line.lenght - tr.position < 3) {
				setSpeed(0);
				wait = true;
			}
		}
	}
	return true;
}

bool TrainDriver::checkLine(Graph_Line line, Train& train)
{
	for (auto tr : line.trains) {
		if (tr.idx != train.idx) {
			if (tr.speed != train.speed ) {
				return false;
			}
			else {
				if (tr.position > train.position && tr.speed == 1 && (tr.position - train.position) < 3) {
					setSpeed(0);
					wait = true;
					return false;
				}
				else if (train.position > tr.position && tr.speed == -1 && (train.position - tr.position) < 3) {
					setSpeed(0);
					wait = true;
					return false;
				}
			}
		}
	}
	return true;
}

bool TrainDriver::checkPoint(Graph_Point point, Train& train)
{
	for (auto tr : point.trains) {
		if (tr.idx != train.idx) {
			return false;
		}
	}
	return true;
}

bool TrainDriver::checkAndSetRout()
{
	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idx);
	int curLineIdx = train.getLineIdx();
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	auto& lines = Data_manager::getInstance().getMapLayer0().getLines();
	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(curLineIdx);
	int position = train.getPosition();
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());

	if (!checkPoint(points[getRoute().pathTop()], train)) {
		setSpeed(0);
		wait = true;
		auto& trains = points[getRoute().pathTop()].trains;
		auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
			if (tr.idx == train.idx)
				return true;
			return false;
		});
		if (it != trains.end()) trains.erase(it);
	}
	else if (!checkLine(line, train)) {
		int point;
		if (train.position == 0 || train.position == line.lenght) {
			getRoute().path_seq.clear();
			setStatus(true);
			return true;
		}
		if (getRoute().pathTop() == line.points.first) {
			point = line.points.second;
			setSpeed(1);
		}
		else {
			point = line.points.first;
			setSpeed(-1);
		}
		auto& trains = points[getRoute().pathTop()].trains;
		auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
			if (tr.idx == train.idx)
				return true;
			return false;
		});
		if (it != trains.end()) trains.erase(it);
		points[point].trains.push_back(train);
		getRoute().path_seq.clear();
		getRoute().path_seq.push_back(point);
	}
	else {
		wait = false;
		if (points[getRoute().pathTop()].idx == line.points.first) {
			setSpeed(-1);
		}
		else {
			setSpeed(1);
		}
	}
	return true;
}
