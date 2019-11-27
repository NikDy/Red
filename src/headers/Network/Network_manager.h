#pragma once
#include "SFML/Network.hpp"
#include "Game_object.h"
#include "Json_Parser.h"
#include <list>
#include <iostream>
#include <cctype>

class Network_manager
{
private:
	const int server_port = 443;
	const char* server_adress = "wgforge-srv.wargaming.net";

	


	std::list<std::shared_ptr<Game_object>> response_list;
	sf::TcpSocket socket;


	static char* shortToCharArray(short num);
	std::string createPackageString(short code, short messageLength, std::string message);
	bool trySend(std::string packageString);
	std::string receiveJsonString();

public:
	Network_manager();
	~Network_manager();

	enum class Actioncode
	{
		LOGIN = 1,
		LOGOUT = 2,
		MOVE = 3,
		UPGRADE = 4,
		TURN = 5,
		PLAYER = 6,
		GAMES = 7,
		MAP = 10
	};

	enum class Result
	{
		OKEY = 0,
		BAD_COMMAND = 1,
		RESOURCE_NOT_FOUND = 2,
		ACCESS_DENIED = 3,
		INAPPROPRIATE_GAME_STATE = 4,
		TIMEOUT = 5,
		INTERNAL_SERVER_ERROR = 500
	};

	std::list<std::shared_ptr<Game_object>> getResponseList();
	bool Login(std::vector<std::pair<std::string, std::string>> login_data);
	bool Action(int action_code, std::vector<std::pair<std::string, std::string>> key_value_pairs);
	bool Action(int action_code, std::pair<std::string, std::string> key_value_pairs);
	bool Logout();
};