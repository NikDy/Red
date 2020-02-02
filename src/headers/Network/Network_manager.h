#pragma once
#include "Game_object.h"
#include "Json_Parser.h"
#include <list>
#include <iostream>
#include <cctype>

class Network_manager
{
private:
	const int server_port = 443;
	std::string server_adress = "92.223.2.79";

	char buf[4096];


	static char* shortToCharArray(int num);
	std::string createPackageString(int code, int messageLength, std::string message);
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
	bool forceTurn(std::pair<std::string, std::string> key_value_pairs);
	bool makeMove(std::vector<std::pair<std::string, std::string>> key_value_pairs);
	std::shared_ptr<Game_object> Login(std::vector<std::pair<std::string, std::string>> login_data);
	std::shared_ptr<Game_object> Action(int action_code, std::vector<std::pair<std::string, std::string>> key_value_pairs);
	std::shared_ptr<Game_object> Action(int action_code, std::pair<std::string, std::string> key_value_pairs);
	bool ActionToUpgrade(std::pair<std::string, int> posts, std::pair<std::string, int> trains);
	//bool Update(int action_code, std::pair<std::string, std::string> key_value_pair);
	bool Logout();
};