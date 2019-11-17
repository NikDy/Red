#pragma once
#include "Event_game.h"
#include <vector>

class Train
{
public:
	Train(int idx_, int line_idx_, const std::string& player_idx_, int position_, int speed_);
	bool addEvent(Event_game event_);
	int getIdx() const;
	const std::string& getPlayerIdx() const;
	std::vector<Event_game>& getEvents();
	~Train();

	int cooldown = 0;
	int fuel = 0;
	int fuel_capacity = 0;
	int fuel_consumption = 0;
	int goods = 0;
	int goods_capacity = 0;
	int goods_type = 0;
	int level = 0;
	int line_idx = 0;
	int next_level_price = 0;
	int position = 0;
	int speed = 0;
private:
	int idx = 0;
	std::string player_idx = "";
	std::vector<Event_game> events = std::vector<Event_game>();
};

