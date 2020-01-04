#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>



class DrawerContainer : public sf::Drawable
{
private:
	std::vector<std::pair<sf::Text, sf::Vector2f>> text_vector;
	std::vector<std::pair<sf::CircleShape, sf::Vector2f>> shape_vector;
	std::vector<std::pair<sf::Sprite, sf::Vector2f>> sprite_vector;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	DrawerContainer() {};
	DrawerContainer(sf::Vector2f pos);
	~DrawerContainer();



	sf::Vector2f position;

	void addText(sf::Text t, sf::Vector2f local_pos);
	void addShape(sf::CircleShape sh, sf::Vector2f local_pos);
	void addSprite(sf::Sprite srt, sf::Vector2f local_pos);

	void clearText();
	void clearShape();
	void clearSprite();
};