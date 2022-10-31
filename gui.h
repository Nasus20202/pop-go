#include "go.h"
#include "conio2.h"

#define TITLE "Krzysztof Nasuta, 193328"
#define MENU_X 2
#define MENU_Y 2
#define BOARD_X 25
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
	void printBoard(Board* board, bool cursor = true);
	void printGameBoard(bool cursor = true);
	void frame(const char key); // Render a new frame
	void createPopup(const int x, const int y, const int height, const int width); // Create a popup
	// Get a number from user
	int numberInput(const int x, const int y, const char fontColor = FOREGROUND, const char backgroundColor = CONSOLE_COLOR, 
		const bool negative = false, const int maxValue = INT32_MAX, const int minValue = INT32_MIN); 
public:
	Gui();
	~Gui();
	void init(); // Intialize GUI
};

// Convert int to char array (string)
char* intToString(long long n);

