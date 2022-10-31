#include "go.h"
#include "conio2.h"

#define TITLE "Krzysztof Nasuta, 193328"
#define MENU_X 2
#define MENU_Y 1
#define STATS_X 5 + MENU_X
#define STATS_Y 22 + MENU_Y
#define MENU_HEIGHT 30
#define MENU_WIDTH 30
#define BOARD_X 40
#define BOARD_Y 2
#define BOARD_SIZE 27
#define BACKGROUND DARKGRAY
#define FOREGROUND BLACK
#define THEME_COLOR LIGHTRED
#define CONSOLE_COLOR LIGHTGRAY
#define POPUP_X 50
#define POPUP_Y 8


class Gui
{
private:
	Game game;
	int x, y;
	void printMenu(); // Print static menu
	void printStats(); // Prints points and turn
	void printBoard(Board* board, bool cursor = true); // Print any board
	void printGameBoard(bool cursor = true); // Print board of game object
	void frame(const char key); // Render a new frame
	void createPopup(const int x, const int y, const int height, const int width); // Create a popup
	// Get a number from user
	int numberInput(const int x, const int y, const char fontColor = FOREGROUND, const char backgroundColor = CONSOLE_COLOR, 
		const bool negative = false, const int maxValue = INT32_MAX, const int minValue = INT32_MIN); 
	void printNumber(const long long n); 
public:
	Gui();
	~Gui();
	void init(); // Intialize GUI
};

// Convert int to char array (string)
char* intToString(long long n);

