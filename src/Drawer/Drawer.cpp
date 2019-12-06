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
<<<<<<< HEAD
		sf::Sprite sprite;
		auto position = map_to_draw[town.first].getPosition();
		sprite.setTexture(town_texture);
		sprite.setPosition(position.x + town_texture.getSize().x/4.0, position.y + town_texture.getSize().y / 4.0);
		posts_to_draw.emplace(town.first, sprite);
		sf::Text text("Town # " + town.second->name + " product - " + std::to_string(town.second->product), font, 50);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setPosition(sprite.getPosition().x + 0.7f * rad, sprite.getPosition().y - 0.7f * rad);
		text_lay1_to_draw.emplace_back(text);
	}
	return true;
}


bool Drawer::storagesToShapes(std::map<int, std::shared_ptr<Storage>>& storages, std::vector<sf::Text>& text_lay1_to_draw, std::map<int, sf::Sprite>& posts_to_draw)
{
	for (auto storage : storages)
	{
		sf::Sprite sprite;
		auto position = map_to_draw[storage.first].getPosition();
		sprite.setTexture(storage_texture);
		sprite.setPosition(position.x + storage_texture.getSize().x / 4.0, position.y + storage_texture.getSize().y / 4.0);
		posts_to_draw.emplace(storage.first, sprite);
		sf::Text text("Storage # " + storage.second->name + " armor - " + std::to_string(storage.second->armor), font, 50);
=======
		int pidx = market.second->point_idx;
		points[pidx].clearSprite();
		points[pidx].clearText();
		points[pidx].addSprite(sf::Sprite(market_texture), 
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0f, -points_radius + market_texture.getSize().y / 4.0f));
		sf::Text text(market.second->name + " : " + std::to_string(market.second->product), font, (unsigned int)(font_size));
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
>>>>>>> Task3_drawer_fix
		text.setFillColor(sf::Color::Black);
		points[pidx].addText(text, sf::Vector2f(0, -points_radius - font_size));
	}

	for (auto storage : graph_info.getStorages())
	{
		int pidx = storage.second->point_idx;
		points[pidx].clearSprite();
		points[pidx].clearText();
		points[pidx].addSprite(sf::Sprite(storage_texture),
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0f, -points_radius + market_texture.getSize().y / 4.0f));
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
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0f, -points_radius + market_texture.getSize().y / 4.0f));
		sf::Text text(town.second->name + " : " + std::to_string(town.second->armor), font, (unsigned int)(font_size));
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setFillColor(sf::Color::Black);
		points[pidx].addText(text, sf::Vector2f(0, -points_radius - font_size));
	}
}


void Drawer::updateLines()
{
	auto graph = Data_manager::getInstance().getMapLayer0();
	lines.clear();
	for (auto line : graph.getLines())
	{
<<<<<<< HEAD
		sf::Sprite sprite;
		auto position = map_to_draw[market.first].getPosition();
		sprite.setTexture(market_texture);
		sprite.setPosition(position.x + market_texture.getSize().x/4.0, position.y + market_texture.getSize().y/4.0);
		posts_to_draw.emplace(market.first, sprite);
		sf::Text text("Market # " + market.second->name + " product - " + std::to_string(market.second->product), font, 50);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setPosition(sprite.getPosition().x + 0.7f * rad, sprite.getPosition().y - 0.7f * rad);
		text_lay1_to_draw.emplace_back(text);
=======
		selbaward::Line new_line(points[line.second.points.first].position, points[line.second.points.second].position);
		new_line.setColor(sf::Color::Black);
		new_line.setThickness(lines_thickness);
		lines.emplace_back(new_line);
>>>>>>> Task3_drawer_fix
	}
}


void Drawer::reforceGraph()
{
	points = Forces::recalcForces(points);
	updateLines();
}


void Drawer::drawAll()
{
	sf::RenderWindow window(sf::VideoMode((unsigned int)w_sizeX, (unsigned int)w_sizeY), w_name.c_str());
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX * 3.f, w_sizeY * 3.f));
	updateShapes();
	sf::Clock clock;
	while (window.isOpen())
	{
		window.clear(w_background_color);
		updateLines();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		this->reforceGraph();
		if (clock.getElapsedTime().asMilliseconds() == updateTime)
		{
			updateShapes();
			clock.restart();
		}
<<<<<<< HEAD
		for (auto shape : map_to_draw)
		{
			window.draw(shape.second);
		}
		for (auto train : trains_to_draw)
		{
			window.draw(train.second);
		}
		for (auto sprite : posts_to_draw)
		{
			window.draw(sprite.second);
		}
		/*for (auto text : text_lay0_to_draw)
		{
			window.draw(text);
		}*/
		for (auto text : text_lay1_to_draw)
=======


		for (auto l : lines)
>>>>>>> Task3_drawer_fix
		{
			window.draw(l);
		}
<<<<<<< HEAD
=======
		for (auto p : points)
		{
			window.draw(p.second);
		}
		
>>>>>>> Task3_drawer_fix

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





