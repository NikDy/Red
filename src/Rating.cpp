#include "Rating.h"



<<<<<<< HEAD

=======
>>>>>>> pr/3

const std::type_info & Rating::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> Rating::getObjectPtr()
{
	return std::make_shared<Rating>(*this);
}


Rating::~Rating()
{
}
