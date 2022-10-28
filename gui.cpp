#include "gui.h"
#include "conio2.h"

#define TITLE "Krzysztof, Nasuta, 193328"

GUI::GUI() {
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
	getch();
}

GUI::~GUI()
{
	_setcursortype(_NORMALCURSOR);
	cputs("Program terminated");
}