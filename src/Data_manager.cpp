#include "Data_manager.h"



Data_manager::Data_manager()
{
}

bool Data_manager::login(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	setLoginData(name, password, game, num_turns, num_players);

	net.Login(login_data);
	return true;
}

Graph Data_manager::getMapLayer0()
{
	map_layer_0 = getMapLayer0FromServer();
	return *map_layer_0;
}

MapLayer1 Data_manager::getMapLayer1()
{
	map_layer_1 = getMapLayer1FromServer();
	return *map_layer_1;
}

Player Data_manager::getPlayer()
{
	player = getPlayerFromServer();
	return *player;
}


Data_manager::~Data_manager()
{
	net.Logout();
}

void Data_manager::setLoginData(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	login_data.emplace_back(std::pair<std::string, std::string>("name", name));
	if (password != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", password));
	}
	if (game != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", game));
	}
	if (num_turns != -1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_turns)));
	}
	if (num_players != 1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_players)));
	}
}

std::shared_ptr<Graph> Data_manager::getMapLayer0FromServer()
{
	std::vector<std::pair<std::string, std::string>> a = { std::pair<std::string, std::string>("layer", "0") };
	net.Action(10, a);
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<Graph, Game_object>(list_objects.back());
}

std::shared_ptr<MapLayer1> Data_manager::getMapLayer1FromServer()
{
	std::vector<std::pair<std::string, std::string>> a = { std::pair<std::string, std::string>("layer", "1") };
	net.Action(10, a);
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<MapLayer1, Game_object>(list_objects.back());
}

std::shared_ptr<Player> Data_manager::getPlayerFromServer()
{
	std::vector<std::pair<std::string, std::string>> a = { std::pair<std::string, std::string>("", "") };
	net.Action(6, a);
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<Player, Game_object>(list_objects.back());
}
