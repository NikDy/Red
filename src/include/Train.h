#pragma once
#include "Event_game.h"
#include <vector>
#include <iostream>
#include "Game_object.h"
#include <typeinfo>

class Train: public Game_object
{
public:
	
	Train(int idx_, int line_idx_, const std::string& player_idx_, int position_, int speed_);

	int getIdx() const;
	void setIdx(int idx_);
	void setPlayerIdx(const std::string& player_idx_);
	const std::string& getPlayerIdx() const;
	bool addEvent(Event_game event_);
	std::vector<Event_game>& getEvents();

	Game_object* clone();
	const std::type_info& getObjectType();
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

