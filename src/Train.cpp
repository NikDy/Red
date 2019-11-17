#include "Train.h"




Train::Train(int idx_, int line_idx_, const std::string & player_idx_, int position_, int speed_)
{
	idx = idx_;
	line_idx = line_idx_;
	player_idx = player_idx_;
	position = position_;
	speed = speed_;
}

const std::string & Train::getPlayerIdx() const
{
	return player_idx;
}

Train::~Train()
{
}
