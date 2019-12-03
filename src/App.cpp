#include "Drawer.h"
#include "Json_Parser.h"
#include <string>
#include <iostream>
#include "Network_manager.h"
#include "Data_manager.h"
#include "Regulator.h"
#include <Windows.h>
#include <mutex>
#include <thread>


int main(int argc, char* argv[])
{
	Data_manager::getInstance().loadConfigFromFile();
	Data_manager::getInstance().login("nik");
	Drawer::getInstance().startDraw();
	return 0;
}