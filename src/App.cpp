#include "Drawer.h"
#include "Data_manager.h"
#include "Decision.h"
#include "Menu.h"

#include <string>
#include <iostream>




int main()
{
	Decision dec;
	Regulator reg;
	
	Data_manager::getInstance().loadConfigFromFile();
	Menu::getInstance().gameSelect();
	dec.Insertion();
	
	Drawer::getInstance().draw();
	int i = 0;

	while(true) {
		++i;
		if (i == 2 || i == 22) {
			std::cout << "lol" << std::endl;
		}
			//Data_manager::getInstance().tryUpdateInGame();
			Data_manager::getInstance().makeMove(dec.makeTurn());
			Data_manager::getInstance().forceTurn();
			while (Data_manager::getInstance().turn != false);
	}

	return 0;
}