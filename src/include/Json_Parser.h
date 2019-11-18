#pragma once
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <vector>
#include <algorithm>

#include "Game_object.h"
#include "Graph.h"


class Json_Parser
{
private:
	static bool is_number(const std::string& s);

public:
	Json_Parser();
	~Json_Parser();

	static Graph createGraph(std::string filepath);
	static Graph fromMapLayer0(std::string json_string);
	static std::string toJson(std::vector<std::pair<std::string, std::string>>);
};

