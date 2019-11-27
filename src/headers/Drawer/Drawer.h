#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <numeric>
#include "Line.h"

#include "Data_manager.h"

class Drawer
{
private:
	std::map<int, sf::CircleShape> map_to_draw;
	std::map<int, sf::Sprite> posts_to_draw;
	std::vector<selbaward::Line> lines_to_draw;
	std::vector<sf::Text> text_lay0_to_draw;
	std::vector<sf::Text> text_lay1_to_draw;
	float w_sizeX = 800.f;
	float w_sizeY = 600.f;
	std::string w_name = "Drawer window";
	sf::Color w_background_color = sf::Color::White;
	sf::Color w_outline_color = sf::Color::Black;
	sf::Color w_shapes_color = sf::Color::White;
	sf::Color w_town_color = sf::Color(119, 136, 153);
	sf::Color w_market_color = sf::Color(154, 205, 50);
	sf::Color w_storage_color = sf::Color(189, 183, 107);
	int w_shape_scale = 2;
	sf::Font font;
	sf::VertexArray lines;
	sf::Texture market_texture;
	sf::Texture storage_texture;
	sf::Texture town_texture;

	const float lines_thickness = 4.f;
	const float camera_movement_speed = 1.5f;
	const float camera_zoom_speed = 0.003f;
	


public:
	Drawer(float w_sizeX_, float w_sizeY_, std::string w_name_, sf::Color w_background_color_, sf::Color w_shapes_color_);
	Drawer(float w_sizeX_, float w_sizeY_, std::string w_name_);
	~Drawer();

	bool graphToShapes(Graph graph, MapLayer1 mapLayer1);
	void drawAll();
};

