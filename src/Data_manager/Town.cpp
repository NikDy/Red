#include "Town.h"


const std::type_info & Town::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Town::getObjectPtr()
{
	return std::make_shared<Town>(*this);
}

std::shared_ptr<Post> Town::getPostPtr()
{
	return std::make_shared<Town>(*this);
}

/*int Town::getIdx() const
{
	return idx;
}

void Town::setIdx(int idx_)
{
	idx = idx_;
}

void Town::setType(int type_)
{
	type = type_;
}

int Town::getType() const
{
	return type;
}

void Town::setName(std::string & name_)
{
	name = name_;
}

const std::string & Town::getName() const
{
	return name;
}*/


Town::~Town()
{
}
