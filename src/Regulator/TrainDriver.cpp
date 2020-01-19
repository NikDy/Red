#include "TrainDriver.h"
#include "RoutePlaner.h"

TrainDriver::TrainDriver()
{
}

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
	int homeIdx = Data_manager::getInstance().getPlayer().getHome().idx;
	int position = train.getPosition();
	
	if (getRoute().path_seq.size() == 0) {
		setStatus(true);
		return false;
	}
	else if (train.goods == train.goods_capacity && getRoute().path_seq.back() != homeIdx && (position == 0 || position == curLine.lenght)) {
		getRoute().path_seq.clear();
		setStatus(true);
		return false;
	}

	if (getRoute().onePoint() && ((position == 0 && getSpeed() == -1) || (curLine.lenght == position && getSpeed() == 1))) {
			setStatus(true);
			getRoute().pathPop();
			return false;

	}
	else if ((position == 0 || position == curLine.lenght)) {
		if (wait == false) {
			int firstPoint = getRoute().pathTop();//first point of carrent route
			getRoute().pathPop();
			int secondPoint = getRoute().pathTop();//second point of current route

			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(firstPoint, secondPoint);
			setLineToGo(line.idx);

			setSpeed(Data_manager::getInstance().getMapLayer0().getLineDirectionByTwoPoints(firstPoint, secondPoint));

			train.speed = getSpeed();
			train.line_idx = getLineToGo();
			if (getSpeed() == 1) train.position = 0;
			else train.position = line.lenght;
		
			if (checkNextPoint(points[getRoute().pathTop()], train, line) == false || isNextLineInRouteAvailable(line, train) == false) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			} 
			else {
				if (getSpeed() == 0) {
					wait = true;
					train.speed = getSpeed();
				}
				else {
					deleteTrainInPoint(idx, firstPoint);
					train.position += getSpeed() * 1;
					points[secondPoint].trains.push_back(train);
					lines[line.points].trains.push_back(train);
				}
			}
		}
		else {
			wait = false;
			Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
			if (points[getRoute().pathTop()].idx == line.points.first) {
				setSpeed(-1);
			}
			else {
				setSpeed(1);
			}
			if (getSpeed() == 1) train.position = 0;
			else train.position = line.lenght;
			if (countOfWait > 4) {
				if (getSpeed() == 1) deleteTrainInPoint(idx, line.points.first);
				else deleteTrainInPoint(idx, line.points.second);
				train.position = getSpeed() * 1;
				if (Data_manager::getInstance().isTown(getRoute().pathTop()) == false) points[getRoute().pathTop()].trains.push_back(train);
				lines[line.points].trains.push_back(train);
				return true;
			}
			train.speed = getSpeed();
			if (checkNextPoint(points[getRoute().pathTop()], train, line) == false || isNextLineInRouteAvailable(line, train) == false) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			}
			else {
				if (getSpeed() == 0) {
					wait = true;
					train.speed = getSpeed();
				}
				else {
					if (getSpeed() == 1) deleteTrainInPoint(idx, line.points.first);
					else deleteTrainInPoint(idx, line.points.second);
					train.position += getSpeed() * 1;
					if (Data_manager::getInstance().isTown(getRoute().pathTop()) == false) points[getRoute().pathTop()].trains.push_back(train);
					lines[line.points].trains.push_back(train);
				}
			}
		}
	}
	else if (checkAndSetRout(train) == false) {
		return false;
	}
	else setNewDataForTrain(train.idx);
	
	return true;
}



bool TrainDriver::isNextLineInRouteAvailable(Graph_Line line, Train& train)
{
	Train nearTrain = nearestTrain(line, train);
	if (nearTrain.idx != train.idx)
	{
		if (nearTrain.getPlayerIdx() == train.getPlayerIdx() && nearTrain.speed == 0) {
			int pointNow = 0;
			if (train.speed == -1) pointNow = line.points.second;
			else pointNow = line.points.first;
			Route dri = RoutePlaner::getInstance().getRouteByIdx(nearTrain.idx);
			if (dri.path_seq.size() >= 1) {
				if (pointNow == dri.pathTop()) return false;
			}
			if (countOfWait > 2) return false;
			else setSpeed(0);
		}
		else if (nearTrain.speed != train.speed) return false;
		else {
			if (train.speed == 1 && nearTrain.position < 3) setSpeed(0);
			if (train.speed == -1 && line.lenght - nearTrain.position < 3) setSpeed(0);
		}
	}
	return true;
}


bool TrainDriver::checkPoint(Graph_Point point, Train& train, Graph_Line line)
{
	int trainToPoint = lengthToPoint(point, train);
	int pointNow = 0;
	if (point.idx == line.points.first) pointNow = line.points.second;
	else pointNow = line.points.first;
	for (auto tr : point.trains) {
		if (tr.idx == train.idx) continue;
		if (tr.line_idx == line.idx && tr.speed == train.speed) continue;
		else {
			int trToPoint = lengthToPoint(point, tr);
			if (trainToPoint < trToPoint) continue;
			if (tr.getPlayerIdx() == train.getPlayerIdx() && trainToPoint >= trToPoint && trainToPoint > 2) {
				Route dri = RoutePlaner::getInstance().getRouteByIdx(tr.idx);
				if (dri.path_seq.size() >= 2) {
					if (dri.path_seq[1] != pointNow && trainToPoint != 1) continue;
				}
				else if (dri.path_seq.size() >= 1) {
					if (dri.path_seq[0] != pointNow && trainToPoint != 1) continue;
				}
			}
			//if (trToPoint == 0 && trainToPoint == 1 && tr.getPlayerIdx() != train.getPlayerIdx()) setSpeed(getSpeed()*-1);
			else setSpeed(0);
		}
	}
	return true;
}

