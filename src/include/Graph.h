#pragma once
#include "Graph_Line.h"
#include "Graph_Point.h"
#include <map>
#include <string>
#include "Game_object.h"
#include <typeinfo>

class Graph : Game_object
{
private:
	std::string name = "";
	std::map<int, Graph_Point> points = std::map<int, Graph_Point>();
	std::map<int, Graph_Line> lines = std::map<int, Graph_Line>();
	int idx = 0;

public:
	bool addPoint(int idx_, Graph_Point point_); //return false if point with idx already exist
	bool addLine(int idx_, Graph_Line line_); //return false if line with idx already exist
	bool createAdjacencyLists();
	std::map<int, Graph_Point>& getPoints();
	std::map<int, Graph_Line>& getLines();
	Game_object* clone();
	const std::type_info& getType();

	Graph(std::string name_, int idx_);
	~Graph();
};

