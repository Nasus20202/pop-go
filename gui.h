#include "go.h"
#include "conio2.h"

#define TITLE "Krzysztof Nasuta, 193328"
#define MENU_X 2
#define MENU_Y 2
#define BOARD_X 25
#define BOARD_Y 2
#define BOARD_SIZE 3
#define BACKGROUND DARKGRAY
#define FOREGROUND BLACK
#define THEME_COLOR LIGHTRED

class Gui
{
private:
	Game game;
	int x, y;
	void printBoard(Board* board, bool cursor = true);
	void printGameBoard(bool cursor = true);
	void frame(const char key); // Render a new frame
public:
	Gui();
	~Gui();
	void init(); // Intialize GUI
};

// Convert int to char array (string)
char* intToString(long long n);

