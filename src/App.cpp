#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"


int main(int argc, char* argv[])
{
	//if (argc == 2)
	//{
		
	Data_manager::getInstance().login("nik");

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