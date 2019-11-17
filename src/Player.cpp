#include "Player.h"




Player::Player(std::string idx_, bool in_game_, std::string name_, int rating_)
{
	idx = idx_;
	in_game = in_game_;
	name = name_;
	rating = rating_;
}

void Player::setHome(Graph_Point home_)
{
	home = home_;
}

void Player::setTown(Town town_)
{
	town = town_;
}

std::map<int, Train>& Player::getTrains()
{
	return trains;
}

bool Player::addTrain(int idx_, Train train_)
{
	return trains.emplace(idx_, train_).second;
}


Player::~Player()
{
}
