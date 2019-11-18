#pragma once
#include "Game_object.h"
#include <typeinfo>
#include <map>
#include "Train.h"
#include "Post.h"
#include "Rating.h"


class MapLayer1 : public Game_object
{
public:
	MapLayer1(int idx_) : idx(idx_) {};

	std::map<int, Train>& getTrains();
	bool addTrain(int idx_, Train train_);
	std::map<int, Post*>& getPosts();
	bool addPost(int idx_, Post* post_);
	std::map<std::string, Rating>& getRaiting();
	bool addRaiting(int idx_, Rating rating_);

	const std::type_info& getObjectType();
	Game_object* clone();
	~MapLayer1();

	int idx = 0;
private:
	std::map<int, Train> trains = std::map<int, Train>();
	std::map<int, Post*> posts = std::map<int, Post*>();
	std::map<std::string, Rating> ratings = std::map<std::string, Rating>();

};

