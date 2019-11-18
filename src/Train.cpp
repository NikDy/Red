#include "Train.h"




Train::Train(int idx_, int line_idx_, const std::string & player_idx_, int position_, int speed_)
{
	idx = idx_;
	line_idx = line_idx_;
	player_idx = player_idx_;
	position = position_;
	speed = speed_;
}


const std::type_info& Train::getObjectType()
{
	return typeid(*this);
}

int Train::getIdx() const
{
	return idx;
}

void Train::setIdx(int idx_)
{
	idx = idx_;
}

void Train::setPlayerIdx(const std::string & player_idx_)
{
	player_idx = player_idx_;
}

const std::string & Train::getPlayerIdx() const
{
	return player_idx;
}

Game_object * Train::clone()
{
	return new Train(*this);
}

Train::~Train()
{
}
