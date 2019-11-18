#pragma once
#include <string>
#include "Game_object.h"
#include <typeinfo>

class Rating : public Game_object
{
public:
	Rating(const std::string& idx_, const std::string& name_, int rating_) : idx(idx_), name(name_), rating(rating_) {};
	const std::type_info& getObjectType();
	Game_object* clone();
	~Rating();

	std::string idx = "";
	std::string name = "";
	int rating = 0;
};

