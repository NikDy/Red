#include "Drawer.h"
#include "Data_manager.h"
#include "Regulator.h"

#include <string>
#include <iostream>


int main()
{
	Regulator reg;
	Data_manager::getInstance().loadConfigFromFile();
	auto username = Data_manager::getInstance().config["name"];
	Data_manager::getInstance().login(username);
	Drawer::getInstance().draw();
	while (true) {
		Data_manager::getInstance().makeMove(reg.makeTurn());
		Data_manager::getInstance().forceTurn();
	}

	return 0;
}