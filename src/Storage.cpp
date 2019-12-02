#include "Storage.h"





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
