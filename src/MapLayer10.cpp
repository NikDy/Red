#include "MapLayer10.h"


MapLayer10::MapLayer10()
{

}


const std::type_info& MapLayer10::getObjectType()
{
	return typeid(*this);
}


std::shared_ptr<Game_object> MapLayer10::getObjectPtr()
{
	return std::make_shared<MapLayer10>(*this);
}


MapLayer10::~MapLayer10()
{

}
