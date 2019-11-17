#include "Town.h"


Town::Town(int idx_, int type_, const std::string name_)
{
	idx = idx_;
	name = name_;
	type = type_;
}

const std::type_info & Town::getType()
{
	return typeid(*this);
}

Game_object * Town::clone()
{
	return new Town(*this);
}


Town::~Town()
{
}
