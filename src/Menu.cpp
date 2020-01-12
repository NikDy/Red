#include "Menu.h"


void Menu::gameSelect()
{
	exist_games = Data_manager::getInstance().getGamesFromServer();
	
	std::cout << std::setw(20) << std::left << "Game name"
		<< std::left << std::setw(15) << "num of plr"
		<< std::left << std::setw(15) << "num of turns"
		<< std::left << std::setw(15) << "state" << std::endl;

	if (exist_games->games.size() == 0) std::cout << "No games available" << std::endl;
	for (auto game : exist_games->games)
	{
		std::cout << std::setw(20) << game.name
			<< std::setw(15) << game.num_player
			<< std::setw(15) << game.num_turns
			<< std::setw(15) << game.state << std::endl;
	}
	std::string game_name;
	int number_of_players = 1;
	std::cout << "Enter game name to connect or create new game: ";
	while (true)
	{
		std::getline(std::cin, game_name);
		auto it = std::find_if(exist_games->games.begin(), exist_games->games.end(), [&game_name](const Game& obj) {return obj.name == game_name; });
		if (it != exist_games->games.end())
		{
			if (it->state == 2) std::cout << "Game already begin, try another or create new one." << std::endl;
			else break;
		}
		else
		{
			std::cout << "Enter numer of players: ";
			std::cin >> number_of_players;
			break;
		}
	}
	if (number_of_players <= 4 && number_of_players > 0)
	{
		Data_manager::getInstance().login(Data_manager::getInstance().config["name"], "", game_name, -1, number_of_players);
	}
	waitUntilStart(game_name);
}


void Menu::waitUntilStart(std::string game_name)
{
	std::cout << "Wait for other players" << std::endl;
	while (true)
	{
		exist_games = Data_manager::getInstance().getGamesFromServer();
		auto it = std::find_if(exist_games->games.begin(), exist_games->games.end(), [&game_name](const Game& obj) {return obj.name == game_name; });
		if (it != exist_games->games.end())
		{
			if (it->state == 2) break;
		}
		else
		{
			std::cout << "Game was canceled :(" << std::endl;
			Data_manager::getInstance().logout();
			break;
		}
	}
}
