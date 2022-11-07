#include "go.h"
#include "conio2.h"

// Window title
#define TITLE "Krzysztof Nasuta, 193328"
// Menu location
#define MENU_X 2
#define MENU_Y 1
// Menu size
#define MENU_HEIGHT 30
#define MENU_WIDTH 30
// Statistics location
#define STATS_X 5 + MENU_X
#define STATS_Y 22 + MENU_Y
// Board location
#define BOARD_X 40
#define BOARD_Y 2
// Max board size without scrolling
#define BOARD_SIZE 27
// Colors
#define BACKGROUND DARKGRAY
#define FOREGROUND BLACK
#define THEME_COLOR CYAN
#define CONSOLE_COLOR LIGHTGRAY
// Popup location
#define POPUP_X 50
#define POPUP_Y 8
// Keyboard keys define
#define ESC 0x1B
#define ENTER 0x0D
#define BACKSPACE 0x08
// Limits
#define INT32_MAX 2147483647
#define INT32_MIN -2147483648

class Gui
{
private:
	Game game;
	int x, y;
	// All function avaiable for the user
	void move(); // Allows changing x and y
	void placeStone(); // Allows placing a stone
	void newGame(); // Allows starting a new game

	// All functions that are used to draw the GUI
	void printMenu(); // Print static menu
	void printStats(); // Prints points and turn
	void printBoard(Board* board, bool cursor = true); // Print any board
	void printGameBoard(bool cursor = true); // Print board of game object
	void saveGame(const char* fileName); // Save game state from file
	void loadGame(const char* fileName); // Load game state from file
	void frame(const char key); // Render a new frame
	void createPopup(const int x, const int y, const int height, const int width); // Create a popup
	// Get a number from user, returns INT32_MIN if user pressed ESC
	int numberInput(const int x, const int y, const char fontColor = FOREGROUND, const char backgroundColor = CONSOLE_COLOR, 
		const bool negative = false, const int maxValue = INT32_MAX, const int minValue = INT32_MIN); 
	// Get a string from user, returns empty string if user pressed ESC
	char* stringInput(const int x, const int y, const char fontColor = FOREGROUND, const char backgroundColor = CONSOLE_COLOR, const int printLength = 10, const int maxLength = 100);
	void printInt(const long long n); // Print integer type variable on screen
	void printDouble(const double n); // Print double type variable on screen
public:
	Gui();
	~Gui();
	void init(); // Intialize GUI
};

