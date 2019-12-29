#pragma once
#include "Graph_Line.h"
#include "Graph_Point.h"
#include <map>
#include <string>
#include "Game_object.h"
#include <typeinfo>

class Graph : public Game_object
{
private:
	std::string name = "";
	std::map<int, Graph_Point> points = std::map<int, Graph_Point>();
	std::map<std::pair<int, int>, Graph_Line> lines;
	int idx = 0;

public:
	Graph(std::string name_, int idx_);

	Graph_Line getLineByTwoPoints(const int& first_idx, const int& second_idx);
	Graph_Line getLineByIdx(const int& idx);
	bool addPoint(int idx_, Graph_Point point_); //return false if point with idx already exist
	bool addLine(int first_idx_, int second_idx_, Graph_Line line_); //return false if line with idx already exist
	bool createAdjacencyLists();
	std::map<int, Graph_Point>& getPoints();
	std::map<std::pair<int, int>, Graph_Line>& getLines(); //key by pair of points  from -> to
	int getIdx() const;
	void setIdx(int idx_);
	void setName(const std::string& name_);
	const std::string& getName() const;
	int getLineDirectionByTwoPoints(int first, int second);


	std::shared_ptr<Game_object> getObjectPtr();
	const std::type_info& getObjectType();
	~Graph();
};

