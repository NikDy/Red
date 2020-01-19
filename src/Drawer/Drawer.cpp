#include "Drawer.h"


Drawer::Drawer()
{
	font.loadFromFile(Data_manager::getInstance().config["font"]);
	market_texture.loadFromFile(Data_manager::getInstance().config["market_texture"]);
	storage_texture.loadFromFile(Data_manager::getInstance().config["storage_texture"]);
	town_texture.loadFromFile(Data_manager::getInstance().config["town_texture"]);
	train_texture.loadFromFile(Data_manager::getInstance().config["train_texture"]);
	enemy_texture.loadFromFile(Data_manager::getInstance().config["enemy_texture"]);
}


Drawer::~Drawer()
{
	windowThread.join();
}


void Drawer::updateShapes()
{
	std::lock_guard<std::mutex> locker(Data_manager::getInstance().update_mutex);
	if (points.empty()) buildVisualGraph();
	updatePosts();
	updateTrains();
}


void Drawer::buildVisualGraph()
{
	auto graph = Data_manager::getInstance().getMapLayer10();
	for (auto i : graph.points)
	{
		sf::Vector2f position((float)i.second.first * std::stof(Data_manager::getInstance().config["RANGE_SCALE"]),
							  (float)i.second.second * std::stof(Data_manager::getInstance().config["RANGE_SCALE"]));
		DrawerContainer new_point = DrawerContainer(position);
		sf::CircleShape point_shape(points_radius);
		point_shape.setOutlineColor(w_outline_color);
		point_shape.setOutlineThickness(outline_thickness);
		new_point.addShape(point_shape, sf::Vector2f(-points_radius, -points_radius));
		sf::Text text(std::to_string(i.first), font, (unsigned int)(font_size));
		text.setFillColor(sf::Color::Black);
		new_point.addText(text, sf::Vector2f(0, -points_radius - font_size));

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
			sf::Vector2f(-points_radius + market_texture.getSize().x / 4.0f, -points_radius + market_texture.getSize().y / 4.0f));
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
		sf::Text text1(town.second->name + " : " + std::to_string(town.second->armor) + " : " + std::to_string(town.second->product),
			font, (unsigned int)(font_size));
		text1.setOrigin(text1.getLocalBounds().width / 2.f, text1.getLocalBounds().height / 2.f);
		text1.setFillColor(sf::Color::Black);
		points[pidx].addText(text1, sf::Vector2f(0, -points_radius - font_size));
		sf::Text text2("pop : " + std::to_string(town.second->population), font, (unsigned int)(font_size));
		text2.setOrigin(text2.getLocalBounds().width / 2.f, text2.getLocalBounds().height / 2.f);
		text2.setFillColor(sf::Color::Black);
		points[pidx].addText(text2, sf::Vector2f(0, -points_radius - 2 * font_size));
	}

}


void Drawer::updateGui()
{
	gui.clear();
	for (auto rating : Data_manager::getInstance().getMapLayer1().getRaiting())
	{
		DrawerContainer ui = DrawerContainer(sf::Vector2f(-10000.f, -10000.f - gui.size() * font_size));
		sf::Text text(rating.second.name + " : " + std::to_string(rating.second.rating), font, (unsigned int)(font_size));
		text.setFillColor(sf::Color::Black);
		ui.addText(text, sf::Vector2f(0, 0));
		gui.emplace(rating.second.idx, ui);
	}
}


void Drawer::updateTrains()
{
	auto graph_info = Data_manager::getInstance().getMapLayer1();
	auto graph = Data_manager::getInstance().getMapLayer0();
	auto player = Data_manager::getInstance().getPlayer();
	for (auto train : graph_info.getTrains())
	{
		Graph_Line line = graph.getLineByIdx(train.second.getLineIdx());
		auto start_pos = points[line.points.first].position;
		auto end_pos = points[line.points.second].position;
		int pidx = train.second.getIdx();
		trains[pidx].clearSprite();
		if (player.getIdx() == train.second.getPlayerIdx())
		{
			trains[pidx].addSprite(sf::Sprite(train_texture),
				sf::Vector2f(start_pos.x + (((float)train.second.position / line.lenght) * (end_pos.x - start_pos.x)) - train_texture.getSize().x / 2.f, start_pos.y + (((float)train.second.position / line.lenght) * (end_pos.y - start_pos.y)) - train_texture.getSize().y / 2.f));
		}
		else
		{
			trains[pidx].addSprite(sf::Sprite(enemy_texture),
				sf::Vector2f(start_pos.x + (((float)train.second.position / line.lenght) * (end_pos.x - start_pos.x)) - train_texture.getSize().x / 2.f, start_pos.y + (((float)train.second.position / line.lenght) * (end_pos.y - start_pos.y)) - train_texture.getSize().y / 2.f));
		}
	}
}


void Drawer::updateLines()
{
	auto graph = Data_manager::getInstance().getMapLayer01();
	lines.clear();
	for (auto line : graph.getLines())
	{
		selbaward::Line new_line(points[line.second.points.first].position, points[line.second.points.second].position);
		new_line.setColor(sf::Color::Black);
		new_line.setThickness(lines_thickness);
		lines.emplace_back(new_line);
	}
}


void Drawer::reforceGraph()
{
	if (!is_graph_stable)
	{
		points = Forces::recalcForces(points, is_graph_stable);
		updateLines();
	}
}


sf::FloatRect Drawer::graphCenter()
{
	float max_h = 0.f;
	float min_h = 0.f;
	float max_w = 0.f;
	float min_w = 0.f;
	bool first = true;
	for (auto point : points)
	{
		if (first)
		{
			max_h = point.second.position.y;
			min_h = point.second.position.y;
			max_w = point.second.position.x;
			min_w = point.second.position.x;
			first = false;
		}
		else
		{
			if (point.second.position.y > max_h) max_h = point.second.position.y;
			if (point.second.position.y < min_h) min_h = point.second.position.y;
			if (point.second.position.x > max_w) max_w = point.second.position.x;
			if (point.second.position.x < min_w) min_w = point.second.position.x;
		}
	}
	return sf::FloatRect(max_w, max_h, max_w - min_w, max_h - min_h);
}


void Drawer::draw()
{
		windowThread = std::thread(&Drawer::drawAll, this);
}

void Drawer::drawAll()
{
	sf::RenderWindow window(sf::VideoMode((unsigned int)w_sizeX, (unsigned int)w_sizeY), w_name.c_str());
	window.setFramerateLimit(60u);
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX * 3.f, w_sizeY * 3.f));
	updateShapes();
	updateLines();
	sf::Clock clock;
	while (window.isOpen())
	{
		window.clear(w_background_color);
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Data_manager::getInstance().logout();
				window.close();
			}
		}

		if (Data_manager::getInstance().turn == false) {
				updateGui();
				updateShapes();
				clock.restart();
		}

		for (auto l : lines)
		{
			window.draw(l);
		}
		for (auto p : points)
		{
			window.draw(p.second);
		}
		for (auto t : trains)
		{
			window.draw(t.second);
		}
		for (auto g : gui)
		{
			window.draw(g.second);
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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			auto new_size = graphCenter();
			camera.setCenter(new_size.top - new_size.height / 2, new_size.left - new_size.width / 2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			auto home_town_position = points[Data_manager::getInstance().getPlayer().getTown().point_idx].position;
			camera.setCenter(home_town_position.x, home_town_position.y);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
		{
			camera.setCenter(-10000.f, -10000.f);
		}
		window.setView(camera);
		window.display();
	}
}





