#pragma once
#include "Game_object.h"
#include <typeinfo>
#include <string>


enum PostType
{
	TOWN = 1,
	MARKET = 2,
	STORAGE = 3
};

class Post : public Game_object
{
public:
	Post(int idx_, int type_, const std::string& name_) : idx(idx_), type(type_), name(name_) {};
	Post() {};
	const std::type_info& getObjectType();
<<<<<<< HEAD
=======
	//Game_object* clone();
	virtual std::shared_ptr<Post> getPostPtr() = 0;
>>>>>>> pr/3
	~Post();
	Game_object* clone();

	int idx = 0;
	int type = 0;
	std::string name = "";
};

