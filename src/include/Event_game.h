#pragma once
#include "Game_object.h"
#include <typeinfo>


class Event_game : public Game_object
{
public:
	Event_game();
	const std::type_info& getObjectType();
	Game_object* clone();
	~Event_game();
};

