#include "gui.h"
#include "go.h"
#include "conio2.h"
#include <stdlib.h>

// Reverse a char array
void reverseString(char* string, int size) {
	char* stringReversed = (char*)malloc(sizeof(char) * (size));
	if (stringReversed == NULL)
		exit(1);
	for (int i = 0; i < size; i++)
		stringReversed[i] = string[size - i - 1];
	for (int i = 0; i < size; i++)
		string[i] = stringReversed[i];
	free(stringReversed);
}

// Represent int as a char array (string), returned pointer has to be freed
char* intToString(long long n) {
	if (n == 0) {
		// if c is 0, rest of that function would return empty array of chars
		char* c = (char*) malloc(sizeof(char) * 2);
		if (c == NULL)
			exit(1);
		c[0] = '0'; c[1] = '\0';
		return c;
	}
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
	reverseString(string, size);
	string = (char*)realloc(string, sizeof(char) * (size + 1));
	string[size] = '\0';
	return string;
}

// Represnt double as a char array (string), returned pointer has to be freed, precision is the max number of consecutive zeros in decimal part
char* doubleToString(double n, int precision = 2) {
	if (n == 0) {
		// if c is 0, rest of that function would return empty array of chars
		char* c = (char*)malloc(sizeof(char) * 2);
		if (c == NULL)
			exit(1);
		c[0] = '0'; c[1] = '\0';
		return c;
	}
	char* string = NULL; int size = 0; bool negative = n < 0 ? true : false;
	if (negative) // Remove minus from number if negative
		n = -n;
	int integerPart = (int)n;
	double decimalPart = n - integerPart;
	if (integerPart == 0) {
		string = (char*)realloc(string, sizeof(char) * (size + 1));
		if (string == NULL)
			exit(1);
		string[0] = '0'; size++;
	}
	while (integerPart > 0) {
		char digit = integerPart % 10 + '0';
		integerPart /= 10;
		string = (char*)realloc(string, sizeof(char) * (size + 1));
		if (string == NULL)
			exit(1);
		string[size] = digit; size++;
	}
	reverseString(string, size);
	double eps = 1;
	for (int i = 0; i < precision; i++)
		eps /= 10;
	if (decimalPart > eps) {
		string = (char*)realloc(string, sizeof(char) * (size + 1));
		if (string == NULL)
			exit(1);
		string[size] = '.'; size++;
		while (decimalPart > eps) {
			char digit = (int)(decimalPart * 10) + '0';
			decimalPart = decimalPart * 10 - (int)(decimalPart * 10);
			string = (char*)realloc(string, sizeof(char) * (size + 1));
			if (string == NULL)
				exit(1);
			string[size] = digit; size++;
		}
	}
	string = (char*)realloc(string, sizeof(char) * (size + 1));
	if (string == NULL)
		exit(1);
	string[size] = '\0'; size += 1;
	return string;
}

void Gui::init() {
	// Prepare console output style
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
	textcolor(FOREGROUND);
	textbackground(CONSOLE_COLOR);
	clrscr();
	printMenu();
	char key = -1;
	while (key != 'q') {
		frame(key);
		key = getch();
	}
	// Restore console output style
	_setcursortype(_NORMALCURSOR);
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	clrscr();
}

void Gui::frame(const char key) {
	// Pressed key is a special key - arrow
	if (key == 0) { 
		move();
	}
	// Place a new stone
	else if (key == '1') {
		placeStone();
	}
	// Create new game
	else if (key == 'n') {
		newGame();
	}
	printGameBoard();
	printStats();
	if (game.getPoints(WHITE_STATE) > 0)
		int a = 0;
}

// Allows to change x and y
void Gui::move() {
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
	if (x < 0)
		x = 0;
	else if (x > size - 1)
		x = size - 1;
	if (y < 0)
		y = 0;
	else if (y > size - 1)
		y = size - 1;
}

// Allows placing a stone
void Gui::placeStone() {
	bool validMove = game.checkIfLegalMove(y, x);
	if (validMove) {
		Board board = Board::Board(game.getBoard()->getSize());
		board = *game.getBoard();
		board.set(y, x, game.getCurrentPlayer());
		printBoard(&board, false);
		char confirm = getch();
		// Confirmation by clicking enter
		if (confirm == 0x0d) {
			game.placeStone(y, x);
		}
	}
}

