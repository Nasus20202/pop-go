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
	Board* board = game.getBoard();
	Board board2 = Board::Board(7);
	board->set(0, 0, 'X');
	board2[0][0] = 'X';
	std::cout << (*board == board2) << std::endl;
	return 0;
}
