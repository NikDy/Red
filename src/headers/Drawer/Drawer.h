#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <numeric>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <Windows.h>


#include "Line.h"
#include "Forces.h"
#include "Data_manager.h"
#include "DrawerContainer.h"


class Drawer
{
private:

	std::map<int, DrawerContainer> points;
	std::vector<selbaward::Line> lines;

	float w_sizeX = std::stof(Data_manager::getInstance().config["window_W"]);
	float w_sizeY = std::stof(Data_manager::getInstance().config["window_H"]);
	std::string w_name = Data_manager::getInstance().config["window_title"];


	sf::Color w_background_color = sf::Color::White;
	sf::Color w_outline_color = sf::Color::Black;
	sf::Color w_points_color = sf::Color::White;
	sf::Color w_town_color = sf::Color(119, 136, 153);
	sf::Color w_market_color = sf::Color(154, 205, 50);
	sf::Color w_storage_color = sf::Color(189, 183, 107);


	int w_shape_scale = 2;
	sf::Font font;
	sf::Texture market_texture;
	sf::Texture storage_texture;
	sf::Texture town_texture;
	sf::Texture train_texture;

	int grid_size = 0;
	float updateTime = std::stof(Data_manager::getInstance().config["drawer_update_time"]);
	float font_size = std::stof(Data_manager::getInstance().config["font_size"]);
	float outline_thickness = std::stof(Data_manager::getInstance().config["outline_thickness"]);
	float points_radius = std::stof(Data_manager::getInstance().config["points_radius"]);
	const float lines_thickness = std::stof(Data_manager::getInstance().config["lines_thickness"]);
	const float camera_movement_speed = std::stof(Data_manager::getInstance().config["camera_move_speed"]);
	const float camera_zoom_speed = std::stof(Data_manager::getInstance().config["camera_zoom_speed"]);
	
	void reforceGraph();
	void updateShapes();
	void buildVisualGraph();
	void updatePosts();
	void updateLines();
	
	bool update_window = true;
	std::thread updateThread;
	std::thread windowThread;
	std::mutex update_mutex;
	std::condition_variable update_check;


	Drawer();
	~Drawer();
public:
	static Drawer& getInstance()
	{
		static Drawer instance;
		return instance;
	}
	Drawer(Data_manager const&) = delete;
	Drawer operator=(Drawer const&) = delete;

	void drawAll();
};

