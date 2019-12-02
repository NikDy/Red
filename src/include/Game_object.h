#pragma once
#include <iostream>

class Game_object
{
public:
	virtual std::shared_ptr<Game_object> getObjectPtr() = 0;
	virtual const std::type_info& getObjectType() = 0;
	virtual ~Game_object() {};
};