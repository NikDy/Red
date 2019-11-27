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
#include "Player.h"
#include "MapLayer1.h"
#include "MapLayer10.h"

class Json_Parser
{
private:
	static Town addTown(const rapidjson::Value& doc);
	static Storage addStorage(const rapidjson::Value& doc);
	static Market addMarket(const rapidjson::Value& doc);
	static Train addTrain(const rapidjson::Value& doc);
	static bool is_number(const std::string& s);
	enum class PostCode
	{
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

public:
	Json_Parser();
	~Json_Parser();

	//static Graph createGraph(std::string filepath);
	static MapLayer1 fromMapLayer1(std::string json_string);
	static Graph fromMapLayer0(std::string json_string);
	static MapLayer10 fromMapLayer10(std::string json_string);
	static Player fromPlayer(std::string json_string);
	//static bool updateMapLayer1(std::string json_string, std::shared_ptr<MapLayer1>);
	static std::string toJson(std::vector<std::pair<std::string, std::string>>);
};

