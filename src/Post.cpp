#include "Post.h"




const std::type_info & Post::getObjectType()
{
	return typeid(*this);
}



Post::~Post()
{
}
