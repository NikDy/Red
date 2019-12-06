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
	//float range = 3;
	//float angle = -2.35619;
	//
	//Forces::rangeAngleToVector(range, angle);

	Data_manager::getInstance().loadConfigFromFile();
	Data_manager::getInstance().login("nik");
	Drawer::getInstance().drawAll();
	return 0;
}