#include "gui.h"
#include "go.h"
#include "conio2.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
		addCharToString(string, digit, size);
		size++;
	}
	if (negative) { // Add minus to the beginning of the string
		addCharToString(string, '-', size);
		size++;
	}
	reverseString(string, size);
	addCharToString(string, '\0', size);
	return string;
}

// Represnt double as a char array (string), returned pointer has to be freed, precision is the max number of consecutive zeros in decimal part
char* doubleToString(double n, int precision) {
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
		addCharToString(string, '0', size);
		size++;
	}
	while (integerPart > 0) {
		char digit = integerPart % 10 + '0';
		integerPart /= 10;
		addCharToString(string, digit, size);
		size++;
	}
	reverseString(string, size);
	double eps = 1;
	for (int i = 0; i < precision; i++)
		eps /= 10;
	if (decimalPart > eps) {
		addCharToString(string, '.', size);
		size++;
		double tempDecimalPart = decimalPart;
		while (decimalPart > eps) {
			char digit = (char)(tempDecimalPart * 10) + '0';
			tempDecimalPart = tempDecimalPart * 10 - (int)(tempDecimalPart * 10);
			decimalPart /= 10;
			addCharToString(string, digit, size);
			size++;
		}
	}
	addCharToString(string, '\0', size);
	size += 1;
	return string;
}

// Converts string to integer, returns 0 if string is not a number
long long stringToInt(const char* string) {
	long long n = 0; int i = 0; bool negative = false;
	while (string[i] != '\0') {
		char c = string[i++];
		if (c == '-')
			negative = !negative;
		else if (c >= '0' && c <= '9')
			n = n * 10 + (c - '0');
		else
			return 0; // Not a number
	}
	return negative ? -n : n;
}

// Converts string to double, returns 0 if string is not a number
double stringToDouble(const char* string) {
	double decimalPart = 0, decimalBase = 0.1; int i = 0, integerPart = 0; bool negative = false, dotSpotted = false;
	while (string[i] != '\0') {
		char c = string[i++];
		if (c == '-')
			negative = !negative;
		else if (c >= '0' && c <= '9') {
			c -= '0'; 
			if (!dotSpotted) { // interger part
				integerPart = integerPart * 10 + c;
			}
			else { // decimal part
				decimalPart += decimalBase * c;
				decimalBase /= 10;
			}
			
		}
		else if (c == '.' && !dotSpotted) { // check if decimal part has already started
			dotSpotted = true;
		}
		else
			return 0; // Not a number
	}
	double n = decimalPart + integerPart;
	return negative ? -n : n;
}

// Write char array (string) to file, end with '\n' which is required for save loading
void writeLine(char* line, FILE *file) {
	fwrite(line, sizeof(char), strlen(line), file);
	fwrite("\n", sizeof(char), 1, file);
}

// Add char to the end of char array (string)
void addCharToString(char* &string, char c, int size) {
	if (size < 0)
		size = strlen(string);
	string = (char*)realloc(string, sizeof(char) * (size + 1));
	if (string == NULL)
		exit(1);
	string[size] = c;
}

// Initialize GUI
void Gui::init() {
	// Prepare console output style
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
	textcolor(FOREGROUND);
	textbackground(CONSOLE_COLOR);
	clrscr();
	printMenu();
	char key = -1;
	while (key != QUIT_GAME) {
		frame(key);
		key = (char) getch();
	}
	// Restore console output style
	_setcursortype(_NORMALCURSOR);
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	clrscr();
}

void Gui::frame(const char key) {
	// Pressed key is a special key - arrow
	if (key == ARROW_SPECIAL_KEY) 
		move();
	// Place a new stone
	else if (key == PLACE_STONE) 
		placeStone();
	// Create new game
	else if (key == NEW_GAME)
		newGame();
	// load game
	else if (key == LOAD_GAME)
		loadGame();
	// save game
	else if (key == SAVE_GAME)
		saveGame();
	else if (key == ENTER && gameStateEditor && moveId != 0) {
		gameStateEditor = false; game.setCurrentPlayer(WHITE_STATE); // leave game state editor
	}
	printGameBoard();
	printStats();
}

