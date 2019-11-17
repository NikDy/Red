#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{


	if (argc == 2)
	{
		Drawer drawer = Drawer(800, 600, "Drawer");
		drawer.graphToShapes(Json_Parser::createGraph(argv[1]));
		drawer.drawAll();
	}
	else 
	{
		std::cout << "Usage: Graph_drawer <path_to_graph.json>" << std::endl;
	}

	return 0;
}