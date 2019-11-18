#include "Player.h"




const std::string & Player::getIdx() const
{
	return idx;
}

void Player::setIdx(const std::string & idx_)
{
	idx = idx_;
}


const std::type_info & Player::getObjectType()
{
	return typeid(*this);
}


Game_object * Player::clone()
{
	return new Player(*this);
}

void Player::setHome(Graph_Point home_)
{
	home = home_;
}

Graph_Point Player::getHome()
{
	return home;
}

void Player::setTown(Town town_)
{
	town = town_;
}

Town Player::getTown()
{
	return town;
}

bool Player::getInGame()
{
	return in_game;
}

void Player::setInGame(bool in_game_)
{
	in_game = in_game_;
}

void Player::setName(const std::string & name_)
{
	name = name_;
}

const std::string & Player::getName() const
{
	return name;
}


int Player::getRating() const
{
	return rating;
}

void Player::setRating(int rating_)
{
	rating = rating_;
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
