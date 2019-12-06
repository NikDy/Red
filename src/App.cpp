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
#include "Forces.h"


int main()
{
	Data_manager::getInstance().loadConfigFromFile();
	Data_manager::getInstance().login("nik");
	Drawer::getInstance().drawAll();
	return 0;
}