#include "Post.h"




const std::type_info & Post::getObjectType()
{
	return typeid(*this);
}

Game_object* Post::clone()
{
	return new Post(*this);
}

Post::~Post()
{
}
