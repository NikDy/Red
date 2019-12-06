#pragma once
#include <cmath>


#include "Drawer.h"
#include "DrawerContainer.h"



class Forces
{
public:

	static std::map<int, DrawerContainer> recalcForces(std::map<int, DrawerContainer>& current_points);

	Forces();
	~Forces();
private:

	static float range(sf::Vector2f, sf::Vector2f);
	static float delta(float, float);
};

