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
		std::cout << i << std::endl;
		RoutePlaner::getInstance().makeTurn();
		Data_manager::getInstance().forceTurn();
		while (Data_manager::getInstance().turn != false);
	}

	return 0;
}