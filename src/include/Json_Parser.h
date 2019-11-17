#pragma once
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <vector>
#include "Graph.h"


class Json_Parser
{
private:
	
public:
	Json_Parser();
	~Json_Parser();

	static Graph createGraph(std::string filepath);
	static char* enLogin(std::vector<std::pair<std::string, std::string>>);
};

