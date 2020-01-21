#pragma once
#include "Data_manager.h"
#include <iostream>
#include <iomanip>


class Menu
{
public:
	static Menu& getInstance()
	{
		static Menu instance;
		return instance;
	}
	Menu(Menu const&) = delete;
	Menu operator=(Menu const&) = delete;


	void gameSelect();

private:
	std::shared_ptr<Games> exist_games;

	void waitUntilStart(std::string);

	Menu() {};
	~Menu() {};
};