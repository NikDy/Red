#include "Data_manager.h"




bool Data_manager::loadConfigFromFile()
{
	std::ifstream cfg_file("config.cfg");
	if (!cfg_file.is_open()) return false;
	std::string arg;
	std::string val;
	std::string eql;
	while (cfg_file >> arg >> eql >> val)
	{
		config[arg] = val;
	}
	return true;
}


bool Data_manager::login(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	setLoginData(name, password, game, num_turns, num_players);


	net.Login(login_data);
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	player = std::dynamic_pointer_cast<Player, Game_object>(list_objects.back());
	map_layer_1 = getMapLayer1FromServer();
	this->map_layer_0 = getMapLayer0FromServer();
	this->map_layer_0->createAdjacencyLists();
	updateThread = std::thread(&Data_manager::updateGame, this);
	return true;
}

bool Data_manager::makeMove(std::map<int, std::pair<int, int>> turn)
{
	std::lock_guard<std::mutex> locker(update_mutex);
	for (auto train : turn) {
		net.Action(3, setMoveData(std::to_string(train.first), std::to_string(train.second.first), std::to_string(train.second.second)));
	}
	return true;
}

Graph& Data_manager::getMapLayer0()
{

	return *this->map_layer_0;
}

MapLayer1& Data_manager::getMapLayer1()
{
	return *this->map_layer_1;
}


Player& Data_manager::getPlayer()
{
	return *this->player;
}


Data_manager::~Data_manager()
{
	updateThread.join();
	net.Logout();
}


bool Data_manager::forceTurn()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::lock_guard<std::mutex> locker(update_mutex);
	if (!net.Action(5, std::pair<std::string, std::string>("", ""))) return false;
	turn = true;
	update_check.notify_one();

	return true;
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

std::vector<std::pair<std::string, std::string>> Data_manager::setMoveData(std::string lineIdx, std::string speed, std::string trainIdx)
{
	std::vector<std::pair<std::string, std::string>> move_data;
	move_data.emplace_back(std::pair<std::string, std::string>("line_idx", lineIdx));
	move_data.emplace_back(std::pair<std::string, std::string>("speed", speed));
	move_data.emplace_back(std::pair<std::string, std::string>("train_idx", trainIdx));
	return move_data;
}


std::shared_ptr<Graph> Data_manager::getMapLayer0FromServer()
{
	net.Action(10, std::pair<std::string, std::string>("layer", "0"));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<Graph, Game_object>(list_objects.back());
}


std::shared_ptr<MapLayer1> Data_manager::getMapLayer1FromServer()
{
	net.Action(10, std::pair<std::string, std::string>("layer", "1"));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<MapLayer1, Game_object>(list_objects.back());
}


std::shared_ptr<Player> Data_manager::getPlayerFromServer()
{
	net.Action(6, std::pair<std::string, std::string>("", ""));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<Player, Game_object>(list_objects.back());
}

void Data_manager::updateGame()
{
	while (update_on) 
	{
		std::unique_lock<std::mutex> locker(update_mutex);
		update_check.wait_for(locker, std::chrono::seconds(10), [&]() {return (this->turn); });
		map_layer_1 = getMapLayer1FromServer();
		player = getPlayerFromServer();
		turn = false;
	}
}
