#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"
#include "Regulator.h"


int main(int argc, char* argv[])
{
	//if (argc == 2)
	//{
		
	Data_manager::getInstance().login("ni");
	Regulator reg;
	auto a = reg.findWay(1, 5);
	for (auto i : Data_manager::getInstance().getMapLayer0().getLines())
	{
		std::cout << i.first.first << " " << i.first.second << std::endl;
	}
	auto b = reg.nearestMarket(2, 0);
	auto c = reg.whereToGo(0, 2, 7);
	auto d = reg.makeTurn();

	Drawer drawer = Drawer(800, 600, "Drawer");
	drawer.graphToShapes(Data_manager::getInstance().getMapLayer0(), Data_manager::getInstance().getMapLayer1());
	drawer.drawAll();
	//}
	//else 
	//{
	//	std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	//}

	return 0;
}