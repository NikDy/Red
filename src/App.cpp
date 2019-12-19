#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"
#include "Decision.h"
#include "RoutePlaner.h"
#include <Windows.h>
#include <mutex>
#include <thread>
#include "Forces.h"


int main()
{
	Decision dec;
	Regulator reg;
	
	Data_manager::getInstance().loadConfigFromFile();
	auto username = Data_manager::getInstance().config["name"];
	Data_manager::getInstance().login(username);
	dec.Insertion();
	
	Drawer::getInstance().draw();

	while(true) {
		Data_manager::getInstance().tryUpdateInGame();
		Data_manager::getInstance().makeMove(dec.makeTurn());
		Data_manager::getInstance().forceTurn();
		while (Data_manager::getInstance().turn != false);
	}

	return 0;
}