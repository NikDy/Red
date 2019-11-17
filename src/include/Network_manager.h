#pragma once
#include "SFML/Network.hpp"
#include "Game_object.h"
#include "Json_Parser.h"
#include <list>
#include <iostream>

class Network_manager
{
private:
	const int server_port = 443;
	const char* server_adress = "wgforge-srv.wargaming.net";


	std::list<Game_object*> response_list;
	sf::TcpSocket socket = sf::TcpSocket();



	Network_manager();
	~Network_manager();



public:

	std::list<Game_object*> getResponseList();
	bool Login(std::string name, std::string password = "", std::string game = "", int num_turns = -1, int num_players = 1);
	bool sendAction();
	bool Logout();


};
//LOGIN = 1,
//LOGOUT = 2,
//MOVE = 3,
//UPGRADE = 4,
//TURN = 5,
//PLAYER = 6,
//GAMES = 7,
//MAP = 10