#include "gui.h"
#include "go.h"
#include "conio2.h"

#define TITLE "Krzysztof Nasuta, 193328"
#define MENU_X 2
#define MENU_Y 2
#define BOARD_X 25
#define BOARD_Y 2
#define BOARD_SIZE 19
#define BACKGROUND DARKGRAY
#define FOREGROUND BLACK

void Gui::init() {
	printBoard();
}

void Gui::frame() {
	
}

// Print the board on the screen
void Gui::printBoard() {
	const int size = game.getBoard()->getSize();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			textbackground(BACKGROUND); // Background color
			// Create border
			if (x == 0) {
				textcolor(BLACK);
				gotoxy(BOARD_X + 2 * x - 1, BOARD_Y + y);
				putch('_');
			}
			if (y == size - 1) {
				textcolor(BLACK);
				gotoxy(BOARD_X + 2 * x, BOARD_Y + y + 1);
				cputs("| ");
			}
			if (x == 0 && y == size - 1) {
				gotoxy(BOARD_X + 2 * x - 1, BOARD_Y + y + 1);
				putch(' ');
			}
			gotoxy(BOARD_X + 2*x, BOARD_Y + y); // Move the cursor to the right position, 2*x for nice output
			char c = game.getBoard()->get(y, x); // get state of field
			if (c == WHITE_STATE) {
				textbackground(WHITE); textcolor(WHITE);
				putch(' ');
			}
			else if (c == BLACK_STATE) {
				textbackground(BLACK); textcolor(BLACK);
				putch(' ');
			}
			else {
				textcolor(FOREGROUND); putch('|');
			}
			gotoxy(BOARD_X + 2*x + 1, BOARD_Y + y); putch('_'); // Stone is 2x1 chars
		}
	}
	textcolor(LIGHTGRAY);
	textbackground(BLACK); //reset colors
}

Gui::Gui() {
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
	game = Game::Game(DEFAULT_SIZE);
}

Gui::~Gui()
{
	_setcursortype(_NORMALCURSOR);
}