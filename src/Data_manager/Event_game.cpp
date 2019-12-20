#include "Event_game.h"



Event_game::Event_game()
{
}

const std::type_info & Event_game::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Event_game::getObjectPtr()
{
	return std::make_shared<Event_game>(*this);
}


Event_game::~Event_game()
{
}
