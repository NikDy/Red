#include "Graph_Line.h"

std::shared_ptr<Game_object> Graph_Line::getObjectPtr()
{
	return std::make_shared<Graph_Line>(*this);
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

Graph_Line::Graph_Line()
{

}

Graph_Line::~Graph_Line()
{

}
