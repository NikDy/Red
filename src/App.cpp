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
	auto list = net.getResponseList();
	net.Logout();



	if (argc == 2)
	{
		Drawer drawer = Drawer(800, 600, "Drawer");
		//drawer.graphToShapes(Json_Parser::createGraph(argv[1]));
		drawer.drawAll();
	}
	else 
	{
		std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	}

	return 0;
}