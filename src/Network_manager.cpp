#include "Network_manager.h"





//void Net::doTry()
//{
//	// Ask for the server address
//	sf::IpAddress server("wgforge-srv.wargaming.net");
//
//	// Create a socket for communicating with the server
//	sf::TcpSocket socket;
//
//	// Connect to the server
//	if (socket.connect(server, 443) != sf::Socket::Done)
//		return;
//	std::cout << "Connected to server " << server << std::endl;
//
//	// Receive a message from the server
//	
//
//	// Send an answer to the server
//	//b'\x01\x00\x00\x00\x10\x00\x00\x00{"name":"Boris"}'
//	//const char out[] = {1, 0, 0, 0, 16, 0, 0, 0, '{', '"', 'n', 'a', 'm', 'e', '"', ':', '"', 'B', 'a', 'r', 'i', 's', '"', '}'};
//	const char out[] = "\x01\x00\x00\x00\x10\x00\x00\x00{\"name\":\"Boris\"}";
//	//b'\x02\x00\x00\x00\x0b\x00\x00\x00{"layer":0}'
//	//const char out[] = {10, 0, 0, 0, 11, 0, 0, 0, '{', '"', 'l', 'a', 'y', 'e', 'r', '"', ':', '0', '}'};
//	socket.setBlocking(true);
//
//	if (socket.send(out, 24) != sf::Socket::Done)
//		return;
//	std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
//
//
//	char in[300000];
//	std::size_t received;
//	if (socket.receive(in, 300000, received) != sf::Socket::Done)
//		return;
//	std::cout << "Message received from the server: \"" << in << "\"" << std::endl;
//	if (socket.receive(in, 300000, received) != sf::Socket::Done)
//		return;
//	for (auto i : in)
//	{
//		std::cout << i;
//	}
//}

Network_manager::Network_manager()
{
	sf::IpAddress server(server_adress);
	if (this->socket.connect(server, server_port) != sf::Socket::Status::Done)
	{
		std::cout << "Fail to create socket" << std::endl;
	}
	else
	{
		std::cout << "Connected to server: " << server << std::endl;
	}
}


Network_manager::~Network_manager()
{

}


std::list<std::shared_ptr<Game_object>> Network_manager::getResponseList()
{
	return response_list;
}


char* Network_manager::shortToCharArray(short num)
{
	char* bytes = new char[4];
	bytes[0] = num & 0xff;
	bytes[1] = (num >> 8) & 0xff;
	bytes[2] = (num >> 16) & 0xff;
	bytes[3] = (num >> 24) & 0xff;
	return bytes;
}



bool Network_manager::Login(std::string name, std::string password /*= ""*/, std::string game /*= ""*/, int num_turns /*= -1*/, int num_players /*= 1*/)
{
	std::vector<std::pair<std::string, std::string>> login_data;
	login_data.emplace_back(std::pair<std::string, std::string>("name", name));
	if (password != "") 
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", name));
	}
	if (game != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", game));
	}
	if (num_turns != -1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_turns)));
	}
	if (num_players != 1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_players)));
	}
	

	auto json_string = Json_Parser::toJson(login_data);
	std::string message = "";
	message.append(shortToCharArray(1), 4);
	message.append(shortToCharArray((short)json_string.length()), 4);
	message.append(json_string);


	this->socket.setBlocking(true);

	if (this->socket.send(message.c_str(), message.length()) != sf::Socket::Done)
		return false;

	short receive_code;
	std::size_t received;
	if (this->socket.receive(&receive_code, 4, received) != sf::Socket::Done)
		return false;
	if (receive_code == 0)
	{
		short respose_size = 0;
		if (this->socket.receive(&respose_size, 4, received) != sf::Socket::Done)
			return false;

		std::string respounse = "";
		char* in = new char[1024];
		size_t already_received = 0;
		while (already_received < respose_size)
		{
			this->socket.receive(in, 1024, received);
			already_received += received;
			respounse.append(in, received);
		}


		return true;
	}
	return false;
}


bool Network_manager::Action(int action_code, std::vector<std::pair<std::string, std::string>> key_value_pairs)
{
	auto json_string = Json_Parser::toJson(key_value_pairs);
	std::string message = "";
	if (action_code == 10)
	{
		message.append(shortToCharArray(10), 4);
		message.append(shortToCharArray((short)json_string.length()), 4);
		message.append(json_string);
	}
	if (this->socket.send(message.c_str(), message.length()) != sf::Socket::Done)
		return false;

	short receive_code;
	std::size_t received;
	if (this->socket.receive(&receive_code, 4, received) != sf::Socket::Done)
		return false;
	std::string respounse = "";
	if (receive_code == 0)
	{
		short respose_size = 0;
		if (this->socket.receive(&respose_size, 4, received) != sf::Socket::Done)
			return false;

		char* in = new char[1024];
		size_t already_received = 0;
		while (already_received < respose_size)
		{
			this->socket.receive(in, 1024, received);
			already_received += received;
			respounse.append(in, received);
		}
	}
	std::shared_ptr<Game_object> result = std::make_shared<Graph>(Json_Parser::fromMapLayer0(respounse));
	response_list.push_back(result);
	return true;
}


bool Network_manager::Logout()
{
	std::string message = "";
	message.append(shortToCharArray(1), 4);
	message.append(shortToCharArray(0), 4);

	if (this->socket.send(message.c_str(), message.length()) != sf::Socket::Done)
		return false;
	return true;
}
