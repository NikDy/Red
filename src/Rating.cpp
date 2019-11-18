#include "Rating.h"





const std::type_info & Rating::getObjectType()
{
	return typeid(*this);
}

Game_object * Rating::clone()
{
	return new Rating(*this);
}


Rating::~Rating()
{
}
