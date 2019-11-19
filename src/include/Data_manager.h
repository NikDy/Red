#pragma once
#include "Network_manager.h"
#include "Game_object.h"
#include "Graph.h"
#include "MapLayer1.h"
#include "Player.h"


class Data_manager
{
public:
	Data_manager();
	bool login(std::string name, std::string password = "", std::string game = "", int num_turns = -1, int num_players = 1);
	Graph getMapLayer0();
	MapLayer1 getMapLayer1();
	Player getPlayer();
	~Data_manager();
private:
	std::shared_ptr<Player> player = nullptr;
	std::shared_ptr<MapLayer1> map_layer_1 = nullptr;
	std::shared_ptr<Graph> map_layer_0 = nullptr;
	Network_manager net;
	std::vector<std::pair<std::string, std::string>> login_data;
	void setLoginData(std::string name, std::string password = "", std::string game = "", int num_turns = -1, int num_players = 1);
	std::shared_ptr<Graph> getMapLayer0FromServer();
	std::shared_ptr<MapLayer1> getMapLayer1FromServer();
	std::shared_ptr<Player> getPlayerFromServer();
};

