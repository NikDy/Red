#include "Graph_Line.h"

Game_object * Graph_Line::clone()
{
	return new Graph_Line(*this);
}

const std::type_info & Graph_Line::getObjectType()
{
	return typeid(*this);
}

Graph_Line::Graph_Line(int idx_, int lenght_, std::pair<int, int> points_)
{
	idx = idx_;
	lenght = lenght_;
	points = points_;
}

Graph_Line::~Graph_Line()
{

}
