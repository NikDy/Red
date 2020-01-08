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
	
	if (getRoute().path_seq.size() == 0) {
		setStatus(true);
		return false;
	}

		if (getRoute().onePoint()) {
			if (checkAndSetRout(train) == false) {
				return false;
			}
			if ((position == 1 && getSpeed() == -1) || (curLine.lenght - position == 1 && getSpeed() == 1)) {
				setStatus(true);
				getRoute().pathPop();
			} 
			else if ((position == 0 && getSpeed() == -1) || (curLine.lenght - position == 0 && getSpeed() == 1)) {
				setStatus(true);
				getRoute().pathPop();
				return false;
			}
		}
		else if ((position == 0 || position == curLine.lenght) && wait == false) {
			int firstPoint = getRoute().pathTop();//first point of carrent route
			getRoute().pathPop();
			int secondPoint = getRoute().pathTop();//second point of current route

			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(firstPoint, secondPoint);
			setLineToGo(line.idx);
			
			setSpeed(Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(firstPoint, secondPoint));
			if (isNextLineInRouteAvailable(line, getSpeed()) == false || checkPoint(points[getRoute().pathTop()], train, line.lenght) == false) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			} 
			else {
				train.speed = getSpeed();
				train.line_idx = getLineToGo();
				if (getSpeed() == 1) train.position = 0;
				else train.position = line.lenght;
				lines[line.points].trains.push_back(train);
			}
			if (getRoute().onePoint()) {
				if (line.lenght == 1) {
					setStatus(true);
					getRoute().pathPop();
				}
			}
		}
		else if (checkAndSetRout(train) == false) {
			return false;
		}
		setNewDataForTrain(idx);
		setNewPoint(train);
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

bool TrainDriver::checkLine(Graph_Line line, Train& train)
{
	for (auto tr : line.trains) {
		if (tr.idx != train.idx) {
			if (tr.speed != train.speed && tr.speed != 0) {
				return false;
			}
			/*else {
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
			}*/
		}
	}
	return true;
}

bool TrainDriver::checkPoint(Graph_Point point, Train& train, int length)
{
	/*if ((getSpeed() == 1 && (length - train.position > 2)) || (getSpeed() == -1 && train.position > 2)) return true;
	Graph_Line trainLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(train.line_idx);
	int trainToPoint = 0;
	if (trainLine.points.second == point.idx) {
		trainToPoint = trainLine.lenght - train.position;
	}
	else if (trainLine.points.first == point.idx) {
		trainToPoint = train.position;
	}*/
	for (auto tr : point.trains) {
		if (tr.idx != train.idx && tr.line_idx != train.line_idx) {
			/*Graph_Line trLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(tr.line_idx);
			int trToPoint = 1000;
			if (tr.speed == 1) {
				trToPoint = trLine.lenght - tr.position;
			}
			else if(tr.speed == -1){
				trToPoint = tr.position;
			}
			if(trainToPoint >= trToPoint)	*/
				return false;
		}
	}
	return true;
}

bool TrainDriver::checkAndSetRout(Train& train)
{
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	int position = train.getPosition();
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
	int idxNearestTrain = nearestTrain(line, train);
		if(wait == true) {
			wait = false;
			if (points[getRoute().pathTop()].idx == line.points.first) {
				setSpeed(-1);
			}
			else {
				setSpeed(1);
			}
		}
	if (!checkPoint(points[getRoute().pathTop()], train, line.lenght)) {
		setSpeed(0);
		wait = true;
	}
	if (idxNearestTrain != -1) {
		Train nearestTrain = Data_manager::getInstance().getMapLayer1().getTrainByIdx(idxNearestTrain);
		if (nearestTrain.speed != getSpeed()) {
			int point;
			if (train.position == 0 || train.position == line.lenght) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			}
			if (nearestTrain.speed == 0 && nearestTrain.getPlayerIdx() == train.getPlayerIdx()) {
				setSpeed(0);
				wait = true;
				deleteTrainInPoint(train.idx);
				return true;
			}
			if (nearestTrain.speed == 1) {
				setSpeed(nearestTrain.speed);
				point = line.points.second;
			}
			else if (nearestTrain.speed == -1) {
				setSpeed(nearestTrain.speed);
				point = line.points.first;
			}
			else {
				if (train.position < nearestTrain.position) {
					point = line.points.first;
					setSpeed(-1);
				}
				else if (train.position > nearestTrain.position) {
					point = line.points.second;
					setSpeed(1);
				}
			}
			getRoute().path_seq.clear();
			getRoute().path_seq.push_back(point);
			
		}
	}
	deleteTrainInPoint(train.idx);
	return true;
}

int TrainDriver::nearestTrain(Graph_Line line, Train& train)
{
	Train nearesTrain = Train(train);
	int point = getRoute().pathTop();
	int speed;
	if (line.points.first == point) speed = -1;
	else speed = 1;
	if (getSpeed() == 1) {
		nearesTrain.position = 10000;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (tr.position > train.position && tr.position < nearesTrain.position) {
				nearesTrain = tr;
			}
		}
		if (nearesTrain.position == 10000) return -1;
	}
	else if (getSpeed() == -1) {
		nearesTrain.position = 0;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (tr.position < train.position && tr.position > nearesTrain.position) {
				nearesTrain = tr;
			}
		}
		if (nearesTrain.position == 0) return -1;
	}
	else {
		nearesTrain.position = -1;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (((speed == 1 && tr.position > train.position ) || (speed == -1 && tr.position < train.position)) 
				&& (fabs(tr.position - train.position) < fabs(train.position - nearesTrain.position))) {
				nearesTrain = tr;
			}
		}
		if (nearesTrain.position == -1) return -1;
	}
	return nearesTrain.idx;
}

void TrainDriver::deleteTrainInPoint(int trainIdx)
{
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	auto& trains = points[getRoute().pathTop()].trains;
	auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
		if (tr.idx == trainIdx)
			return true;
		return false;
	});
	if (it != trains.end()) trains.erase(it);
}

void TrainDriver::setNewDataForTrain(int trainIdx)
{
	auto& lines = Data_manager::getInstance().getMapLayer0().getLines();
	Graph_Line curLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
	auto& trains = lines[curLine.points].trains;
	auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
		if (tr.idx == trainIdx)
			return true;
		return false;
	});
	if (it != trains.end()) {
		it->line_idx = getLineToGo();
		it->speed = getSpeed();
	}
}

void TrainDriver::setNewPoint(Train train)
{
	auto& player = Data_manager::getInstance().getPlayer();
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
	if (getSpeed() == 1) {
		if (line.lenght - train.position <= 2 && line.points.second != player.getHome().idx) {
			points[line.points.second].trains.push_back(train);
		}
	}
	else if (getSpeed() == -1) {
		if (train.position <= 2 && line.points.first != player.getHome().idx) {
			points[line.points.first].trains.push_back(train);
		}
	}
	else if (getSpeed() == 0) {
		if (train.position == 0) {
			points[line.points.first].trains.push_back(train);
		}
		else if (train.position == line.lenght) {
			points[line.points.second].trains.push_back(train);
		}
	}
}
