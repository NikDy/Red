#include "Data_manager.h"




bool Data_manager::loadConfigFromFile()
{
	std::ifstream cfg_file("config.cfg");
	if (!cfg_file.is_open()) return false;
	std::string arg;
	std::string val;
	std::string eql;
	while (cfg_file >> arg >> eql >> val)
	{
		config[arg] = val;
	}
	return true;
}


bool Data_manager::login(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	setLoginData(name, password, game, num_turns, num_players);

	player = std::dynamic_pointer_cast<Player, Game_object>(net.Login(login_data));
	map_layer_1 = getMapLayer1FromServer();
	takeTownsIdx();
	this->map_layer_0 = getMapLayer0FromServer();
	this->map_layer_0->createAdjacencyLists();
	this->map_layer_0_copy = *this->map_layer_0;
	this->map_layer_01 = std::shared_ptr<Graph>(&map_layer_0_copy);
	this->map_layer_01->createAdjacencyLists();
	this->map_layer_10 = getMapLayer10FromServer();
	waitUntilStart(game);
	updateThread = std::thread(&Data_manager::updateGame, this);
	return true;
}


void Data_manager::waitUntilStart(std::string game_name)
{
	std::cout << "Wait for other players" << std::endl;
	while (true)
	{
		exist_games = getGamesFromServer();
		auto it = std::find_if(exist_games->games.begin(), exist_games->games.end(), [&game_name](const Game& obj) {return obj.name == game_name; });
		if (it != exist_games->games.end())
		{
			if (it->state == 2) break;
		}
		else
		{
			std::cout << "Game was canceled :(" << std::endl;
			logout();
			break;
		}
	}
}


void Data_manager::logout()
{
	{
		std::lock_guard<std::mutex> locker(update_mutex);
		update_on = false;
	}
	if (updateThread.joinable()) {
		updateThread.join();
	}
	net.Logout();
}


/*bool Data_manager::makeMove(std::map<int, std::pair<int, int>> turn)
{

	for (auto train : turn) {
		if (train.second.second != 0) {
			net.Action(3, setMoveData(std::to_string(train.second.second), std::to_string(train.second.first), std::to_string(train.first)));
		}
	}
	return true;
}*/

bool Data_manager::makeMove(int trainIdx, int lineIdx, int speed)
{
	return net.Action(3, setMoveData(std::to_string(lineIdx), std::to_string(speed), std::to_string(trainIdx))) != nullptr;
}


bool Data_manager::tryUpgradeInGame(std::pair<std::string, int> postsToSend, std::pair<std::string, int> trainsToSend)
{

	if(net.ActionToUpgrade(postsToSend, trainsToSend)) return true;
	return false;
}

Graph& Data_manager::getMapLayer0()
{

	return *this->map_layer_0;
}


Graph & Data_manager::getMapLayer01()
{
	return *this->map_layer_01;
}

MapLayer1& Data_manager::getMapLayer1()
{
	return *this->map_layer_1;
}


MapLayer10& Data_manager::getMapLayer10()
{
	return *this->map_layer_10;
}


Player& Data_manager::getPlayer()
{
	return *this->player;
}


Data_manager::~Data_manager()
{
	updateThread.join();
	net.Logout();
}


bool Data_manager::forceTurn()
{
	std::lock_guard<std::mutex> locker(update_mutex);
	turn = true;
	while(net.forceTurn(std::pair<std::string, std::string>("", "")) == false);

	update_check.notify_one();

	return true;
}


bool Data_manager::isTown(int point_idx)
{
	auto it = std::find(townsIdx.begin(), townsIdx.end(), point_idx);
	if (it == townsIdx.end()) return false;
	return true;
}

void Data_manager::setLoginData(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	login_data.emplace_back(std::pair<std::string, std::string>("name", name));
	//login_data.emplace_back(std::pair<std::string, std::string>("password", name));
	if (game != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("game", game));
	}
	if (num_turns != 0)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("num_turns", std::to_string(num_turns)));
	}
	if (num_players != -1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("num_players", std::to_string(num_players)));
	}
}


