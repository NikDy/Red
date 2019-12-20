#pragma once
#include <string>
#include "Event_game.h"
#include <vector>
#include "Game_object.h"
#include <typeinfo>
#include "Post.h"

class Storage : public Post
{
public:
	Storage(int idx_, int type_, const std::string& name_) : Post(idx_, type_, name_) {};

	std::vector<Event_game>& getEvents();
	bool addEvent(Event_game event_);

	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();
	std::shared_ptr<Post> getPostPtr();
	~Storage();

	int armor = 0;
	int armor_capacity = 0;
	int replenishment = 0;
	int point_idx = 0;
private:
	std::vector<Event_game> events = std::vector<Event_game>();
};