bool TrainDriver::checkNextPoint(Graph_Point point, Train& train, Graph_Line line)
{
	int trainToPoint = line.lenght;
	int pointNow = 0;
	int homeIdx = Data_manager::getInstance().getPlayer().getHome().idx;
	if (point.idx == line.points.first) pointNow = line.points.second;
	else pointNow = line.points.first;
	for (auto tr : point.trains) {
		if (tr.idx == train.idx) continue;
		if (tr.line_idx == line.idx && tr.speed == train.speed) continue;
		else {
			int trToPoint = lengthToPoint(point, tr);
			/*if (trainToPoint < trToPoint) {
				continue;
			}*/
			if (tr.getPlayerIdx() == train.getPlayerIdx() && trToPoint <= trainToPoint && trainToPoint > 2) {
				Route dri = RoutePlaner::getInstance().getRouteByIdx(tr.idx);
				if (dri.path_seq.size() == 1) {
					if (dri.path_seq[0] == pointNow) {
						return false;
					}
					else continue;
				}
				else if (dri.path_seq.size() >= 2) {
					if (dri.path_seq[0] == pointNow || dri.path_seq[1] == pointNow) {
						Regulator reg;
						TrainDriver trr = RoutePlaner::getInstance().getTrainDriverByIdx(tr.idx);
						int trNewLength = reg.wayLength(reg.findWay(dri.path_seq.front(), dri.path_seq.back(), tr, trr.goodsType));
						if (trr.getRoute().route_lenght >= trNewLength) continue;
						return false;
					}
					else continue;
				}
				else continue;
			}
			if (tr.getPlayerIdx() != train.getPlayerIdx() && trToPoint == -1 && line.lenght <= 2) setSpeed(0);
			else if (trToPoint == 0 && line.lenght != 1 && train.getPlayerIdx() != tr.getPlayerIdx()) setSpeed(0);
			else if (trainToPoint >= trToPoint) return false;
		}
	}

	return true;
}

bool TrainDriver::checkAndSetRout(Train& train)
{
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	int position = train.getPosition();
	Graph_Line line = Data_manager::getInstance().getMapLayer0().getLineByIdx(getLineToGo());
	if(wait == true) {
		wait = false;
		if (points[getRoute().pathTop()].idx == line.points.first) {
			setSpeed(-1);
		}
		else {
			setSpeed(1);
		}
		if (countOfWait > 4) {
			return true;
		}
	}
	int needSpeed = getSpeed();
	train.speed = needSpeed;
	Train nearTrain = nearestTrain(line, train);
	checkPoint(points[getRoute().pathTop()], train, line);
	if (nearTrain.idx != train.idx) {
		if (nearTrain.speed != train.speed) {
			int point = getRoute().pathTop();
			if (train.position == 0 || train.position == line.lenght) {
				getRoute().path_seq.clear();
				setStatus(true);
				return false;
			}
			if (nearTrain.speed == 0 && nearTrain.getPlayerIdx() == train.getPlayerIdx()) {
				setSpeed(0);
				wait = true;
				deleteTrainInPoint(train.idx, getRoute().pathTop());
				return true;
			}
			if (nearTrain.speed == 1) {
				train.speed = nearTrain.speed;
				setSpeed(nearTrain.speed);
			}
			else if (nearTrain.speed == -1) {
				train.speed = nearTrain.speed;
				setSpeed(nearTrain.speed);
			}
		}
	}
	if (getSpeed() != needSpeed) {
		deleteTrainInPoint(train.idx, getRoute().pathTop());
		if (getSpeed() == 0) wait = true;
	}
	return true;
}

Train TrainDriver::nearestTrain(Graph_Line line, Train& train)
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
	}
	else if (getSpeed() == -1) {
		nearesTrain.position = -1;
		for (auto tr : line.trains) {
			if (tr.idx == train.idx) continue;
			if (tr.position < train.position && tr.position > nearesTrain.position) {
				nearesTrain = tr;
			}
		}
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
	}
	return nearesTrain;
}

void TrainDriver::deleteTrainInPoint(int trainIdx, int pointIdx)
{
	auto& points = Data_manager::getInstance().getMapLayer0().getPoints();
	auto& trains = points[pointIdx].trains;
	while (true) {
		auto it = std::find_if(trains.begin(), trains.end(), [&](const Train & tr) {
			if (tr.idx == trainIdx)
				return true;
			return false;
		});
		if (it == trains.end()) break;
		trains.erase(it);
	}
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
		it->position += getSpeed() * 1;
	}
}


int TrainDriver::lengthToPoint(Graph_Point point, Train & train)
{
	Graph_Line trainLine = Data_manager::getInstance().getMapLayer0().getLineByIdx(train.line_idx);
	int trainToPoint = -1;
	if (trainLine.points.second == point.idx) {
		trainToPoint = trainLine.lenght - train.position;
	}
	else if (trainLine.points.first == point.idx) {
		trainToPoint = train.position;
	}
	return trainToPoint;
}
