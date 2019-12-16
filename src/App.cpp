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
	for (auto driver : RoutePlaner::getInstance().getDrivers()) {
		driver.second.setSpeed(3);
	}
	std::cout << "IIIIIIIIIIIIIIIII" << std::endl;
	for (auto driver : RoutePlaner::getInstance().getDrivers()) {
		std::cout << driver.second.getSpeed() << std::endl;
	}
	/*Drawer::getInstance().draw();*/
	/*while (true) {*/
		Data_manager::getInstance().makeMove(dec.makeTurn());
		Data_manager::getInstance().forceTurn();
	/*}*/

	return 0;
}