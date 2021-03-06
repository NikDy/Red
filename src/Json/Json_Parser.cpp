#include "Json_Parser.h"


Json_Parser::Json_Parser()
{

}

Json_Parser::~Json_Parser()
{

}

MapLayer1 Json_Parser::fromMapLayer1(std::string json_string)
{
	//std::cout << json_string;
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	MapLayer1 new_mapLayer1 = MapLayer1(doc["idx"].GetInt());
	for (int i = 0; i < (int)doc["posts"].Size(); i++)
	{
		const rapidjson::Value& post_json = doc["posts"][i];
		if (doc["posts"][i]["type"].GetInt() == int(PostCode::MARKET)) 
		{
			new_mapLayer1.addPost(doc["posts"][i]["idx"].GetInt(), addMarket(post_json).getPostPtr());
		} 
		else if (doc["posts"][i]["type"].GetInt() == int(PostCode::STORAGE))
		{
			new_mapLayer1.addPost(doc["posts"][i]["idx"].GetInt(), addStorage(post_json).getPostPtr());
		}
		else if (doc["posts"][i]["type"].GetInt() == int(PostCode::TOWN))
		{
			new_mapLayer1.addPost(doc["posts"][i]["idx"].GetInt(), addTown(post_json).getPostPtr());
		}
	}

	for (auto town : new_mapLayer1.getTowns())
	{
		if (town.second->player_idx != "Null")
		{
			Rating new_rating = Rating(doc["ratings"][town.second->player_idx.c_str()]["idx"].GetString(),
				doc["ratings"][town.second->player_idx.c_str()]["name"].GetString(),
				doc["ratings"][town.second->player_idx.c_str()]["rating"].GetInt());
			new_mapLayer1.addRaiting(new_rating.idx, new_rating);
		}
	}
	
	for (int i = 0; i < (int)doc["trains"].Size(); i++)
	{
		new_mapLayer1.addTrain(doc["trains"][i]["idx"].GetInt(), addTrain(doc["trains"][i]));
	}

	return new_mapLayer1;

}


Graph Json_Parser::fromMapLayer0(std::string json_string)
{
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	Graph new_graph(doc["name"].GetString(), doc["idx"].GetInt());
	for (int i = 0; i < (int)doc["points"].Size(); i++)
	{
		Graph_Point new_point(doc["points"][i]["idx"].GetInt());
		if (doc["points"][i]["post_idx"].IsNull())
		{
			new_point.post_idx = 0;
		}
		else
		{
			new_point.post_idx = doc["points"][i]["post_idx"].GetInt();
		}
		new_graph.addPoint(doc["points"][i]["idx"].GetInt(), new_point);
	}
	for (int i = 0; i < (int)doc["lines"].Size(); i++)
	{
		Graph_Line new_line(doc["lines"][i]["idx"].GetInt(),
			doc["lines"][i]["length"].GetInt(),
			std::pair<int, int>(doc["lines"][i]["points"][0].GetInt(),
				doc["lines"][i]["points"][1].GetInt()));
		new_graph.addLine(new_line.points.first, new_line.points.second, new_line);
	}
	return new_graph;
}


MapLayer10 Json_Parser::fromMapLayer10(std::string json_string)
{
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	MapLayer10 new_maplayer10(doc["idx"].GetInt(), std::make_pair(doc["size"][0].GetInt(), doc["size"][1].GetInt()));
	for (int i = 0; i < (int)doc["coordinates"].Size(); i++)
	{
		new_maplayer10.points.emplace(doc["coordinates"][i]["idx"].GetInt(),
			std::make_pair(doc["coordinates"][i]["x"].GetInt(), doc["coordinates"][i]["y"].GetInt()));
	}
	return new_maplayer10;
}


Player Json_Parser::fromPlayer(std::string json_string)
{
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	Player new_player = Player(
						doc["idx"].GetString(),
						doc["in_game"].GetBool(),
						doc["name"].GetString(),
						doc["rating"].GetInt());
	new_player.setHome(Graph_Point(	doc["home"]["idx"].GetInt(),
									doc["home"]["post_idx"].GetInt()));

	Town player_town = addTown(doc["town"]);

	
	new_player.setTown(player_town);

	for (int i = 0; i < (int)doc["trains"].Size(); i++)
	{
		Train new_train = addTrain(doc["trains"][i]);
			
		new_player.addTrain(new_train.getIdx(), new_train);
	}

	return new_player;
}




Games Json_Parser::fromGames(std::string json_string)
{
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	Games new_games = Games();
	for (int i = 0; i < (int)doc["games"].Size(); i++)
	{
		new_games.addGame(doc["games"][i]["name"].GetString(),
						  doc["games"][i]["num_players"].GetInt(),
						  doc["games"][i]["num_turns"].GetInt(),
						  doc["games"][i]["state"].GetInt());
	}
	return new_games;
}




