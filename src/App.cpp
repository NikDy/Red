#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"
#include "Regulator.h"
#include <Windows.h>
#include <mutex>
#include <thread>


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
	Drawer drawer(800, 600, "Drawer");
	drawer.graphToShapes(Data_manager::getInstance().getMapLayer0(), Data_manager::getInstance().getMapLayer1());
	for (int i = 0; i < 20; ++i) {
		Data_manager::getInstance().makeMove(reg.makeTurn());
		Data_manager::getInstance().forceTurn();
		drawer.update(Data_manager::getInstance().getMapLayer1());
		std::cout << i << std::endl;
}
	
	//drawer.drawAll();
	//}
	//else 
	//{
	//	std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	//}
	return 0;
}