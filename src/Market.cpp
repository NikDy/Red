#include "Market.h"




const std::type_info & Market::getObjectType()
{
	return typeid(*this);
}

Game_object * Market::clone()
{
	return new Market(*this);
}

Post * Market::clonePost()
{
	return new Market(*this);
}

Market::~Market()
{
}
