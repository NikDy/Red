#include "Forces.h"

const float REPULSION_CONSTANT = 0.5f;
const float ATTRACTION_CONSTANT = 0.1f;
const float MIN_LENGTH = 100.f;
const float EPS = 1.f;

std::map<int, sf::CircleShape> Forces::recalcForces(const std::map<int, sf::CircleShape>& current_points)
{
	std::map<int, sf::CircleShape> new_points;
	for (auto point : current_points)
	{
		force total_force(0, 0);
		for (auto influence : current_points)
		{
			auto line_to_influence = Data_manager::getInstance().getMapLayer0().getLineByTwoPoints(point.first, influence.first);
			auto x_diff = point.second.getPosition().x - influence.second.getPosition().x;
			auto y_diff = point.second.getPosition().y - influence.second.getPosition().y;
			if (line_to_influence.idx != -1)
			{
				total_force.x += AttractionForce(x_diff, MIN_LENGTH);
				total_force.y += AttractionForce(y_diff, MIN_LENGTH);
			}
			total_force.x += RepulsionForce(x_diff);
			total_force.y += RepulsionForce(y_diff);

		}
		sf::CircleShape new_shape = point.second;
		if (std::abs(total_force.x) < EPS) total_force.x = 0.f;
		if (std::abs(total_force.y) < EPS) total_force.y = 0.f;
		new_shape.move(total_force);
		new_points.emplace(point.first, new_shape);
	}
	return new_points;
}


float Forces::AttractionForce(float range, float length)
{
	if (std::abs(range) < length) return 0.f;
	else return -ATTRACTION_CONSTANT * range;
}


float Forces::RepulsionForce(float range)
{
	if (range != 0.f)
	{
		if (range > 0.f) return REPULSION_CONSTANT * std::log(std::abs(range));
		else return -REPULSION_CONSTANT * std::log(std::abs(range));
	}
	return 0.f;
}


Forces::Forces()
{
}


Forces::~Forces()
{
}


