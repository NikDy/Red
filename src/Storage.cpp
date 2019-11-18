#include "Storage.h"





const std::type_info & Storage::getObjectType()
{
	return typeid(*this);
}

Game_object * Storage::clone()
{
	return new Storage(*this);
}

Post * Storage::clonePost()
{
	return new Storage(*this);
}

Storage::~Storage()
{
}
