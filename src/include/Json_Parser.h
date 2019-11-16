#pragma once
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "Graph.h"


class Json_Parser
{
private:
	
public:
	Json_Parser();
	~Json_Parser();

	static Graph createGraph(std::string filepath);
};

