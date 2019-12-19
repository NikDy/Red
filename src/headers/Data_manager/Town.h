#pragma once
#include <string>
#include "Event_game.h"
#include <vector>
#include "Game_object.h"
#include <typeinfo>
#include "Post.h"

class Town: public Post
{
public:
	Town(int idx_, int type_, const std::string& name_) : Post(idx_, type_, name_) {};
	Town() {};

	std::vector<Event_game>& getEvents();
	bool addEvent(Event_game event_);

	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	std::shared_ptr<Post> getPostPtr();

	int armor = 0;
	int armor_capacity = 0;
	int level = 0;
	int next_level_price = 0;
	std::string player_idx = "";
	int point_idx = 0;
	int population = 0;
	int population_capacity = 0;
	int product = 0;
	int product_capacity = 0;
	int train_cooldown = 0;
	~Town();
private:
	std::vector<Event_game> events = std::vector<Event_game>();
};

