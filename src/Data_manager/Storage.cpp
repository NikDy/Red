#include "Storage.h"





std::vector<Event_game>& Storage::getEvents()
{
	return events;
}

bool Storage::addEvent(Event_game event_)
{
	events.push_back(event_);
	return true;
}

const std::type_info & Storage::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Storage::getObjectPtr()
{
	return std::make_shared<Storage>(*this);
}


std::shared_ptr<Post> Storage::getPostPtr()
{
	return std::make_shared<Storage>(*this);
}

Storage::~Storage()
{
}
