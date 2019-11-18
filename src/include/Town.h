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

	/*int getIdx() const;
	void setIdx(int idx_);
	void setType(int type_);
	int getType() const;
	void setName(std::string& name_);
	const std::string& getName() const;*/
	std::vector<Event_game>& getEvents();
	bool addEvent(Event_game event_);

	const std::type_info& getObjectType();
	Game_object* clone();
	Post* clonePost();
	/*bool setArmor(int armor_);
	bool setArmorCapacity(int armor_capacity);
	bool setLevel(int level_);
	bool setNextLevelPrice(int next_level_capacity_);
	bool setPlayerIdx(const std::string& player_idx_);
	bool setPointIdx(int point_idx_);
	bool setPopulation(int population_);
	bool setPopulationCapacity(int population_capacity_);
	bool setProduct(int product_);
	bool setProductCapacity(int product_capacity_);
	bool setTrainCooldown(int train_cooldown_);*/
	~Town();

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
private:
	std::vector<Event_game> events = std::vector<Event_game>();
};

