#pragma once
#include <iostream>

class Game_object
{
public:
	virtual Game_object* clone() = 0;
	virtual const std::type_info& getObjectType() = 0;
	virtual ~Game_object() {};
};