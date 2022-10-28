#include "gui.h"
#include "go.h"
#include "conio2.h"
#include <iostream>

int main() {
	// if the program is compiled in pure C, then we need to initialize the library ourselves; __cplusplus is defined only if a C++ compiler is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	Gui gui = Gui::Gui();
	Game game = Game::Game(7);
	Board board = Board::Board(7);
	board.set(0, 0, 'X');
	board.set(0, 1, 'X');
	board.set(0, 2, 'X');
	return 0;
}
