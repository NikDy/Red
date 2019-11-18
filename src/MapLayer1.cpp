#include "MapLayer1.h"




std::map<int, Train>& MapLayer1::getTrains()
{
	return trains;
}

bool MapLayer1::addTrain(int idx_, Train train_)
{
	return trains.emplace(idx_, train_).second;
}

std::map<int, Post*>& MapLayer1::getPosts()
{
	return posts;
}

bool MapLayer1::addPost(int idx_, Post * post_)
{
	return posts.emplace(idx_, post_).second;
}

std::map<std::string, Rating>& MapLayer1::getRaiting()
{
	return ratings;
}

bool MapLayer1::addRaiting(int idx_, Rating rating_)
{
	return ratings.emplace(idx_, rating_).second;
}

const std::type_info & MapLayer1::getObjectType()
{
	return typeid(*this);
}

Game_object * MapLayer1::clone()
{
	return new MapLayer1(*this);
}


MapLayer1::~MapLayer1()
{
}
