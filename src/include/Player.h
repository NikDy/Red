#pragma once
#include "Graph_Point.h"
#include "Town.h"
#include "Train.h"
#include <vector>
#include <map>
#include <string>

class Player
{
public:
	Player(std::string idx_, bool in_game_, std::string name_, int rating_);
	void setHome(Graph_Point home_);
	void setTown(Town town_);
	Graph_Point getHome();
	Graph_Point getTown();
	bool getInGame();
	void setInGame(bool in_game_);
	void setName(std::string& name);
	std::string& getName();
	int getRating();
	std::map<int, Train>& getTrains();
	bool addTrain(int idx_, Train train_);
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

