#include "Forces.h"


std::map<int, DrawerContainer> Forces::recalcForces(std::map<int, DrawerContainer>& current_points)
{
	
	auto graph = Data_manager::getInstance().getMapLayer01();
	std::map<int, DrawerContainer> new_points;
	for (auto point : current_points)
	{
		sf::Vector2f total_force;
		for (auto influence : current_points)
		{
			if (point.first != influence.first)
			{
				if (graph.getLineByTwoPoints(point.first, influence.first).idx != -1)
				{
					total_force.x += std::stof(Data_manager::getInstance().config["ATTRACTION_CONSTANT"])
						* delta(influence.second.position.x, point.second.position.x);
					total_force.y += std::stof(Data_manager::getInstance().config["ATTRACTION_CONSTANT"])
						* delta(influence.second.position.y, point.second.position.y);
				}
				total_force.x += std::stof(Data_manager::getInstance().config["REPULSION_CONSTANT"])
					* delta(point.second.position.x, influence.second.position.x)
					/ range(point.second.position, influence.second.position);
				total_force.y += std::stof(Data_manager::getInstance().config["REPULSION_CONSTANT"])
					* delta(point.second.position.y, influence.second.position.y)
					/ range(point.second.position, influence.second.position);
			}
		}
		point.second.position += total_force * 0.8f;
		new_points[point.first] = point.second;
	}
	return new_points;
}


float Forces::range(sf::Vector2f first, sf::Vector2f second)
{
	return std::sqrt((second.x - first.x) * (second.x - first.x) + (second.y - first.y) * (second.y - first.y));
}


float Forces::delta(float first, float second)
{
	return first - second;
}


Forces::Forces()
{
}


Forces::~Forces()
{
}


