#include "DrawerContainer.h"


DrawerContainer::DrawerContainer(sf::Vector2f pos)
{
	position = pos;
}


DrawerContainer::~DrawerContainer()
{
}


void DrawerContainer::addText(sf::Text t, sf::Vector2f local_pos)
{
	text_vector.emplace_back(std::make_pair(t, local_pos));
}


void DrawerContainer::addShape(sf::CircleShape sh, sf::Vector2f local_pos)
{
	shape_vector.emplace_back(std::make_pair(sh, local_pos));
}


void DrawerContainer::addSprite(sf::Sprite srt, sf::Vector2f local_pos)
{
	sprite_vector.emplace_back(std::make_pair(srt, local_pos));
}


void DrawerContainer::clearText()
{
	text_vector.clear();
}


void DrawerContainer::clearShape()
{
	shape_vector.clear();
}


void DrawerContainer::clearSprite()
{
	sprite_vector.clear();
}


void DrawerContainer::draw(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates::Default) const
{
	for (auto i : shape_vector)
	{
		i.first.setPosition(position + i.second);
		target.draw(i.first, states);
	}
	for (auto i : sprite_vector)
	{
		i.first.setPosition(position + i.second);
		target.draw(i.first, states);
	}
	for (auto i : text_vector)
	{
		i.first.setPosition(position + i.second);
		target.draw(i.first, states);
	}
}


//std::vector<sf::Drawable&> DrawerContainer::getAllDrawable()
//{
//	std::vector<sf::Drawable&> all_drawable;
//	for (auto i : text_vector)
//	{
//		all_drawable.emplace_back(i);
//	}
//	for (auto i : shape_vector)
//	{
//		all_drawable.emplace_back(i);
//	}
//	for (auto i : sprite_vector)
//	{
//		all_drawable.emplace_back(i);
//	}
//	return ;
//}
