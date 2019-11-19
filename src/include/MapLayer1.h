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
<<<<<<< HEAD
	std::map<int, Post*>& getPosts();
	bool addPost(int idx_, Post* post_);
	std::map<int, Rating>& getRaiting();
	bool addRaiting(int idx_, Rating rating_);
=======
	std::map<int, std::shared_ptr<Post>>& getPosts();
	bool addPost(int idx_, std::shared_ptr<Post> post_);
	std::map<std::string, Rating>& getRaiting();
	bool addRaiting(std::string idx_, Rating rating_);
	std::map<int, Town>& getTowns();
	std::map<int, Storage>& getStorages();
	std::map<int, Market>& getMarkets();
>>>>>>> pr/3

	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	~MapLayer1();

	int idx = 0;
private:
	std::map<int, Train> trains = std::map<int, Train>();
<<<<<<< HEAD
	std::map<int, Post*> posts = std::map<int, Post*>();
	std::map<int, Rating> ratings = std::map<int, Rating>();
=======
	std::map<int, std::shared_ptr<Post>> posts = std::map<int, std::shared_ptr<Post>>();
	std::map<std::string, Rating> ratings = std::map<std::string, Rating>();
>>>>>>> pr/3

};

