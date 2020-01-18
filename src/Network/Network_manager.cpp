#include "Network_manager.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET sock;

Network_manager::Network_manager()
{
	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(server_port);
	inet_pton(AF_INET, server_adress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "connected : " << server_adress << std::endl;
	}
}


Network_manager::~Network_manager()
{

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
	if (send(sock, packageString.c_str(), packageString.length(), 0 ) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}


inline int pack4chars(char c1, char c2, char c3, char c4) {
	return ((int)(((unsigned char)c4) << 24)
		| (int)(((unsigned char)c3) << 16)
		| (int)(((unsigned char)c2) << 8)
		| (int)((unsigned char)c1));
}


std::string Network_manager::receiveJsonString()
{
	ZeroMemory(buf, 4096);
	int bytesRecived = recv(sock, buf, 4, 0);
	int action_code = pack4chars(buf[0], buf[1], buf[2], buf[3]);
	std::cout << action_code << std::endl;
	if (action_code != 0) return "None";
	int responseSize = -1;
		bytesRecived = recv(sock, buf, 4, 0);
		responseSize = pack4chars(buf[0], buf[1], buf[2], buf[3]);
	if (responseSize == 0) return "";
	bytesRecived = recv(sock, buf, 4096, 0);
	int totalRecived = 0;
	std::string jsonString = "";

	do {
		if (bytesRecived > 0)
		{
			totalRecived += bytesRecived;
			jsonString.append(std::string(buf, 0, bytesRecived));
		}
		if (totalRecived < responseSize)
		{
			bytesRecived = recv(sock, buf, 4096, 0);
		}
		else
		{
			break;
		}
	} while (true);
	if (jsonString == "") return "None";
	return jsonString;
}


bool Network_manager::forceTurn(std::pair<std::string, std::string> key_value_pairs)
{
	const int action_code = 5;
	auto json_string = Json_Parser::toJson(std::vector<std::pair<std::string, std::string>>{key_value_pairs});
	auto message = Network_manager::createPackageString(action_code, (short)json_string.length(), json_string);
	if (!trySend(message)) return false;
	auto response = receiveJsonString();
	if (response == "None") return false;
	return true;
}


std::shared_ptr<Game_object> Network_manager::Login(std::vector<std::pair<std::string, std::string>> login_data)
{
	auto json_string = Json_Parser::toJson(login_data);
	auto message = Network_manager::createPackageString(1, (short)json_string.length(), json_string);
	if(!trySend(message)) return nullptr;
	auto response = receiveJsonString();
	if (response == "None") return nullptr;
	std::shared_ptr<Game_object> result = Json_Parser::fromPlayer(response).getObjectPtr();
	return result;
}


std::shared_ptr<Game_object> Network_manager::Action(int action_code, std::vector<std::pair<std::string, std::string>> key_value_pairs)
{
	auto json_string = Json_Parser::toJson(key_value_pairs);
	auto message = Network_manager::createPackageString(action_code, (short)json_string.length(), json_string);
	//std::cout << message << std::endl;
	if (!trySend(message)) return false;
	//auto begin = std::chrono::steady_clock::now();
	auto response = receiveJsonString();
	/*auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << elapsed_ms.count() << std::endl;*/
	if (response == "None") return false;
	//std::cout << response << std::endl;
	if (action_code == 10)
	{
		if (key_value_pairs[0].second == "0")
		{
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer0(response).getObjectPtr();
			return result;
		}
		else if (key_value_pairs[0].second == "1") {
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer1(response).getObjectPtr();
			return result;
		}
		else if (key_value_pairs[0].second == "10") {
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer10(response).getObjectPtr();
			return result;
		}
	}
	else if (action_code == 7)
	{
		std::shared_ptr<Game_object> result = Json_Parser::fromGames(response).getObjectPtr();
		return result;
	}
	else if (action_code == 6)
	{
		std::shared_ptr<Game_object> result = Json_Parser::fromPlayer(response).getObjectPtr();
		return result;
		//std::cout << response << std::endl;
	}
	else if (action_code == 3)
	{
		//std::shared_ptr<Game_object> result = Json_Parser::fromPlayer(response).getObjectPtr();
		//response_list.push_back(result);
	}
	return nullptr;
}


std::shared_ptr<Game_object> Network_manager::Action(int action_code, std::pair<std::string, std::string> key_value_pair)
{
	auto json_string = Json_Parser::toJson(std::vector<std::pair<std::string, std::string>>{key_value_pair});
	auto message = Network_manager::createPackageString(action_code, (short)json_string.length(), json_string);
	//std::cout << message << std::endl;
	if (!trySend(message)) return false;
	auto response = receiveJsonString();
	if (response == "None") return false;
	//std::cout << response << std::endl;
	if (action_code == 10)
	{
		if (key_value_pair.second == "0")
		{
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer0(response).getObjectPtr();
			return result;
		}
		else if (key_value_pair.second == "1") {
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer1(response).getObjectPtr();
			return result;
		}
		else if (key_value_pair.second == "10") {
			std::shared_ptr<Game_object> result = Json_Parser::fromMapLayer10(response).getObjectPtr();
			return result;
		}
	}
	else if (action_code == 7)
	{
		std::shared_ptr<Game_object> result = Json_Parser::fromGames(response).getObjectPtr();
		return result;
	}
	else if (action_code == 6) 
	{
		//std::cout << response << std::endl;
		std::shared_ptr<Game_object> result = Json_Parser::fromPlayer(response).getObjectPtr();
		return result;
		//std::cout << response << std::endl;
	}
	else if (action_code == 3)
	{
		//std::shared_ptr<Game_object> result = Json_Parser::fromPlayer(response).getObjectPtr();
		//response_list.push_back(result);
	}
	return nullptr;
}


bool Network_manager::ActionToUpgrade(std::pair<std::string, int> posts, std::pair<std::string, int> trains)
{
	int action_code = 4;

	auto json_string = Json_Parser::toJsonWithArray(posts, trains);
	auto message = Network_manager::createPackageString(action_code, (short)json_string.length(), json_string);
	//std::cout << message << std::endl;
	if (!trySend(message)) return false;
	auto response = receiveJsonString();
	//std::cout << response << std::endl;
	return true;
}


bool Network_manager::Logout()
{
	std::string message = "";
	message.append(shortToCharArray(1), 4);
	message.append(shortToCharArray(0), 4);

	if (send(sock, message.c_str(), message.length(), 0) == SOCKET_ERROR)
		return false;
	std::cout << "Logged out";
	return true;
}
