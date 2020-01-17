#pragma once
#include "Network_manager.h"
#include "Game_object.h"
#include "Graph.h"
#include "MapLayer1.h"
#include "Player.h"

#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>


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

	bool loadConfigFromFile();
	std::map<std::string, std::string> config;

	bool login(std::string name, std::string password, std::string game, int num_turns, int num_players);
	void logout();
	bool makeMove(int trainIdx, int lineIdx, int speed);
	bool tryUpgradeInGame(std::pair<std::string, int> postsToSend, std::pair<std::string, int> trainsToSend);
	bool forceTurn();

	Graph& getMapLayer0();
	Graph& getMapLayer01();
	MapLayer1& getMapLayer1();
	Player& getPlayer();
	MapLayer10& getMapLayer10();


	std::shared_ptr<Games> getGamesFromServer();


	bool update_on = true;
	~Data_manager();
	bool turn = false;

	bool stopUpdate = false;
	int count_Refuges = 0;
	int last_tick_Refuges = 0;

	std::mutex update_mutex;

	int countOfCol = 0;
	int maxRating = 0;
private:
	Data_manager() {};

	//bool turn = false;
	std::shared_ptr<Player> player = nullptr;
	std::shared_ptr<MapLayer1> map_layer_1 = nullptr;
	std::shared_ptr<Graph> map_layer_0 = nullptr;
	std::shared_ptr<Graph> map_layer_01 = nullptr;
	std::shared_ptr<MapLayer10> map_layer_10 = nullptr;
	Network_manager net;
	std::vector<std::pair<std::string, std::string>> login_data;
	void setLoginData(std::string name, std::string password, std::string game, int num_turns, int num_players);
	std::vector<std::pair<std::string, std::string>> setMoveData(std::string lineIdx, std::string speed, std::string trainIdx);
	std::shared_ptr<Graph> getMapLayer0FromServer();
	std::shared_ptr<MapLayer1> getMapLayer1FromServer();
	std::shared_ptr<MapLayer10> getMapLayer10FromServer();
	std::shared_ptr<Player> getPlayerFromServer();
	std::thread updateThread;
	void updateGame();
	void updateRefuges();
	std::condition_variable update_check;
	void markPoints();
};

