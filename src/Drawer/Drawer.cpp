#include "Drawer.h"


Drawer::Drawer()
{
	font.loadFromFile(Data_manager::getInstance().config["font"]);
	market_texture.loadFromFile(Data_manager::getInstance().config["market_texture"]);
	storage_texture.loadFromFile(Data_manager::getInstance().config["storage_texture"]);
	town_texture.loadFromFile(Data_manager::getInstance().config["town_texture"]);
	train_texture.loadFromFile(Data_manager::getInstance().config["train_texture"]);
}


Drawer::~Drawer()
{
}


void Drawer::updateShapes()
{
	if (points.empty()) buildVisualGraph();
	updatePosts();
}


void Drawer::buildVisualGraph()
{
	auto graph = Data_manager::getInstance().getMapLayer0();
	for (auto i : graph.getPoints())
	{
		sf::Vector2f position((float)(std::rand() % (int)w_sizeX), (float)(std::rand() % (int)w_sizeY));
		DrawerContainer new_point = DrawerContainer(position);
		sf::CircleShape point_shape(points_radius);
		point_shape.setFillColor(w_points_color);
		point_shape.setOutlineColor(w_outline_color);
		point_shape.setOutlineThickness(outline_thickness);
		new_point.addShape(point_shape, sf::Vector2f(-points_radius, -points_radius));

		points.emplace(i.first, new_point);
	}
}


void Drawer::updatePosts()
{
	auto graph_info = Data_manager::getInstance().getMapLayer1();
	for (auto market : graph_info.getMarkets())
	{
		int pidx = market.second->point_idx;
		points[pidx].clearSprite();
		points[pidx].clearText();
		points[pidx].addSprite(sf::Sprite(market_texture), 
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0, -points_radius + market_texture.getSize().y / 4.0));
		sf::Text text(market.second->name + " : " + std::to_string(market.second->product), font, (unsigned int)(font_size));
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setFillColor(sf::Color::Black);
		points[pidx].addText(text, sf::Vector2f(0, -points_radius - font_size));
	}

	for (auto storage : graph_info.getStorages())
	{
		int pidx = storage.second->point_idx;
		points[pidx].clearSprite();
		points[pidx].clearText();
		points[pidx].addSprite(sf::Sprite(storage_texture),
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0, -points_radius + market_texture.getSize().y / 4.0));
		sf::Text text(storage.second->name + " : " + std::to_string(storage.second->armor), font, (unsigned int)(font_size));
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setFillColor(sf::Color::Black);
		points[pidx].addText(text, sf::Vector2f(0, -points_radius - font_size));
	}

	for (auto town : graph_info.getTowns())
	{
		int pidx = town.second->point_idx;
		points[pidx].clearSprite();
		points[pidx].clearText();
		points[pidx].addSprite(sf::Sprite(town_texture),
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0, -points_radius + market_texture.getSize().y / 4.0));
		sf::Text text(town.second->name + " : " + std::to_string(town.second->armor), font, (unsigned int)(font_size));
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setFillColor(sf::Color::Black);
		points[pidx].addText(text, sf::Vector2f(0, -points_radius - font_size));
	}
}


//void Drawer::reforceGraph()
//{
//	visual_graph = Forces::recalcForces(visual_graph);
//	recalcLinesShapes();
//}


void Drawer::drawAll()
{
	sf::RenderWindow window(sf::VideoMode((unsigned int)w_sizeX, (unsigned int)w_sizeY), w_name.c_str());
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX * 3.f, w_sizeY * 3.f));
	this->updateShapes();
	while (window.isOpen())
	{
		window.clear(w_background_color);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		for (auto p : points)
		{
			window.draw(p.second);
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





