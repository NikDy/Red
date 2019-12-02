#include "Graph.h"


bool Graph::addPoint(int idx_, Graph_Point point_)
{
	return points.emplace(idx_, point_).second;
}

bool Graph::addLine(int first_idx_, int second_idx_, Graph_Line line_)
{
	return lines.emplace(std::make_pair(first_idx_, second_idx_), line_).second;
}

bool Graph::createAdjacencyLists()
{
	for (auto& point : points)
	{
		for (auto& line : lines)
		{
			auto point_idx = point.second.idx;
			auto line_begin = line.second.points.first;
			auto line_end = line.second.points.second;
			if (line_end == point_idx &&
				std::find(point.second.adjacency_list.begin(), point.second.adjacency_list.end(), line_begin) == point.second.adjacency_list.end())
			{
				point.second.adjacency_list.emplace_back(line_begin);
			}
			else if (line_begin == point_idx &&
				std::find(point.second.adjacency_list.begin(), point.second.adjacency_list.end(), line_end) == point.second.adjacency_list.end())
			{
				point.second.adjacency_list.emplace_back(line_end);
			}
		}
	}

	return true;
}

std::map<int, Graph_Point>& Graph::getPoints()
{
	return points;
}

std::map<std::pair<int, int>, Graph_Line>& Graph::getLines()
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

std::shared_ptr<Game_object> Graph::getObjectPtr()
{
	return std::make_shared<Graph>(*this);
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


Graph_Line Graph::getLineByTwoPoints(const int& first_idx, const int& second_idx)
{
	if (lines.find(std::pair<int, int>(first_idx, second_idx)) != lines.end())
	{
		return lines[std::make_pair(first_idx, second_idx)];
	}
	else if (lines.find(std::pair<int, int>(second_idx, first_idx)) != lines.end())
	{
		return lines[std::make_pair(second_idx, first_idx)];
	}
	else
	{
		return Graph_Line(-1, -1, std::make_pair(-1, -1));
	}
}

Graph_Line Graph::getLineByIdx(const int& idx) {
	for (auto line : lines) {
		if (line.second.idx == idx) {
			return line.second;
		}
	}
}

Graph::~Graph()
{

}
