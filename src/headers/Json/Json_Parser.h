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
#include "Event_game.h"
#include "Games.h"

#include <iostream>
#include <cctype>
#include <string>

class Json_Parser
{
private:
	static Town addTown(const rapidjson::Value& doc);
	static Storage addStorage(const rapidjson::Value& doc);
	static Market addMarket(const rapidjson::Value& doc);
	static Train addTrain(const rapidjson::Value& doc);
	static bool is_number(const std::string& s);
	static Event_game addEvent(const rapidjson::Value& doc);
	enum class PostCode
	{
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

public:
	Json_Parser();
	~Json_Parser();

	static MapLayer1 fromMapLayer1(std::string json_string);
	static Graph fromMapLayer0(std::string json_string);
	static Player fromPlayer(std::string json_string);
	static Games fromGames(std::string json_string);
	static std::string toJson(std::vector<std::pair<std::string, std::string>>);
	static std::string toJsonWithArray(std::pair<std::string, std::vector<int>> posts, std::pair<std::string, std::vector<int>> trains);
};

