#include "Town.h"




Town::Town(int idx_, int type_, std::string name_)
{
	idx = idx_;
	name = name_;
	type = type_;
}

bool Town::setPlayerIdx(const std::string & player_idx_)
{
	player_idx = player_idx_;
	return false;
}

Town::~Town()
{
}