// Allows to start a new game
void Gui::newGame() {
	char option = 0, c = 0; int n;
	char basicSizes[] = { 9, 13, 19 }; // basic sizes of board
	int basicSizesCount = sizeof(basicSizes) / sizeof(char); int customInputX, customInputY; // For custom number input location
	// Wait for enter or esc
	while (c != 0x0d && c != 0x1B) {
		// Create popup for user to choose size 
		int popupX = POPUP_X, popupY = POPUP_Y;
		createPopup(popupX, popupY, 6 + basicSizesCount, 20);
		textcolor(FOREGROUND); textbackground(THEME_COLOR);
		popupX++; popupY++;
		gotoxy(popupX, popupY);
		cputs("  Wybierz rozmiar"); popupY++;
		gotoxy(popupX, popupY);
		cputs("  planszy:"); popupY += 2;
		for (int i = 0; i < basicSizesCount; i++) {
			gotoxy(popupX, popupY);
			if (option == i)
				cputs("    > ");
			else
				cputs("      ");
			printInt(basicSizes[i]); cputs("x"); printInt(basicSizes[i]);
			popupY++;
		}
		gotoxy(popupX, popupY);
		if (option == basicSizesCount)
			cputs("    > ");
		else
			cputs("      ");
		cputs("Dowolna");
		c = getch(); // choose from menu
		if (c == 0) {
			switch (getch()) {
			case 0x48:
				option--; break;
			case 0x50:
				option++; break;
			}
			if (option < 0)
				option += basicSizesCount + 1;
			option %= basicSizesCount + 1;
		}
		customInputX = popupX, customInputY = popupY;
	}
	if (option < basicSizesCount)
		n = basicSizes[option];
	else {
		gotoxy(customInputX, customInputY); cputs("               ");
		n = numberInput(customInputX + 6, customInputY, FOREGROUND, THEME_COLOR, false, MAX_SIZE, 0);
	}
	// If ESC not pressed
	if (c != 0x1B) {
		game.newBoard(n);
		x = 0, y = 0;
	}
	textcolor(FOREGROUND); textbackground(CONSOLE_COLOR);
	clrscr();
	printMenu();
}

// Print static menu
void Gui::printMenu() {
	textbackground(THEME_COLOR);
	int menuY = MENU_Y, menuX;
	// prepare nice char array to reduce fliccer
	char* menuBackground = (char*)malloc((MENU_WIDTH + 1) * sizeof(char));
	if (menuBackground == NULL)
		exit(1); // allocation error
	for (int i = 0; i < MENU_HEIGHT; i++) {
		menuBackground[i] = ' ';
	}
	menuBackground[MENU_WIDTH] = '\0'; // remove last new line
	for (int i = 0; i < MENU_WIDTH; i++) {
		gotoxy(MENU_X, menuY++);
		cputs(menuBackground);
	}
	free(menuBackground);
	const char* info[] = { "q - zakoncz program", "n - nowa gra", "Strzalki - ruch", "1 - wybierz pole", "ENTER - potwierdz pole"}; // Custom info on menu
	const int infoCount = 5; // number of elements in that array
	menuX = MENU_X + 4, menuY = MENU_Y + 3;
	for (int i = 0; i < infoCount; i++) {
		gotoxy(menuX, menuY++);
		cputs(info[i]);
	}
}

// Prints points and turn
void Gui::printStats() {
	textbackground(THEME_COLOR);
	textcolor(CONSOLE_COLOR);
	gotoxy(STATS_X, STATS_Y);
	if (game.getCurrentPlayer() == BLACK_STATE)
		cputs("   Gracz: Czarny");
	else
		cputs("   Gracz: Bialy ");
	gotoxy(STATS_X, STATS_Y+2);
	for (int i = 0; i < MENU_WIDTH - 5; i++)
		cputs(" "); // Clear space for points
	gotoxy(STATS_X, STATS_Y + 2);
	cputs("Czarny: "); printDouble(game.getPoints(BLACK_STATE)); cputs("  Bialy: "); printDouble(game.getPoints(WHITE_STATE));
	gotoxy(STATS_X, STATS_Y + 4);
	for (int i = 0; i < MENU_WIDTH - 5; i++)
		cputs(" "); // Clear space for coors
	gotoxy(STATS_X, STATS_Y + 4);
	cputs("   X: "); printInt(x); cputs("   Y: "); printInt(y);
}

