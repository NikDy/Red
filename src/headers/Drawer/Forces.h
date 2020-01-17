#pragma once
#include <cmath>


#include "Drawer.h"
#include "DrawerContainer.h"



class Forces
{
public:

	static std::map<int, DrawerContainer> recalcForces(std::map<int, DrawerContainer>& current_points, bool& is_stable);

	Forces();
	~Forces();
private:

	static bool isStable(std::map<int, DrawerContainer>& prev_points, std::map<int, DrawerContainer>& new_points);
	static float range(sf::Vector2f, sf::Vector2f);
	static float delta(float, float);
};

