#include "go.h"
#pragma once

class Gui
{
private:
	Game game;
	void printBoard();
	void frame();
public:
	Gui();
	~Gui();
	void init();
};