bool Json_Parser::is_number(const std::string& s)
{
	auto first_char = s.begin();
	if (s[0] == '-') first_char++;
	return !s.empty() && std::find_if(first_char,
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

Event_game Json_Parser::addEvent(const rapidjson::Value & doc)
{
	Event_game event_;
	event_.tick = doc["tick"].GetInt();
	event_.type = doc["type"].GetInt();
	switch (event_.type)
	{
		case (int)EVENT_TYPE::TRAIN_COLLISION:
			event_.value = event_.type = doc["train"].GetInt();
			break;
		case (int)EVENT_TYPE::REFUGEES_ARRIVAL:
			event_.value = event_.type = doc["refugees_number"].GetInt();
			break;
		case (int)EVENT_TYPE::HIJACKERS_ASSAULT:
			event_.value = event_.type = doc["hijackers_power"].GetInt();
			break;
		case (int)EVENT_TYPE::PARASITES_ASSAULT:
			event_.value = event_.type = doc["parasites_power"].GetInt();
			break;
	default:
		break;
	}
	return event_;
}

Town Json_Parser::addTown(const rapidjson::Value& doc)
{
	Town town_map = Town(doc["idx"].GetInt(),
		doc["type"].GetInt(),
		doc["name"].GetString());
	town_map.armor = doc["armor"].GetInt();
	if (doc.HasMember("events"))
	{
		for (int i = 0; i < (int)doc["events"].Size(); ++i) {
			town_map.addEvent(addEvent(doc["events"][i]));
		}
	}
	town_map.armor_capacity = doc["armor_capacity"].GetInt();
	if(doc.HasMember("level")) town_map.level = doc["level"].GetInt();
	if (doc.HasMember("next_level_price")) 
		if (!doc["next_level_price"].IsNull())
			town_map.next_level_price = doc["next_level_price"].GetInt();
	if(!doc["player_idx"].IsNull()) town_map.player_idx = doc["player_idx"].GetString();
	else town_map.player_idx = "Null";
	town_map.point_idx = doc["point_idx"].GetInt();
	town_map.population = doc["population"].GetInt();
	town_map.population_capacity = doc["population_capacity"].GetInt();
	town_map.product = doc["product"].GetInt();
	town_map.product_capacity = doc["product_capacity"].GetInt();
	if (doc.HasMember("train_cooldown")) town_map.train_cooldown = doc["train_cooldown"].GetInt();
	//town_map.type = doc["type"].GetInt();
	return town_map;
}


Storage Json_Parser::addStorage(const rapidjson::Value& doc)
{
	Storage storage_map = Storage(doc["idx"].GetInt(), doc["type"].GetInt(), doc["name"].GetString());
	storage_map.armor = doc["armor"].GetInt();
	storage_map.armor_capacity = doc["armor_capacity"].GetInt();
	if (doc.HasMember("events"))
	{
		for (int i = 0; i < (int)doc["events"].Size(); ++i) {
			storage_map.addEvent(addEvent(doc["events"][i]));
		}
	}
	storage_map.point_idx = doc["point_idx"].GetInt();
	storage_map.replenishment = doc["replenishment"].GetInt();
	return storage_map;
}

Market Json_Parser::addMarket(const rapidjson::Value& doc)
{
	Market market_map = Market(doc["idx"].GetInt(), doc["type"].GetInt(), doc["name"].GetString());
	market_map.replenishment = doc["replenishment"].GetInt();
	if (doc.HasMember("events"))
	{
		for (int i = 0; i < (int)doc["events"].Size(); ++i) {
			market_map.addEvent(addEvent(doc["events"][i]));
		}
	}
	market_map.point_idx = doc["point_idx"].GetInt();
	market_map.product = doc["product"].GetInt();
	market_map.product_capacity = doc["product_capacity"].GetInt();
	return market_map;
}

Train Json_Parser::addTrain(const rapidjson::Value & doc)
{
	Train new_train = Train(doc["idx"].GetInt(),
		doc["line_idx"].GetInt(),
		doc["player_idx"].GetString(),
		doc["position"].GetInt(),
		doc["speed"].GetInt());
	//new_train.fuel = 		doc["fuel"].GetInt();
	//new_train.fuel_capacity =		doc["fuel_cpacity"].GetInt();
	//new_train.fuel_consumption =	doc["fuel_consumption"].GetInt();
	if (doc.HasMember("events"))
	{
		for (int i = 0; i < (int)doc["events"].Size(); ++i) {
			new_train.addEvent(addEvent(doc["events"][i]));
		}
	}
	new_train.goods = doc["goods"].GetInt();
	new_train.goods_capacity = doc["goods_capacity"].GetInt();
	if (doc.HasMember("goods_type"))
		if (!doc["goods_type"].IsNull())
			new_train.goods_type = doc["goods_type"].GetInt();
	if(doc.HasMember("level")) new_train.level = doc["level"].GetInt();
	if(doc.HasMember("next_level_price")) 
		if(!doc["next_level_price"].IsNull())
			new_train.next_level_price = doc["next_level_price"].GetInt();
	if(doc.HasMember("cooldown")) new_train.cooldown = doc["cooldown"].GetInt();

	return new_train;
}




std::string Json_Parser::toJson(std::vector<std::pair<std::string, std::string>> key_value_pairs)
{
	rapidjson::StringBuffer str;
	rapidjson::Writer<rapidjson::StringBuffer> writer(str);

	writer.StartObject();
	for (auto p : key_value_pairs)
	{
		writer.Key(p.first.c_str());
		if (is_number(p.second))
		{
			writer.Int(std::stoi(p.second));
		}
		else
		{
			writer.String(p.second.c_str());
		}
	}
	writer.EndObject();
	return str.GetString();
}


std::string Json_Parser::toJsonWithArray(std::pair<std::string, int> posts, std::pair<std::string, int> trains)
{
	rapidjson::StringBuffer str;
	rapidjson::Writer<rapidjson::StringBuffer> writer(str);

	writer.StartObject();

	writer.Key(posts.first.c_str());
	writer.StartArray();
	if(posts.second != -1) writer.Int(posts.second);
	writer.EndArray();

	writer.Key(trains.first.c_str());
	writer.StartArray();
	if (trains.second != -1) writer.Int(trains.second);
	writer.EndArray();

	writer.EndObject();

	return str.GetString();
}