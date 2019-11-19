#include "Market.h"




const std::type_info & Market::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Market::getObjectPtr()
{
	return std::make_shared<Market>(*this);
}


std::shared_ptr<Post> Market::getPostPtr()
{
	return std::make_shared<Market>(*this);
}

Market::~Market()
{
}
