#pragma once
#include "Game_object.h"
#include <typeinfo>

enum EVENT_TYPE
{
	TRAIN_COLLISION = 1,
	HIJACKERS_ASSAULT = 2,
	PARASITES_ASSAULT = 3,
	REFUGEES_ARRIVAL = 4,
	RESOURCE_OVERFLOW = 5,
	RESOURCE_LACK = 6,
	GAME_OVER = 100
};

class Event_game : public Game_object
{

public:
	Event_game();
	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	int value = 0;
	int tick = 0;
	int type = 0;
	~Event_game();

};

