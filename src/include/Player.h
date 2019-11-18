#pragma once
#include "Graph_Point.h"
#include "Town.h"
#include "Train.h"
#include <vector>
#include <map>
#include <string>
#include "Game_object.h"
#include <typeinfo>

class Player : public Game_object
{
public:
	Player(std::string idx_, bool in_game_, std::string name_, int rating_);

	const std::string& getIdx() const;
	void setIdx(const std::string& idx_);
	void setHome(Graph_Point home_);
	Graph_Point getHome();
	void setTown(Town town_);
	Town getTown();
	bool getInGame();
	void setInGame(bool in_game_);
	void setName(const std::string& name_);
	const std::string& getName() const;
	int getRating() const;
	void setRating(int rating_);
	std::map<int, Train>& getTrains();
	bool addTrain(int idx_, Train train_);

	const std::type_info& getObjectType();
	Game_object* clone();
	~Player();
private:
	Graph_Point home = Graph_Point(0); // mb change in future
	Town town;
	std::string idx = "";
	bool in_game = false;
	std::string name = "";
	int rating = 0;
	std::map<int, Train> trains = std::map<int, Train>();
};

