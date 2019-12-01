#pragma once
#include "Network_manager.h"
#include "Game_object.h"
#include "Graph.h"
#include "MapLayer1.h"
#include "Player.h"
#include "MapLayer10.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class Data_manager
{
public:
	static Data_manager& getInstance()
	{
		static Data_manager instance;
		return instance;
	}
	Data_manager(Data_manager const&) = delete;
	Data_manager operator=(Data_manager const&) = delete;


	bool login(std::string name, std::string password = "", std::string game = "", int num_turns = -1, int num_players = 1);
	bool makeMove(std::map<int, std::pair<int, int>> turn);
	Graph& getMapLayer0();
	MapLayer1& getMapLayer1();
	Player& getPlayer();
	bool update_on = true;
	~Data_manager();
	bool forceTurn();
	bool turn = false;
private:
	Data_manager() {};

	//bool turn = false;
	std::shared_ptr<Player> player = nullptr;
	std::shared_ptr<MapLayer1> map_layer_1 = nullptr;
	std::shared_ptr<Graph> map_layer_0 = nullptr;
	std::shared_ptr<MapLayer10> map_layer_10 = nullptr;
	Network_manager net;
	std::vector<std::pair<std::string, std::string>> login_data;
	void setLoginData(std::string name, std::string password = "", std::string game = "", int num_turns = -1, int num_players = 1);
	std::vector<std::pair<std::string, std::string>> setMoveData(std::string lineIdx, std::string speed, std::string trainIdx);
	std::shared_ptr<Graph> getMapLayer0FromServer();
	std::shared_ptr<MapLayer1> getMapLayer1FromServer();
	std::shared_ptr<MapLayer10> getMapLayer10FromServer();
	std::shared_ptr<Player> getPlayerFromServer();
	std::thread updateThread;
	void updateGame();
	std::mutex update_mutex;
	std::condition_variable update_check;
};

