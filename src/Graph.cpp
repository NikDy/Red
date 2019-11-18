#include "Graph.h"


bool Graph::addPoint(int idx_, Graph_Point point_)
{
	return points.emplace(idx_, point_).second;
}

bool Graph::addLine(int idx_, Graph_Line line_)
{
	return lines.emplace(idx_, line_).second;
}

bool Graph::createAdjacencyLists()
{
	for (auto point : points)
	{
		for (auto line : lines)
		{
			if (line.second.points.first == point.second.idx &&
				std::find(point.second.adjacency_list.begin(), point.second.adjacency_list.end(), line.second.points.second) == point.second.adjacency_list.end())
					point.second.adjacency_list.emplace_back(line.second.points.second);
		}
	}
	return true;
}

std::map<int, Graph_Point>& Graph::getPoints()
{
	return points;
}

std::map<int, Graph_Line>& Graph::getLines()
{
	return lines;
}

int Graph::getIdx() const
{
	return idx;
}

void Graph::setIdx(int idx_)
{
	idx = idx_;
}

void Graph::setName(const std::string & name_)
{
	name = name_;
}

const std::string & Graph::getName() const
{
	return name;
}

Game_object * Graph::clone()
{
	return new Graph(*this);
}

const std::type_info & Graph::getObjectType()
{
	return typeid(*this);
}

Graph::Graph(std::string name_, int idx_)
{
	name = name_;
	idx = idx_;
}

Graph::~Graph()
{

}
