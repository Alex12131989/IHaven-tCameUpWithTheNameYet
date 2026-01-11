#pragma once
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <string>
#include <vector>

#include "GenericFunctions.h"

class Game
{
public:
	bool MainLoop();
	void Menu(std::vector<std::string> options);

private:
	const float menuGapPerc = 0.05f;
	const float menuOptionWPerc = 0.60f;
	const float menuOptionHPerc = 0.10f;
	int FPS = 60;
	int screenWidth = 640;
	int screenHeight = 360;
	int menuFontValue = 20;

	std::vector<std::string> welcoming_options{"Load Alias", "Start New", "Settings", "Exit"};

};
