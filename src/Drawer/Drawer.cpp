#include "Drawer.h"
#include <Windows.h>

Drawer::Drawer(float w_sizeX_, float w_sizeY_ , std::string w_name_, sf::Color w_background_color_, sf::Color w_shapes_color_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
	w_background_color = w_background_color_;
	w_shapes_color = w_shapes_color_;
	market_texture.loadFromFile("Market.png");
	storage_texture.loadFromFile("Storage.png");
	town_texture.loadFromFile("Town.png");
	train_texture.loadFromFile("Trainn.png");
}

Drawer::Drawer(float w_sizeX_, float w_sizeY_, std::string w_name_)
{
	font.loadFromFile("font.ttf");
	w_sizeX = w_sizeX_;
	w_sizeY = w_sizeY_;
	w_name = w_name_;
	market_texture.loadFromFile("Market.png");
	storage_texture.loadFromFile("Storage.png");
	town_texture.loadFromFile("Town.png");
	train_texture.loadFromFile("Trainn.png");
}


Drawer::~Drawer()
{
	windowThread.join();
	updateThread.join();
}


bool Drawer::update(MapLayer1 mapLayer1)
{
	std::unique_lock<std::mutex> locker(update_mutex);
	layer1 = mapLayer1;
	update_window = true;
	update_check.notify_all();
	return true;
}


bool Drawer::graphToShapes(Graph _graph, MapLayer1 _layer1)
{
	graph = _graph;
	layer1 = _layer1;
	if (graph.getPoints().empty()) return false;
	grid_size = (int)std::ceil(std::sqrt(graph.getPoints().size()));
	int grid_mark = 0;
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
	marketsToShapes(layer1.getMarkets(), text_lay1_to_draw, posts_to_draw);
	townsToShapes(layer1.getTowns(), text_lay1_to_draw, posts_to_draw);
	storagesToShapes(layer1.getStorages(), text_lay1_to_draw, posts_to_draw);
	trainsToShapes(layer1.getTrains(), trains_to_draw);
	windowThread = std::thread(&Drawer::drawAll, this);
	updateThread = std::thread(&Drawer::updateShapes, this);
	return true;
}


bool Drawer::townsToShapes(std::map<int, std::shared_ptr<Town>>& towns, std::vector<sf::Text>& text_lay1_to_draw, std::map<int, sf::Sprite>& posts_to_draw)
{
	for (auto town : towns)
	{
		sf::Sprite sprite;
		sprite.setTexture(town_texture);
		sprite.setPosition(4.f * rad * ((town.first - 1) % grid_size) + 2.3f * rad, 4.f * rad * ((town.first - 1) / grid_size) + 2.3f * rad);
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
		sprite.setTexture(storage_texture);
		sprite.setPosition(4.f * rad * ((storage.first - 1) % grid_size) + 2.3f * rad, 4.f * rad * ((storage.first - 1) / grid_size) + 2.3f * rad);
		posts_to_draw.emplace(storage.first, sprite);
		sf::Text text("Storage # " + storage.second->name + " armor - " + std::to_string(storage.second->armor), font, 50);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setPosition(sprite.getPosition().x + 0.7f * rad, sprite.getPosition().y - 0.7f * rad);
		text_lay1_to_draw.emplace_back(text);
	}
	return true;
}


void Drawer::recalcLinesShapes()
{
	this->lines_to_draw.clear();
	for (auto line : graph.getLines())
	{
		auto start_pos = map_to_draw[line.second.points.first].getPosition();
		auto end_pos = map_to_draw[line.second.points.second].getPosition();
		selbaward::Line drawable_line(sf::Vector2f(start_pos.x + rad, start_pos.y + rad), sf::Vector2f(end_pos.x + rad, end_pos.y + rad));
		drawable_line.setColor(sf::Color::Black);
		drawable_line.setThickness(lines_thickness);
		this->lines_to_draw.emplace_back(drawable_line);
	}
}


bool Drawer::updateDataShapes()
{

	std::map<int, sf::Sprite> posts = std::map<int, sf::Sprite>();
	std::map<int, sf::Sprite> trains = std::map<int, sf::Sprite>();
	std::vector<sf::Text> text_lay1 = std::vector<sf::Text>();
	marketsToShapes(layer1.getMarkets(), text_lay1, posts);
	townsToShapes(layer1.getTowns(), text_lay1, posts);
	storagesToShapes(layer1.getStorages(), text_lay1, posts);
	trainsToShapes(layer1.getTrains(), trains);
	text_lay1_to_draw = text_lay1;
	posts_to_draw = posts;
	trains_to_draw = trains;
	return true;
}


void Drawer::updateShapes()
{
	while (update_on)
	{
		std::unique_lock<std::mutex> locker(update_mutex);
		update_check.wait(locker, [&]() {return (this->update_window); });
		updateDataShapes();
		this->update_window = false;
	}
}


bool Drawer::trainsToShapes(std::map<int, Train>& trains, std::map<int, sf::Sprite>& trains_to_draw)
{
	for (auto train : trains)
	{
		Graph_Line line = graph.getLineByIdx(train.second.getLineIdx());
		auto start_pos = map_to_draw[line.points.first].getPosition();
		auto end_pos = map_to_draw[line.points.second].getPosition();
		sf::Sprite sprite;
		std::string s;
		sprite.setTexture(train_texture);
		std::cout << std::endl << std::endl << train.second.position << std::endl;
		sprite.setPosition(start_pos.x + (((float)train.second.position / line.lenght) * (end_pos.x - start_pos.x)) + rad / 2.0, start_pos.y + (((float)train.second.position / line.lenght) * (end_pos.y - start_pos.y)) + rad / 2.0);
		trains_to_draw.emplace(train.first, sprite);
	}
	return true;
}


bool Drawer::marketsToShapes(std::map<int, std::shared_ptr<Market>>& markets, std::vector<sf::Text>& text_lay1_to_draw, std::map<int, sf::Sprite>& posts_to_draw)
{
	for (auto market : markets)
	{
		sf::Sprite sprite;
		sprite.setTexture(market_texture);
		sprite.setPosition(4.f * rad * ((market.first - 1) % grid_size) + 2.3f * rad, 4.f * rad * ((market.first - 1) / grid_size) + 2.3f * rad);
		posts_to_draw.emplace(market.first, sprite);
		sf::Text text("Market # " + market.second->name + " product - " + std::to_string(market.second->product), font, 50);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
		text.setPosition(sprite.getPosition().x + 0.7f * rad, sprite.getPosition().y - 0.7f * rad);
		text_lay1_to_draw.emplace_back(text);
	}
	return true;
}


void Drawer::reforceGraph()
{
	map_to_draw = Forces::recalcForces(map_to_draw);
	recalcLinesShapes();
	
}


void Drawer::drawAll()
{	
	sf::RenderWindow window(sf::VideoMode((unsigned int)w_sizeX, (unsigned int)w_sizeY), w_name.c_str());
	sf::View camera(sf::FloatRect(0.f, 0.f, w_sizeX * 3.f, w_sizeY * 3.f));
	while (window.isOpen())
	{
		std::vector<sf::Text> text_lay1_to_draw1 = text_lay1_to_draw;
		std::lock_guard<std::mutex> lock(update_mutex);
		reforceGraph();
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
		for (auto train : trains_to_draw)
		{
			window.draw(train.second);
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