std::vector<std::pair<std::string, std::string>> Data_manager::setMoveData(std::string lineIdx, std::string speed, std::string trainIdx)
{
	std::vector<std::pair<std::string, std::string>> move_data;
	move_data.emplace_back(std::pair<std::string, std::string>("line_idx", lineIdx));
	move_data.emplace_back(std::pair<std::string, std::string>("speed", speed));
	move_data.emplace_back(std::pair<std::string, std::string>("train_idx", trainIdx));
	return move_data;
}


std::shared_ptr<Graph> Data_manager::getMapLayer0FromServer()
{
	return std::dynamic_pointer_cast<Graph, Game_object>(net.Action(10, std::make_pair("layer", "0")));
}


std::shared_ptr<MapLayer1> Data_manager::getMapLayer1FromServer()
{
	return std::dynamic_pointer_cast<MapLayer1, Game_object>(net.Action(10, std::make_pair("layer", "1")));
}


std::shared_ptr<MapLayer10> Data_manager::getMapLayer10FromServer()
{
	return std::dynamic_pointer_cast<MapLayer10, Game_object>(net.Action(10, std::make_pair("layer", "10")));
}


std::shared_ptr<Player> Data_manager::getPlayerFromServer()
{
	return std::dynamic_pointer_cast<Player, Game_object>(net.Action(6, std::make_pair("", "")));
}

std::shared_ptr<Games> Data_manager::getGamesFromServer()
{
	return std::dynamic_pointer_cast<Games, Game_object>(net.Action(7, std::make_pair("", "")));
}


void Data_manager::updateGame()
{
	sf::Clock clock;
	while (update_on) 
	{
		std::unique_lock<std::mutex> locker(update_mutex);
		update_check.wait_for(locker, std::chrono::seconds(20), [&]() {return (this->turn); });
		map_layer_1 = getMapLayer1FromServer();
		map_layer_0 = std::make_shared<Graph>(getMapLayer01());
		player = getPlayerFromServer();
		markPoints();
		maxRating = std::max(maxRating, player->getRating());
		turn = false;
	}
}

void Data_manager::updateRefuges()
{
	Town town = player->getTown();
	for (auto& event_ : town.getEvents()) {
		last_tick_ = event_.tick;
	}
}

void Data_manager::markPoints()
{
	auto& lines = map_layer_0->getLines();
	auto& points = map_layer_0->getPoints();
	auto trains = map_layer_1->getTrains();
	for (auto train : trains) {
		Graph_Line line = map_layer_0->getLineByIdx(train.second.line_idx);
		if (train.second.position != 0 && train.second.position != line.lenght) {
			lines[line.points].trains.push_back(train.second);
		}
		if (train.second.speed == 1) {
			if (isTown(line.points.second) == false) {
				points[line.points.second].trains.push_back(train.second);
			}
		}
		else if (train.second.speed == -1) {
			if (isTown(line.points.first) == false) {
				points[line.points.first].trains.push_back(train.second);
			}
		}
		else if (train.second.speed == 0) {
			if (train.second.position == 0) {
				points[line.points.first].trains.push_back(train.second);
				for (auto point : points[line.points.first].adjacency_list) {
					Graph_Line nextLine = map_layer_0->getLineByTwoPoints(line.points.first, point);
					if (nextLine.idx != line.idx && nextLine.lenght == 1 && isTown(point) == false) points[point].trains.push_back(train.second);
				}
			}
			else if (train.second.position == line.lenght) {
				points[line.points.second].trains.push_back(train.second);
				for (auto point : points[line.points.second].adjacency_list) {
					Graph_Line nextLine = map_layer_0->getLineByTwoPoints(line.points.second, point);
					if (nextLine.idx != line.idx && nextLine.lenght == 1 && isTown(point) == false) points[point].trains.push_back(train.second);
				}
			}
			else {
				if (train.second.position == 1 && isTown(line.points.first) == false) {
					points[line.points.first].trains.push_back(train.second);
				}
				if (train.second.position == (line.lenght - 1) && isTown(line.points.second) == false) {
					points[line.points.second].trains.push_back(train.second);
				}
			}
		}

	}
}

void Data_manager::takeTownsIdx()
{
	for (auto& town : map_layer_1->getTowns()) {
		townsIdx.push_back(town.second->point_idx);
	}
}
