#include "go.h"
#pragma once

class Gui
{
private:
	Game game;
	void init();
	void printBoard();
public:
	Gui();
	~Gui();
};
