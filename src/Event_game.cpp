#include "Event_game.h"



Event_game::Event_game()
{
}

const std::type_info & Event_game::getObjectType()
{
	return typeid(*this);
}

Game_object * Event_game::clone()
{
	return new Event_game(*this);
}


Event_game::~Event_game()
{
}
