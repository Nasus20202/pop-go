#include "gui.h"
#include "go.h"
#include "conio2.h"
#include <stdlib.h>

// Represent int as a char array (string), returned pointer has to be freed
char* intToString(long long n) {
	char* string = NULL; int size = 0; bool negative = n < 0 ? true : false;
	if (negative) // Remove minus from number if negative
		n = -n;
	while (n > 0) {
		char digit = n % 10 + '0';
		n /= 10;
		string = (char*)realloc(string, sizeof(char) * (size+1));
		if (string == NULL)
			exit(1);
		string[size] = digit; size++;
	}
	if (negative) { // Add minus to the beginning of the string
		string = (char*)realloc(string, sizeof(char) * (size + 1));
		if (string == NULL)
			exit(1);
		string[size] = '-'; size++;
	}
	char* stringReversed = (char*)malloc(sizeof(char) * (size + 1));
	if (stringReversed == NULL)
		exit(1);
	for (int i = 0; i < size; i++)
		stringReversed[i] = string[size - i - 1];
	stringReversed[size] = '\0';
	free(string);
	return stringReversed;
}

void Gui::init() {
	// Prepare console output style
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
	
	char key = -1;
	while (key != 'q') {
		frame(key);
		key = getch();
	}
	
	// Restore console output style
	cputs("\n\n");
	_setcursortype(_NORMALCURSOR);
}

void Gui::frame(const char key) {
	if (key == 0) { // Pressed key is a special key - arrow
		switch (getch()) {
		case 0x48:
			y--; break;
		case 0x50:
			y++; break;
		case 0x4b:
			x--; break;
		case 0x4d:
			x++; break;
		}
		const int size = game.getBoard()->getSize();
		if(x < 0)
			x = 0;
		else if(x > size - 1)
			x = size - 1;
		if(y < 0)
			y = 0;
		else if (y > size - 1)
			y = size - 1;
	}
	printBoard();
	printCursor();
}

// Print the board on the screen
void Gui::printBoard() {
	const int size = game.getBoard()->getSize();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			textbackground(BACKGROUND); // Background color
			// Create border
			if (x == 0) {
				textcolor(FOREGROUND);
				gotoxy(BOARD_X + 2 * x - 1, BOARD_Y + y);
				putch('_');
			}
			if (y == size - 1) {
				textcolor(FOREGROUND);
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

// Print the cursor on the screen
void Gui::printCursor() {
	textbackground(THEME_COLOR);
	gotoxy(BOARD_X + 2 * x, BOARD_Y + y);
	putch(' ');
	gotoxy(BOARD_X + 2 * x + 1, BOARD_Y + y);
	putch(' ');
	textbackground(BACKGROUND);
}

Gui::Gui() {
	game = Game::Game(DEFAULT_SIZE);
	x = 0, y = 0;
}

Gui::~Gui()
{

}