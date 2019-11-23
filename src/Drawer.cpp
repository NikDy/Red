#include "Drawer.h"


Drawer::Drawer(float w_sizeX_, float w_sizeY_ , std::string w_name_, sf::Color w_background_color_, sf::Color w_shapes_color_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
	w_background_color = w_background_color_;
	w_shapes_color = w_shapes_color_;
}

Drawer::Drawer(float w_sizeX_, float w_sizeY_, std::string w_name_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
}

Drawer::~Drawer()
{
}

bool Drawer::graphToShapes(Graph _graph, MapLayer1 _layer1)
{
	Graph graph = _graph;
	MapLayer1 layer1 = _layer1;
	if (graph.getPoints().empty()) return false;
	int rad = 135;
	int grid_size = (int)std::ceil(std::sqrt(graph.getPoints().size()));
	int grid_mark = 0;
	market_texture.loadFromFile("Market.png");
	storage_texture.loadFromFile("Storage.png");
	town_texture.loadFromFile("Town.png");
	for (auto point: graph.getPoints())
	{
		sf::Text text(std::to_string(point.first), font, 50);
		sf::CircleShape shape;
		shape.setFillColor(w_shapes_color);
		shape.setOutlineColor(w_outline_color);
		shape.setOutlineThickness(6.f);
		text.setFillColor(sf::Color::Black);
		shape.setRadius(float(rad));
		shape.setPosition(float(4 * rad * (grid_mark % grid_size) + 2 * rad), float(4 * rad * (grid_mark / grid_size) + 2 * rad));
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(shape.getPosition().x + rad, shape.getPosition().y + 2.2f*rad );

		map_to_draw.emplace(point.first, shape);
		text_lay0_to_draw.emplace_back(text);
		grid_mark++;
	}
	
	for (auto line : graph.getLines())
	{
		auto start_pos = map_to_draw[line.second.points.first].getPosition();
		auto end_pos = map_to_draw[line.second.points.second].getPosition();
		selbaward::Line drawable_line(sf::Vector2f(start_pos.x + rad, start_pos.y + rad), sf::Vector2f(end_pos.x + rad, end_pos.y + rad));
		drawable_line.setColor(sf::Color::Black);
		drawable_line.setThickness(lines_thickness);
		lines_to_draw.emplace_back(drawable_line);
	}
	grid_mark = 0;
	for (auto post : layer1.getPosts())
	{
		sf::Sprite sprite;
		std::string s;
		if (post.second->getObjectType() == typeid(Market)) {			
			sprite.setTexture(market_texture);
			s = "Market # " + std::to_string(post.first);
		}
		else if (post.second->getObjectType() == typeid(Storage)) {			
			sprite.setTexture(storage_texture);
			s = "Storage # " + std::to_string(post.first);
		}
		else if (post.second->getObjectType() == typeid(Town)) {			
			sprite.setTexture(town_texture);
			s = "Town # " + std::to_string(post.first);
		}

		sprite.setPosition(4.f * rad * (grid_mark % grid_size) + 2.3f * rad, 4.f * rad * (grid_mark / grid_size) + 2.3f * rad);
		posts_to_draw.emplace(post.first, sprite);
		sf::Text text(s, font, 50);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setPosition(sprite.getPosition().x + 0.7f*rad, sprite.getPosition().y -0.7f*rad);
		text_lay1_to_draw.emplace_back(text);
		grid_mark++;
	}
	return true;
}

void Drawer::drawAll()
{	
	sf::RenderWindow window(sf::VideoMode((unsigned int)w_sizeX, (unsigned int)w_sizeY), w_name.c_str());
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX * 3.f, w_sizeY * 3.f));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();

			}
		}


		window.clear(w_background_color);
		for (auto line : lines_to_draw)
		{
			window.draw(line);
		}
		for (auto shape : map_to_draw)
		{
			window.draw(shape.second);
		}
		for (auto sprite : posts_to_draw)
		{
			window.draw(sprite.second);
		}
		for (auto text : text_lay0_to_draw)
		{
			window.draw(text);
		}
		for (auto text : text_lay1_to_draw)
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
