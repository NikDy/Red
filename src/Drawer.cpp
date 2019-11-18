#include "Drawer.h"


Drawer::Drawer(int w_sizeX_, int w_sizeY_ , std::string w_name_, sf::Color w_background_color_, sf::Color w_shapes_color_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
	w_background_color = w_background_color_;
	w_shapes_color = w_shapes_color_;
}

Drawer::Drawer(int w_sizeX_, int w_sizeY_, std::string w_name_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
}

Drawer::~Drawer()
{
}

bool Drawer::graphToShapes(Graph _graph)
{
	Graph graph = _graph;
	if (graph.getPoints().empty()) return false;
	int rad = 50;
	int grid_size = std::ceil(std::sqrt(graph.getPoints().size()));
	int grid_mark = 0;
	for (auto point : graph.getPoints())
	{
		sf::Text text(std::to_string(point.first), font, 18);
		sf::CircleShape shape;

		text.setFillColor(sf::Color::Blue);
		shape.setRadius(float(rad));
		shape.setFillColor(w_shapes_color);
		shape.setPosition(4 * rad * (grid_mark % grid_size) + 2 * rad, 4 * rad * (grid_mark / grid_size) + 2 * rad);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(shape.getPosition().x + rad , shape.getPosition().y + rad);
		
		shapes_to_draw.emplace(point.first, shape);
		text_to_draw.emplace_back(text);
		grid_mark++;
	}
	for (auto line : graph.getLines())
	{
		auto start_pos = shapes_to_draw[line.second.points.first].getPosition();
		auto end_pos = shapes_to_draw[line.second.points.second].getPosition();
		selbaward::Line drawable_line(sf::Vector2f(start_pos.x + rad, start_pos.y + rad), sf::Vector2f(end_pos.x + rad, end_pos.y + rad));
		drawable_line.setColor(sf::Color::Black);
		drawable_line.setThickness(lines_thickness);
		lines_to_draw.emplace_back(drawable_line);
	}
	return true;
}

void Drawer::drawAll()
{
	sf::RenderWindow window(sf::VideoMode(w_sizeX, w_sizeY), w_name.c_str());
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX, w_sizeY));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		window.clear(w_background_color);
		for (auto line : lines_to_draw)
		{
			window.draw(line);
		}
		for (auto shape : shapes_to_draw)
		{
			window.draw(shape.second);
		}
		for (auto text : text_to_draw)
		{
			window.draw(text);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera.zoom(1.f + camera_zoom_speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			camera.zoom(1.f - camera_zoom_speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.move(-camera_movement_speed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			camera.move(camera_movement_speed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			camera.move(0, -camera_movement_speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			camera.move(0, camera_movement_speed);
		}



		window.setView(camera);
		window.display();
	}
}
