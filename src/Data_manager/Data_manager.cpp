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


	net.Login(login_data);
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	player = std::dynamic_pointer_cast<Player, Game_object>(list_objects.back());
	map_layer_1 = getMapLayer1FromServer();
	this->map_layer_0 = getMapLayer0FromServer();
	this->map_layer_0->createAdjacencyLists();
	this->map_layer_01 = this->map_layer_0;
	this->map_layer_01->createAdjacencyLists();
	//////////////

	//////////////
	updateThread = std::thread(&Data_manager::updateGame, this);
	return true;
}

void Data_manager::logout()
{
	net.Logout();
}


bool Data_manager::makeMove(std::map<int, std::pair<int, int>> turn)
{
	for (auto train : map_layer_1->getTrains())
	{
		if (train.second.cooldown != 0) std::cout << "collision!" << std::endl;
	}
	for (auto train : turn) {
		if (train.second.second != 0) {
			net.Action(3, setMoveData(std::to_string(train.second.second), std::to_string(train.second.first), std::to_string(train.first)));
		}
	}
	return true;
}


bool Data_manager::tryUpdateInGame()
{
	if (stopUpdate == false) {
		int armor_in_towm = player->getTown().armor;
		std::vector<int> trains;
		std::vector<int> posts;
		int count = 0;
		for (auto train : player->getTrains()) {
			if (train.second.next_level_price == 0) count++;
		}
		if (player->getTown().next_level_price == 0) count++;
		if(count == (player->getTrains().size() +1)) stopUpdate = true;
		for (auto train : player->getTrains()) {
			if (train.second.next_level_price <= armor_in_towm && train.second.next_level_price != 0) {
				trains.push_back(train.first);
				armor_in_towm -= train.second.next_level_price;
			}
		}
		if (player->getTown().next_level_price <= armor_in_towm && player->getTown().next_level_price != 0) {
			posts.push_back(player->getTown().idx);
		}
		if (posts.size() != 0 || trains.size() != 0) {
			auto postsToSend = std::make_pair("posts", posts);
			auto trainsToSend = std::make_pair("trains", trains);
			net.ActionToUpdate(postsToSend, trainsToSend);
		}
	}
	return true;
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
	while (!net.Action(5, std::pair<std::string, std::string>("", "")));

	update_check.notify_one();

	return true;
}


void Data_manager::setLoginData(std::string name, std::string password, std::string game, int num_turns, int num_players)
{
	login_data.emplace_back(std::pair<std::string, std::string>("name", name));
	if (password != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", password));
	}
	if (game != "")
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", game));
	}
	if (num_turns != -1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_turns)));
	}
	if (num_players != -1)
	{
		login_data.emplace_back(std::pair<std::string, std::string>("", std::to_string(num_players)));
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
	net.Action(10, std::make_pair("layer", "0"));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	return std::dynamic_pointer_cast<Graph, Game_object>(list_objects.back());
}


std::shared_ptr<MapLayer1> Data_manager::getMapLayer1FromServer()
{
	net.Action(10, std::make_pair("layer", "1"));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	if (!list_objects.empty()) return std::dynamic_pointer_cast<MapLayer1, Game_object>(list_objects.back());
	else return NULL;
}


std::shared_ptr<Player> Data_manager::getPlayerFromServer()
{
	net.Action(6, std::make_pair("", ""));
	std::list<std::shared_ptr<Game_object>> list_objects = net.getResponseList();
	if (!list_objects.empty()) return std::dynamic_pointer_cast<Player, Game_object>(list_objects.back());
	else return NULL;
}


void Data_manager::updateGame()
{
	sf::Clock clock;
	while (update_on) 
	{
		std::unique_lock<std::mutex> locker(update_mutex);
		update_check.wait_for(locker, std::chrono::seconds(60), [&]() {return (this->turn); });
		map_layer_1 = getMapLayer1FromServer();
		map_layer_0 = getMapLayer0FromServer();
		this->map_layer_0->createAdjacencyLists();
		player = getPlayerFromServer();
		markPoints();
		updateRefuges();
		turn = false;
	}
}

void Data_manager::updateRefuges()
{
	Town town = player->getTown();
	for (auto& event_ : town.getEvents()) {
		if (event_.type == int(EVENT_TYPE::REFUGEES_ARRIVAL)) {
			last_tick_Refuges = 0;
			count_Refuges = event_.value;
		}
	}
}

void Data_manager::markPoints()
{
	auto& lines = map_layer_0->getLines();
	auto& points = map_layer_0->getPoints();
	auto& trains = map_layer_1->getTrains();
	for (auto& train : trains) {
		Graph_Line line = map_layer_0->getLineByIdx(train.second.line_idx);
		if (train.second.position != 0 && train.second.position != line.lenght) {
			lines[line.points].trains.push_back(train.second);
		}
		if (train.second.speed == 1) {
			if (line.lenght - train.second.position <= 2 && line.points.second != player->getHome().idx) {
				points[line.points.second].pointBusy = true;
				points[line.points.second].trainBusy = train.first;
				points[line.points.second].trains.push_back(train.second);
			}
		}
		else if (train.second.speed == -1) {
			if (train.second.position <= 2 && line.points.first != player->getHome().idx) {
				points[line.points.first].pointBusy = true;
				points[line.points.first].trainBusy = train.first;
				points[line.points.first].trains.push_back(train.second);
			}
		}

	}
}
