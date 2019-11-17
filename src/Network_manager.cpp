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

}

Network_manager::~Network_manager()
{

}

std::list<Game_object*> Network_manager::getResponseList()
{
	return response_list;
}
