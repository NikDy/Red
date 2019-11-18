#include "Json_Parser.h"
#include <iostream>

Json_Parser::Json_Parser()
{

}

Json_Parser::~Json_Parser()
{

}

//Graph Json_Parser::createGraph(std::string filepath)
//{
//	std::ifstream ifs(filepath);
//	if (!ifs.is_open())
//	{
//		std::cout << "File can't be open or doesn't exist" << std::endl;
//		std::exit(1);
//	}
//	rapidjson::IStreamWrapper isw(ifs);
//	rapidjson::Document doc;
//	doc.ParseStream(isw);
//	assert(doc.IsObject());
//
//	Graph new_graph(doc["name"].GetString(), doc["idx"].GetInt());
//	for (int i = 0; i < doc["points"].Size(); i++)
//	{
//		Graph_Point new_point(doc["points"][i]["idx"].GetInt());
//		if (doc["points"][i]["post_idx"].IsNull())
//		{
//			new_point.post_idx = 0;
//		}
//		else
//		{
//			new_point.post_idx = doc["points"][i]["post_idx"].GetInt();
//		}
//		new_graph.addPoint(doc["points"][i]["idx"].GetInt(), new_point);
//	}
//	for (int i = 0; i < doc["lines"].Size(); i++)
//	{
//		Graph_Line new_line(doc["lines"][i]["idx"].GetInt(),
//							doc["lines"][i]["length"].GetInt(),
//		std::pair<int, int>(doc["lines"][i]["points"][0].GetInt(), 
//							doc["lines"][i]["points"][1].GetInt()));
//		new_graph.addLine(doc["lines"][i]["idx"].GetInt(), new_line);
//	}
//	new_graph.createAdjacencyLists();
//	return new_graph;
//}


Graph Json_Parser::fromMapLayer0(std::string json_string)
{
	rapidjson::Document doc;
	doc.Parse(json_string.c_str());

	Graph new_graph(doc["name"].GetString(), doc["idx"].GetInt());
	for (int i = 0; i < doc["points"].Size(); i++)
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
	for (int i = 0; i < doc["lines"].Size(); i++)
	{
		Graph_Line new_line(doc["lines"][i]["idx"].GetInt(),
			doc["lines"][i]["length"].GetInt(),
			std::pair<int, int>(doc["lines"][i]["points"][0].GetInt(),
				doc["lines"][i]["points"][1].GetInt()));
		new_graph.addLine(doc["lines"][i]["idx"].GetInt(), new_line);
	}
	return new_graph;
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

	Town player_town =		   Town(doc["town"]["idx"].GetInt(),
									doc["town"]["type"].GetInt(),
									doc["town"]["name"].GetString());
	player_town.armor =				doc["town"]["armor"].GetInt();
	player_town.armor_capacity =	doc["town"]["armor_capacity"].GetInt();
	player_town.level =				doc["town"]["level"].GetInt();
	player_town.next_level_price =  doc["town"]["next_level_price"].GetInt();
	player_town.player_idx =		doc["town"]["player_idx"].GetString();
	player_town.point_idx =			doc["town"]["point_idx"].GetInt();
	player_town.population =		doc["town"]["population"].GetInt();
	player_town.population_capacity = doc["town"]["population_capacity"].GetInt();
	player_town.product =			doc["town"]["product"].GetInt();
	player_town.product_capacity =  doc["town"]["product_capacity"].GetInt();
	player_town.train_cooldown =    doc["town"]["train_cooldown"].GetInt();
	player_town.type =				doc["town"]["type"].GetInt();
	
	new_player.setTown(player_town);

	for (int i = 0; i < doc["trains"].Size(); i++)
	{
		Train new_train = Train(doc["trains"][i]["idx"].GetInt(),
								doc["trains"][i]["line_idx"].GetInt(),
								doc["trains"][i]["player_idx"].GetString(),
								doc["trains"][i]["position"].GetInt(),
								doc["trains"][i]["speed"].GetInt());
		new_train.fuel = 		doc["trains"][i]["fuel"].GetInt();
		new_train.fuel_capacity =		doc["trains"][i]["fuel_cpacity"].GetInt();
		new_train.fuel_consumption =	doc["trains"][i]["fuel_consumption"].GetInt();
		new_train.goods =				doc["trains"][i]["goods"].GetInt();
		new_train.goods_capacity =		doc["trains"][i]["goods_capacity"].GetInt();
		new_train.goods_type = 0;
		new_train.level =				doc["trains"][i]["level"].GetInt();
		new_train.next_level_price	=	doc["trains"][i]["next_level_price"].GetInt();
		new_train.cooldown =			doc["trains"][i]["cooldown"].GetInt();


		//event
		new_player.addTrain(new_train.getIdx(), new_train);
	}

	return new_player;
}


bool Json_Parser::is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
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
