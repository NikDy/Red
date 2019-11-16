#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <numeric>
#include "Graph.h"
#include "Line.h"

class Drawer
{
private:
	std::map<int, sf::CircleShape> shapes_to_draw;
	std::vector<selbaward::Line> lines_to_draw;
	std::vector<sf::Text> text_to_draw;
	int w_sizeX = 800;
	int w_sizeY = 600;
	std::string w_name = "Drawer window";
	sf::Color w_background_color = sf::Color::White;
	sf::Color w_shapes_color = sf::Color::Green;
	int w_shape_scale = 2;
	sf::Font font;
	sf::VertexArray lines;

	const float lines_thickness = 3.f;
	const float camera_movement_speed = 1.5f;
	const float camera_zoom_speed = 0.005f;
	


public:
	Drawer(int w_sizeX_, int w_sizeY_, std::string w_name_, sf::Color w_background_color_, sf::Color w_shapes_color_);
	Drawer(int w_sizeX_, int w_sizeY_, std::string w_name_);
	~Drawer();

	bool graphToShapes(Graph graph);
	void drawAll();
};

