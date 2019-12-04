#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>



class DrawerContainer : public sf::Drawable
{
private:
	std::vector<sf::Text> text_vector;
	std::vector<sf::CircleShape> shape_vector;
	std::vector<sf::Sprite> sprite_vector;
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


	//std::vector<sf::Drawable&> getAllDrawable();
};