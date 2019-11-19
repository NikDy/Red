#include "Network_manager.h"

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
<<<<<<< HEAD
	auto response_copy = response_list;
	response_list.clear();
	return response_copy;
=======
	std::list<std::shared_ptr<Game_object>> list = response_list;
	response_list = std::list<std::shared_ptr<Game_object>>();
	return list;
>>>>>>> pr/3
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


std::string Network_manager::createPackageString(short code, short messageLength, std::string message)
{
	std::string package;
	package.append(shortToCharArray(code), 4);
	package.append(shortToCharArray(messageLength), 4);
	package.append(message);
	return package;
}


bool Network_manager::trySend(std::string packageString)
{
	if (this->socket.send(packageString.c_str(), packageString.length()) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}


std::string Network_manager::receiveJsonString()
{
	int result_code;
	size_t received;
	if (this->socket.receive(&result_code, 4, received) != sf::Socket::Done)
		return "";
	if (result_code == 0)
	{
		int response_size = 0;
		if (this->socket.receive(&response_size, 4, received) != sf::Socket::Done)
			return "";

		std::string jsonString = "";
		char* in = new char[sizeof(unsigned short)];
		size_t already_received = 0;
		while (already_received < response_size)
		{
			this->socket.receive(in, sizeof(unsigned short), received);
			already_received += received;
			jsonString.append(in, received);
		}
		return jsonString;
	}
	return "";
}


bool Network_manager::Login(std::vector<std::pair<std::string, std::string>> login_data)
{
	/*std::vector<std::pair<std::string, std::string>> login_data;
	login_data.emplace_back(std::pair<std::string, std::string>("name", name));
	if (password != "") 
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", password));
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
	}*/
	

	auto json_string = Json_Parser::toJson(login_data);
<<<<<<< HEAD
	auto message = Network_manager::createPackageString(1, (short)json_string.length(), json_string);
	if(!trySend(message)) return false;
	auto response = receiveJsonString();
	std::shared_ptr<Game_object> result = std::make_shared<Player>(Json_Parser::fromPlayer(response));
	response_list.push_back(result);
	return true;
=======
	std::string message = "";
	message.append(shortToCharArray(1), 4);
	message.append(shortToCharArray((short)json_string.length()), 4);
	message.append(json_string);


	//this->socket.setBlocking(true);

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
>>>>>>> pr/3
}


bool Network_manager::Action(int action_code, std::vector<std::pair<std::string, std::string>> key_value_pairs)
{
	auto json_string = Json_Parser::toJson(key_value_pairs);
	auto message = Network_manager::createPackageString(action_code, (short)json_string.length(), json_string);
	if (!trySend(message)) return false;
	auto response = receiveJsonString();
	if (action_code == 10)
	{
		std::shared_ptr<Game_object> result = std::make_shared<Graph>(Json_Parser::fromMapLayer0(response));
		response_list.push_back(result);
	}
<<<<<<< HEAD
=======
	//std::shared_ptr<Game_object> result = std::make_shared<Graph>(Json_Parser::fromMapLayer0(respounse));
	response_list.push_back(Json_Parser::fromMapLayer0(respounse).getObjectPtr());
>>>>>>> pr/3
	return true;
}


bool Network_manager::Logout()
{
	std::string message = "";
	message.append(shortToCharArray(1), 4);
	message.append(shortToCharArray(0), 4);

	if (socket.send(message.c_str(), message.length()) != sf::Socket::Done)
		return false;
	return true;
}
