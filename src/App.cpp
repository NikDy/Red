#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"


int main(int argc, char* argv[])
{
	Network_manager net;
	net.Login("Nik");
	std::vector<std::pair<std::string, std::string>> a = { std::pair<std::string, std::string>("layer", "0") };
	net.Action(10, a);
	std::list<std::shared_ptr<Game_object>> list = net.getResponseList();
	auto ptr = std::dynamic_pointer_cast<Graph, Game_object>(list.back());


	net.Logout();

	Drawer drawer = Drawer(800, 600, "Drawer");
	drawer.graphToShapes(*(ptr));
	drawer.drawAll();

	if (argc == 2)
	{
		Drawer drawer = Drawer(800, 600, "Drawer");
		//drawer.graphToShapes(*ptr);
		drawer.drawAll();
	}
	else 
	{
		std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	}

	return 0;
}