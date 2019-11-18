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
