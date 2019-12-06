#pragma once
#include "Drawer.h"


class Forces
{
public:

	static std::map<int, sf::CircleShape> recalcForces(const std::map<int, sf::CircleShape>& current_points);
	Forces();
	~Forces();
private:
	
	static float AttractionForce(float range, float length);
	static float RepulsionForce(float range);

	typedef sf::Vector2f force;
	//typedef std::pair<float, float> force; //force vector x, y
};

