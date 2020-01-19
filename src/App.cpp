#include "Drawer.h"
#include "Data_manager.h"
#include "Menu.h"
#include "RoutePlaner.h"
#include <string>
#include <iostream>




int main()
{
	Data_manager::getInstance().loadConfigFromFile();
	Menu::getInstance().gameSelect();
	RoutePlaner::getInstance().loadDrivers();
	
	Drawer::getInstance().draw();
	int i = 0;

	while(true) {
		++i;
		++Data_manager::getInstance().tick;
		std::cout << "turn " << i << std::endl;
		Data_manager::getInstance().updateRefuges();
		RoutePlaner::getInstance().makeTurn();
		Data_manager::getInstance().forceTurn();
		while (Data_manager::getInstance().turn != false);
	}

	return 0;
}