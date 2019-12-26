#include "Drawer.h"
#include "Data_manager.h"
#include "Decision.h"

#include <string>
#include <iostream>




int main()
{
	Decision dec;
	Regulator reg;
	
	Data_manager::getInstance().loadConfigFromFile();
	auto username = Data_manager::getInstance().config["name"];
	Data_manager::getInstance().login(username, "", Data_manager::getInstance().config["game_name"], -1, 1);
	dec.Insertion();
	
	Drawer::getInstance().draw();


	while(true) {
		if (Data_manager::getInstance().turn == false)
		{
			//Data_manager::getInstance().tryUpdateInGame();
			Data_manager::getInstance().makeMove(dec.makeTurn());
			Data_manager::getInstance().forceTurn();
		}
	}

	return 0;
}