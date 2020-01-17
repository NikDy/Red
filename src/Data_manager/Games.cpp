#include "Games.h"

const std::type_info& Games::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Games::getObjectPtr()
{
	return std::make_shared<Games>(*this);
}