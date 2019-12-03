#include "Drawer.h"


Drawer::~Drawer()
{
	windowThread.join();
	updateThread.join();
}


void Drawer::startDraw()
{
	windowThread = std::thread(&Drawer::drawAll, this);
	updateThread = std::thread(&Drawer::updateData, this);
}


void Drawer::updateShapes()
{
	if (points.empty()) buildVisualGraph();
	
}


void Drawer::buildVisualGraph()
{
	auto graph = Data_manager::getInstance().getMapLayer0();
	auto posts = Data_manager::getInstance().getMapLayer1();
	for (auto i : graph.getPoints())
	{
		sf::Vector2f position((float)(std::rand() % (int)w_sizeX), (float)(std::rand() % (int)w_sizeY));
		std::shared_ptr<DrawerContainer> new_point = std::make_shared<DrawerContainer>(DrawerContainer(position));
		sf::CircleShape point_shape(points_radius);
		point_shape.setFillColor(w_points_color);
		point_shape.setFillColor(w_outline_color);
		new_point->addShape(point_shape, sf::Vector2f(-points_radius, -points_radius));

		points.emplace(i.first, std::dynamic_pointer_cast<sf::Drawable, DrawerContainer>(new_point));
	}
}


void Drawer::updateData()
{
	sf::Clock clock;
	while (update_on)
	{
		if (clock.getElapsedTime().asMilliseconds() == 300)
		{
			std::unique_lock<std::mutex> lock(update_mutex);
			update_check.wait(lock, [&]() {return (this->update_window); });
			updateShapes();
			clock.restart();
			lock.release();
		}
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
	while (window.isOpen())
	{
		std::lock_guard<std::mutex> lock(update_mutex);
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
			window.draw(*p.second);
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
		lock.();
	}
}