// Allows to change x and y
void Gui::move() {
	switch (getch()) {
	case ARROW_UP:
		y--; break;
	case ARROW_DOWN:
		y++; break;
	case ARROW_LEFT:
		x--; break;
	case ARROW_RIGHT:
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
	if (!validMove)
		return;
	Board board = Board::Board(game.getBoard()->getSize());
	board = *game.getBoard();
	board.set(y, x, game.getCurrentPlayer());
	printBoard(&board, false);
	if (getch() == ENTER) {
		game.placeStone(y, x); moveId++;
		if (gameStateEditor) {
			game.setCurrentPlayer(BLACK_STATE);
			if(moveId > 1) // if handicap is introduced, change white points to 0.5
				game.setPoints(WHITE_STATE, 0.5);
		}
	}
}

// Allows to start a new game
void Gui::newGame() {
	char option = 0, c = 0; int n = 0;
	char basicSizes[] = { 9, 13, 19 }; // basic sizes of board
	char basicSizesCount = sizeof(basicSizes) / sizeof(char); int customInputX = 0, customInputY = 0; // For custom number input location
	// Wait for enter or esc
	while (c != ENTER && c != ESC) {
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
		c = (char) getch(); // choose from menu
		if (c == ARROW_SPECIAL_KEY) {
			switch (getch()) {
			case ARROW_UP:
				option--; break;
			case ARROW_DOWN:
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
	else if(c != ESC) {
		gotoxy(customInputX, customInputY); cputs("               ");
		n = numberInput(customInputX + 6, customInputY, FOREGROUND, THEME_COLOR, false, MAX_SIZE, 0);
	}
	// If ESC not pressed
	if (c != ESC && n > 0) { // Check if action wasn't cancelled (n == INT32_MIN)
		game.newBoard(n);
		x = 0, y = 0, moveId= 0; gameStateEditor = true;
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
	const char* info[] = { "Krzysztof Nasuta 193328", "", "q - zakoncz program", "n - nowa gra", "Strzalki - ruch",
		"i - wybierz pole", "ENTER - potwierdz pole", "s - zapisz gre", "l - wczytaj gre", "Funkcjonalnosci: a-k"}; // Custom info on menu
	const int infoCount = 10; // number of elements in that array
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
	gotoxy(STATS_X - 4, STATS_Y + 1);
	if (gameStateEditor) { // print that game state editor is active
		textcolor(FOREGROUND);
		cputs("Zakoncz edycje stanu - ENTER");
		textcolor(CONSOLE_COLOR);
	}
	else {
		for (int i = 0; i < MENU_WIDTH - 1; i++)
			putch(' '); // clear space
	}
	gotoxy(STATS_X, STATS_Y+2);
	for (int i = 0; i < MENU_WIDTH - 5; i++)
		putch(' ');; // Clear space for points
	gotoxy(STATS_X, STATS_Y + 2);
	cputs("Czarny: "); printDouble(game.getPoints(BLACK_STATE)); cputs("  Bialy: "); printDouble(game.getPoints(WHITE_STATE));
	gotoxy(STATS_X, STATS_Y + 4);
	for (int i = 0; i < MENU_WIDTH - 5; i++)
		putch(' ');; // Clear space for coors
	gotoxy(STATS_X, STATS_Y + 4);
	cputs("   X: "); printInt(x); cputs("   Y: "); printInt(y);
}

void Gui::printEmptyBoard(const int size, const int boardX, const int boardY) {
	for (int j = 0; j < size; j++) {
		gotoxy(boardX, boardY + j);
		char* line = (char*)malloc((size * 2 + 1) * sizeof(char));
		if (line == NULL)
			exit(1); // allocation error
		for (int i = 0; i < size; i++) {
			line[2 * i] = BOARD_CROSS;
			line[2 * i + 1] = BOARD_HORIZONTAL_LINE;
		}
		line[size * 2] = '\0';
		textbackground(BACKGROUND); textcolor(FOREGROUND);
		cputs(line);
		free(line);
	}
}

// Print the board on the screen
void Gui::printBoard(Board* board, bool cursor) {
	const int size = board->getSize();
	text_info ti;
	gettextinfo(&ti);
	int boardSize = ti.screenheight - 5;
	if (boardSize > ti.screenwidth - BOARD_X - 15)
		boardSize = ti.screenwidth - BOARD_X - 15;
	// Print whole board
	if (size <= boardSize) {
		printEmptyBoard(size, BOARD_X, BOARD_Y);
		for (int localX = 0; localX < size; localX++) {
			for (int localY = 0; localY < size; localY++) {
				char c = board->get(localY, localX); // get state of field
				if (c == WHITE_STATE) {
					gotoxy(BOARD_X + 2 * localX, BOARD_Y + localY); // Move the cursor to the right position, 2*x for nice output
					textbackground(WHITE); textcolor(WHITE);
					putch(' '); putch(' ');
				}
				else if (c == BLACK_STATE) {
					gotoxy(BOARD_X + 2 * localX, BOARD_Y + localY);
					textbackground(BLACK); textcolor(BLACK);
					putch(' '); putch(' ');
				}
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
		int startX = x - boardSize / 2, startY = y - boardSize / 2; // Print board starting from startX and startY
		if(startX < 0)
			startX = 0;
		else if (startX > size - boardSize)
			startX = size - boardSize;
		if (startY < 0)
			startY = 0;
		else if(startY > size - boardSize)
			startY = size - boardSize;
		printEmptyBoard(boardSize, BOARD_X, BOARD_Y);
		for (int localX = startX; localX < startX + boardSize; localX++) {
			for (int localY = startY; localY < startY + boardSize; localY++) {
				char c = board->get(localY, localX); // get state of field
				if (c == WHITE_STATE) {
					gotoxy(BOARD_X + 2 * (localX - startX), BOARD_Y + (localY - startY));  // Move the cursor to the right position, 2*x for nice output
					textbackground(WHITE); textcolor(WHITE);
					putch(' '); putch(' ');
				}
				else if (c == BLACK_STATE) {
					gotoxy(BOARD_X + 2 * (localX - startX), BOARD_Y + (localY - startY));
					textbackground(BLACK); textcolor(BLACK);
					putch(' '); putch(' ');
				}
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

/* File lines:
* 1. Board size
* 2. Current player
* 3. Black points
* 4. White points
* 5. Board
* 6. Previous board
*/

// Save game state to file
void Gui::saveGame() {
	createPopup(POPUP_X, POPUP_Y, 5, 22);
	textcolor(FOREGROUND); textbackground(THEME_COLOR);
	gotoxy(POPUP_X + 1, POPUP_Y + 1);
	cputs("       Zapisz");
	gotoxy(POPUP_X + 1, POPUP_Y + 2);
	cputs(" Podaj nazwe zapisu:");
	char* filename = stringInput(POPUP_X + 2, POPUP_Y + 3, FOREGROUND, THEME_COLOR, 18, MAX_FILE_NAME_LENGTH);
	if (filename[0] == '\0') {  // saving aborted
		textbackground(CONSOLE_COLOR);
		clrscr();
		printMenu(); // clear popup
		return;
	}
	const int totalFileNameSize = (strlen(filename) + strlen(FILE_EXTENSION) + 1) * sizeof(char);
	filename = (char*)realloc(filename, totalFileNameSize);
	if (filename == NULL)
		exit(1);
	strcat_s(filename, totalFileNameSize, (char*)FILE_EXTENSION); // Add extension to filename
	FILE* file;
	if (fopen_s(&file, filename, "w") != 0) {
		gotoxy(POPUP_X + 1, POPUP_Y + 3);
		textcolor(RED); textbackground(THEME_COLOR);
		cputs("  Nie mozna zapisac  ");
		textcolor(FOREGROUND);
		getch();
	}
	else {
		// Size of the board
		char* sizeString = intToString(game.getBoard()->getSize());
		writeLine(sizeString, file);
		// Current player
		char* currentPlayer = (char*)malloc(3*sizeof(char));
		if (currentPlayer == NULL)
			exit(1);
		currentPlayer[0] = game.getCurrentPlayer(); currentPlayer[1] = '\0';
		writeLine(currentPlayer, file);
		// Points
		char* blackPoints = doubleToString(game.getPoints(BLACK_STATE)), *whitePoints = doubleToString(game.getPoints(WHITE_STATE));
		writeLine(blackPoints, file);
		writeLine(whitePoints, file);
		// Game board
		char* gameBoard = game.getBoard()->toString(), *previousGameBoard = game.getPreviousBoard()->toString();
		writeLine(gameBoard, file);
		writeLine(previousGameBoard, file);
		// Free the pointers to string
		free(sizeString);
		free(currentPlayer);
		free(blackPoints);
		free(whitePoints);
		free(gameBoard);
		free(previousGameBoard);
		fclose(file); // close the file
	}
	free(filename);
	textbackground(CONSOLE_COLOR);

	clrscr();
	printMenu(); // clear popup
}

// Load game state from file
void Gui::loadGame() {
	createPopup(POPUP_X, POPUP_Y, 5, 22);
	textcolor(FOREGROUND); textbackground(THEME_COLOR);
	gotoxy(POPUP_X + 1, POPUP_Y + 1);
	cputs("       Wczytaj");
	gotoxy(POPUP_X + 1, POPUP_Y + 2);
	cputs(" Podaj nazwe zapisu:");
	char* filename = stringInput(POPUP_X + 2, POPUP_Y + 3, FOREGROUND, THEME_COLOR, 18, MAX_FILE_NAME_LENGTH);
	if (filename[0] == '\0') { // loading aborted
		textbackground(CONSOLE_COLOR);
		clrscr();
		printMenu(); // clear popup
		return;
	}
	const int totalFileNameSize = (strlen(filename) + strlen(FILE_EXTENSION) + 1) * sizeof(char);
	filename = (char*)realloc(filename, totalFileNameSize);
	if (filename == NULL)
		exit(1);
	strcat_s(filename, totalFileNameSize, (char*)FILE_EXTENSION); // Add extension to filename
	FILE* file; int errorCode; bool brokenSave = false;
	if ((errorCode = fopen_s(&file, filename, "rb")) != 0) { // open in binary read mode
		gotoxy(POPUP_X + 1, POPUP_Y + 3);
		textcolor(RED); textbackground(THEME_COLOR);
		if (errorCode == 2) // code 2 - file doesn't exits
			cputs("  Plik nie istnieje ");
		else
			cputs(" Nie mozna otworzyc ");
		textcolor(FOREGROUND); textbackground(CONSOLE_COLOR);
		getch();
	}
	else {
		char buffer[1]; int line = 0; // buffer size is 1 because we read only one char at a time, don't want to lose any data
		char* currentString = NULL; int currentStringSize = 0, boardSize = -1, memSize; double blackPoints = -1, whitePoints = -1; char currentPlayer = EMPTY_STATE;
		char* boardString = NULL, * prevBoardString = NULL;
		while (fread(buffer, sizeof *buffer, 1, file) == 1 && !brokenSave) {
			char c = buffer[0];
			if (c == '\r') // skip carriage return
				continue;
			else if (c == '\n') { // '\n' is required after every line
				addCharToString(currentString, '\0', currentStringSize);
				switch (line) {
				case 0: // Line 1 - board size
					boardSize = (int) stringToInt(currentString); break;
				case 1: // Line 2 - current player
					currentPlayer = currentString[0]; break;
				case 2: // Line 3 - black plaayer points
					blackPoints = stringToDouble(currentString); break;
				case 3: // Line 4 - white player points
					whitePoints = stringToDouble(currentString); break;
				case 4: // Line 5 - game board
					memSize = (currentStringSize + 1) * sizeof(char);
					boardString = (char*)malloc(memSize);
					strcpy_s(boardString, currentStringSize+1, currentString); break;
				case 5: // Line 6 - previous game board
					memSize = (currentStringSize + 1) * sizeof(char);
					prevBoardString = (char*)malloc(memSize);
					strcpy_s(prevBoardString, currentStringSize + 1, currentString); break;
				}
				free(currentString);
				currentString = NULL;
				currentStringSize = 0;
				line++;
			}
			else {
				if (line == 4 || line == 5) { // If loading board, then use this optimized version for fixed string length
					if (currentStringSize == 0) {
						currentString = (char*)malloc((boardSize * boardSize + 1) * sizeof(char)); // we know the wanted size of current string
						if (currentString == NULL) // allocation error
							exit(1);
					}
					if (currentStringSize >= boardSize * boardSize)
						brokenSave = true;
					else 
						currentString[currentStringSize++] = c;
				} else 
					addCharToString(currentString, c, currentStringSize++); // this works without know size, but uses realloc a lot, good for short strings
			}
		}
		fclose(file); // close the file
		if (boardString == NULL || prevBoardString == NULL || boardSize < 0 || blackPoints < 0 || whitePoints < 0 || currentPlayer == EMPTY_STATE || brokenSave) {
			brokenSave = true;  // Game save is broken, don't continue
		}
		else { // Game save was correctly parsed
			Board board = Board::Board(boardSize), prevBoard = Board::Board(boardSize);
			int iterator = 0;
			while (boardString[iterator] != '\0' && prevBoardString[iterator] != '\0') {
				int localX = iterator / boardSize, localY = iterator % boardSize; char c = boardString[iterator], prevC = prevBoardString[iterator];
				if (c == BLACK_STATE || c == WHITE_STATE)
					board.set(localX, localY, c);
				else
					board.set(localX, localY, EMPTY_STATE); // if char is not BLACK_STATE or WHITE_STATE then it's empty (or broken)
				if (prevC == BLACK_STATE || prevC == WHITE_STATE)
					prevBoard.set(localX, localY, prevC);
				else
					prevBoard.set(localX, localY, EMPTY_STATE);
				iterator++;
			}
			if (iterator != boardSize * boardSize) { // Game save is broken
				brokenSave = true;
			}
			else { // Set game state
				game.setPoints(BLACK_STATE, blackPoints); game.setPoints(WHITE_STATE, whitePoints);
				game.setBoard(board); game.setPreviousBoard(prevBoard); x = 0, y = 0;
				game.setCurrentPlayer(currentPlayer); gameStateEditor = false; moveId = 0;
			}
			// Free the pointers to string
			free(boardString); free(prevBoardString); free(currentString);
		}
	}
	if (brokenSave) {
		gotoxy(POPUP_X + 1, POPUP_Y + 3);
		textcolor(RED); textbackground(THEME_COLOR);
		cputs("Plik jest uszkodzony"); getch();
		textcolor(FOREGROUND);
	}
	free(filename);
	textbackground(CONSOLE_COLOR);
	clrscr();
	printMenu(); // clear popup
}

void Gui::createPopup(const int popupX, const int popupY, const int height, const int width) {
	textbackground(THEME_COLOR);
	textcolor(FOREGROUND);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			gotoxy(popupX + i, popupY + j); putch(' ');
		}
	}
	textcolor(FOREGROUND);
	textbackground(CONSOLE_COLOR);
}

// Create input for a number, return INT32_MIN if esc pressed
int Gui::numberInput(const int inputX, const int inputY, const char fontColor, const char backgroundColor, const bool negative, const int maxValue, const int minValue) {
	textcolor(fontColor); textbackground(backgroundColor);
	int maxLength = 0, temp = maxValue, n = 0; char c = 0;
	if (-minValue > maxValue)
		temp = -minValue;
	while (temp > 0) {
		temp /= 10;
		maxLength++;
	}
	maxLength += negative; // Need space for '-'
	while (c != ENTER || n < minValue || n > maxValue) {
		gotoxy(inputX, inputY);
		for (int i = 0; i < maxLength; i++)
			cputs(" ");
		gotoxy(inputX, inputY);
		printInt(n);
		c = (char) getch();
		if (c == BACKSPACE)
			n /= 10; // backspace
		else if (c == ESC)
			return INT32_MIN; // return if action cancelled
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

// Create input for a string, returns empty string if action cancelled
char* Gui::stringInput(const int inputX, const int inputY, const char fontColor, const char backgroundColor, const int printLength, const int maxLength) {
	textcolor(fontColor); textbackground(backgroundColor);
	char c = 0, * string = NULL; int size = 0;
	string = (char*) malloc((maxLength + 1) * sizeof(char));
	if (string == NULL)
		exit(1); // allocation error
	while (c != ENTER) {
		string[size] = '\0'; // end of string
		gotoxy(inputX, inputY);
		for (int i = 0; i < printLength; i++)
			cputs(" ");
		gotoxy(inputX, inputY);
		for (int i = size-printLength; i < size; i++) { // print only last printLength characters
			if (i >= 0)
				putch(string[i]);
		}
		c = (char) getch();
		if (c == BACKSPACE) {
			size = size > 0 ? size - 1 : size; // backspace, removes one char of size
		}
		else if (c == ESC) {
			string[0] = '\0'; // return empty string if action cancelled
			return string;
		}
		else if(size < maxLength) {
			string[size] = c;
			size++;
		}
	}
	string[size-1] = '\0'; // end of string
	textcolor(FOREGROUND); textbackground(CONSOLE_COLOR);
	return string;
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
	x = 0, y = 0, moveId = 0; gameStateEditor = true;
}

Gui::~Gui()
{

}