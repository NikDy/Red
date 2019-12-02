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
	return posts.emplace(idx_, post_).second;
}

std::map<std::string, Rating>& MapLayer1::getRaiting()
{
	return ratings;
}

bool MapLayer1::addRaiting(std::string idx_, Rating rating_)
{
	return ratings.emplace(idx_, rating_).second;
}

std::map<int, Town>& MapLayer1::getTowns()
{
	std::map<int, Town> towns;
	std::shared_ptr<Town> town_= nullptr;
	for (auto post : posts) {
		if (post.second->getObjectType() == typeid(Town)) {
			town_ = std::dynamic_pointer_cast<Town, Game_object>(post.second);
			towns.emplace(post.first, *town_);
		}
	}
	return towns;
}

std::map<int, Storage>& MapLayer1::getStorages()
{
	std::map<int, Storage> storages;
	std::shared_ptr<Storage> storage_ = nullptr;
	for (auto post : posts) {
		if (post.second->getObjectType() == typeid(Storage)) {
			storage_ = std::dynamic_pointer_cast<Storage, Game_object>(post.second);
			storages.emplace(post.first, *storage_);
		}
	}
	return storages;
}

std::map<int, Market>& MapLayer1::getMarkets()
{
	std::map<int, Market> markets;
	std::shared_ptr<Market> market_ = nullptr;
	for (auto post : posts) {
		if (post.second->getObjectType() == typeid(Market)) {
			market_ = std::dynamic_pointer_cast<Market, Game_object>(post.second);
			markets.emplace(post.first, *market_);
		}
	}
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
