#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>



class DrawerContainer : protected sf::Drawable
{
private:
	std::vector<sf::Text> text_vector;
	std::vector<sf::CircleShape> shape_vector;
	std::vector<sf::Sprite> sprite_vector;

public:
	DrawerContainer(sf::Vector2f pos);
	~DrawerContainer();



	sf::Vector2f position;

	void addText(sf::Text& t, sf::Vector2f local_pos);
	void addShape(sf::CircleShape& sh, sf::Vector2f local_pos);
	void addSprite(sf::Sprite& srt, sf::Vector2f local_pos);

	void clearText();
	void clearShape();
	void clearSprite();


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//std::vector<sf::Drawable&> getAllDrawable();
};