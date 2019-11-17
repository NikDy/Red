#pragma once
#include <string>
#include "Event_game.h"

class Town
{
public:
	Town(int idx_, int type_, const std::string name_);
	bool setArmor(int armor_);
	bool setArmorCapacity(int armor_capacity);
	bool setLevel(int level_);
	bool setNextLevelPrice(int next_level_capacity_);
	bool setPlayerIdx(const std::string& player_idx_);
	bool setPointIdx(int point_idx_);
	bool setPopulation(int population_);
	bool setPopulationCapacity(int population_capacity_);
	bool setProduct(int product_);
	bool setProductCapacity(int product_capacity_);
	bool setTrainCooldown(int train_cooldown_);
	bool addEvent(Event_game event_);

	~Town();
private:
	int idx = 0;
	int type = 1;
	std::string name = "";
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
	std::vector<Event_game> events = std::vector<Event_game>();

};

