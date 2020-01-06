#include "Game_object.h"

#include <vector>

class Game
{
public:
	Game(std::string _name, int _num_player, int _num_turns, int _state) : name(_name), num_player(_num_player), num_turns(_num_turns), state(_state) {};
	~Game() {};
	std::string name = "";
	int num_player = 0;
	int num_turns = 0;
	int state = 0;
};


class Games : public Game_object
{
public:
	Games() {};
	const std::type_info& getObjectType();
	std::shared_ptr<Game_object> getObjectPtr();

	void addGame(std::string name, int num_player, int num_turns, int state) { games.emplace_back(Game(name, num_player, num_turns, state)); };
	~Games() {};

	std::vector<Game> games;
};