#pragma once
#include "Game_object.h"
#include <typeinfo>


class Event_game : Game_object
{
public:
	Event_game();
	const std::type_info& getType();
	Game_object* clone();
	~Event_game();
};

