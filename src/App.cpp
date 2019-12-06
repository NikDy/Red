#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"
#include "Regulator.h"
#include <mutex>
#include <thread>
#include "Forces.h"


int main()
{
<<<<<<< HEAD
	//if (argc == 2)
	//{
		
	Data_manager::getInstance().login("ni");
	Regulator reg;
	//auto a = reg.findWay(1, 5);
	/*for (auto i : a)
	{
		std::cout << i.first << " " << i.second << std::endl;
	}*/
	//auto b = reg.nearestMarket(2, 0);
	//auto c = reg.whereToGo(0, 2, 7);
	//auto d = reg.makeTurn();
	Drawer drawer(800, 600, "Drawer");
	drawer.graphToShapes(Data_manager::getInstance().getMapLayer0(), Data_manager::getInstance().getMapLayer1());
	while(true) {
		Data_manager::getInstance().makeMove(reg.makeTurn());
		Data_manager::getInstance().forceTurn();
		while (Data_manager::getInstance().turn == true);
		drawer.update(Data_manager::getInstance().getMapLayer1());
}
	/*std::map<int, std::pair<int, int>> turn;
	std::pair<int, int> k(-1, 1);
	turn.emplace(1, k);
	for (int i = 0; i < 10; ++i) {
	Data_manager::getInstance().makeMove(turn);
	Data_manager::getInstance().forceTurn();
	while (Data_manager::getInstance().turn == true);
	drawer.update(Data_manager::getInstance().getMapLayer1());
	}*/
	//drawer.drawAll();
	//}
	//else 
	//{
	//	std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	//}
=======
	//float range = 3;
	//float angle = -2.35619;
	//
	//Forces::rangeAngleToVector(range, angle);

	Data_manager::getInstance().loadConfigFromFile();
	Data_manager::getInstance().login("nik");
	Drawer::getInstance().drawAll();
>>>>>>> Task3_drawer_fix
	return 0;
}