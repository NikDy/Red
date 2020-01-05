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

bool TrainDriver::foundSpeedNLine(TrainDriver& driver) { //to found speedToSet
	Train train = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idx);
	int curLineIdx = train.getLineIdx();
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	auto& lines = Data_manager::getInstance().getMapLayer0().getLines();
	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(curLineIdx);
	int position = train.getPosition();

	//driver.getRoute().showRoute();
	
	if (driver.getRoute().path_seq.size() == 0) {
		setStatus(true);
		return false;
	}

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
			
			setSpeed(Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(firstPoint, secondPoint));
			if (isNextLineInRouteAvailable(line, getSpeed()) == false || points[secondPoint].trains.size() != 1) {
				driver.getRoute().path_seq.clear();
				setStatus(true);
				return false;
			} 
			else {
				lines[line.points].trains.push_back(train);
				points[secondPoint].trains.push_back(train);
			}
			if (driver.getRoute().onePoint()) {
				if (line.lenght == 1) {
					setStatus(true);
					driver.getRoute().pathPop();
				}
			}
		}
		else {
			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
			if (!checkPoint(points[driver.getRoute().pathTop()], train)) {
				setSpeed(0);
				auto& trains = points[driver.getRoute().pathTop()].trains;
				auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
					if (tr.idx == train.idx)
						return true;
					return false;
				});
				if(it != trains.end()) trains.erase(it);
			}
			else if (!checkLine(line, train)) {
				int point;
				if (driver.getRoute().pathTop() == line.points.first) {
					point = line.points.second;
					setSpeed(1);

				}
				else {
					point = line.points.first;
					setSpeed(-1);
				}
				auto& trains = points[driver.getRoute().pathTop()].trains;
				auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
					if (tr.idx == train.idx)
						return true;
					return false;
				});
				if (it != trains.end()) trains.erase(it);
				points[point].trains.push_back(train);
				driver.getRoute().path_seq.clear();
				driver.getRoute().path_seq.push_back(point);
			}
			else {
				if (points[driver.getRoute().pathTop()].idx == line.points.first) {
					setSpeed(-1);
				}
				else {
					setSpeed(1);
				}
			}
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
	}
	return true;
}

bool TrainDriver::checkLine(Graph_Line line, Train train)
{
	for (auto tr : line.trains) {
		if (tr.idx != train.idx) {
			if (tr.speed != train.speed) {
				return false;
			}
		}
	}
	return true;
}

bool TrainDriver::checkPoint(Graph_Point point, Train train)
{
	for (auto tr : point.trains) {
		if (tr.idx != train.idx) {
			return false;
		}
	}
	return true;
}
