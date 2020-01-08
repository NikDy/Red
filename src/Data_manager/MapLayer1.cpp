#include "MapLayer1.h"


std::map<int, Train>& MapLayer1::getTrains()
{
	return trains;
}

bool MapLayer1::addTrain(int idx_, Train train_)
{
	return trains.emplace(idx_, train_).second;
}

std::map<int, std::shared_ptr<Post>>& MapLayer1::getPosts()
{
	return posts;
}

bool MapLayer1::addPost(int idx_, std::shared_ptr<Post> post_)
{
	if (post_->getObjectType() == typeid(Market)) 
	{
		addMarket(post_);
	}
	else if (post_->getObjectType() == typeid(Town))
	{
		addTown(post_);
	}
	else if (post_->getObjectType() == typeid(Storage))
	{
		addStorage(post_);
	} 
	else
	{
		return false;
	}
	return posts.emplace(idx_, post_).second;
}

std::map<std::string, Rating>& MapLayer1::getRaiting()
{
	return ratings;
}

bool MapLayer1::addRaiting(const std::string idx_, Rating rating_)
{
	return ratings.emplace(idx_, rating_).second;
}

std::map<int, std::shared_ptr<Town>>& MapLayer1::getTowns()
{
	return towns;
}

std::map<int, std::shared_ptr<Storage>>& MapLayer1::getStorages()
{
	return storages;
}

std::map<int, std::shared_ptr<Market>>& MapLayer1::getMarkets()
{

	return markets;
}

const std::type_info & MapLayer1::getObjectType()
{
	return typeid(*this);
}

std::shared_ptr<Game_object> MapLayer1::getObjectPtr()
{
	return std::make_shared<MapLayer1>(*this);
}

MapLayer1::~MapLayer1()
{
}

bool MapLayer1::addTown(std::shared_ptr<Game_object> post)
{
	std::shared_ptr<Town> town_ = std::dynamic_pointer_cast<Town, Game_object>(post);
	return towns.emplace(town_->point_idx, town_).second;
}

bool MapLayer1::addMarket(std::shared_ptr<Game_object> post)
{
	std::shared_ptr<Market> market_ = std::dynamic_pointer_cast<Market, Game_object>(post);
	return markets.emplace(market_->point_idx, market_).second;
}

bool MapLayer1::addStorage(std::shared_ptr<Game_object> post)
{
	std::shared_ptr<Storage> storage_ = std::dynamic_pointer_cast<Storage, Game_object>(post);
	return storages.emplace(storage_->point_idx, storage_).second;
}

Train MapLayer1::getTrainByIdx(const int& idx) {
	for (auto train : trains) {
		if (train.second.getIdx() == idx) {
			return train.second;
		}
	}
	std::cout << "Wrong train Idx" << std::endl;
}
