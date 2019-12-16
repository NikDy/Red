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
	
	/*Drawer::getInstance().draw();*/
	for (int i = 1; i < 10; i++) {
			std::cout << i << std::endl;
			Data_manager::getInstance().makeMove(dec.makeTurn());
			Data_manager::getInstance().forceTurn();
	}

	return 0;
}