#pragma once
#include "Game_object.h"
#include <typeinfo>
#include <map>
#include "Train.h"
#include "Post.h"
#include "Rating.h"
#include "Town.h"
#include "Storage.h"
#include "Market.h"


class MapLayer1 : public Game_object
{
public:
	MapLayer1(int idx_) : idx(idx_) {};

	std::map<int, Train>& getTrains();
	bool addTrain(int idx_, Train train_);

	std::map<int, std::shared_ptr<Post>>& getPosts();
	bool addPost(int idx_, std::shared_ptr<Post> post_);
	std::map<std::string, Rating>& getRaiting();
	bool addRaiting(const std::string idx_, Rating rating_);
	std::map<int, std::shared_ptr<Town>>& getTowns();
	std::map<int, std::shared_ptr<Storage>>& getStorages();
	std::map<int, std::shared_ptr<Market>>& getMarkets();
	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	~MapLayer1();

	int idx = 0;
private:
	bool addTown(std::shared_ptr<Game_object> post);
	bool addStorage(std::shared_ptr<Game_object> post);
	bool addMarket(std::shared_ptr<Game_object> post);
	std::map<int, Train> trains = std::map<int, Train>();
	std::map<int, std::shared_ptr<Post>> posts = std::map<int, std::shared_ptr<Post>>();
	std::map<std::string, Rating> ratings = std::map<std::string, Rating>();
	std::map<int, std::shared_ptr<Town>> towns = std::map<int, std::shared_ptr<Town>>();
	std::map<int, std::shared_ptr<Storage>> storages = std::map<int, std::shared_ptr<Storage>>();
	std::map<int, std::shared_ptr<Market>> markets = std::map<int, std::shared_ptr<Market>>();
};

