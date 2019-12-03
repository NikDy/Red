#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>



class DrawableContainer : public sf::Drawable
{
private:
	DrawableContainer();
	~DrawableContainer();

	std::vector<sf::Text> text_vector;
	std::vector<std::shared_ptr<sf::Shape>> shape_vector;
	std::vector<sf::Sprite> sprite_vector;

public:
	
	sf::Vector2f position;
	sf::Vector2f bounds;



};