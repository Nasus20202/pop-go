#include "gui.h"
#include "go.h"

int main() {
	// if the program is compiled in pure C, then we need to initialize the library ourselves; __cplusplus is defined only if a C++ compiler is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	Gui gui = Gui::Gui();
	Game game = Game::Game(7);
	return 0;
}
