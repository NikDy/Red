#include "DrawerContainer.h"


DrawerContainer::DrawerContainer(sf::Vector2f pos)
{
	position = pos;
}


DrawerContainer::~DrawerContainer()
{
}


void DrawerContainer::addText(sf::Text& t, sf::Vector2f local_pos)
{
	auto new_text = t;
	new_text.setPosition(position + local_pos);
	text_vector.emplace_back(new_text);
}


void DrawerContainer::addShape(sf::CircleShape& sh, sf::Vector2f local_pos)
{
	auto new_shape = sh;
	new_shape.setPosition(position + local_pos);
	shape_vector.emplace_back(new_shape);
}


void DrawerContainer::addSprite(sf::Sprite& srt, sf::Vector2f local_pos)
{
	auto new_sprite = srt;
	new_sprite.setPosition(position + local_pos);
	sprite_vector.emplace_back(new_sprite);
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
		target.draw(i);
	}
	for (auto i : sprite_vector)
	{
		target.draw(i);
	}
	for (auto i : text_vector)
	{
		target.draw(i);
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