// Print the board on the screen
void Gui::printBoard(Board* board, bool cursor) {
	const int size = board->getSize();
	// Print whole board
	if (size <= BOARD_SIZE) {
		for (int localX = 0; localX < size; localX++) {
			for (int localY = 0; localY < size; localY++) {
				textbackground(BACKGROUND); // Background color
				// Create border
				if (localX == 0) {
					textcolor(FOREGROUND);
					gotoxy(BOARD_X + 2 * localX - 1, BOARD_Y + localY);
					putch('_');
				}
				if (localY == size - 1) {
					textcolor(FOREGROUND);
					gotoxy(BOARD_X + 2 * localX, BOARD_Y + localY + 1);
					cputs("| ");
				}
				if (localX == 0 && localY == size - 1) {
					gotoxy(BOARD_X + 2 * localX - 1, BOARD_Y + localY + 1);
					putch(' ');
				}
				gotoxy(BOARD_X + 2 * localX, BOARD_Y + localY); // Move the cursor to the right position, 2*x for nice output
				char c = board->get(localY, localX); // get state of field
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
				gotoxy(BOARD_X + 2 * localX + 1, BOARD_Y + localY); putch('_'); // Stone is 2x1 chars
			}
		}
		// Print cursor
		if (cursor) {
			textbackground(THEME_COLOR);
			gotoxy(BOARD_X + 2 * x, BOARD_Y + y);
			putch(' ');
		}
	} 
	// Print part of the board
	else {
		int startX = x - BOARD_SIZE / 2, startY = y - BOARD_SIZE / 2; // Print board starting from startX and startY
		if(startX < 0)
			startX = 0;
		else if (startX > size - BOARD_SIZE)
			startX = size - BOARD_SIZE;
		if (startY < 0)
			startY = 0;
		else if(startY > size - BOARD_SIZE)
			startY = size - BOARD_SIZE;
		for (int localX = startX; localX < startX + BOARD_SIZE; localX++) {
			for (int localY = startY; localY < startY + BOARD_SIZE; localY++) {
				textbackground(BACKGROUND);
				gotoxy(BOARD_X + 2 * (localX - startX), BOARD_Y + (localY - startY)); // Move the cursor to the right position, 2*x for nice output
				char c = board->get(localY, localX); // get state of field
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
				gotoxy(BOARD_X + 2 * (localX - startX) + 1, BOARD_Y + (localY - startY)); putch('_'); // Stone is 2x1 chars
			}
		}
		// Print cursor
		if (cursor) {
			textbackground(THEME_COLOR);
			gotoxy(BOARD_X + 2 * (x - startX), BOARD_Y + (y - startY));
			putch(' ');
		}
	}
	textcolor(THEME_COLOR);
	textbackground(BLACK); //reset colors
}

// Print the game's board
void Gui::printGameBoard(bool cursor) {
	printBoard(game.getBoard(), cursor);
}

void Gui::createPopup(const int x, const int y, const int height, const int width) {
	textbackground(THEME_COLOR);
	textcolor(FOREGROUND);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			gotoxy(x + i, y + j); putch(' ');
		}
	}
	textcolor(FOREGROUND);
	textbackground(CONSOLE_COLOR);
}

// Create input for a number
int Gui::numberInput(const int x, const int y, const char fontColor, const char backgroundColor, const bool negative, const int maxValue, const int minValue) {
	textcolor(fontColor); textbackground(backgroundColor);
	int maxLength = 0, temp = maxValue, n = 0; char c = 0;
	if (-minValue > maxValue)
		temp = -minValue;
	while (temp > 0) {
		temp /= 10;
		maxLength++;
	}
	maxLength += negative; // Need space for '-'
	while (c != 0x0d || n < minValue || n > maxValue) {
		gotoxy(x, y);
		for (int i = 0; i < maxLength; i++)
			cputs(" ");
		gotoxy(x, y);
		printInt(n);
		c = getch();
		if (c == '\b')
			n /= 10; // backspace
		else if (c >= '0' && c <= '9') {
			n *= 10; n += c - '0';
			if (n > maxValue)
				n = maxValue;
		}
		else if (c == '-' && negative) {
			n = -n;
		}
	}
	textcolor(FOREGROUND); textbackground(CONSOLE_COLOR);
	return n;
}

void Gui::printInt(const long long n) {
	char* str = intToString(n);
	cputs(str);
	free(str);
}

void Gui::printDouble(const double n) {
	char* str = doubleToString(n);
	cputs(str);
	free(str);
}

Gui::Gui() {
	game = Game::Game(DEFAULT_SIZE);
	x = 0, y = 0;
}

Gui::~Gui()
{

}