#pragma once
#include "Route.h"
#include "Data_manager.h"

class TrainDriver {
public:

	int& getIdx();
	void setIdx(int _idx);
	int& getSpeed();
	void setSpeed(int _speed);
	Route& getRoute();
	void setRoute(Route _route);
	int& getLineToGo();
	void setLineToGo(int _lineToGo);

	bool foundSpeedNLine(TrainDriver& driver); //to found speedToSet
	//bool checkSituation(Route _route); //check situation on the road
	//Route makeNewRoute(int firstPoint, int lastPoin); //if the situation is bad, driver makes new route with first and last point from an old route

	bool onWay = true;

	TrainDriver(int _idx);
	~TrainDriver();
	
private:
	int idx;
	Route route;
	//Route reserveRoute--if something have happened
	int speed = 0;
	int lineToGo = 0;//idx of line where we need to go


	bool isNextLineInRouteAvailable(Graph_Line line, int speed);
	bool checkLine(Graph_Line line, Train train);
	bool checkPoint(Graph_Point point, Train train);
};