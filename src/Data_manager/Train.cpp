#include "Train.h"


Train::Train(int idx_, int line_idx_, const std::string& player_idx_, int position_, int speed_)
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

void Train::setPlayerIdx(const std::string& player_idx_)
{
	player_idx = player_idx_;
}

int Train::getLineIdx() const {
	return line_idx;
}

void Train::setLineIdx(int lineIdx_) {
	line_idx = lineIdx_;
}

int Train::getPosition() const {
	return position;
}

void Train::setPosition(int position_) {
	position = position_;
}

int Train::getSpeed() const {
	return speed;
}

void Train::setSpeed(int speed_) {
	speed = speed_;
}

int Train::getGoods() const {
	return goods;
}

int Train::getGoodsCapacity() const {
	return goods_capacity;
}

const std::string& Train::getPlayerIdx() const
{
	return player_idx;
}

bool Train::addEvent(Event_game event_)
{
	events.push_back(event_);
	return true;
}

std::vector<Event_game>& Train::getEvents()
{
	return events;
}

std::shared_ptr<Game_object> Train::getObjectPtr()
{
	return std::make_shared<Train>(*this);
}

Train::~Train()
{
}
