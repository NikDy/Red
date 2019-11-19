#include "Graph_Point.h"

std::shared_ptr<Game_object> Graph_Point::getObjectPtr()
{
	return std::make_shared<Graph_Point>(*this);
}

const std::type_info & Graph_Point::getObjectType()
{
	return typeid(*this);
}

Graph_Point::Graph_Point(int idx_, int post_idx_)
{
	idx = idx_;
	post_idx = post_idx_;
}

Graph_Point::Graph_Point(int idx_)
{
	idx = idx_;
}

Graph_Point::~Graph_Point()
{

}
